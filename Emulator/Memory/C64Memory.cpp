// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "C64Memory.h"
#include "C64.h"
#include "IO.h"

#include <random>

#define CHECK_WATCHPOINT(x) \
if (checkWatchpoints && cpu.debugger.watchpointMatches(x)) { \
    c64.signalWatchpoint(); \
}

C64Memory::C64Memory(C64 &ref) : C64Component(ref)
{    		
    memset(rom, 0, sizeof(rom));

    /* Memory bank map
     *
     * If x == (EXROM, GAME, CHAREN, HIRAM, LORAM) then
     *
     *   map[x][0] == mapping for range $1000 - $7FFF
     *   map[x][1] == mapping for range $8000 - $9FFF
     *   map[x][2] == mapping for range $A000 - $BFFF
     *   map[x][3] == mapping for range $C000 - $CFFF
     *   map[x][4] == mapping for range $D000 - $DFFF
     *   map[x][5] == mapping for range $E000 - $FFFF
     */
    MemoryType map[32][6] = {
        
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_RAM,  M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_RAM,  M_RAM    },
        { M_RAM,  M_RAM,   M_CRTHI, M_RAM,  M_CHAR, M_KERNAL },
        { M_RAM,  M_CRTLO, M_CRTHI, M_RAM,  M_CHAR, M_KERNAL },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_RAM,  M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_IO,   M_RAM    },
        { M_RAM,  M_RAM,   M_CRTHI, M_RAM,  M_IO,   M_KERNAL },
        { M_RAM,  M_CRTLO, M_CRTHI, M_RAM,  M_IO,   M_KERNAL },
        
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_RAM,  M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_CHAR, M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_CHAR, M_KERNAL },
        { M_RAM,  M_CRTLO, M_BASIC, M_RAM,  M_CHAR, M_KERNAL },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_RAM,  M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_IO,   M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_IO,   M_KERNAL },
        { M_RAM,  M_CRTLO, M_BASIC, M_RAM,  M_IO,   M_KERNAL },
        
        { M_NONE, M_CRTLO, M_NONE,  M_NONE, M_IO,   M_CRTHI  },
        { M_NONE, M_CRTLO, M_NONE,  M_NONE, M_IO,   M_CRTHI  },
        { M_NONE, M_CRTLO, M_NONE,  M_NONE, M_IO,   M_CRTHI  },
        { M_NONE, M_CRTLO, M_NONE,  M_NONE, M_IO,   M_CRTHI  },
        { M_NONE, M_CRTLO, M_NONE,  M_NONE, M_IO,   M_CRTHI  },
        { M_NONE, M_CRTLO, M_NONE,  M_NONE, M_IO,   M_CRTHI  },
        { M_NONE, M_CRTLO, M_NONE,  M_NONE, M_IO,   M_CRTHI  },
        { M_NONE, M_CRTLO, M_NONE,  M_NONE, M_IO,   M_CRTHI  },
        
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_RAM,  M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_CHAR, M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_CHAR, M_KERNAL },
        { M_RAM,  M_RAM,   M_BASIC, M_RAM,  M_CHAR, M_KERNAL },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_RAM,  M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_IO,   M_RAM    },
        { M_RAM,  M_RAM,   M_RAM,   M_RAM,  M_IO,   M_KERNAL },
        { M_RAM,  M_RAM,   M_BASIC, M_RAM,  M_IO,   M_KERNAL }
    };
    
    for (unsigned i = 0; i < 32; i++) {
        
        bankMap[i][0x0] = M_PP;
        bankMap[i][0x1] = map[i][0];
        bankMap[i][0x2] = map[i][0];
        bankMap[i][0x3] = map[i][0];
        bankMap[i][0x4] = map[i][0];
        bankMap[i][0x5] = map[i][0];
        bankMap[i][0x6] = map[i][0];
        bankMap[i][0x7] = map[i][0];
        bankMap[i][0x8] = map[i][1];
        bankMap[i][0x9] = map[i][1];
        bankMap[i][0xA] = map[i][2];
        bankMap[i][0xB] = map[i][2];
        bankMap[i][0xC] = map[i][3];
        bankMap[i][0xD] = map[i][4];
        bankMap[i][0xE] = map[i][5];
        bankMap[i][0xF] = map[i][5];
    }
    
    // Initialize peekSource and pokeTarket tables
    peekSrc[0x0] = pokeTarget[0x0] = M_PP;
    for (unsigned i = 0x1; i <= 0xF; i++) {
        peekSrc[i] = pokeTarget[i] = M_RAM;
    }
}

void
C64Memory::_reset(bool hard)
{
    RESET_SNAPSHOT_ITEMS(hard)
    
    if (hard) {
        
        // Erase RAM
        eraseWithPattern(config.ramPattern);
        
        // Initialize color RAM with random numbers
        srand(1000);
        for (unsigned i = 0; i < sizeof(colorRam); i++) {
            colorRam[i] = (rand() & 0xFF);
        }
    }
}

MemConfig
C64Memory::getDefaultConfig()
{
    MemConfig defaults;
    
    defaults.ramPattern = RAM_PATTERN_VICE;
    
    return defaults;
}

void
C64Memory::resetConfig()
{
    MemConfig defaults = getDefaultConfig();
    
    setConfigItem(OPT_RAM_PATTERN, defaults.ramPattern);
}

i64
C64Memory::getConfigItem(Option option) const
{
    switch (option) {
            
        case OPT_RAM_PATTERN:  return config.ramPattern;
            
        default:
            assert(false);
            return 0;
    }
}

void
C64Memory::setConfigItem(Option option, i64 value)
{
    switch (option) {
            
        case OPT_RAM_PATTERN:
            
            if (!RamPatternEnum::isValid(value)) {
                throw VC64Error(ERROR_OPT_INV_ARG, RamPatternEnum::keyList());
            }
            
            config.ramPattern = (RamPattern)value;
            return;
                        
        default:
            return;
    }
}

void
C64Memory::_inspect()
{
    synchronized {
        
        info.exrom = expansionport.getExromLine();
        info.game = expansionport.getGameLine();
        info.loram = cpu.pport.getLoram();
        info.hiram = cpu.pport.getHiram();
        info.charen = cpu.pport.getCharen();
        
        info.bankMap = cpu.pport.read();
        if (expansionport.getGameLine()) info.bankMap |= 0x08;
        if (expansionport.getExromLine()) info.bankMap |= 0x10;
        
        for (int i = 0; i < 16; i++) info.peekSrc[i] = peekSrc[i];
        for (int i = 0; i < 16; i++) info.vicPeekSrc[i] = vic.memSrc[i];
    }
}

void
C64Memory::_dump(dump::Category category, std::ostream& os) const
{
    using namespace util;
    
    if (category & dump::Config) {
    
        os << tab("Ram pattern");
        os << RamPatternEnum::key(config.ramPattern) << std::endl;
    }
    
    if (category & dump::State) {
        
        os << tab("Basic ROM");
        os << bol(c64.hasRom(ROM_TYPE_BASIC)) << std::endl;
        os << tab("Character ROM");
        os << bol(c64.hasRom(ROM_TYPE_CHAR)) << std::endl;
        os << tab("Kernal ROM");
        os << bol(c64.hasRom(ROM_TYPE_KERNAL)) << std::endl;
    }
}
    
void
C64Memory::eraseWithPattern(RamPattern pattern)
{
    /* Note: The RAM init pattern is not unique across C64 models (for details,
     * see the README file in the VICE test suite C64/raminitpattern). By
     * default, VirtualC64 utilizes the same patters as VICE. This pattern has
     * been selected, because it is pretty close to what can be seen on most
     * real machines and it make all four tests from the VICE test suite pass
     * (cyberloadtest.prg, darkstarbbstest.prg, platoontest.prg, and
     * typicaltet.prg). The CCS scheme is the one that is used by CCS 3.9. Note
     * that the darkstarbbstest fails with this pattern. The remainung patterns
     * can't be found in the wild. They allow the user to initialize the RAM
     * with all zeroes, all ones, or random values, respectively.
     */
    
    switch (pattern) {
            
        case RAM_PATTERN_VICE:
        
            // $00 $00 $FF $FF $FF $FF $00 $00 ...
            for (isize i = 0; i < isizeof(ram); i++)
                ram[i] = (i & 0x6) == 0x2 || (i & 0x6) == 0x4 ? 0xFF : 0x00;

            // In addition, the 2nd and 3rd 16K bank are inverted
            for (isize i = 0; i < isizeof(ram); i++)
                ram[i] ^= (i & 0x4000) ? 0xFF : 0x00;
            
            break;
        
        case RAM_PATTERN_CCS:
        
            // (64 x $FF) (64 x $00) ...
            for (isize i = 0; i < isizeof(ram); i++)
                ram[i] = (i & 0x40) ? 0x00 : 0xFF;
            
            break;
        
        case RAM_PATTERN_ZEROES:
        
            for (isize i = 0; i < isizeof(ram); i++)
                ram[i] = 0;
            
            break;

        case RAM_PATTERN_ONES:
        
            for (isize i = 0; i < isizeof(ram); i++)
                ram[i] = 0xFF;
            
            break;

        case RAM_PATTERN_RANDOM:
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, 0xFF);
         
            for (isize i = 0; i < isizeof(ram); i++)
                ram[i] = distrib(gen);
        
            break;
        }
        default:
            assert(false);
    }
}

void 
C64Memory::updatePeekPokeLookupTables()
{
    // Read game line, exrom line, and processor port bits
    u8 game  = expansionport.getGameLine() ? 0x08 : 0x00;
    u8 exrom = expansionport.getExromLine() ? 0x10 : 0x00;
    u8 index = (cpu.pport.read() & 0x07) | exrom | game;

    // Set ultimax flag
    c64.setUltimax(exrom && !game);

    // Update table entries
    for (unsigned bank = 1; bank < 16; bank++) {
        peekSrc[bank] = pokeTarget[bank] = bankMap[index][bank];
    }
    
    // Call the Cartridge's delegation method
    expansionport.updatePeekPokeLookupTables();
}

u8
C64Memory::peek(u16 addr, MemoryType source)
{
    CHECK_WATCHPOINT(addr)
    
    switch(source) {
        
        case M_RAM:
        return ram[addr];
        
        case M_BASIC:
        case M_CHAR:
        case M_KERNAL:
        return rom[addr];
        
        case M_IO:
        return peekIO(addr);
        
        case M_CRTLO:
        case M_CRTHI:
        return expansionport.peek(addr);
        
        case M_PP:
        if (likely(addr >= 0x02)) {
            return ram[addr];
        } else if (addr == 0x00) {
            return cpu.pport.readDirection();
        } else {
            return cpu.pport.read();
        }
        
        case M_NONE:
        return vic.getDataBusPhi1();
        
        default:
        assert(false);
        return 0;
    }
}

u8
C64Memory::peek(u16 addr, bool gameLine, bool exromLine)
{
    u8 game  = gameLine ? 0x08 : 0x00;
    u8 exrom = exromLine ? 0x10 : 0x00;
    u8 index = (cpu.pport.read() & 0x07) | exrom | game;
    
    return peek(addr, bankMap[index][addr >> 12]);
}

u8
C64Memory::peekZP(u8 addr)
{
    CHECK_WATCHPOINT(addr)
    
    if (likely(addr >= 0x02)) {
        return ram[addr];
    } else if (addr == 0x00) {
        return cpu.pport.readDirection();
    } else {
        return cpu.pport.read();
    }
}

u8
C64Memory::peekStack(u8 sp)
{
    CHECK_WATCHPOINT(sp)
    
    return ram[0x100 + sp];
}

u8
C64Memory::peekIO(u16 addr)
{
    CHECK_WATCHPOINT(addr)
    
    assert(addr >= 0xD000 && addr <= 0xDFFF);
    
    switch ((addr >> 8) & 0xF) {
            
        case 0x0: // VICII
        case 0x1: // VICII
        case 0x2: // VICII
        case 0x3: // VICII
            
            // Only the lower 6 bits are used for adressing the VIC I/O space.
            // As a result, VICII's I/O memory repeats every 64 bytes.
            return vic.peek(addr & 0x003F);

        case 0x4: // SID
        case 0x5: // SID
        case 0x6: // SID
        case 0x7: // SID
            
            return sid.peek(addr);

        case 0x8: // Color RAM
        case 0x9: // Color RAM
        case 0xA: // Color RAM
        case 0xB: // Color RAM
 
            return (colorRam[addr - 0xD800] & 0x0F) | (vic.getDataBusPhi1() & 0xF0);

        case 0xC: // CIA 1
 
            // Only the lower 4 bits are used for adressing the CIA I/O space.
            // As a result, CIA's I/O memory repeats every 16 bytes.
            return cia1.peek(addr & 0x000F);
	
        case 0xD: // CIA 2
            
            return cia2.peek(addr & 0x000F);
            
        case 0xE: // I/O space 1
            
            return expansionport.peekIO1(addr);
            
        case 0xF: // I/O space 2

            return expansionport.peekIO2(addr);
	}
    
	assert(false);
	return 0;
}

u8
C64Memory::spypeek(u16 addr, MemoryType source) const
{
    switch(source) {
            
        case M_RAM:
            return ram[addr];
            
        case M_BASIC:
        case M_CHAR:
        case M_KERNAL:
            return rom[addr];
            
        case M_IO:
            return spypeekIO(addr);
            
        case M_CRTLO:
        case M_CRTHI:
            return expansionport.spypeek(addr);
            
        case M_PP:
            if (addr >= 0x02) {
                return ram[addr];
            } else if (addr == 0x00) {
                return cpu.pport.readDirection();
            } else {
                return cpu.pport.read();
            }
      
        case M_NONE:
            return ram[addr];
            
        default:
            assert(false);
            return 0;
    }
}

u8
C64Memory::spypeekIO(u16 addr) const
{
    assert(addr >= 0xD000 && addr <= 0xDFFF);
    
    switch ((addr >> 8) & 0xF) {
            
        case 0x0: // VICII
        case 0x1: // VICII
        case 0x2: // VICII
        case 0x3: // VICII
            
            return vic.spypeek(addr & 0x003F);
            
        case 0x4: // SID
        case 0x5: // SID
        case 0x6: // SID
        case 0x7: // SID
            
            return sid.spypeek(addr & 0x001F);
            
        case 0x8: // Color Ram
        case 0x9: // Color Ram
        case 0xA: // Color Ram
        case 0xB: // Color Ram
            return spypeekColor(addr - 0xD8000);

        case 0xC: // CIA 1
            
            // Only the lower 4 bits are used for adressing the CIA I/O space.
            // As a result, CIA's I/O memory repeats every 16 bytes.
            return cia1.spypeek(addr & 0x000F);
            
        case 0xD: // CIA 2
            
            return cia2.spypeek(addr & 0x000F);
            
        case 0xE: // I/O space 1
            
            return expansionport.spypeekIO1(addr);
            
        case 0xF: // I/O space 2
            
            return expansionport.spypeekIO2(addr);

        default:
            assert(false);
            return 0;
    }
}

u8
C64Memory::spypeekColor(u16 addr) const
{
    assert(addr <= 0x400);
    return colorRam[addr];
}

void
C64Memory::poke(u16 addr, u8 value, MemoryType target)
{
    CHECK_WATCHPOINT(addr)
    
    switch(target) {
            
        case M_RAM:
        case M_BASIC:
        case M_CHAR:
        case M_KERNAL:
            ram[addr] = value;
            return;
            
        case M_IO:
            pokeIO(addr, value);
            return;
            
        case M_CRTLO:
        case M_CRTHI:
            expansionport.poke(addr, value);
            return;
            
        case M_PP:
            if (likely(addr >= 0x02)) {
                ram[addr] = value;
            } else if (addr == 0x00) {
                cpu.pport.writeDirection(value);
            } else {
                cpu.pport.write(value);
            }
            return;
            
        case M_NONE:
            return;
            
        default:
            assert(false);
            return;
    }
}

void
C64Memory::poke(u16 addr, u8 value, bool gameLine, bool exromLine)
{
    u8 game  = gameLine ? 0x08 : 0x00;
    u8 exrom = exromLine ? 0x10 : 0x00;
    u8 index = (cpu.pport.read() & 0x07) | exrom | game;
    
    poke(addr, value, bankMap[index][addr >> 12]);
}

void
C64Memory::pokeZP(u8 addr, u8 value)
{
    CHECK_WATCHPOINT(addr)
    
    if (likely(addr >= 0x02)) {
        ram[addr] = value;
    } else if (addr == 0x00) {
        cpu.pport.writeDirection(value);
    } else {
        cpu.pport.write(value);
    }
}

void
C64Memory::pokeStack(u8 sp, u8 value)
{
    CHECK_WATCHPOINT(sp)
    
    ram[0x100 + sp] = value;
}

void
C64Memory::pokeIO(u16 addr, u8 value)
{
    CHECK_WATCHPOINT(addr)
    
    assert(addr >= 0xD000 && addr <= 0xDFFF);
    
    switch ((addr >> 8) & 0xF) {
            
        case 0x0: // VICII
        case 0x1: // VICII
        case 0x2: // VICII
        case 0x3: // VICII
            
            // Only the lower 6 bits are used for adressing the VICII I/O space.
            // As a result, VICII's I/O memory repeats every 64 bytes.
            vic.poke(addr & 0x003F, value);
            return;
            
        case 0x4: // SID
        case 0x5: // SID
        case 0x6: // SID
        case 0x7: // SID
            
            sid.poke(addr, value);

            // Check the error register (debugcart feature)
            if (addr == 0xD7FF) c64.regressionTester.debugcart(value);
            return;
            
        case 0x8: // Color RAM
        case 0x9: // Color RAM
        case 0xA: // Color RAM
        case 0xB: // Color RAM
            
            colorRam[addr - 0xD800] = (value & 0x0F) | (rand() & 0xF0);
            return;
            
        case 0xC: // CIA 1
            
            // Only the lower 4 bits are used for adressing the CIA I/O space.
            // As a result, CIA's I/O memory repeats every 16 bytes.
            cia1.poke(addr & 0x000F, value);
            return;
            
        case 0xD: // CIA 2
            
            cia2.poke(addr & 0x000F, value);
            return;
            
        case 0xE: // I/O space 1
            
            expansionport.pokeIO1(addr, value);
            return;
            
        case 0xF: // I/O space 2
            
            expansionport.pokeIO2(addr, value);
            return;
    }
    
    assert(false);
}

u16
C64Memory::nmiVector() {
    
    if (peekSrc[0xF] != M_KERNAL || c64.hasRom(ROM_TYPE_KERNAL)) {
        return LO_HI(peek(0xFFFA), peek(0xFFFB));
    } else {
        return 0xFE43;
    }
}

u16
C64Memory::irqVector() {
    
    if (peekSrc[0xF] != M_KERNAL || c64.hasRom(ROM_TYPE_KERNAL)) {
        return LO_HI(peek(0xFFFE), peek(0xFFFF));
    } else {
        return 0xFF48;
    }
}

u16
C64Memory::resetVector() {
    
    if (peekSrc[0xF] != M_KERNAL || c64.hasRom(ROM_TYPE_KERNAL)) {
        return LO_HI(peek(0xFFFC), peek(0xFFFD));
    } else {
        return 0xFCE2;
    }
}

char *
C64Memory::memdump(u16 addr, long num, bool hex, MemoryType src)
{
    static char result[128];
    char *p = result;
    
    assert(num <= 16);

    if (src == M_NONE) {
        
        for (int i = 0; i < num; i++) {
            if (hex) {
                *p++ = ' '; *p++ = ' '; *p++ = '-'; *p++ = '-';
            } else {
                *p++ = ' '; *p++ = '-'; *p++ = '-'; *p++ = '-';
            }
        }
        *p = 0;

    } else {
        
        for (int i = 0; i < num; i++) {
            if (hex) {
                *p++ = ' ';
                *p++ = ' ';
                util::sprint8x(p, spypeek(addr++, src));
                p += 2;
            } else {
                *p++ = ' ';
                util::sprint8d(p, spypeek(addr++, src));
                p += 3;
            }
        }
    }
    
    return result;
}

char *
C64Memory::txtdump(u16 addr, long num, MemoryType src)
{
    static char result[17];
    char *p = result;
    
    assert(num <= 16);
    
    if (src != M_NONE) {
        
        for (int i = 0; i < num; i++) {
            
            u8 byte = spypeek(addr++, src);
            
            // if (byte >=  1 && byte <= 26) { *p++ = (byte - 1) + 'A'; continue; }
            // if (byte >= 48 && byte <= 57) { *p++ = (byte - 48) + '0'; continue; }
            if (byte >= 65 && byte <= 90) { *p++ = byte; continue; }
            if (byte >= 32 && byte <= 57) { *p++ = byte; continue; }
            *p++ = '.';
        }
    }
    *p = 0;

    return result;
}
