// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "ParCable.h"
#include "C64.h"
#include "IO.h"

ParCable::ParCable(C64& ref) : C64Component(ref)
{
};

const char *
ParCable::getDescription() const
{
    return "ParCable";
}

void
ParCable::_reset(bool hard)
{
    RESET_SNAPSHOT_ITEMS(hard)
}

void
ParCable::resetConfig()
{
    setConfigItem(OPT_PAR_CABLE_TYPE, PAR_CABLE_STANDARD);
    setConfigItem(OPT_PAR_CABLE_CONNECT, DRIVE8, true);
    setConfigItem(OPT_PAR_CABLE_CONNECT, DRIVE9, true);
}

i64
ParCable::getConfigItem(Option option) const
{
    switch (option) {
            
        case OPT_PAR_CABLE_TYPE:  return (i64)config.type;

        default:
            assert(false);
            return 0;
    }
}

i64
ParCable::getConfigItem(Option option, long id) const
{
    assert(id == DRIVE8 || id == DRIVE9);
    
    switch (option) {
            
        case OPT_PAR_CABLE_CONNECT:
            
            return id == DRIVE8 ? config.connect8 : config.connect9;

        default:
            assert(false);
            return 0;
    }
}

bool
ParCable::setConfigItem(Option option, i64 value)
{
    switch (option) {
            
        case OPT_PAR_CABLE_TYPE:
            
            if (!ParCableTypeEnum::isValid(value)) {
                throw VC64Error(ERROR_OPT_INV_ARG, ParCableTypeEnum::keyList());
            }
            
            config.type = (ParCableType)value;
            return true;

        default:
            return false;
    }
}

bool
ParCable::setConfigItem(Option option, long id, i64 value)
{
    switch (option) {
            
        case OPT_PAR_CABLE_CONNECT:
        {
            assert(id == DRIVE8 || id == DRIVE9);

            if (id == 8) {
                config.connect8 = value;
            } else {
                config.connect9 = value;
            }
            return true;
        }
        default:
            return false;
    }
}
            
void
ParCable::_dump(dump::Category category, std::ostream& os) const
{
    using namespace util;

    if (category & dump::Config) {

        os << tab("Type");
        os << ParCableTypeEnum::key(config.type) << std::endl;
    }

    if (category & dump::State) {
        
    }
}

u8
ParCable::getValue()
{
    // Values from the C64 side
    u8 ciaprb = cia2.portBinternal();
    u8 ciaddr = cia2.getDDRB();
    u8 cia    = (ciaprb & ciaddr) | (0xFF & ~ciaddr);
    
    // Values from drive 8
    u8 viapra8 = config.connect8 ? drive8.via1.portAinternal() : 0xFF;
    u8 viaddr8 = config.connect8 ? drive8.via1.getDDRA() : 0xFF;
    u8 via8    = (viapra8 & viaddr8) | (0xFF & ~viaddr8);
    
    // Values from drive 9
    u8 viapra9 = config.connect9 ? drive9.via1.portAinternal() : 0xFF;
    u8 viaddr9 = config.connect9 ? drive9.via1.getDDRA() : 0xFF;
    u8 via9    = (viapra9 & viaddr9) | (0xFF & ~viaddr9);
    
    u8 result = cia & via8 & via9;
    
    debug(PAR_DEBUG, "CPU: %x [%x] VIA8: %x [%x] VIA9: %x [%x] -> %x\n",
          ciaprb, ciaddr, viapra8, viaddr8, viapra9, viaddr9, result);

    return result;
}

u8
ParCable::getValue(DriveID id)
{
    return getValue();
}

void
ParCable::driveHandshake()
{
    trace(PAR_DEBUG, "driveHandshake()\n");
    cia2.triggerFallingEdgeOnFlagPin();
}

void
ParCable::c64Handshake()
{
    trace(PAR_DEBUG, "c64Handshake()\n");
    drive8.via1.setInterruptFlag_CB1();
}
