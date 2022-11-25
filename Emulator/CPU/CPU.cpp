// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "PeddleConfig.h"
#include "CPU.h"
#include "C64.h"
#include "IOUtils.h"

CPU::CPU(C64& ref) : Peddle(ref)
{
    subComponents = std::vector<C64Component *> {
        
        &pport
    };    
}

CPU::CPU(CPURevision cpuModel, C64& ref) : CPU(ref)
{
    setModel(cpuModel);
}

const char *
CPU::getDescription() const
{
    switch (id) {

        case 0: return "CPU";
        case 1: return "CPU(8)";
        case 2: return "CPU(9)";

        default:
            return "???";
    }
}

void
CPU::_reset(bool hard)
{
    RESET_SNAPSHOT_ITEMS(hard)
    
    setB(1);
    setI(1);
	rdyLine = true;
	next = fetch;
    
    assert(levelDetector.isClear());
    assert(edgeDetector.isClear());
}

void
CPU::_inspect() const
{    
    {   SYNCHRONIZED
        
        info.cycle = clock;

        info.pc0 = reg.pc0;
        info.sp = reg.sp;
        info.a = reg.a;
        info.x = reg.x;
        info.y = reg.y;
        info.sr = getSr();

        info.irq = irqLine;
        info.nmi = nmiLine;
        info.rdy = rdyLine;
        info.jammed = isJammed();
        
        info.processorPort = pport.read();
        info.processorPortDir = pport.readDirection();
    }
}

void
CPU::_debugOn()
{
    // We only allow the C64 CPU to enter debug mode
    if (!isC64CPU()) return;

    debug(RUN_DEBUG, "Enabling debug mode\n");
    debugger.enableLogging();
}

void
CPU::_debugOff()
{
    debug(RUN_DEBUG, "Disabling debug mode\n");
    debugger.disableLogging();
}

void
CPU::_dump(Category category, std::ostream& os) const
{
    using namespace util;

    if (category == Category::Registers) {

        os << tab("PC") << hex(reg.pc) << std::endl;
        os << tab("SP") << hex(reg.sp) << std::endl;
        os << tab("A") << hex(reg.a) << std::endl;
        os << tab("X") << hex(reg.x) << std::endl;
        os << tab("Y") << hex(reg.y) << std::endl;
        os << tab("Flags");
        os << (reg.sr.n ? "N" : "n");
        os << (reg.sr.v ? "V" : "v");
        os << (reg.sr.b ? "B" : "b");
        os << (reg.sr.d ? "D" : "d");
        os << (reg.sr.i ? "I" : "i");
        os << (reg.sr.z ? "Z" : "z");
        os << (reg.sr.c ? "C" : "c");
        os << std::endl;
    }
    
    if (category == Category::State) {

        auto append = [&](const string &s1, const string &s2) {
            return s1.empty() ? s2 : s1 + ", " + s2;
        };

        string str = "";
        if (flags & CPU_LOG_INSTRUCTION) str = append(str, "LOG_INSTRUCTION");
        if (flags & CPU_CHECK_BP) str = append(str, "CHECK_BP");
        if (flags & CPU_CHECK_WP) str = append(str, "CHECK_WP");

        os << tab("Cycle");
        os << dec(clock) << std::endl;
        os << tab("Flags");
        os << (str.empty() ? "-" : str) << std::endl;
        os << tab("Rdy line");
        os << bol(rdyLine, "high", "low") << std::endl;
        os << tab("Nmi line");
        os << hex(nmiLine) << std::endl;
        os << tab("Edge detector");
        os << hex(edgeDetector.current()) << std::endl;
        os << tab("doNmi");
        os << bol(doNmi) << std::endl;
        os << tab("Irq line");
        os << hex(irqLine) << std::endl;
        os << tab("Edge detector");
        os << hex(levelDetector.current()) << std::endl;
        os << tab("doIrq");
        os << bol(doIrq) << std::endl;
        os << tab("IRQ routine");
        os << hex(HI_W_LO_W(read8Dasm(0xFFFF), read8Dasm(0xFFFE))) << std::endl;
        os << tab("NMI routine");
        os << hex(HI_W_LO_W(read8Dasm(0xFFFB), read8Dasm(0xFFFA))) << std::endl;
    }
}

u8
CPU::read8(u16 addr) const
{
    switch (id) {

        case 0: return mem.peek(addr);
        case 1: return drive8.mem.peek(addr);
        case 2: return drive9.mem.peek(addr);

        default:
            fatalError;
    }
}

u8
CPU::read8Reset(u16 addr) const
{
    return read8(addr);
}

u8
CPU::read8Dasm(u16 addr) const
{
    switch (id) {

        case 0: return mem.spypeek(addr);
        case 1: return drive8.mem.spypeek(addr);
        case 2: return drive9.mem.spypeek(addr);

        default:
            fatalError;
    }
}

void
CPU::write8(u16 addr, u8 val) const
{
    switch (id) {

        case 0: mem.poke(addr, val); break;
        case 1: drive8.mem.poke(addr, val); break;
        case 2: drive9.mem.poke(addr, val); break;

        default:
            fatalError;
    }
}

void
CPU::nmiWillTrigger()
{
    if (isC64CPU()) {
        expansionport.nmiWillTrigger();
    }
}

void
CPU::nmiDidTrigger()
{
    if (isC64CPU()) {
        expansionport.nmiDidTrigger();
    }
}

void
CPU::breakpointReached(u16 addr)
{
    c64.signalBreakpoint();
}

void
CPU::watchpointReached(u16 addr)
{

}

void
CPU::cpuDidJam()
{
    c64.signalJammed();
}

void
CPU::instructionDidFinish()
{
    // Perform an inspection from time to time
    c64.autoInspect();
}

namespace peddle {

/*
template <CPURevision C> u8
Peddle::peek(u16 addr)
{
    if constexpr (ENABLE_WATCHPOINTS) {
        if ((flags & CPU_CHECK_WP) && debugger.watchpointMatches(addr)) {
            c64.signalWatchpoint();
        }
    }
    
    switch (C) {

        case MOS_6510: return mem.peek(addr);
        case MOS_6502: return id == 1 ? drive8.mem.peek(addr) : drive9.mem.peek(addr);

        default:
            fatalError;
    }
}

template <CPURevision C> u8
Peddle::peekZP(u8 addr)
{
    if constexpr (ENABLE_WATCHPOINTS) {
        if ((flags & CPU_CHECK_WP) && debugger.watchpointMatches(addr)) {
            c64.signalWatchpoint();
        }
    }

    switch (C) {

        case MOS_6510: return mem.peekZP(addr);
        case MOS_6502: return id == 1 ? drive8.mem.peekZP(addr) : drive9.mem.peekZP(addr);

        default:
            fatalError;
    }
}

template <CPURevision C> u8
Peddle::peekStack(u8 sp)
{
    if constexpr (ENABLE_WATCHPOINTS) {
        if ((flags & CPU_CHECK_WP) && debugger.watchpointMatches((u16)sp + 0x100)) {
            c64.signalWatchpoint();
        }
    }

    switch (C) {

        case MOS_6510: return mem.peekStack(sp);
        case MOS_6502: return id == 1 ? drive8.mem.peekStack(sp) : drive9.mem.peekStack(sp);

        default:
            fatalError;
    }
}

u8
Peddle::spypeek(u16 addr) const
{
    switch (cpuModel) {

        case MOS_6502: return spypeek<MOS_6502>(addr);
        case MOS_6510: return spypeek<MOS_6510>(addr);

        default:
            fatalError;
    }
}

template <CPURevision C> u8
Peddle::spypeek(u16 addr) const
{
    switch (C) {

        case MOS_6510: return mem.spypeek(addr);
        case MOS_6502: return id == 1 ? drive8.mem.spypeek(addr) : drive9.mem.spypeek(addr);

        default:
            fatalError;
    }
}

template <CPURevision C> void
Peddle::poke(u16 addr, u8 value)
{
    if constexpr (ENABLE_WATCHPOINTS) {
        if ((flags & CPU_CHECK_WP) && debugger.watchpointMatches(addr)) {
            c64.signalWatchpoint();
        }
    }

    switch (C) {

        case MOS_6510: mem.poke(addr, value); break;
        case MOS_6502: id == 1 ? drive8.mem.poke(addr, value) : drive9.mem.poke(addr, value); break;

        default:
            fatalError;
    }
}

template <CPURevision C> void
Peddle::pokeZP(u8 addr, u8 value)
{
    if constexpr (ENABLE_WATCHPOINTS) {
        if ((flags & CPU_CHECK_WP) && debugger.watchpointMatches(addr)) {
            c64.signalWatchpoint();
        }
    }

    switch (C) {

        case MOS_6510: mem.pokeZP(addr, value); break;
        case MOS_6502: id == 1 ? drive8.mem.pokeZP(addr, value) : drive9.mem.pokeZP(addr, value); break;

        default:
            fatalError;
    }
}

template <CPURevision C> void
Peddle::pokeStack(u8 addr, u8 value)
{
    if constexpr (ENABLE_WATCHPOINTS) {
        if ((flags & CPU_CHECK_WP) && debugger.watchpointMatches((u16)addr + 0x100)) {
            c64.signalWatchpoint();
        }
    }

    switch (C) {

        case MOS_6510: mem.pokeStack(addr, value); break;
        case MOS_6502: id == 1 ? drive8.mem.pokeStack(addr, value) : drive9.mem.pokeStack(addr, value); break;

        default:
            fatalError;
    }
}
*/

/*
// template u8 Peddle::peek<MOS_6502>(u16 addr);
template u8 Peddle::peekZP<MOS_6502>(u8 addr);
template u8 Peddle::peekStack<MOS_6502>(u8 sp);
template u8 Peddle::spypeek<MOS_6502>(u16 addr) const;
template void Peddle::peekIdle<MOS_6502>(u16 addr);
template void Peddle::peekZPIdle<MOS_6502>(u8 addr);
template void Peddle::peekStackIdle<MOS_6502>(u8 sp);
template void Peddle::poke<MOS_6502>(u16 addr, u8 value);
template void Peddle::pokeZP<MOS_6502>(u8 addr, u8 value);
template void Peddle::pokeStack<MOS_6502>(u8 sp, u8 value);

// template u8 Peddle::peek<MOS_6510>(u16 addr);
template u8 Peddle::peekZP<MOS_6510>(u8 addr);
template u8 Peddle::peekStack<MOS_6510>(u8 sp);
template u8 Peddle::spypeek<MOS_6510>(u16 addr) const;
template void Peddle::peekIdle<MOS_6510>(u16 addr);
template void Peddle::peekZPIdle<MOS_6510>(u8 addr);
template void Peddle::peekStackIdle<MOS_6510>(u8 sp);
template void Peddle::poke<MOS_6510>(u16 addr, u8 value);
template void Peddle::pokeZP<MOS_6510>(u8 addr, u8 value);
template void Peddle::pokeStack<MOS_6510>(u8 sp, u8 value);
*/

}
