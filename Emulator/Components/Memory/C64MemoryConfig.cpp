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

#include "config.h"
#include "C64Memory.h"

namespace vc64 {

i64
C64Memory::getOption(Option option) const
{
    switch (option) {

        case OPT_RAM_PATTERN:   return config.ramPattern;
        case OPT_SAVE_ROMS:     return config.saveRoms;

        default:
            fatalError;
    }
}

void
C64Memory::setOption(Option option, i64 value)
{
    switch (option) {

        case OPT_RAM_PATTERN:

            if (!RamPatternEnum::isValid(value)) {
                throw VC64Error(ERROR_OPT_INVARG, RamPatternEnum::keyList());
            }

            config.ramPattern = (RamPattern)value;
            return;

        case OPT_SAVE_ROMS:

            config.saveRoms = value;
            return;

        default:
            fatalError;
    }
}

}