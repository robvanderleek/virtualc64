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

#include "OptionTypes.h"
#include "Reflection.h"
#include "Defaults.h"
#include <algorithm>

namespace vc64 {

typedef std::vector<Option> ConfigOptions;

class Configurable
{
    constexpr static ConfigOptions options = { };

public:

    virtual ~Configurable() = default;

    // Returns the available config options
    virtual const ConfigOptions &getOptions() const { return options; }

    // Returns true iff a specific option is available
    bool isValidOption(Option opt) const;

    // Gets a config options
    virtual i64 getOption(Option opt) const { return 0; }

    // Sets a config option
    virtual void setOption(Option opt, i64 value) { }
    void setOption(Option opt, const string &value);
    void setOption(const string &opt, const string &value);

    // Resets all config options
    void resetConfig(const Defaults &defaults);
    void resetConfig(const Defaults &defaults, isize i);

    // Dumps the current configuration
    void dumpConfig(std::ostream& os) const;

    // Returns a textual description for all available options
    string keyList() { return OptionEnum::keyList([&](Option i) { return isValidOption(i); }); }
    string argList() { return OptionEnum::argList([&](Option i) { return isValidOption(i); }); }
};

}
