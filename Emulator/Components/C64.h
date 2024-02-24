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

#include "C64Types.h"
#include "MsgQueue.h"
#include "Thread.h"

// Sub components
#include "ExpansionPort.h"
#include "IEC.h"
#include "Keyboard.h"
#include "ControlPort.h"
#include "C64Memory.h"
#include "DriveMemory.h"
#include "FlashRom.h"
#include "VICII.h"
#include "Muxer.h"
#include "TOD.h"
#include "CIA.h"
#include "CPU.h"
#include "PowerSupply.h"
#include "Recorder.h"
#include "RegressionTester.h"
#include "RetroShell.h"

// Cartridges
#include "Cartridge.h"
#include "CustomCartridges.h"

// Peripherals
#include "Drive.h"
#include "ParCable.h"
#include "Datasette.h"
#include "Mouse.h"

// Media files
#include "Snapshot.h"
#include "T64File.h"
#include "D64File.h"
#include "G64File.h"
#include "PRGFile.h"
#include "Folder.h"
#include "P00File.h"
#include "RomFile.h"
#include "TAPFile.h"
#include "CRTFile.h"
#include "FileSystem.h"

namespace vc64 {

//
// Macros and constants
//

// Checks the category of an event slot
static constexpr bool isPrimarySlot(isize s) { return s <= SLOT_SEC; }
static constexpr bool isSecondarySlot(isize s) { return s > SLOT_SEC && s <= SLOT_TER; }
static constexpr bool isTertiarySlot(isize s) { return s > SLOT_TER; }

// Time stamp used for messages that never trigger
static constexpr Cycle NEVER = INT64_MAX;

// Inspection interval in seconds (interval between INS_xxx events)
static constexpr double inspectionInterval = 0.1;


/* A complete virtual C64. This class is the most prominent one of all. To run
 * the emulator, it is sufficient to create a single object of this type. All
 * subcomponents are created automatically. The public API gives you control
 * over the emulator's behaviour such as running and pausing the emulation.
 * Please note that most subcomponents have their own public API. E.g., to
 * query information from VICII, you need to invoke a method on c64.vicii.
 */
class C64 final : public CoreComponent, public Inspectable<C64Info, Void> {

    friend class Emulator;

private:
    
    // The current configuration
    C64Config config = {};

    // Result of the latest inspection
    // mutable C64Info eventInfo = {};
    mutable EventSlotInfo slotInfo[SLOT_COUNT];


    //
    // Sub components
    //
    
public:

    // Core components
    C64Memory mem = C64Memory(*this);
    CPU cpu = CPU(MOS_6510, *this);
    CIA1 cia1 = CIA1(*this);
    CIA2 cia2 = CIA2(*this);
    VICII vic = VICII(*this);
    Muxer muxer = Muxer(*this);

    // Logic board
    PowerSupply supply = PowerSupply(*this);
    ControlPort port1 = ControlPort(*this, PORT_1);
    ControlPort port2 = ControlPort(*this, PORT_2);
    ExpansionPort expansionport = ExpansionPort(*this);
    IEC iec = IEC(*this);
    
    // Peripherals
    Keyboard keyboard = Keyboard(*this);
    Drive drive8 = Drive(DRIVE8, *this);
    Drive drive9 = Drive(DRIVE9, *this);
    ParCable parCable = ParCable(*this);
    Datasette datasette = Datasette(*this);
    
    // Misc
    RetroShell retroShell = RetroShell(*this);
    RegressionTester regressionTester = RegressionTester(*this);
    Recorder recorder = Recorder(*this);
    MsgQueue msgQueue = MsgQueue();


    //
    // Event scheduler
    //

public:

    // Trigger cycle
    Cycle trigger[SLOT_COUNT] = { };

    // The event identifier
    EventID id[SLOT_COUNT] = { };

    // An optional data value
    i64 data[SLOT_COUNT] = { };

    // Next trigger cycle
    Cycle nextTrigger = NEVER;

    
    //
    // Emulator thread
    //
    
private:

    /* Run loop flags. This variable is checked at the end of each runloop
     * iteration. Most of the time, the variable is 0 which causes the runloop
     * to repeat. A value greater than 0 means that one or more runloop control
     * flags are set. These flags are flags processed and the loop either
     * repeats or terminates depending on the provided flags.
     */
    RunLoopFlags flags = 0;

    
    //
    // Storage
    //
    
private:
    
    Snapshot *autoSnapshot = nullptr;
    Snapshot *userSnapshot = nullptr;

    typedef struct { Cycle trigger; i64 payload; } Alarm;
    std::vector<Alarm> alarms;

    
    //
    // State
    //
    
public:
    
    // The total number of frames drawn since power up
    u64 frame = 0;
    
    // The currently drawn scanline (first scanline = 0)
    u16 scanline = 0;
    
    // The currently executed scanline cycle (first cylce = 1)
    u8 rasterCycle = 1;

private:

    /* Indicates whether C64 is running in ultimax mode. Ultimax mode can be
     * enabled by external cartridges by pulling game line low and keeping
     * exrom line high. In ultimax mode, most of the C64's RAM and ROM is
     * invisible.
     */
    bool ultimax = false;

    /* Indicates if headless mode is activated. If yes, the pixel drawing code
     * is skipped. Headless mode is used to accelerate warp mode and to speed
     * up the computation of some frames in run-ahead mode.
     */
    bool headless = false;

    /* Indicates whether the state has been altered by an external event.
     * This flag is used to determine whether the run-ahead instance needs to
     * be recreated.
     */
    bool isDirty = false;

    // Duration of a CPU cycle in 1/10 nano seconds
    i64 durationOfOneCycle;


    //
    // Static methods
    //

public:

    // Returns a version string for this release
    static string version();

    // Returns a build number string for this release
    static string build();

    // Returns a textual description for an event
    static const char *eventName(EventSlot slot, EventID id);


    //
    // Initializing
    //
    
public:
    
    C64(class Emulator& ref);
    ~C64();


    //
    // Methods from CoreObject
    //

public:

    void prefix() const override;

private:

    const char *getDescription() const override { return "C64"; }
    void _dump(Category category, std::ostream& os) const override;


    //
    // Methods from CoreComponent
    //

public:
    
    void hardReset();
    void softReset();

private:

    void initialize();

    
    //
    // Configuring
    //

public:

    const C64Config &getConfig() const { return config; }

    // Updates the clock frequency and all variables derived from it
    void updateClockFrequency();
    
    // Indicates that the run-ahead instance needs an update
    void markAsDirty() { isDirty = true; }

    // Enables or disables headless mode
    bool getHeadless() const { return headless; }
    void setHeadless(bool value) { headless = value; }


    //
    // Analyzing
    //
    
public:

    InspectionTarget getInspectionTarget() const;
    void setInspectionTarget(InspectionTarget target, Cycle trigger = 0);
    void removeInspectionTarget() { setInspectionTarget(INSPECTION_NONE); }

    
    //
    // Methods from CoreComponent
    //
    
public:

    C64& operator= (const C64& other) {

        CLONE(mem)
        CLONE(cpu)
        CLONE(cia1)
        CLONE(cia2)
        CLONE(vic)
        CLONE(muxer)
        CLONE(supply)
        CLONE(port1)
        CLONE(port2)
        CLONE(expansionport)
        CLONE(iec)
        CLONE(keyboard)
        CLONE(drive8)
        CLONE(drive9)
        CLONE(parCable)
        CLONE(datasette)
        CLONE(retroShell)
        CLONE(regressionTester)
        CLONE(recorder)

        CLONE_ARRAY(trigger)
        CLONE_ARRAY(id)
        CLONE_ARRAY(data)
        CLONE(nextTrigger)
        CLONE(frame)
        CLONE(scanline)
        CLONE(rasterCycle)
        CLONE(ultimax)

        CLONE(durationOfOneCycle)

        return *this;
    }

    template <class T>
    void serialize(T& worker)
    {
        worker

        << mem
        << cpu
        << cia1
        << cia2
        << vic
        << muxer
        << supply
        << port1
        << port2
        << expansionport
        << iec
        << keyboard
        << drive8
        << drive9
        << parCable
        << datasette
        << retroShell
        << regressionTester
        << recorder

        << trigger
        << id
        << data
        << nextTrigger
        << frame
        << scanline
        << rasterCycle
        << ultimax;

        if (isResetter(worker)) return;

        worker

        << durationOfOneCycle;
    }

    void operator << (SerResetter &worker) override;
    void operator << (SerChecker &worker) override { serialize(worker); }
    void operator << (SerCounter &worker) override { serialize(worker); }
    void operator << (SerReader &worker) override { serialize(worker); }
    void operator << (SerWriter &worker) override { serialize(worker); }

public:

    isize size();
    isize load(const u8 *buffer);
    isize save(u8 *buffer);


    //
    // Methods from Inspectable
    //

public:

    virtual void record() const override;
    bool autoInspect() const override;
    void recordState(C64Info &result) const override;

    EventSlotInfo getSlotInfo(isize nr) const;

private:

    void inspectSlot(EventSlot nr) const;


    //
    // Executing
    //

private:

    void execute();
    void execute(bool headless);
    void executeHeadless() { execute(true); }
    template <bool enable8, bool enable9> void execute();
    bool processFlags();

    // Experimental (for runahead)
    void fastForward(isize frames);


    //
    // Controlling
    //

private:

    void _isReady() const throws override;
    void _powerOn() override;
    void _powerOff() override;
    void _run() override;
    void _pause() override;
    void _halt() override;
    void _warpOn() override;
    void _warpOff() override;
    void _trackOn() override;
    void _trackOff() override;


    //
    // Running the emulator
    //

public:

    bool getUltimax() const { return ultimax; }
    void setUltimax(bool b) { ultimax = b; }

    /* Sets or clears a flag for controlling the run loop. The functions are
     * thread-safe and can be called safely from outside the emulator thread.
     */
    void setFlag(u32 flags);
    void clearFlag(u32 flags);
    
    // Convenience wrappers
    void signalBreakpoint() { setFlag(RL::BREAKPOINT); }
    void signalWatchpoint() { setFlag(RL::WATCHPOINT); }
    void signalJammed() { setFlag(RL::CPU_JAM); }
    void signalStop() { setFlag(RL::STOP); }
    void signalExpPortNmi() { setFlag(RL::EXTERNAL_NMI); }
    void signalBrk() { setFlag(RL::EXTERNAL_BRK); }

    // Runs or pauses the emulator
    void stopAndGo();

    /* Executes a single instruction. This function is used for single-stepping
     * through the code inside the debugger. It starts the execution thread and
     * terminates it after the next instruction has been executed.
     */
    void stepInto();
    
    /* Emulates the C64 until the instruction following the current one is
     * reached. This function is used for single-stepping through the code
     * inside the debugger. It sets a soft breakpoint to PC+n where n is the
     * length bytes of the current instruction and starts the emulator thread.
     */
    void stepOver();

    // Executes a single clock cycle.
    void executeOneCycle();

    /* Finishes the current instruction. This function is called when the
     * emulator threads terminates in order to reach a clean state. It emulates
     * the CPU until the next fetch cycle is reached.
     */
    void finishInstruction();
    
    // Finishes the current frame
    void finishFrame();
    
private:

    // Invoked after executing the last cycle of a scanline
    void endScanline();
    
    // Invoked after executing the last scanline of a frame
    void endFrame();
    

    //
    // Managing commands and events
    //

public:

    // Processes a command from the command queue
    void process(const Cmd &cmd);

    // Processes all pending events
    void processEvents(Cycle cycle);

    // Returns true iff the specified slot contains any event
    template<EventSlot s> bool hasEvent() const { return this->id[s] != (EventID)0; }

    // Returns true iff the specified slot contains a specific event
    template<EventSlot s> bool hasEvent(EventID id) const { return this->id[s] == id; }

    // Returns true iff the specified slot contains a pending event
    template<EventSlot s> bool isPending() const { return this->trigger[s] != NEVER; }

    // Returns true iff the specified slot contains a due event
    template<EventSlot s> bool isDue(Cycle cycle) const { return cycle >= this->trigger[s]; }

    // Schedules an event in certain ways
    template<EventSlot s> void scheduleAbs(Cycle cycle, EventID id)
    {
        this->trigger[s] = cycle;
        this->id[s] = id;

        if (cycle < nextTrigger) nextTrigger = cycle;

        if constexpr (isTertiarySlot(s)) {
            if (cycle < trigger[SLOT_TER]) trigger[SLOT_TER] = cycle;
            if (cycle < trigger[SLOT_SEC]) trigger[SLOT_SEC] = cycle;
        }
        if constexpr (isSecondarySlot(s)) {
            if (cycle < trigger[SLOT_SEC]) trigger[SLOT_SEC] = cycle;
        }
    }

    template<EventSlot s> void scheduleAbs(Cycle cycle, EventID id, i64 data)
    {
        scheduleAbs<s>(cycle, id);
        this->data[s] = data;
    }

    template<EventSlot s> void rescheduleAbs(Cycle cycle)
    {
        trigger[s] = cycle;
        if (cycle < nextTrigger) nextTrigger = cycle;

        if constexpr (isTertiarySlot(s)) {
            if (cycle < trigger[SLOT_TER]) trigger[SLOT_TER] = cycle;
        }
        if constexpr (isSecondarySlot(s)) {
            if (cycle < trigger[SLOT_SEC]) trigger[SLOT_SEC] = cycle;
        }
    }

    template<EventSlot s> void scheduleImm(EventID id)
    {
        scheduleAbs<s>(cpu.clock, id);
    }

    template<EventSlot s> void scheduleImm(EventID id, i64 data)
    {
        scheduleAbs<s>(cpu.clock, id);
        this->data[s] = data;
    }

    template<EventSlot s> void scheduleRel(Cycle cycle, EventID id) {
        scheduleAbs<s>(cpu.clock + cycle, id);
    }

    template<EventSlot s> void scheduleRel(Cycle cycle, EventID id, i64 data) {
        scheduleAbs<s>(cpu.clock + cycle, id, data);
    }

    template<EventSlot s> void rescheduleRel(Cycle cycle) {
        rescheduleAbs<s>(cpu.clock + cycle);
    }

    template<EventSlot s> void scheduleInc(Cycle cycle, EventID id)
    {
        scheduleAbs<s>(trigger[s] + cycle, id);
    }

    template<EventSlot s> void scheduleInc(Cycle cycle, EventID id, i64 data)
    {
        scheduleAbs<s>(trigger[s] + cycle, id);
        this->data[s] = data;
    }

    template<EventSlot s> void rescheduleInc(Cycle cycle)
    {
        rescheduleAbs<s>(trigger[s] + cycle);
    }

    template<EventSlot s> void cancel()
    {
        id[s] = (EventID)0;
        data[s] = 0;
        trigger[s] = NEVER;
    }

private:

    // Services an inspection event
    void processINSEvent(EventID id);


    //
    // Handling snapshots
    //
    
public:

    // Returns the most recent snapshot or nullptr if none was taken
    Snapshot *latestAutoSnapshot();
    Snapshot *latestUserSnapshot();
    
    // Loads the current state from a snapshot file
    void loadSnapshot(const Snapshot &snapshot) throws;
    
    
    //
    // Handling Roms
    //
    
public:
    
    // Queries information about the installed ROMs
    RomInfo getRomInfo(RomType type) const;

    // Computes a Rom checksum
    u32 romCRC32(RomType type) const;
    u64 romFNV64(RomType type) const;

    // Returns a unique identifier for the installed ROMs
    RomIdentifier romIdentifier(RomType type) const;
    
    // Returns printable titles for the installed ROMs
    const char *romTitle(RomType type) const;
    
    // Returns printable sub titles for the installed ROMs
    const char *romSubTitle(u64 fnv) const;
    const char *romSubTitle(RomType type) const;

    // Returns printable revision strings or hash values for the installed ROMs
    const char *romRevision(RomType type) const;
    
    // Checks if a certain Rom is present
    bool hasRom(RomType type) const;
    bool hasMega65Rom(RomType type) const;

private:
    
    // Returns a revision string if a Mega65 Rom is installed
    const char *mega65BasicRev() const;
    const char *mega65KernalRev() const;

public:
    
    // Installs a Rom
    void loadRom(const string &path) throws;
    void loadRom(const RomFile &file);
    
    // Erases an installed Rom
    void deleteRom(RomType type);
    
    // Saves a Rom to disk
    void saveRom(RomType rom, const string &path) throws;

    
    //
    // Flashing files
    //
    
    // Flashes a single file into memory
    void flash(const AnyFile &file) throws;
    void flash(const AnyCollection &file, isize item) throws;
    void flash(const FileSystem &fs, isize item) throws;


    //
    // Handling alarms
    //

public:

    /* Alarms are scheduled notifications set by the client (GUI). Once the
     * trigger cycle of an alarm has been reached, the emulator sends a
     * MSG_ALARM to the client.
     */
    void setAlarmAbs(Cycle trigger, i64 payload);
    void setAlarmRel(Cycle trigger, i64 payload);

    // Services an alarm event
    void processAlarmEvent();

private:

    // Schedules the next alarm event
    void scheduleNextAlarm();

    
    //
    // Miscellaneous
    //

public:

    // Gets or sets an internal debug variable (only available in debug builds)
    static bool getDebugVariable(DebugFlag flag);
    static void setDebugVariable(DebugFlag flag, bool val);
};

}
