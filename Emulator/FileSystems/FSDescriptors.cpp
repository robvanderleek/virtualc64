// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v2
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

// #include "FSDescriptors.h"
#include "C64.h"

FSDeviceDescriptor::FSDeviceDescriptor(DiskType type, DOSType dos)
{
    assert(dos == DOSType_CBM);
 
    this->dos = dos;
    
    switch (type) {
            
        case DiskType_SS_SD:
            numCyls = 35;
            numHeads = 1;
            break;
            
        case DiskType_DS_SD:
            numCyls = 35;
            numHeads = 2;
            break;
            
        default:
            assert(false);
    }
}

FSDeviceDescriptor::FSDeviceDescriptor(D64File *d64)
{
    assert(d64);
    
    this->dos = DOSType_CBM;
    numCyls = d64->numberOfTracks();
    numHeads = 1;
}

bool
FSDeviceDescriptor::isValidLink(TSLink ref)
{
    return isTrackNr(ref.t) && ref.s >= 0 && ref.s < numSectors(ref.t);
}

u32
FSDeviceDescriptor::speedZone(Track t)
{
    assert(isTrackNr(t));
    
    return
    (t <= 17) ? 3 :
    (t <= 24) ? 2 :
    (t <= 30) ? 1 :
    (t <= 35) ? 0 :
    (t <= 52) ? 3 :     // Second side (DS disks only)
    (t <= 59) ? 2 :     // Second side (DS disks only)
    (t <= 65) ? 1 : 0;  // Second side (DS disks only)
}

u32
FSDeviceDescriptor::numSectors(Track t)
{
    if (!isTrackNr(t)) return 0;
    
    switch (speedZone(t)) {

        case 0: return 17;
        case 1: return 18;
        case 2: return 19;
        case 3: return 21;
    }
    
    assert(false);
    return 0;
}

u32
FSDeviceDescriptor::numBlocks()
{
    u32 result = 0;
    
    for (u32 i = 1; i <= numTracks(); i++) {
        result += numSectors(i);
    }
    
    return result;
}

TSLink
FSDeviceDescriptor::tsLink(Block b)
{
    for (Track i = 1; i <= numTracks(); i++) {

        u32 num = numSectors(i);
        if (b < num) return TSLink{i,b};
        b -= num;
    }
    
    return TSLink{0,0};
}

Block
FSDeviceDescriptor::blockNr(TSLink ts)
{
    if (!isValidLink(ts)) return (Block)(-1);
    
    u32 cnt = ts.s;
    for (Track i = 1; i < ts.t; i++) {
        cnt += numSectors(i);
    }
    
    return cnt;
}

TSLink
FSDeviceDescriptor::nextBlockRef(TSLink ref)
{
    assert(isValidLink(ref));
    
    Track t = ref.t;
    Sector s = ref.s;
    
    // Lookup table for the next sector (interleave patterns)
    Sector next[5][21] = {

        // Speed zone 0 - 3
        { 10,11,12,13,14,15,16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
        { 10,11,12,13,14,15,16,17, 1, 0, 2, 3, 4, 5, 6, 7, 8, 9 },
        { 10,11,12,13,14,15,16,17,18, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        { 10,11,12,13,14,15,16,17,18,19,20, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
        
        // Directory track
        {  3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18, 0, 1, 2 }
    };

    if (t == 18) {

        // Take care of the directory track
        s = next[4][s];

        // Return immediately if we've wrapped over (directory track is full)
        if (s == 0) return {0,0};
    
    } else {
            
        // Take care of all other tracks
        s = next[speedZone(t)][s];
        
        // Move to the next track if we've wrapped over
        if (s == 0) {
            
            if (t >= numTracks()) return {0,0};
            t = t == 17 ? 19 : t + 1;
        }
    }
    
    assert(isValidLink(TSLink{t,s}));
    return TSLink{t,s};
}
