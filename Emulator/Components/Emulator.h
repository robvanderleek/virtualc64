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

#include "C64.h"
#include "Defaults.h"
#include "EmulatorTypes.h"
#include "Host.h"
#include "Thread.h"
#include "CmdQueue.h"

class VirtualC64;

namespace vc64 {

class Emulator : public Thread, public Inspectable<EmulatorInfo, EmulatorStats> {

    friend class API;
    friend class ::VirtualC64;

    EmulatorConfig config = { };

    // The virtual C64
    C64 main = C64(*this);

    // The run-ahead instance
    C64 ahead = C64(*this);

    // Indicates if the run-ahead instance needs an update
    bool updateRunAhead = true;
    
public:

    // Storage for external events
    CmdQueue cmdQueue;

    // User default settings
    static Defaults defaults;

    // Information about the host system
    Host host = Host(*this);

    
    //
    // Initializing
    //

public:

    Emulator();
    ~Emulator();

    // Launches the emulator thread
    void launch(const void *listener, Callback *func);

private:

    // Initializes all components
    void initialize();


    //
    // Methods from Inspectable
    //

public:

    void recordState(EmulatorInfo &result) const override;
    void recordStats(EmulatorStats &result) const override;

    // EmulatorStats getStats() const;


    //
    // Configuring
    //

public:

    // Configures the emulator to match a specific C64 model
    void configure(C64Model model);

    // Sets a single configuration option
    void configure(Option option, i64 value) throws;
    void configure(Option option, long id, i64 value) throws;

    // Queries a single configuration option
    i64 getConfigItem(Option option) const;
    i64 getConfigItem(Option option, long id) const;
    void setConfigItem(Option option, i64 value);

    // Returns the emulated refresh rate of the virtual C64
    double refreshRate() const override;

private:

    static EmulatorConfig getDefaultConfig();
    const EmulatorConfig &getConfig() const { return config; }
    void resetConfig();

    // Overrides a config option if the corresponding debug option is enabled
    i64 overrideOption(Option option, i64 value);

    // Powers off and resets the emulator to it's initial state
    void revertToFactorySettings();


    //
    // Methods from CoreObject
    //

private:

    const char *getDescription() const override { return "Emulator"; }
    void _dump(Category category, std::ostream& os) const override;


    //
    // Methods from Thread
    //

private:

    void isReady() override;
    isize missingFrames() const override;
    void update() override;
    bool shouldWarp();
    void computeFrame() override;

    void _powerOn() override { main.powerOn(); }
    void _powerOff() override { main.powerOff(); }
    void _pause() override { main.pause(); }
    void _run() override { main.run(); }
    void _halt() override { main.halt(); }
    void _warpOn() override { main.warpOn(); }
    void _warpOff() override { main.warpOff(); }
    void _trackOn() override { main.trackOn(); }
    void _trackOff() override { main.trackOff(); }

    
    //
    // Audio and Video
    //

    u32 *getTexture() const;
    u32 *getNoise() const;


    //
    // Command queue
    //

public:

    // Feeds a command into the command queue
    void put(const Cmd &cmd);

private:

    // Processes a command from the command queue
    void process(const Cmd &cmd);
};

//
// Public API
//

class API : public Suspendable, public References {

public:
    
    class Emulator &emulator;

    API(Emulator& ref) : References(ref.main), emulator(ref) { }

    void suspend() { emulator.suspend(); }
    void resume() { emulator.resume(); }

    bool isUserThread() const { return !emulator.isEmulatorThread(); }
};

}
