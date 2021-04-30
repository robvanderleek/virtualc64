// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Datasette.h"
#include "C64.h"

util::Time
Pulse::time() const
{
    return util::Time(cycles * 1000000000 / PAL_CLOCK_FREQUENCY);
}

Datasette::~Datasette()
{
    dealloc();
}

void
Datasette::alloc(isize capacity)
{
    dealloc();
    
    if (capacity) {
        pulses = new (std::nothrow) Pulse[capacity];
    }
}

void
Datasette::dealloc()
{
    if (pulses) {
        delete[] pulses;
        pulses = nullptr;
        size = 0;
    }
}

void
Datasette::_reset()
{
    RESET_SNAPSHOT_ITEMS
    
    rewind();
}

isize
Datasette::_size()
{
    util::SerCounter counter;
    
    applyToPersistentItems(counter);
    applyToResetItems(counter);
    
    counter << size;
    for (isize i = 0; i < size; i++) counter << pulses[i].cycles;

    return counter.count;
}

isize
Datasette::didLoadFromBuffer(const u8 *buffer)
{
    util::SerReader reader(buffer);

    // Free previously allocated memory
    dealloc();

    // Load size
    reader << size;

    // Make sure that corrupted values do not cause any damage
    if (size > 0x8FFFF) { size = 0; }

    // Allocate new memory
    alloc(size);

    // Load pulses from buffer
    for (isize i = 0; i < size; i++) reader << pulses[i].cycles;

    return (isize)(reader.ptr - buffer);
}

isize
Datasette::didSaveToBuffer(u8 *buffer)
{
    util::SerWriter writer(buffer);

    // Save size
    writer << size;
    
    // Save pulses to buffer
    for (isize i = 0; i < size; i++) writer << pulses[i].cycles;
        
    return (isize)(writer.ptr - buffer);
}

util::Time
Datasette::tapeDuration(isize pos)
{
    util::Time result;
    
    for (isize i = 0; i < pos && i < size; i++) {
        result += pulses[i].time();
    }
    
    return result;
}

bool
Datasette::insertTape(TAPFile *file)
{
    suspend();

    debug(TAP_DEBUG, "Inserting tape...\n");

    // Allocate pulse buffer
    alloc(file->size);

    // Read pulses
    size = 0;
    if (file->version() == 0) {

        // TAP type 0 (standard pulse format)
        for (isize i = 0x14; (usize)i < file->size; i++, size++) {
            
            u8 byte = file->getData(i);
            pulses[size].cycles = 8 * (byte ? byte : 256);
        }

    } else {

        // TAP type 1 (extended pulse format)
        for (isize i = 0x14; (usize)i < file->size; i++, size++) {
            
            u8 byte = file->getData(i);
            if (byte) {
                pulses[size].cycles = 8 * byte;
            } else {
                pulses[size].cycles = LO_LO_HI_HI(file->getData(i + 1),
                                                  file->getData(i + 2),
                                                  file->getData(i + 3), 0);
                i += 3;
            }
        }
    }

    rewind();
    
    c64.putMessage(MSG_VC1530_TAPE);
    resume();
    
    return true;
}

void
Datasette::ejectTape()
{
    suspend();
    
    trace(TAP_DEBUG, "Ejecting tape\n");

    if (!hasTape())
        return;
    
    pressStop();
    
    assert(pulses);
    free(pulses);
    pulses = nullptr;
    size = 0;
    type = 0;
    durationInCycles = 0;
    head = -1;

    c64.putMessage(MSG_VC1530_NO_TAPE);
    resume();
}

void
Datasette::advanceHead(bool silent)
{
    assert(head < size);
    
    head++;
    
    // Send message if the tapeCounter (in seconds) changes
    /*
    u32 newHeadInSeconds = (u32)(headInCycles / c64.frequency);
    if (newHeadInSeconds != headInSeconds && !silent)
        c64.putMessage(MSG_VC1530_PROGRESS);
    */
}

void
Datasette::pressPlay()
{
    if (!hasTape())
        return;
    
    trace(TAP_DEBUG, "pressPlay\n");
    playKey = true;

    // Schedule first pulse
    usize length = pulses[head].cycles; //  pulseLength();
    nextRisingEdge = length / 2;
    nextFallingEdge = length;
    advanceHead();
}

void
Datasette::pressStop()
{
    trace(TAP_DEBUG, "pressStop\n");
    motor = false;
    playKey = false;
}

void
Datasette::_execute()
{
    // Only proceed if the datasette is active
    if (!hasTape() || !playKey || !motor) return;
        
    if (--nextRisingEdge == 0) {
        
        cia1.triggerRisingEdgeOnFlagPin();
    }

    if (--nextFallingEdge == 0) {
        
        cia1.triggerFallingEdgeOnFlagPin();

        if (head < size) {

            // Schedule the next pulse
            isize length = pulses[head].cycles; //  pulseLength();
            nextRisingEdge = length / 2;
            nextFallingEdge = length;
            advanceHead();
            
        } else {
            
            // Press the stop key
            pressStop();
        }
    }
}
