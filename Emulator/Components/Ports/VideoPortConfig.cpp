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
#include "VideoPort.h"

namespace vc64 {

i64
VideoPort::getOption(Option option) const
{
    switch (option) {

        case OPT_VID_WHITE_NOISE:   return config.whiteNoise;

        default:
            fatalError;
    }
}

void
VideoPort::checkOption(Option opt, i64 value)
{
    switch (opt) {

        case OPT_VID_WHITE_NOISE:

            return;

        default:
            throw VC64Error(ERROR_OPT_UNSUPPORTED);
    }
}

void
VideoPort::setOption(Option opt, i64 value)
{
    checkOption(opt, value);

    switch (opt) {

        case OPT_VID_WHITE_NOISE:

            config.whiteNoise = (bool)value;
            return;

        default:
            fatalError;
    }
}

}