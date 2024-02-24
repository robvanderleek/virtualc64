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

namespace vc64 {

SID::SID(C64 &ref, int n) : SubComponent(ref), nr(n)
{
    setClockFrequency(PAL_CLOCK_FREQUENCY);
}

u32
SID::getClockFrequency() const
{
    assert(resid.getClockFrequency() == fastsid.getClockFrequency());
    return resid.getClockFrequency();
}

void
SID::setClockFrequency(u32 frequency)
{
    resid.setClockFrequency(frequency);
    fastsid.setClockFrequency(frequency);
}

SIDRevision 
SID::getRevision() const
{
    assert(resid.getRevision() == fastsid.getRevision());
    return resid.getRevision();
}

void 
SID::setRevision(SIDRevision revision)
{
    resid.setRevision(revision);
    fastsid.setRevision(revision);
}

double 
SID::getSampleRate() const
{
    assert(resid.getSampleRate() == fastsid.getSampleRate());
    return resid.getSampleRate();
}

void 
SID::setSampleRate(double rate)
{
    resid.setSampleRate(rate);
    fastsid.setSampleRate(rate);
}


bool 
SID::getAudioFilter() const
{
    assert(resid.getAudioFilter() == fastsid.getAudioFilter());
    return resid.getAudioFilter();
}

void 
SID::setAudioFilter(bool enable)
{
    resid.setAudioFilter(enable);
    fastsid.setAudioFilter(enable);
}

SamplingMethod 
SID::getSamplingMethod() const
{
    // Not available for FastSID
    return resid.getSamplingMethod();
}
void 
SID::setSamplingMethod(SamplingMethod method)
{
    // Not available for FastSID
    resid.setSamplingMethod(method);
}

}