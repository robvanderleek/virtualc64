// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// This FILE is dual-licensed. You are free to choose between:
//
//     - The GNU General Public License v3 (or any later version)
//     - The Mozilla Public License v2
//
// SPDX-License-Identifier: GPL-3.0-or-later OR MPL-2.0
// -----------------------------------------------------------------------------

#pragma once

#include "Cartridge.h"

namespace vc64 {

class Reu final : public Cartridge {

    virtual const CartridgeTraits &getCartridgeTraits() const override {

        static CartridgeTraits traits = {

            .type       = CRT_REU,
            .title      = "REU",
            .memory     = 0,
            .battery    = true
        };

        traits.memory = KB(kb);
        return traits;
    }

private:

    // REU capacity in KB
    isize kb = 0;

    //
    // REU registers
    //

    // Status register
    u8 sr = 0;

    // Command register
    u8 cr = 0;

    // Base address registers
    u16 c64Base = 0;
    u32 reuBase = 0;

    // Address registers used during DMA
    u16 c64Addr = 0;
    u32 reuAddr = 0;

    // Upper bank bits (used by modded REUs with higher capacities)
    u32 upperBankBits = 0;

    // Transfer length register
    u16 tlen = 0;

    // Interrupt mask
    u8 imr = 0;

    // Address control register
    u8 acr = 0;

    // Latest value on the data bus
    u8 bus = 0;


    //
    // Emulation specific variables
    //

    // Remembers the memory type of the uppermost memory bank
    MemoryType memTypeF = M_NONE;


    //
    // Initializing
    //

public:

    Reu(C64 &ref) : Cartridge(ref) { };
    Reu(C64 &ref, isize kb);


    //
    // Methods from CoreObject
    //

private:

    void _dump(Category category, std::ostream& os) const override;


    //
    // Methods from CoreComponent
    //

public:

    Reu& operator= (const Reu& other) {

        Cartridge::operator=(other);

        CLONE(kb)
        CLONE(sr)
        CLONE(cr)
        CLONE(c64Base)
        CLONE(reuBase)
        CLONE(c64Addr)
        CLONE(reuAddr)
        CLONE(upperBankBits)
        CLONE(tlen)
        CLONE(imr)
        CLONE(acr)
        CLONE(bus)
        CLONE(memTypeF)

        return *this;
    }
    virtual void clone(const Cartridge &other) override { *this = (const Reu &)other; }

    template <class T>
    void serialize(T& worker)
    {
        if (isResetter(worker)) return;

        worker

        << kb
        << sr
        << cr
        << c64Base
        << reuBase
        << c64Addr
        << reuAddr
        << upperBankBits
        << tlen
        << imr
        << acr
        << bus
        << memTypeF;

    } CARTRIDGE_SERIALIZERS(serialize);

    void _didReset(bool hard) override;


    //
    // Querying properties
    //

    bool isREU1700() const { return getRamCapacity() == KB(128); }
    bool isREU1764() const { return getRamCapacity() == KB(256); }
    bool isREU1750() const { return getRamCapacity() >= KB(512); }

    // Returns the bitmask of the REU address register
    u32 wrapMask() const { return isREU1700() ? 0x1FFFF : 0x7FFFF; }

    /* Emulation speed
     *
     * This value indicates how many bytes are transfered during a single DMA
     * cycle. A value of 1 means the REU is emulated at native speed. A value
     * of 2 emulates a REU at twice the speed etc. A very high value emulates
     * a turbo REU. In that case, the entire data transfer is performed in a
     * single DMA cycle.
     */
    isize bytesPerDmaCycle() { return expansionPort.getConfig().reuSpeed; }


    //
    // Accessing REU registers
    //

    bool autoloadEnabled() const { return GET_BIT(cr, 5); }
    bool ff00Enabled() const { return !GET_BIT(cr, 4); }
    bool ff00Disabled() const { return GET_BIT(cr, 4); }

    bool irqEnabled() const { return GET_BIT(imr, 7); }
    bool irqOnEndOfBlock() const { return GET_BIT(imr, 6); }
    bool irqOnVerifyError() const { return GET_BIT(imr, 5); }

    bool isArmed() const { return GET_BIT(cr, 7) && ff00Enabled(); }

    isize memStep() const { return GET_BIT(acr,7) ? 0 : 1; }
    isize reuStep() const { return GET_BIT(acr,6) ? 0 : 1; }


    //
    // Accessing cartridge memory
    //

public:

    u8 peekIO2(u16 addr) override;
    u8 spypeekIO2(u16 addr) const override;
    void pokeIO2(u16 addr, u8 value) override;
    void poke(u16 addr, u8 value) override;

private:

    u8 readFromReuRam(u32 addr);
    void writeToReuRam(u32 addr, u8 value);


    //
    // Performing DMA
    //

private:

    void incMemAddr(u16 &addr) { addr = U16_ADD(addr, 1); }
    void incReuAddr(u32 &addr) { addr = U32_ADD(addr, 1) & wrapMask(); }

    void prepareDma();
    bool doDma(EventID id);
    void finalizeDma(EventID id);

    // [[deprecated]] void doDma();
    [[deprecated]] void stash(isize len);
    [[deprecated]] void fetch(isize len);
    [[deprecated]] void swap(isize len);
    [[deprecated]] void verify(isize len);


    //
    // Processing events
    //

    void processEvent(EventID id) override;


    //
    // Managing interrupts
    //

    void triggerEndOfBlockIrq();
    void triggerVerifyErrorIrq();


    //
    // Handling delegation calls
    //

public:

    void updatePeekPokeLookupTables() override;
};

}
