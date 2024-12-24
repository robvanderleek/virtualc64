## CodeLimit Report

### Overview
| **Language** | **Files** | **Lines of Code** | **Functions** | **⚠** | **❌** |
| --- | ---: | ---: | ---: | ---: | ---: |
| C++ | 178 | 19472 | 1640 | 62 | 36 |
| C | 221 | 7553 | 806 | 12 | 21 |
| JavaScript | 5 | 225 | 17 | 1 | 0 |
| **Totals** | **404** | **27250** | **2463** | **75** | **57** |

### Summary
| **Easy / Verbose** | **Hard-to-maintain ⚠** | **Unmaintainable ❌** |
| ---: | ---: | ---: |
| 59% | 12% | 29% |

🛑 29% of the functions are unmaintainable, refactoring necessary.

### Findings
| **Function** | **Length** | **File** |
| --- | ---: | --- |
| ❌ [execute](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CPU/Peddle/PeddleExec_cpp.h#L363-L3093) | 1485 | Emulator/Components/CPU/Peddle/PeddleExec_cpp.h |
| ❌ [output](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/filter.h#L736-L1290) | 528 | Emulator/Components/SID/resid/filter.h |
| ❌ [DebugConsole::initCommands](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Misc/RetroShell/DebugConsole.cpp#L90-L587) | 338 | Emulator/Misc/RetroShell/DebugConsole.cpp |
| ❌ [CommandConsole::initCommands](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Misc/RetroShell/CommandConsole.cpp#L87-L719) | 338 | Emulator/Misc/RetroShell/CommandConsole.cpp |
| ❌ [enum_long](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CPU/Peddle/PeddleTypes.h#L81-L373) | 240 | Emulator/Components/CPU/Peddle/PeddleTypes.h |
| ❌ [CIA::poke](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CIA/CIARegs.cpp#L213-L510) | 198 | Emulator/Components/CIA/CIARegs.cpp |
| ❌ [Error](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/Error.cpp#L19-L260) | 194 | Emulator/Base/Error.cpp |
| ❌ [Peddle::registerLegalInstructions](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CPU/Peddle/PeddleInit_cpp.h#L34-L219) | 154 | Emulator/Components/CPU/Peddle/PeddleInit_cpp.h |
| ❌ [C64::eventName](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64.cpp#L45-L235) | 139 | Emulator/Components/C64.cpp |
| ❌ [Defaults](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/Defaults.cpp#L21-L184) | 139 | Emulator/Base/Defaults.cpp |
| ❌ [create](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/Option.cpp#L22-L181) | 136 | Emulator/Base/Option.cpp |
| ❌ [_key](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/OptionTypes.h#L210-L365) | 132 | Emulator/Base/OptionTypes.h |
| ❌ [help](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/OptionTypes.h#L367-L521) | 132 | Emulator/Base/OptionTypes.h |
| ❌ [Monitor::getColor](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Monitor/Monitor.cpp#L40-L250) | 132 | Emulator/Peripherals/Monitor/Monitor.cpp |
| ❌ [enum_long](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/OptionTypes.h#L25-L201) | 129 | Emulator/Base/OptionTypes.h |
| ❌ [Filter](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/filter.cc#L198-L433) | 127 | Emulator/Components/SID/resid/filter.cc |
| ❌ [executeOneCycle](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CIA/CIA.cpp#L168-L503) | 126 | Emulator/Components/CIA/CIA.cpp |
| ❌ [VICII::peek](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIIMemory.cpp#L126-L305) | 122 | Emulator/Components/VICII/VICIIMemory.cpp |
| ❌ [VICII::poke](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIIMemory.cpp#L455-L616) | 115 | Emulator/Components/VICII/VICIIMemory.cpp |
| ❌ [clock](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/filter.h#L580-L709) | 104 | Emulator/Components/SID/resid/filter.h |
| ❌ [update](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64.cpp#L603-L736) | 99 | Emulator/Components/C64.cpp |
| ❌ [poke](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/VIA.cpp#L478-L646) | 99 | Emulator/Peripherals/Drive/VIA.cpp |
| ❌ [Peddle::registerIllegalInstructions](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CPU/Peddle/PeddleInit_cpp.h#L222-L335) | 96 | Emulator/Components/CPU/Peddle/PeddleInit_cpp.h |
| ❌ [translate](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Keyboard/C64Key.cpp#L152-L251) | 94 | Emulator/Peripherals/Keyboard/C64Key.cpp |
| ❌ [VICII::drawCanvasFastPath](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIIDraw.cpp#L75-L197) | 88 | Emulator/Components/VICII/VICIIDraw.cpp |
| ❌ [clock](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/filter.h#L476-L574) | 87 | Emulator/Components/SID/resid/filter.h |
| ❌ [poke](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/FlashRom.cpp#L193-L325) | 86 | Emulator/Media/Cartridges/FlashRom.cpp |
| ❌ [_key](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/CartridgeTypes.h#L117-L202) | 85 | Emulator/Media/Cartridges/CartridgeTypes.h |
| ❌ [set_sampling_parameters](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/sid.cc#L578-L710) | 84 | Emulator/Components/SID/resid/sid.cc |
| ❌ [CIA::peek](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CIA/CIARegs.cpp#L20-L145) | 83 | Emulator/Components/CIA/CIARegs.cpp |
| ❌ [write](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/sid.cc#L219-L303) | 83 | Emulator/Components/SID/resid/sid.cc |
| ❌ [serialize](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICII.h#L854-L942) | 82 | Emulator/Components/VICII/VICII.h |
| ❌ [enum_long](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/CartridgeTypes.h#L24-L108) | 82 | Emulator/Media/Cartridges/CartridgeTypes.h |
| ❌ [Monitor::setOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Monitor/MonitorBase.cpp#L130-L258) | 81 | Emulator/Peripherals/Monitor/MonitorBase.cpp |
| ❌ [encodeSector](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/Disk.cpp#L560-L679) | 80 | Emulator/Peripherals/Drive/Disk.cpp |
| ❌ [set](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64.cpp#L480-L573) | 78 | Emulator/Components/C64.cpp |
| ❌ [_key](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/ErrorTypes.h#L129-L219) | 76 | Emulator/Base/ErrorTypes.h |
| ❌ [MD5Transform](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Utilities/Checksum.cpp#L282-L367) | 74 | Emulator/Utilities/Checksum.cpp |
| ❌ [processEvents](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64.cpp#L1130-L1224) | 73 | Emulator/Components/C64.cpp |
| ❌ [enum_long](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/ErrorTypes.h#L21-L120) | 73 | Emulator/Base/ErrorTypes.h |
| ❌ [startRecording](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Misc/Recorder/Recorder.cpp#L167-L312) | 70 | Emulator/Misc/Recorder/Recorder.cpp |
| ❌ [press](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Misc/RetroShell/Console.cpp#L179-L283) | 69 | Emulator/Misc/RetroShell/Console.cpp |
| ❌ [poke](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/PIA.cpp#L233-L342) | 69 | Emulator/Peripherals/Drive/PIA.cpp |
| ❌ [getDebugVariable](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/Emulator.cpp#L321-L408) | 67 | Emulator/Components/Emulator.cpp |
| ❌ [setDebugVariable](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/Emulator.cpp#L411-L497) | 67 | Emulator/Components/Emulator.cpp |
| ❌ [initCommands](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Misc/RetroShell/Console.cpp#L758-L851) | 67 | Emulator/Misc/RetroShell/Console.cpp |
| ❌ [_key](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/MsgQueueTypes.h#L129-L211) | 67 | Emulator/Base/MsgQueueTypes.h |
| ❌ [pokeIO2](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/CustomCartridges/Reu.cpp#L235-L334) | 67 | Emulator/Media/Cartridges/CustomCartridges/Reu.cpp |
| ❌ [peek](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/VIA.cpp#L201-L314) | 67 | Emulator/Peripherals/Drive/VIA.cpp |
| ❌ [setOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/DriveBase.cpp#L262-L361) | 67 | Emulator/Peripherals/Drive/DriveBase.cpp |
| ❌ [clock](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/envelope.h#L203-L307) | 66 | Emulator/Components/SID/resid/envelope.h |
| ❌ [VirtualC64](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/VirtualC64.cpp#L51-L139) | 65 | Emulator/VirtualC64.cpp |
| ❌ [enum_long](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/MsgQueueTypes.h#L25-L119) | 65 | Emulator/Base/MsgQueueTypes.h |
| ❌ [_key](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/EmulatorTypes.h#L120-L208) | 64 | Emulator/Components/EmulatorTypes.h |
| ❌ [help](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/EmulatorTypes.h#L210-L298) | 63 | Emulator/Components/EmulatorTypes.h |
| ❌ [Thread::switchState](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/Thread.cpp#L147-L236) | 63 | Emulator/Base/Thread.cpp |
| ❌ [enum_long](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/EmulatorTypes.h#L26-L111) | 61 | Emulator/Components/EmulatorTypes.h |
| ⚠ [SubComponent](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/Memory/Memory.cpp#L22-L101) | 60 | Emulator/Components/Memory/Memory.cpp |
| ⚠ [analyzeHalftrack](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/DiskAnalyzer.cpp#L112-L193) | 59 | Emulator/Peripherals/Drive/DiskAnalyzer.cpp |
| ⚠ [execute](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/CustomCartridges/Reu.cpp#L477-L571) | 58 | Emulator/Media/Cartridges/CustomCartridges/Reu.cpp |
| ⚠ [init](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/G64File.cpp#L53-L126) | 56 | Emulator/Media/G64File.cpp |
| ⚠ [_key](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/CmdQueueTypes.h#L115-L184) | 55 | Emulator/Base/CmdQueueTypes.h |
| ⚠ [T64File::init](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/T64File.cpp#L52-L158) | 55 | Emulator/Media/T64File.cpp |
| ⚠ [drawSpritePixel](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIISprites.cpp#L235-L322) | 54 | Emulator/Components/VICII/VICIISprites.cpp |
| ⚠ [clock](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/sid.cc#L735-L822) | 54 | Emulator/Components/SID/resid/sid.cc |
| ⚠ [enum_long](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/CmdQueueTypes.h#L27-L106) | 54 | Emulator/Base/CmdQueueTypes.h |
| ⚠ [Monitor::checkOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Monitor/MonitorBase.cpp#L55-L127) | 54 | Emulator/Peripherals/Monitor/MonitorBase.cpp |
| ⚠ [read_state](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/sid.cc#L346-L406) | 53 | Emulator/Components/SID/resid/sid.cc |
| ⚠ [doDma](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/CustomCartridges/Reu.cpp#L621-L702) | 53 | Emulator/Media/Cartridges/CustomCartridges/Reu.cpp |
| ⚠ [clock](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/envelope.h#L118-L196) | 49 | Emulator/Components/SID/resid/envelope.h |
| ⚠ [enum_i8](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64Types.h#L155-L248) | 48 | Emulator/Components/C64Types.h |
| ⚠ [computeOverlay](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/DmaDebugger.cpp#L62-L127) | 48 | Emulator/Components/VICII/DmaDebugger.cpp |
| ⚠ [VICII::drawCanvasPixel](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIIDraw.cpp#L276-L362) | 48 | Emulator/Components/VICII/VICIIDraw.cpp |
| ⚠ [setOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/SIDBase.cpp#L158-L226) | 47 | Emulator/Components/SID/SIDBase.cpp |
| ⚠ [autoConfigure](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/DriveBase.cpp#L364-L422) | 46 | Emulator/Peripherals/Drive/DriveBase.cpp |
| ⚠ [solve_gain](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/filter.h#L1332-L1405) | 45 | Emulator/Components/SID/resid/filter.h |
| ⚠ [finalizeRead](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/D64File.cpp#L103-L163) | 45 | Emulator/Media/D64File.cpp |
| ⚠ [processDiskChangeEvent](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/Drive.cpp#L508-L590) | 45 | Emulator/Peripherals/Drive/Drive.cpp |
| ⚠ [describe](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Misc/RetroShell/Console.cpp#L657-L716) | 44 | Emulator/Misc/RetroShell/Console.cpp |
| ⚠ [peek](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/PIA.cpp#L150-L230) | 44 | Emulator/Peripherals/Drive/PIA.cpp |
| ⚠ [build_dac_table](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/dac.cc#L70-L129) | 43 | Emulator/Components/SID/resid/dac.cc |
| ⚠ [serialize](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CPU/CPU.h#L116-L165) | 43 | Emulator/Components/CPU/CPU.h |
| ⚠ [Cartridge::makeWithType](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/Cartridge.cpp#L102-L149) | 43 | Emulator/Media/Cartridges/Cartridge.cpp |
| ⚠ [checkOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/DriveBase.cpp#L202-L259) | 43 | Emulator/Peripherals/Drive/DriveBase.cpp |
| ⚠ [drawSpriteNr](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIISprites.cpp#L69-L138) | 42 | Emulator/Components/VICII/VICIISprites.cpp |
| ⚠ [clock](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/wave.h#L178-L245) | 42 | Emulator/Components/SID/resid/wave.h |
| ⚠ [clock_resample](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/sid.cc#L965-L1026) | 42 | Emulator/Components/SID/resid/sid.cc |
| ⚠ [Cartridge::isSupportedType](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/Cartridge.cpp#L28-L85) | 42 | Emulator/Media/Cartridges/Cartridge.cpp |
| ⚠ [increment](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CIA/TOD.cpp#L35-L91) | 41 | Emulator/Components/CIA/TOD.cpp |
| ⚠ [pokeIO](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/Memory/Memory.cpp#L531-L594) | 41 | Emulator/Components/Memory/Memory.cpp |
| ⚠ [load](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/Defaults.cpp#L229-L297) | 41 | Emulator/Base/Defaults.cpp |
| ⚠ [serialize](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/Drive.h#L315-L361) | 41 | Emulator/Peripherals/Drive/Drive.h |
| ⚠ [init](https://github.com/robvanderleek/virtualc64/blob/HEAD/Docs/Doxygen/doxygen-awesome/doxygen-awesome-tabs.js#L32-L85) | 41 | Docs/Doxygen/doxygen-awesome/doxygen-awesome-tabs.js |
| ⚠ [mixMultiSID](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/Ports/AudioPort.cpp#L208-L272) | 40 | Emulator/Components/Ports/AudioPort.cpp |
| ⚠ [drawSpritesSlowPath](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIISprites.cpp#L145-L232) | 40 | Emulator/Components/VICII/VICIISprites.cpp |
| ⚠ [processCommand](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CPU/CPU.cpp#L157-L203) | 40 | Emulator/Components/CPU/CPU.cpp |
| ⚠ [pokeModeReg](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/CustomCartridges/EasyFlash.cpp#L212-L280) | 40 | Emulator/Media/Cartridges/CustomCartridges/EasyFlash.cpp |
| ⚠ [cutLayers](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/DmaDebugger.cpp#L130-L184) | 39 | Emulator/Components/VICII/DmaDebugger.cpp |
| ⚠ [checkOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/SIDBase.cpp#L102-L155) | 39 | Emulator/Components/SID/SIDBase.cpp |
| ⚠ [serialize](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/CIA/CIA.h#L358-L400) | 38 | Emulator/Components/CIA/CIA.h |
| ⚠ [VICII::setUltimax](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIIMemory.cpp#L20-L64) | 38 | Emulator/Components/VICII/VICIIMemory.cpp |
| ⚠ [getViciiFunc](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIITable.cpp#L110-L149) | 36 | Emulator/Components/VICII/VICIITable.cpp |
| ⚠ [finalizeRead](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/T64File.cpp#L244-L323) | 36 | Emulator/Media/T64File.cpp |
| ⚠ [pokePCR](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/VIA.cpp#L719-L758) | 36 | Emulator/Peripherals/Drive/VIA.cpp |
| ⚠ [checkOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64Base.cpp#L202-L253) | 35 | Emulator/Components/C64Base.cpp |
| ⚠ [setOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64Base.cpp#L256-L308) | 35 | Emulator/Components/C64Base.cpp |
| ⚠ [updateBankMap](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/DriveMemory.cpp#L265-L317) | 35 | Emulator/Peripherals/Drive/DriveMemory.cpp |
| ⚠ [executeUF4](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/Drive.cpp#L79-L170) | 35 | Emulator/Peripherals/Drive/Drive.cpp |
| ⚠ [eraseWithPattern](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/Memory/Memory.cpp#L143-L207) | 34 | Emulator/Components/Memory/Memory.cpp |
| ⚠ [processDelayedActions](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICII.cpp#L795-L833) | 34 | Emulator/Components/VICII/VICII.cpp |
| ⚠ [writeCONTROL_REG](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/wave.cc#L201-L272) | 34 | Emulator/Components/SID/resid/wave.cc |
| ⚠ [write_state](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/sid.cc#L412-L451) | 34 | Emulator/Components/SID/resid/sid.cc |
| ⚠ [writeBAM](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/FileSystems/FSBlock.cpp#L44-L103) | 34 | Emulator/FileSystems/FSBlock.cpp |
| ⚠ [loadChip](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/Cartridges/Cartridge.cpp#L412-L457) | 34 | Emulator/Media/Cartridges/Cartridge.cpp |
| ⚠ [peekORA](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/VIA.cpp#L317-L385) | 34 | Emulator/Peripherals/Drive/VIA.cpp |
| ⚠ [Joystick::trigger](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Joystick/Joystick.cpp#L39-L95) | 34 | Emulator/Peripherals/Joystick/Joystick.cpp |
| ⚠ [VICII::memSpyAccess](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIIMemory.cpp#L642-L681) | 33 | Emulator/Components/VICII/VICIIMemory.cpp |
| ⚠ [set_waveform_output](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/wave.h#L458-L518) | 33 | Emulator/Components/SID/resid/wave.h |
| ⚠ [init](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/FileSystems/FileSystem.cpp#L169-L221) | 33 | Emulator/FileSystems/FileSystem.cpp |
| ⚠ [enum_long](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Base/CoreComponentTypes.h#L22-L60) | 33 | Emulator/Base/CoreComponentTypes.h |
| ⚠ [flash](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64.cpp#L1698-L1738) | 32 | Emulator/Components/C64.cpp |
| ⚠ [setOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/Ports/AudioPortBase.cpp#L182-L224) | 32 | Emulator/Components/Ports/AudioPortBase.cpp |
| ⚠ [setOption](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/DmaDebuggerBase.cpp#L103-L143) | 32 | Emulator/Components/VICII/DmaDebuggerBase.cpp |
| ⚠ [CoreComponent](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64Base.cpp#L20-L54) | 31 | Emulator/Components/C64Base.cpp |
| ⚠ [processFlags](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/C64.cpp#L853-L895) | 31 | Emulator/Components/C64.cpp |
| ⚠ [peekIO](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/Memory/Memory.cpp#L303-L354) | 31 | Emulator/Components/Memory/Memory.cpp |
| ⚠ [VICII::switchBank](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/VICII/VICIIMemory.cpp#L67-L117) | 31 | Emulator/Components/VICII/VICIIMemory.cpp |
| ⚠ [clock_resample_fastmem](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Components/SID/resid/sid.cc#L1032-L1073) | 31 | Emulator/Components/SID/resid/sid.cc |
| ⚠ [MediaFile::type](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Media/MediaFile.cpp#L33-L75) | 31 | Emulator/Media/MediaFile.cpp |
| ⚠ [execute](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/VIA.cpp#L93-L134) | 31 | Emulator/Peripherals/Drive/VIA.cpp |
| ⚠ [serialize](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Drive/VIA.h#L278-L310) | 31 | Emulator/Peripherals/Drive/VIA.h |
| ⚠ [processRxdEvent](https://github.com/robvanderleek/virtualc64/blob/HEAD/Emulator/Peripherals/Network/RS232.cpp#L318-L361) | 31 | Emulator/Peripherals/Network/RS232.cpp |

Generated by [CodeLimit](https://getcodelimit.github.io)
