// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Cartridge.h"

class StarDos : public Cartridge {
        
    i64 voltage = 5000000;
    i64 latestVoltageUpdate = 0;
    
    
    //
    // Initializing
    //
    
public:
    
    StarDos(C64 &ref) : Cartridge(ref) { };
    const char *getDescription() const override { return "StarDos"; }
    CartridgeType getCartridgeType() const override { return CRT_STARDOS; }

private:
    
    void _reset(bool hard) override;


    //
    // Methods from CoreObject
    //

private:

    void _dump(Category category, std::ostream& os) const override;

    
    //
    // Methods from CoreComponent
    //
    
private:
    
    template <class T>
    void applyToPersistentItems(T& worker)
    {
    }
    
    template <class T>
    void applyToResetItems(T& worker, bool hard = true)
    {
        worker
        
        << voltage
        << latestVoltageUpdate;
    }
    
    isize __size() override { COMPUTE_SNAPSHOT_SIZE }
    isize __load(const u8 *buffer) override { LOAD_SNAPSHOT_ITEMS }
    isize __save(u8 *buffer) override { SAVE_SNAPSHOT_ITEMS }

    
    //
    // Accessing cartridge memory
    //
    
    u8 peekIO1(u16 addr) override { charge(); return 0; }
    u8 spypeekIO1(u16 addr) const override { return 0; }
    u8 peekIO2(u16 addr) override { discharge(); return 0; }
    u8 spypeekIO2(u16 addr) const override { return 0; }
    void pokeIO1(u16 addr, u8 value) override { charge(); }
    void pokeIO2(u16 addr, u8 value) override { discharge(); }
    
    //
    // Handling delegation calls
    //
    
public:
    
    void updatePeekPokeLookupTables() override;
    
    
    //
    // Working with the capacitor
    //
    
private:
    
    void updateVoltage();
    void charge();
    void discharge();
    void enableROML();
    void disableROML();
};
