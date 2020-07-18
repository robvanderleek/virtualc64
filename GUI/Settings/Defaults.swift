// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v2
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

// swiftlint:disable colon

import Carbon.HIToolbox

//
// Convenience extensions to UserDefaults
//

extension UserDefaults {
    
    // Registers an item of generic type 'Encodable'
    func register<T: Encodable>(encodableItem item: T, forKey key: String) {
        
        if let data = try? PropertyListEncoder().encode(item) {
            register(defaults: [key: data])
        }
    }

    // Encodes an item of generic type 'Encodable'
    func encode<T: Encodable>(_ item: T, forKey key: String) {
        
        if let encoded = try? PropertyListEncoder().encode(item) {
            set(encoded, forKey: key)
        } else {
            track("Failed to encode \(key)")
        }
    }
    
    // Decodes an item of generic type 'Decodable'
    func decode<T: Decodable>(_ item: inout T, forKey key: String) {
        
        if let data = data(forKey: key) {
            if let decoded = try? PropertyListDecoder().decode(T.self, from: data) {
                item = decoded
            } else {
                track("Failed to decode \(key)")
            }
        }
    }
}

//
// User defaults (all)
//

extension UserDefaults {
    
    static func registerUserDefaults() {
        
        track()
        
        registerGeneralUserDefaults()
        registerDevicesUserDefaults()
        registerKeyboardUserDefaults()
        registerMediaUserDefaults()
        
        registerRomUserDefaults()
        registerHardwareUserDefaults()
        registerVideoUserDefaults()
    }
}

extension MyController {
    
    func loadUserDefaults() {
        
        track()
        
        c64.suspend()
        
        pref.loadGeneralUserDefaults()
        pref.loadDevicesUserDefaults()
        pref.loadKeyboardUserDefaults()
        pref.loadMediaUserDefaults()
        
        config.loadRomUserDefaults()
        config.loadHardwareUserDefaults()
        config.loadVideoUserDefaults()
        
        c64.resume()
    }
    
    func loadUserDefaults(url: URL, prefixes: [String]) {
        
        if let fileContents = NSDictionary(contentsOf: url) {
            
            if let dict = fileContents as? [String: Any] {
                
                let filteredDict = dict.filter { prefixes.contains(where: $0.0.hasPrefix) }
                
                let defaults = UserDefaults.standard
                defaults.setValuesForKeys(filteredDict)
                
                loadUserDefaults()
            }
        }
    }
    
    func saveUserDefaults(url: URL, prefixes: [String]) {
        
        track()
        
        let dict = UserDefaults.standard.dictionaryRepresentation()
        let filteredDict = dict.filter { prefixes.contains(where: $0.0.hasPrefix) }
        let nsDict = NSDictionary.init(dictionary: filteredDict)
        nsDict.write(to: url, atomically: true)
    }
}

//
// User defaults (General)
//

struct Keys {
    
    // Drives
    static let driveBlankDiskFormat   = "VC64_GEN_DriveBlankDiskFormat"
    static let driveEjectUnasked      = "VC64_GEN_EjectUnasked"
    static let driveSounds            = "VC64_GEN_DriveSounds"
    static let driveSoundPan          = "VC64_GEN_DriveSoundPan"
    static let driveInsertSound       = "VC64_GEN_DriveInsertSound"
    static let driveEjectSound        = "VC64_GEN_DriveEjectSound"
    static let driveHeadSound         = "VC64_GEN_DriveHeadSound"
    static let driveConnectSound      = "VC64_GEN_DriveConnectSound"

    // Snapshots and screenshots
    static let autoSnapshots          = "VC64_GEN_AutoSnapshots"
    static let autoSnapshotInterval   = "VC64_GEN_ScreenshotInterval"
    static let autoScreenshots        = "VC64_GEN_AutoScreenshots"
    static let screenshotSource       = "VC64_GEN_ScreenshotSource"
    static let screenshotTarget       = "VC64_GEN_ScreenshotTarget"
    
    // Fullscreen
    static let keepAspectRatio        = "VC64_GEN_FullscreenKeepAspectRatio"
    static let exitOnEsc              = "VC64_GEN_FullscreenExitOnEsc"

    // User dialogs
    static let closeWithoutAsking     = "VC64_GEN_CloseWithoutAsking"
    
    // Warp mode
    static let warpMode               = "VC64_GEN_WarpMode"

    // Miscellaneous
    static let pauseInBackground      = "VC64_GEN_PauseInBackground"
}

struct GeneralDefaults {
    
    // Drives
    let driveBlankDiskFormat: FileSystemType
    let driveEjectUnasked: Bool
    let driveSounds: Bool
    let driveSoundPan: Double
    let driveInsertSound: Bool
    let driveEjectSound: Bool
    let driveHeadSound: Bool
    let driveConnectSound: Bool
    
    // Snapshots and Screenshots
    let autoSnapshots: Bool
    let autoSnapshotInterval: Int
    let autoScreenshots: Bool
    let screenshotSource: Int
    let screenshotTarget: NSBitmapImageRep.FileType
    
    // Fullscreen
    let keepAspectRatio: Bool
    let exitOnEsc: Bool
    
    // Warp mode
    let warpMode: WarpMode
    
    // Miscellaneous
    let pauseInBackground: Bool
    let closeWithoutAsking: Bool
    
    //
    // Schemes
    //
    
    static let std = GeneralDefaults.init(
        
        driveBlankDiskFormat: FS_COMMODORE,
        driveEjectUnasked: false,
        driveSounds: true,
        driveSoundPan: 1.0,
        driveInsertSound: true,
        driveEjectSound: true,
        driveHeadSound: true,
        driveConnectSound: true,
        
        autoSnapshots: false,
        autoSnapshotInterval: 20,
        autoScreenshots: false,
        screenshotSource: 0,
        screenshotTarget: .png,
        
        keepAspectRatio: false,
        exitOnEsc: true,
        
        warpMode: .auto,
        
        pauseInBackground: false,
        closeWithoutAsking: false
    )
}

extension UserDefaults {

    static func registerGeneralUserDefaults() {
        
        let defaults = GeneralDefaults.std
        let dictionary: [String: Any] = [
            
            Keys.driveBlankDiskFormat: Int(defaults.driveBlankDiskFormat.rawValue),
            Keys.driveEjectUnasked: defaults.driveEjectUnasked,
            Keys.driveSounds: defaults.driveSounds,
            Keys.driveSoundPan: defaults.driveSoundPan,
            Keys.driveInsertSound: defaults.driveInsertSound,
            Keys.driveEjectSound: defaults.driveEjectSound,
            Keys.driveHeadSound: defaults.driveHeadSound,
            Keys.driveConnectSound: defaults.driveConnectSound,
            
            Keys.autoSnapshots: defaults.autoSnapshots,
            Keys.autoSnapshotInterval: defaults.autoSnapshotInterval,
            Keys.autoScreenshots: defaults.autoScreenshots,
            Keys.screenshotSource: defaults.screenshotSource,
            Keys.screenshotTarget: Int(defaults.screenshotTarget.rawValue),
            
            Keys.keepAspectRatio: defaults.keepAspectRatio,
            Keys.exitOnEsc: defaults.exitOnEsc,
            
            Keys.warpMode: Int(defaults.warpMode.rawValue),
            
            Keys.pauseInBackground: defaults.pauseInBackground,
            Keys.closeWithoutAsking: defaults.closeWithoutAsking
        ]
        
        let userDefaults = UserDefaults.standard
        
        userDefaults.register(defaults: dictionary)
    }
    
    static func resetGeneralUserDefaults() {
        
        let defaults = UserDefaults.standard
        
        let keys = [ Keys.driveBlankDiskFormat,
                     Keys.driveEjectUnasked,
                     Keys.driveSounds,
                     Keys.driveSoundPan,
                     Keys.driveInsertSound,
                     Keys.driveEjectSound,
                     Keys.driveHeadSound,
                     Keys.driveConnectSound,
                     
                     Keys.autoSnapshots,
                     Keys.autoSnapshotInterval,
                     Keys.autoScreenshots,
                     Keys.screenshotSource,
                     Keys.screenshotTarget,
                     
                     Keys.keepAspectRatio,
                     Keys.exitOnEsc,
                     
                     Keys.warpMode,
                     
                     Keys.pauseInBackground,
                     Keys.closeWithoutAsking
        ]
        
        for key in keys { defaults.removeObject(forKey: key) }
    }
}
    
//
// User defaults (Input Devices)
//

extension Keys {
    
    // Joysticks
    static let joyKeyMap1        = "VC64_DEV_JoyKeyMap1"
    static let joyKeyMap2        = "VC64_DEV_JoyKeyMap2"
    static let disconnectJoyKeys = "VC64_DEV_DisconnectKeys"
    static let autofire          = "VC64_DEV_Autofire"
    static let autofireBullets   = "VC64_DEV_AutofireBullets"
    static let autofireFrequency = "VC64_DEV_AutofireFrequency"
    
    // Mouse
    static let mouseModel        = "VC64_DEV_MouseModel"
}

struct DevicesDefaults {
    
    // Joysticks
    let joyKeyMap1: [MacKey: UInt32]
    let joyKeyMap2: [MacKey: UInt32]
    let disconnectJoyKeys: Bool
    let autofire: Bool
    let autofireBullets: Int
    let autofireFrequency: Float
    
    // Mouse
    let mouseModel: MouseModel
    
    //
    // Schemes
    //
    
    static let stdKeyMap1 = [
         
         MacKey.init(keyCode: kVK_LeftArrow): PULL_LEFT.rawValue,
         MacKey.init(keyCode: kVK_RightArrow): PULL_RIGHT.rawValue,
         MacKey.init(keyCode: kVK_UpArrow): PULL_UP.rawValue,
         MacKey.init(keyCode: kVK_DownArrow): PULL_DOWN.rawValue,
         MacKey.init(keyCode: kVK_Space): PRESS_FIRE.rawValue
     ]
     static let stdKeyMap2 = [
         
         MacKey.init(keyCode: kVK_ANSI_S): PULL_LEFT.rawValue,
         MacKey.init(keyCode: kVK_ANSI_D): PULL_RIGHT.rawValue,
         MacKey.init(keyCode: kVK_ANSI_E): PULL_UP.rawValue,
         MacKey.init(keyCode: kVK_ANSI_X): PULL_DOWN.rawValue,
         MacKey.init(keyCode: kVK_ANSI_C): PRESS_FIRE.rawValue
     ]
    
    static let std = DevicesDefaults.init(
        
        joyKeyMap1: stdKeyMap1,
        joyKeyMap2: stdKeyMap2,
        disconnectJoyKeys: true,
        autofire: false,
        autofireBullets: -3,
        autofireFrequency: 2.5,
        
        mouseModel: MOUSE1350
    )
}

extension UserDefaults {
    
    static func registerDevicesUserDefaults() {
        
        let defaults = DevicesDefaults.std
        let dictionary: [String: Any] = [

            // Joysticks
            Keys.disconnectJoyKeys: defaults.disconnectJoyKeys,
            Keys.autofire: defaults.autofire,
            Keys.autofireBullets: defaults.autofireBullets,
            Keys.autofireFrequency: defaults.autofireFrequency,
            
            // Mouse
            Keys.mouseModel: defaults.mouseModel.rawValue
        ]
        
        let userDefaults = UserDefaults.standard
        userDefaults.register(defaults: dictionary)
        userDefaults.register(encodableItem: defaults.joyKeyMap1, forKey: Keys.joyKeyMap1)
        userDefaults.register(encodableItem: defaults.joyKeyMap2, forKey: Keys.joyKeyMap2)
    }
    
    static func resetDevicesUserDefaults() {
        
        let defaults = UserDefaults.standard
        
        let keys = [ Keys.joyKeyMap1,
                     Keys.joyKeyMap2,
                     Keys.disconnectJoyKeys,
                     Keys.autofire,
                     Keys.autofireBullets,
                     Keys.autofireFrequency,
                     
                     Keys.mouseModel ]
        
        for key in keys { defaults.removeObject(forKey: key) }
    }
}

//
// User defaults (Keyboard)
//

extension Keys {
    
    // Key map
    static let keyMap            = "VC64KeyMap"
    static let mapKeysByPosition = "VC64MapKeysByPosition"
}

struct KeyboardDefaults {
    
    // Key map
    let keyMap: [MacKey: C64Key]
    let mapKeysByPosition: Bool
    
    //
    // Schemes
    //
    
    static let symbolicMapping = KeyboardDefaults.init(
        
        keyMap: [:],
        mapKeysByPosition: false
    )

    static let positionalMapping = KeyboardDefaults.init(
        
        keyMap: KeyboardController.standardKeyMap,
        mapKeysByPosition: true
    )
}

extension UserDefaults {
    
    static func registerKeyboardUserDefaults() {
        
        let defaults = KeyboardDefaults.symbolicMapping
        let dictionary: [String: Any] = [
            
            Keys.mapKeysByPosition: defaults.mapKeysByPosition
        ]
        
        let userDefaults = UserDefaults.standard
        userDefaults.register(defaults: dictionary)
        userDefaults.register(encodableItem: defaults.keyMap, forKey: Keys.keyMap)
    }
    
    static func resetKeyMapUserDefaults() {
        
        let defaults = UserDefaults.standard
        
        let keys = [ Keys.mapKeysByPosition,
                     Keys.keyMap
        ]
        
        for key in keys { defaults.removeObject(forKey: key) }
    }
}

/*
extension MyController {
        
    func loadKeyMapUserDefaults() {
        
        let defaults = UserDefaults.standard
        keyboard.mapKeysByPosition = defaults.bool(forKey: Keys.mapKeysByPosition)
        defaults.decode(&keyboard.keyMap, forKey: Keys.keyMap)
    }
    
    func saveKeyMapUserDefaults() {
        
        let defaults = UserDefaults.standard
        defaults.encode(keyboard.keyMap, forKey: Keys.keyMap)
        defaults.set(keyboard.mapKeysByPosition, forKey: Keys.mapKeysByPosition)
    }
}
*/

//
// User defaults (Media)
//

extension Keys {
    
    // Media files
    static let mountAction          = "VC64_MED_AutoMountAction"
    static let autoType             = "VC64_MED_AutoType"
    static let autoText             = "VC64_MED_AutoText"
}

struct MediaDefaults {
    
    // Media files
    let mountAction: [String: AutoMountAction]
    let autoType: [String: Bool]
    let autoText: [String: String]
   
    //
    // Schemes
    //
    
    static let stdAutoMountAction = [ "D64": AutoMountAction.openBrowser,
                                      "PRG": AutoMountAction.openBrowser,
                                      "T64": AutoMountAction.openBrowser,
                                      "TAP": AutoMountAction.openBrowser,
                                      "CRT": AutoMountAction.openBrowser ]
    
    static let stdAutoType        = [ "D64": true,
                                      "PRG": true,
                                      "T64": true,
                                      "TAP": true,
                                      "CRT": false ]
    
    static let stdAutoText        = [ "D64": "LOAD \"*\",8,1:",
                                      "PRG": "RUN",
                                      "T64": "RUN",
                                      "TAP": "LOAD",
                                      "CRT": "" ]

    static let std = MediaDefaults.init(
        
        mountAction: stdAutoMountAction,
        autoType: stdAutoType,
        autoText: stdAutoText
    )
}

extension UserDefaults {
    
    static func registerMediaUserDefaults() {
        
        let defaults = MediaDefaults.std
                
        let userDefaults = UserDefaults.standard
        userDefaults.register(encodableItem: defaults.mountAction, forKey: Keys.mountAction)
        userDefaults.register(encodableItem: defaults.autoType, forKey: Keys.autoType)
        userDefaults.register(encodableItem: defaults.autoText, forKey: Keys.autoText)
    }

    static func resetMediaDefaults() {
        
        let defaults = UserDefaults.standard
        
        let keys = [Keys.mountAction,
                    Keys.autoType,
                    Keys.autoText
        ]
        
        for key in keys { defaults.removeObject(forKey: key) }
    }
}

//
// User defaults (Roms)
//

extension UserDefaults {
    
    static func romUrl(name: String) -> URL? {
        
        let folder = URL.appSupportFolder("Roms")
        return folder?.appendingPathComponent(name)
    }
    
    static var basicRomUrl:  URL? { return romUrl(name: "basic.bin") }
    static var charRomUrl:   URL? { return romUrl(name: "char.bin") }
    static var kernalRomUrl: URL? { return romUrl(name: "kernal.bin") }
    static var vc1541RomUrl: URL? { return romUrl(name: "vc1541.bin") }
    
    static func registerRomUserDefaults() {
        
    }

    static func resetRomUserDefaults() {
        
        // Delete previously saved Rom files
        let fm = FileManager.default
        
        if let url = basicRomUrl {
            track("Deleting Basic Rom")
            try? fm.removeItem(at: url)
        }
        if let url = charRomUrl {
            track("Deleting Character Rom")
            try? fm.removeItem(at: url)
        }
        if let url = kernalRomUrl {
            track("Deleting Kernal Rom")
            try? fm.removeItem(at: url)
        }
        if let url = vc1541RomUrl {
            track("Deleting Drive Rom")
            try? fm.removeItem(at: url)
        }
    }
}

//
// User defaults (Hardware)
//

extension Keys {
    
    //VICII
    static let vicRevision    = "VC64_HW_VicRev"
    static let grayDotBug     = "VC64_HW_GrayDotBug"
    
    // CIAs
    static let ciaRevision    = "VC64_HW_CiaRev"
    static let timerBBug      = "VC64_HW_TimerBBug"
    
    // SID
    static let sidRevision    = "VC64_HW_SidRev"
    static let reSID          = "VC64_HW_ReSid"
    static let audioFilter    = "VC64_HW_AudioFilter"
    static let sampling       = "VC64_HW_Sampling"
    
    // Logic board and RAM
    static let glueLogic      = "VC64_HW_GlueLogic"
    static let initPattern    = "VC64_HW_RamPattern"
    
    // Drive
    static let drive8Connect  = "VC64_HW_Drive8Connect"
    static let drive8Type     = "VC64_HW_Drive8Type"
    static let drive9Connect  = "VC64_HW_Drive9Connect"
    static let drive9Type     = "VC64_HW_Drive9Type"
    
    // Ports
    static let gameDevice1    = "VC64_HW_GameDevice1"
    static let gameDevice2    = "VC64_HW_GameDevice2"
}

struct HardwareDefaults {
    
    var vicRev: VICRevision
    var grayDotBug: Bool
    
    var ciaRev: CIARevision
    var timerBBug: Bool
    
    var sidRev: SIDRevision
    var reSID: Bool
    var audioFilter: Bool
    var sampling: SamplingMethod
    
    var glueLogic: GlueLogic
    var initPattern: RamInitPattern

    var driveConnect: [Bool]
    var driveType: [DriveType]
    
    var gameDevice1: Int
    var gameDevice2: Int
    
    //
    // Schemes
    //
    
    static let C64_PAL = HardwareDefaults.init(
        
        vicRev:       PAL_6569_R3,
        grayDotBug:   false,
        
        ciaRev:       MOS_6526,
        timerBBug:    true,
        
        sidRev:       MOS_6581,
        reSID:        true,
        audioFilter:  true,
        sampling:     SID_SAMPLE_INTERPOLATE,
        
        glueLogic:    GLUE_DISCRETE,
        initPattern:  INIT_PATTERN_C64,
        
        driveConnect: [true, false],
        driveType:    [DRIVE_VC1541II, DRIVE_VC1541II],
        gameDevice1:  -1,
        gameDevice2:  -1
    )
    
    static let C64_II_PAL = HardwareDefaults.init(
        
        vicRev:       PAL_8565,
        grayDotBug:   true,
        
        ciaRev:       MOS_8521,
        timerBBug:    false,
        
        sidRev:       MOS_8580,
        reSID:        true,
        audioFilter:  true,
        sampling:     SID_SAMPLE_INTERPOLATE,
        
        glueLogic:    GLUE_CUSTOM_IC,
        initPattern:  INIT_PATTERN_C64C,
        
        driveConnect: [true, false],
        driveType:    [DRIVE_VC1541II, DRIVE_VC1541II],
        gameDevice1:  -1,
        gameDevice2:  -1
    )
    
    static let C64_OLD_PAL = HardwareDefaults.init(
        
        vicRev:       PAL_6569_R1,
        grayDotBug:   false,
        
        ciaRev:       MOS_6526,
        timerBBug:    true,
        
        sidRev:       MOS_6581,
        reSID:        true,
        audioFilter:  true,
        sampling:     SID_SAMPLE_INTERPOLATE,
        
        glueLogic:    GLUE_DISCRETE,
        initPattern:  INIT_PATTERN_C64,
        
        driveConnect: [true, false],
        driveType:    [DRIVE_VC1541II, DRIVE_VC1541II],
        gameDevice1:  -1,
        gameDevice2:  -1
    )

    static let C64_NTSC = HardwareDefaults.init(
        
        vicRev:       NTSC_6567,
        grayDotBug:   false,
        
        ciaRev:       MOS_6526,
        timerBBug:    false,
        
        sidRev:       MOS_6581,
        reSID:        true,
        audioFilter:  true,
        sampling:     SID_SAMPLE_INTERPOLATE,
        
        glueLogic:    GLUE_DISCRETE,
        initPattern:  INIT_PATTERN_C64,
        
        driveConnect: [true, false],
        driveType:    [DRIVE_VC1541II, DRIVE_VC1541II],
        gameDevice1:  -1,
        gameDevice2:  -1
    )
    
    static let C64_II_NTSC = HardwareDefaults.init(
        
        vicRev:       NTSC_8562,
        grayDotBug:   true,
        
        ciaRev:       MOS_8521,
        timerBBug:    true,
        
        sidRev:       MOS_8580,
        reSID:        true,
        audioFilter:  true,
        sampling:     SID_SAMPLE_INTERPOLATE,
        
        glueLogic:    GLUE_CUSTOM_IC,
        initPattern:  INIT_PATTERN_C64C,
        
        driveConnect: [true, false],
        driveType:    [DRIVE_VC1541II, DRIVE_VC1541II],
        gameDevice1:  -1,
        gameDevice2:  -1
    )
    
    static let C64_OLD_NTSC = HardwareDefaults.init(
        
        vicRev:       NTSC_6567_R56A,
        grayDotBug:   false,
        
        ciaRev:       MOS_6526,
        timerBBug:    false,
        
        sidRev:       MOS_6581,
        reSID:        true,
        audioFilter:  true,
        sampling:     SID_SAMPLE_INTERPOLATE,
        
        glueLogic:    GLUE_DISCRETE,
        initPattern:  INIT_PATTERN_C64,
        
        driveConnect: [true, false],
        driveType:    [DRIVE_VC1541II, DRIVE_VC1541II],
        gameDevice1:  -1,
        gameDevice2:  -1
    )
}

extension UserDefaults {
    
    static func registerHardwareUserDefaults() {
        
        let defaults = HardwareDefaults.C64_PAL
        let dictionary: [String: Any] = [
            
            Keys.vicRevision:   defaults.vicRev.rawValue,
            Keys.grayDotBug:    defaults.grayDotBug,
            
            Keys.ciaRevision:   defaults.ciaRev.rawValue,
            Keys.timerBBug:     defaults.timerBBug,
            
            Keys.sidRevision:   defaults.sidRev.rawValue,
            Keys.reSID:         defaults.reSID,
            Keys.audioFilter:   defaults.audioFilter,
            Keys.sampling:      defaults.sampling.rawValue,
            
            Keys.glueLogic:     defaults.glueLogic.rawValue,
            Keys.initPattern:   defaults.initPattern.rawValue,
            
            Keys.drive8Connect: defaults.driveConnect[0],
            Keys.drive8Type:    defaults.driveType[0].rawValue,
            Keys.drive9Connect: defaults.driveConnect[1],
            Keys.drive9Type:    defaults.driveType[1].rawValue,
            
            Keys.gameDevice1:   defaults.gameDevice1,
            Keys.gameDevice2:   defaults.gameDevice2
        ]
        
        let userDefaults = UserDefaults.standard
        userDefaults.register(defaults: dictionary)
    }
    
    static func resetHardwareUserDefaults() {
        
        let defaults = UserDefaults.standard
        
        let keys = [Keys.vicRevision,
                    Keys.grayDotBug,
                    
                    Keys.ciaRevision,
                    Keys.timerBBug,
                    
                    Keys.sidRevision,
                    Keys.reSID,
                    Keys.audioFilter,
                    Keys.sampling,
                    
                    Keys.glueLogic,
                    Keys.initPattern,
                    
                    Keys.drive8Connect,
                    Keys.drive8Type,
                    Keys.drive9Connect,
                    Keys.drive9Type,
                    
                    Keys.gameDevice1,
                    Keys.gameDevice2
        ]

        for key in keys { defaults.removeObject(forKey: key) }
    }
}

//
// User defaults (Video)
//

extension Keys {
    
    // Colors
    static let palette            = "VC64_VID_Palette"
    static let brightness         = "VC64_VID_Brightness"
    static let contrast           = "VC64_VID_Contrast"
    static let saturation         = "VC64_VID_Saturation"

    // Geometry
    static let hCenter            = "VC64_VID_HCenter"
    static let vCenter            = "VC64_VID_VCenter"
    static let hZoom              = "VC64_VID_HZoom"
    static let vZoom              = "VC64_VID_VZoom"

    // Upscalers
    static let upscaler           = "VC64_VID_Upscaler"

    // Shader options
    static let blur               = "VC64_VID_Blur"
    static let blurRadius         = "VC64_VID_BlurRadius"
    static let bloom              = "VC64_VID_Bloom"
    static let bloomRadiusR       = "VC64_VID_BloonRadiusR"
    static let bloomRadiusG       = "VC64_VID_BloonRadiusG"
    static let bloomRadiusB       = "VC64_VID_BloonRadiusB"
    static let bloomBrightness    = "VC64_VID_BloomBrightness"
    static let bloomWeight        = "VC64_VID_BloomWeight"
    static let flicker            = "VC64_VID_Flicker"
    static let flickerWeight      = "VC64_VID_FlickerWeight"
    static let dotMask            = "VC64_VID_DotMask"
    static let dotMaskBrightness  = "VC64_VID_DotMaskBrightness"
    static let scanlines          = "VC64_VID_Scanlines"
    static let scanlineBrightness = "VC64_VID_ScanlineBrightness"
    static let scanlineWeight     = "VC64_VID_ScanlineWeight"
    static let disalignment       = "VC64_VID_Disalignment"
    static let disalignmentH      = "VC64_VID_DisalignmentH"
    static let disalignmentV      = "VC64_VID_DisalignmentV"
}

struct VideoDefaults {
    
    // Colors
    let palette: Palette
    let brightness: Double
    let contrast: Double
    let saturation: Double
    
    // Geometry
    let hCenter: Float
    let vCenter: Float
    let hZoom: Float
    let vZoom: Float
    
    // Upscalers
    let upscaler: Int
    
    // Shader options
    let blur: Int32
    let blurRadius: Float
    let bloom: Int32
    let bloomRadiusR: Float
    let bloomRadiusG: Float
    let bloomRadiusB: Float
    let bloomBrightness: Float
    let bloomWeight: Float
    let flicker: Int32
    let flickerWeight: Float
    let dotMask: Int32
    let dotMaskBrightness: Float
    let scanlines: Int32
    let scanlineBrightness: Float
    let scanlineWeight: Float
    let disalignment: Int32
    let disalignmentH: Float
    let disalignmentV: Float
    
    //
    // Schemes
    //
    
    // TFT monitor appearance
    static let tft = VideoDefaults.init(
        
        palette: COLOR_PALETTE,
        brightness: 50.0,
        contrast: 100.0,
        saturation: 50.0,
        
        hCenter: 0,
        vCenter: 0,
        hZoom: 0,
        vZoom: 0,
        
        upscaler: 0,
        
        blur: 1,
        blurRadius: 0,
        bloom: 0,
        bloomRadiusR: 1.0,
        bloomRadiusG: 1.0,
        bloomRadiusB: 1.0,
        bloomBrightness: 0.4,
        bloomWeight: 1.21,
        flicker: 1,
        flickerWeight: 0.5,
        dotMask: 0,
        dotMaskBrightness: 0.7,
        scanlines: 0,
        scanlineBrightness: 0.55,
        scanlineWeight: 0.11,
        disalignment: 0,
        disalignmentH: 0.001,
        disalignmentV: 0.001
    )
    
    // CRT monitor appearance
    static let crt = VideoDefaults.init(
        
        palette: COLOR_PALETTE,
        brightness: 50.0,
        contrast: 100.0,
        saturation: 50.0,
        
        hCenter: 0,
        vCenter: 0,
        hZoom: 0,
        vZoom: 0,
        
        upscaler: 0,
        
        blur: 1,
        blurRadius: 1.5,
        bloom: 1,
        bloomRadiusR: 1.0,
        bloomRadiusG: 1.0,
        bloomRadiusB: 1.0,
        bloomBrightness: 0.4,
        bloomWeight: 1.21,
        flicker: 1,
        flickerWeight: 0.5,
        dotMask: 1,
        dotMaskBrightness: 0.5,
        scanlines: 2,
        scanlineBrightness: 0.55,
        scanlineWeight: 0.11,
        disalignment: 0,
        disalignmentH: 0.001,
        disalignmentV: 0.001
    )
}

extension UserDefaults {
    
    static func registerVideoUserDefaults() {
        
        let defaults = VideoDefaults.tft
        let dictionary: [String: Any] = [
            
            Keys.palette: Int(defaults.palette.rawValue),
            Keys.brightness: defaults.brightness,
            Keys.contrast: defaults.contrast,
            Keys.saturation: defaults.saturation,
            
            Keys.hCenter: defaults.hCenter,
            Keys.vCenter: defaults.vCenter,
            Keys.hZoom: defaults.hZoom,
            Keys.vZoom: defaults.vZoom,
            
            Keys.upscaler: defaults.upscaler,
            
            Keys.blur: defaults.blur,
            Keys.blurRadius: defaults.blurRadius,
            Keys.bloom: defaults.bloom,
            Keys.bloomRadiusR: defaults.bloomRadiusR,
            Keys.bloomRadiusG: defaults.bloomRadiusG,
            Keys.bloomRadiusB: defaults.bloomRadiusB,
            Keys.bloomBrightness: defaults.bloomBrightness,
            Keys.bloomWeight: defaults.bloomWeight,
            Keys.flicker: defaults.flicker,
            Keys.flickerWeight: defaults.flickerWeight,
            Keys.dotMask: defaults.dotMask,
            Keys.dotMaskBrightness: defaults.dotMaskBrightness,
            Keys.scanlines: defaults.scanlines,
            Keys.scanlineBrightness: defaults.scanlineBrightness,
            Keys.scanlineWeight: defaults.scanlineWeight,
            Keys.disalignment: defaults.disalignment,
            Keys.disalignmentH: defaults.disalignmentH,
            Keys.disalignmentV: defaults.disalignmentV
        ]
        
        let userDefaults = UserDefaults.standard
        userDefaults.register(defaults: dictionary)
    }
    
    static func resetVideoUserDefaults() {
        
        let defaults = UserDefaults.standard
                
        let keys = [ Keys.palette,
                     Keys.brightness,
                     Keys.contrast,
                     Keys.saturation,
                     
                     Keys.hCenter,
                     Keys.vCenter,
                     Keys.hZoom,
                     Keys.vZoom,
                     
                     Keys.upscaler,
                     
                     Keys.blur,
                     Keys.blurRadius,
                     Keys.bloom,
                     Keys.bloomRadiusR,
                     Keys.bloomRadiusG,
                     Keys.bloomRadiusB,
                     Keys.bloomBrightness,
                     Keys.bloomWeight,
                     Keys.flicker,
                     Keys.flickerWeight,
                     Keys.dotMask,
                     Keys.dotMaskBrightness,
                     Keys.scanlines,
                     Keys.scanlineBrightness,
                     Keys.scanlineWeight,
                     Keys.disalignment,
                     Keys.disalignmentH,
                     Keys.disalignmentV
        ]
        
        for key in keys { defaults.removeObject(forKey: key) }
    }
}

/*
extension MyController {
        
    func loadVideoUserDefaults() {
        
        let defaults = UserDefaults.standard
        
        c64.suspend()
        
        renderer.upscaler = defaults.integer(forKey: Keys.upscaler)
        c64.vic.setVideoPalette(defaults.integer(forKey: Keys.palette))
        c64.vic.setBrightness(defaults.double(forKey: Keys.brightness))
        c64.vic.setContrast(defaults.double(forKey: Keys.contrast))
        c64.vic.setSaturation(defaults.double(forKey: Keys.saturation))

        renderer.keepAspectRatio = defaults.bool(forKey: Keys.keepAspectRatio)
        /*
        renderer.setEyeX(defaults.float(forKey: VC64Keys.eyeX))
        renderer.setEyeY(defaults.float(forKey: VC64Keys.eyeY))
        renderer.setEyeZ(defaults.float(forKey: VC64Keys.eyeZ))
        */
        
        defaults.decode(&renderer.shaderOptions, forKey: Keys.shaderOptions)
        renderer.buildDotMasks()
 
        c64.resume()
    }
    
    func saveVideoUserDefaults() {
        
        let defaults = UserDefaults.standard
        
        defaults.set(renderer.upscaler, forKey: Keys.upscaler)
        defaults.set(c64.vic.videoPalette(), forKey: Keys.palette)
        defaults.set(c64.vic.brightness(), forKey: Keys.brightness)
        defaults.set(c64.vic.contrast(), forKey: Keys.contrast)
        defaults.set(c64.vic.saturation(), forKey: Keys.saturation)
        
        defaults.set(renderer.keepAspectRatio, forKey: Keys.keepAspectRatio)
        /*
        defaults.set(renderer.eyeX(), forKey: VC64Keys.eyeX)
        defaults.set(renderer.eyeY(), forKey: VC64Keys.eyeY)
        defaults.set(renderer.eyeZ(), forKey: VC64Keys.eyeZ)
        */
        defaults.encode(renderer.shaderOptions, forKey: Keys.shaderOptions)
    }
}
*/
