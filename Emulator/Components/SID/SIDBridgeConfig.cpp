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
#include "SID.h"
#include "Emulator.h"

namespace vc64 {

i64
SIDBridge::getOption(Option option) const
{
    switch (option) {

        case OPT_SID_REVISION:
            return config.revision;

        case OPT_SID_POWER_SAVE:
            return config.powerSave;

        case OPT_SID_FILTER:
            return config.filter;

        case OPT_SID_ENGINE:
            return config.engine;

        case OPT_SID_SAMPLING:
            return config.sampling;

        case OPT_AUD_VOL_L:
            return config.volL;

        case OPT_AUD_VOL_R:
            return config.volR;

        default:
            fatalError;
    }
}

void
SIDBridge::setOption(Option option, i64 value)
{
    bool wasMuted = isMuted();

    switch (option) {

        case OPT_SID_POWER_SAVE:
        {
            {   SUSPENDED

                config.powerSave = bool(value);
            }
            return;
        }
        case OPT_SID_REVISION:
        {
            if (!SIDRevisionEnum::isValid(value)) {
                throw VC64Error(ERROR_OPT_INVARG, SIDRevisionEnum::keyList());
            }

            {   SUSPENDED

                config.revision = SIDRevision(value);
                sid[0].setRevision(SIDRevision(value));
                sid[1].setRevision(SIDRevision(value));
                sid[2].setRevision(SIDRevision(value));
                sid[3].setRevision(SIDRevision(value));
            }
            return;
        }
        case OPT_SID_FILTER:
        {
            {   SUSPENDED

                config.filter = bool(value);
                sid[0].setAudioFilter(bool(value));
                sid[1].setAudioFilter(bool(value));
                sid[2].setAudioFilter(bool(value));
                sid[3].setAudioFilter(bool(value));
            }
            return;
        }
        case OPT_SID_ENGINE:
        {
            if (!SIDEngineEnum::isValid(value)) {
                throw VC64Error(ERROR_OPT_INVARG, SIDEngineEnum::keyList());
            }

            {   SUSPENDED

                config.engine = SIDEngine(value);
            }
            return;
        }
        case OPT_SID_SAMPLING:
        {
            if (!SamplingMethodEnum::isValid(value)) {
                throw VC64Error(ERROR_OPT_INVARG, SamplingMethodEnum::keyList());
            }

            {   SUSPENDED

                config.sampling = SamplingMethod(value);
                sid[0].setSamplingMethod(SamplingMethod(value));
                sid[1].setSamplingMethod(SamplingMethod(value));
                sid[2].setSamplingMethod(SamplingMethod(value));
                sid[3].setSamplingMethod(SamplingMethod(value));
            }
            return;
        }
        case OPT_AUD_VOL_L:

            config.volL = std::clamp(value, 0LL, 100LL);
            volL = powf((float)config.volL / 50, 1.4f);

            if (wasMuted != isMuted()) {
                msgQueue.put(MSG_MUTE, isMuted());
            }
            return;

        case OPT_AUD_VOL_R:

            config.volR = std::clamp(value, 0LL, 100LL);
            volR = powf((float)config.volR / 50, 1.4f);

            if (wasMuted != isMuted()) {
                msgQueue.put(MSG_MUTE, isMuted());
            }
            return;

        default:
            fatalError;
    }
}

}