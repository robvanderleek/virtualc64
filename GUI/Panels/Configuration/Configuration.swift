// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

/* This class stores all items that are specific to an individual emulator
 * instance. Each instance keeps its own object of this class inside the
 * document controller.
 *
 * See class "Preferences" for shared settings.
 */

class Configuration {
    
    var parent: MyController!
    var c64: EmulatorProxy { return parent.c64 }
    var renderer: Renderer { return parent.renderer }
    var gamePadManager: GamePadManager { return parent.gamePadManager }
    var ressourceManager: RessourceManager { return renderer.ressourceManager }
    
    //
    // Hardware
    //
    
    var vicRevision: Int {
        get { return c64.getConfig(.VICII_REVISION) }
        set { c64.configure(.VICII_REVISION, value: newValue) }
    }

    var vicGrayDotBug: Bool {
        get { return c64.getConfig(.VICII_GRAY_DOT_BUG) != 0 }
        set { c64.configure(.VICII_GRAY_DOT_BUG, enable: newValue) }
    }
    
    var vicHideSprites: Bool {
        get { return c64.getConfig(.VICII_HIDE_SPRITES) != 0 }
        set { c64.configure(.VICII_HIDE_SPRITES, enable: newValue) }
    }
        
    var vicCutLayers: Int {
        get { return c64.getConfig(.VICII_CUT_LAYERS) }
        set { c64.configure(.VICII_CUT_LAYERS, value: newValue) }
    }
    
    var vicCutOpacity: Int {
        get { return c64.getConfig(.VICII_CUT_OPACITY) }
        set { c64.configure(.VICII_CUT_OPACITY, value: newValue) }
    }
        
    var ciaRevision: Int {
        get { return c64.getConfig(.CIA_REVISION) }
        set { c64.configure(.CIA_REVISION, value: newValue) }
    }
    
    var ciaTimerBBug: Bool {
        get { return c64.getConfig(.CIA_TIMER_B_BUG) != 0}
        set { c64.configure(.CIA_TIMER_B_BUG, enable: newValue) }
    }
    
    var sidRevision: Int {
        get { return c64.getConfig(.SID_REVISION) }
        set { c64.configure(.SID_REVISION, value: newValue) }
    }
    
    var sidFilter: Bool {
        get { return c64.getConfig(.SID_FILTER) != 0 }
        set { c64.configure(.SID_FILTER, enable: newValue) }
    }
    
    var glueLogic: Int {
        get { return c64.getConfig(.GLUE_LOGIC) }
        set { c64.configure(.GLUE_LOGIC, value: newValue) }
    }

    var powerGrid: Int {
        get { return c64.getConfig(.POWER_GRID) }
        set { c64.configure(.POWER_GRID, value: newValue) }
    }

    var ramPattern: Int {
        get { return c64.getConfig(.RAM_PATTERN) }
        set { c64.configure(.RAM_PATTERN, value: newValue) }
    }
    
    //
    // Peripherals
    //
    
    var drive8Connected: Bool {
        get { return c64.getConfig(.DRV_CONNECT, drive: DRIVE8) != 0 }
        set { c64.configure(.DRV_CONNECT, drive: DRIVE8, enable: newValue ) }
    }
    
    var drive9Connected: Bool {
        get { return c64.getConfig(.DRV_CONNECT, drive: DRIVE9) != 0 }
        set { c64.configure(.DRV_CONNECT, drive: DRIVE9, enable: newValue ) }
    }

    var drive8AutoConf: Bool {
        get { return c64.getConfig(.DRV_AUTO_CONFIG, drive: DRIVE8) != 0 }
        set { c64.configure(.DRV_AUTO_CONFIG, drive: DRIVE8, enable: newValue ) }
    }
    
    var drive9AutoConf: Bool {
        get { return c64.getConfig(.DRV_AUTO_CONFIG, drive: DRIVE9) != 0}
        set { c64.configure(.DRV_AUTO_CONFIG, drive: DRIVE9, enable: newValue ) }
    }
    
    var drive8Type: Int {
        get { return c64.getConfig(.DRV_TYPE, drive: DRIVE8) }
        set { c64.configure(.DRV_TYPE, drive: DRIVE8, value: newValue )}
    }
    
    var drive9Type: Int {
        get { return c64.getConfig(.DRV_TYPE, drive: DRIVE9) }
        set { c64.configure(.DRV_TYPE, drive: DRIVE9, value: newValue )}
    }

    var drive8Ram: Int {
        get { return c64.getConfig(.DRV_RAM, drive: DRIVE8) }
        set { c64.configure(.DRV_RAM, drive: DRIVE8, value: newValue )}
    }

    var drive9Ram: Int {
        get { return c64.getConfig(.DRV_RAM, drive: DRIVE9) }
        set { c64.configure(.DRV_RAM, drive: DRIVE9, value: newValue )}
    }

    var drive8ParCable: Int {
        get { return c64.getConfig(.DRV_PARCABLE, drive: DRIVE8) }
        set { c64.configure(.DRV_PARCABLE, drive: DRIVE8, value: newValue ) }
    }

    var drive9ParCable: Int {
        get { return c64.getConfig(.DRV_PARCABLE, drive: DRIVE9) }
        set { c64.configure(.DRV_PARCABLE, drive: DRIVE9, value: newValue ) }
    }
    
    var drive8PowerSwitch: Bool {
        get { return c64.getConfig(.DRV_POWER_SWITCH, drive: DRIVE8) != 0 }
        set { c64.configure(.DRV_POWER_SWITCH, drive: DRIVE8, enable: newValue )}
    }
    
    var drive9PowerSwitch: Bool {
        get { return c64.getConfig(.DRV_POWER_SWITCH, drive: DRIVE9) != 0 }
        set { c64.configure(.DRV_POWER_SWITCH, drive: DRIVE9, enable: newValue )}
    }

    var datasetteConnected: Bool {
        get { return c64.getConfig(.DAT_CONNECT) != 0 }
        set { c64.configure(.DAT_CONNECT, enable: newValue ) }
    }

    var datasetteModel: Int {
        get { return c64.getConfig(.DAT_MODEL) }
        set { c64.configure(.DAT_MODEL, value: newValue )}
    }

    var gameDevice1 = -1 {
        didSet {
            
            // Try to connect the device (may disconnect the other device)
            gamePadManager.connect(slot: gameDevice1, port: 0)
            gamePadManager.listDevices()
            
            // Avoid double mappings
            if gameDevice1 != -1 && gameDevice1 == gameDevice2 {
                gameDevice2 = -1
            }
            
            parent.toolbar.validateVisibleItems()
        }
    }
    
    var gameDevice2 = -2 {
        didSet {
            
            // Try to connect the device (may disconnect the other device)
            gamePadManager.connect(slot: gameDevice2, port: 1)
            gamePadManager.listDevices()
            
            // Avoid double mappings
            if gameDevice2 != -1 && gameDevice2 == gameDevice1 {
                gameDevice1 = -1
            }
            
            parent.toolbar.validateVisibleItems()
        }
    }
    
    var mouseModel: Int {
        get { return c64.getConfig(.MOUSE_MODEL, id: 1) }
        set { c64.configure(.MOUSE_MODEL, value: newValue) }
    }

    var autofire: Bool {
        get { return c64.getConfig(.AUTOFIRE, id: 1) != 0 }
        set { c64.configure(.AUTOFIRE, enable: newValue) }
    }

    var autofireBursts: Bool {
        get { return c64.getConfig(.AUTOFIRE_BURSTS, id: 1) != 0 }
        set { c64.configure(.AUTOFIRE_BURSTS, enable: newValue) }
    }

    var autofireBullets: Int {
        get { return c64.getConfig(.AUTOFIRE_BULLETS, id: 1) }
        set { c64.configure(.AUTOFIRE_BULLETS, value: newValue) }
    }

    var autofireFrequency: Int {
        get { return c64.getConfig(.AUTOFIRE_DELAY, id: 1) }
        set { c64.configure(.AUTOFIRE_DELAY, value: newValue) }
    }

    //
    // Performance
    //

    var drive8PowerSave: Bool {
        get { return c64.getConfig(.DRV_POWER_SAVE, drive: DRIVE8) != 0 }
        set { c64.configure(.DRV_POWER_SAVE, drive: DRIVE8, enable: newValue) }
    }

    var drive9PowerSave: Bool {
        get { return c64.getConfig(.DRV_POWER_SAVE, drive: DRIVE9) != 0 }
        set { c64.configure(.DRV_POWER_SAVE, drive: DRIVE9, enable: newValue) }
    }

    var viciiPowerSave: Bool {
        get { return c64.getConfig(.VICII_POWER_SAVE) != 0 }
        set { c64.configure(.VICII_POWER_SAVE, enable: newValue) }
    }

    var sidPowerSave: Bool {
        get { return c64.getConfig(.SID_POWER_SAVE) != 0 }
        set { c64.configure(.SID_POWER_SAVE, enable: newValue) }
    }

    var ssCollisions: Bool {
        get { return c64.getConfig(.VICII_SS_COLLISIONS) != 0 }
        set { c64.configure(.VICII_SS_COLLISIONS, enable: newValue) }
    }

    var sbCollisions: Bool {
        get { return c64.getConfig(.VICII_SB_COLLISIONS) != 0 }
        set { c64.configure(.VICII_SB_COLLISIONS, enable: newValue) }
    }

    var warpMode: Int {
        get { return c64.getConfig(.EMU_WARP_MODE) }
        set { c64.configure(.EMU_WARP_MODE, value: newValue) }
    }

    var warpBoot: Int {
        get { return c64.getConfig(.EMU_WARP_BOOT) }
        set { c64.configure(.EMU_WARP_BOOT, value: newValue) }
    }

    var vsync: Bool {
        get { return c64.getConfig(.EMU_VSYNC) != 0 }
        set { c64.configure(.EMU_VSYNC, enable: newValue) }
    }
    var timeLapse: Int {
        get { return c64.getConfig(.EMU_TIME_LAPSE) }
        set { c64.configure(.EMU_TIME_LAPSE, value: newValue) }
    }

    var runAhead: Int {
        get { return c64.getConfig(.EMU_RUN_AHEAD) }
        set { c64.configure(.EMU_RUN_AHEAD, value: newValue) }
    }

    //
    // Audio
    //
    
    var sidEnable1: Bool {
        get { return c64.getConfig(.SID_ENABLE, id: 1) != 0 }
        set { c64.configure(.SID_ENABLE, id: 1, enable: newValue) }
    }
    var sidEnable2: Bool {
        get { return c64.getConfig(.SID_ENABLE, id: 2) != 0 }
        set { c64.configure(.SID_ENABLE, id: 2, enable: newValue) }
    }
    var sidEnable3: Bool {
        get { return c64.getConfig(.SID_ENABLE, id: 3) != 0 }
        set { c64.configure(.SID_ENABLE, id: 3, enable: newValue) }
    }
    var sidAddress1: Int {
        get { return c64.getConfig(.SID_ADDRESS, id: 1) }
        set { c64.configure(.SID_ADDRESS, id: 1, value: newValue) }
    }
    var sidAddress2: Int {
        get { return c64.getConfig(.SID_ADDRESS, id: 2) }
        set { c64.configure(.SID_ADDRESS, id: 2, value: newValue) }
    }
    var sidAddress3: Int {
        get { return c64.getConfig(.SID_ADDRESS, id: 3) }
        set { c64.configure(.SID_ADDRESS, id: 3, value: newValue) }
    }
    var sidEngine: Int {
        get { return c64.getConfig(.SID_ENGINE) }
        set { c64.configure(.SID_ENGINE, value: newValue) }
    }
    var sidSampling: Int {
        get { return c64.getConfig(.SID_SAMPLING) }
        set { c64.configure(.SID_SAMPLING, value: newValue) }
    }
    var vol0: Int {
        get { return c64.getConfig(.AUD_VOL, id: 0) }
        set { c64.configure(.AUD_VOL, id: 0, value: newValue) }
    }
    var vol1: Int {
        get { return c64.getConfig(.AUD_VOL, id: 1) }
        set { c64.configure(.AUD_VOL, id: 1, value: newValue) }
    }
    var vol2: Int {
        get { return c64.getConfig(.AUD_VOL, id: 2) }
        set { c64.configure(.AUD_VOL, id: 2, value: newValue) }
    }
    var vol3: Int {
        get { return c64.getConfig(.AUD_VOL, id: 3) }
        set { c64.configure(.AUD_VOL, id: 3, value: newValue) }
    }
    var pan0: Int {
        get { return c64.getConfig(.AUD_PAN, id: 0) }
        set { c64.configure(.AUD_PAN, id: 0, value: newValue) }
    }
    var pan1: Int {
        get { return c64.getConfig(.AUD_PAN, id: 1) }
        set { c64.configure(.AUD_PAN, id: 1, value: newValue) }
    }
    var pan2: Int {
        get { return c64.getConfig(.AUD_PAN, id: 2) }
        set { c64.configure(.AUD_PAN, id: 2, value: newValue) }
    }
    var pan3: Int {
        get { return c64.getConfig(.AUD_PAN, id: 3) }
        set { c64.configure(.AUD_PAN, id: 3, value: newValue) }
    }
    var volL: Int {
        get { return c64.getConfig(.AUD_VOL_L) }
        set { c64.configure(.AUD_VOL_L, value: newValue) }
    }
    var volR: Int {
        get { return c64.getConfig(.AUD_VOL_R) }
        set { c64.configure(.AUD_VOL_R, value: newValue) }
    }
    var stepVolume: Int {
        get { return c64.getConfig(.DRV_STEP_VOL, drive: DRIVE8) }
        set { c64.configure(.DRV_STEP_VOL, value: newValue) }
    }
    var insertVolume: Int {
        get { return c64.getConfig(.DRV_INSERT_VOL, drive: DRIVE8) }
        set { c64.configure(.DRV_INSERT_VOL, value: newValue) }
    }
    var ejectVolume: Int {
        get { return c64.getConfig(.DRV_EJECT_VOL, drive: DRIVE8) }
        set { c64.configure(.DRV_EJECT_VOL, value: newValue) }
    }
    var drive8Pan: Int {
        get { return c64.getConfig(.DRV_PAN, drive: DRIVE8) }
        set { c64.configure(.DRV_PAN, drive: DRIVE8, value: newValue) }
    }
    var drive9Pan: Int {
        get { return c64.getConfig(.DRV_PAN, drive: DRIVE9) }
        set { c64.configure(.DRV_PAN, drive: DRIVE9, value: newValue) }
    }

    //
    // Video
    //

    var palette: Int {
        get { return c64.getConfig(.MON_PALETTE) }
        set { c64.configure(.MON_PALETTE, value: newValue) }
    }
    var brightness: Int {
        get { return c64.getConfig(.MON_BRIGHTNESS) }
        set { c64.configure(.MON_BRIGHTNESS, value: newValue) }
    }
    var contrast: Int {
        get { return c64.getConfig(.MON_CONTRAST) }
        set { c64.configure(.MON_CONTRAST, value: newValue) }
    }
    var saturation: Int {
        get { return c64.getConfig(.MON_SATURATION) }
        set { c64.configure(.MON_SATURATION, value: newValue) }
    }
    var hCenter: Int {
        get { return c64.getConfig(.MON_HCENTER) }
        set {
            c64.configure(.MON_HCENTER, value: newValue)
            renderer.canvas.updateTextureRect()
        }
    }
    var vCenter: Int {
        get { return c64.getConfig(.MON_VCENTER) }
        set {
            c64.configure(.MON_VCENTER, value: newValue)
            renderer.canvas.updateTextureRect()
        }
    }
    var hZoom: Int {
        get { return c64.getConfig(.MON_HZOOM) }
        set {
            c64.configure(.MON_HZOOM, value: newValue)
            renderer.canvas.updateTextureRect()
        }
    }
    var vZoom: Int {
        get { return c64.getConfig(.MON_VZOOM) }
        set {
            c64.configure(.MON_VZOOM, value: newValue)
            renderer.canvas.updateTextureRect()
        }
    }
    var upscaler: Int = 0 {
        didSet {
            if !ressourceManager.selectUpscaler(upscaler) { upscaler = oldValue }
        }
    }
    var blur: Int = 0 {
        didSet { renderer.shaderOptions.blur = Int32(blur) }
    }
    var blurRadius: Float = 0 {
        didSet { renderer.shaderOptions.blurRadius = blurRadius }
    }
    var bloom: Int = 0 {
        didSet {
            renderer.shaderOptions.bloom = Int32(bloom)
            if !ressourceManager.selectBloomFilter(bloom) { bloom = oldValue }
        }
    }
    var bloomRadiusR: Float = 0 {
        didSet { renderer.shaderOptions.bloomRadiusR = bloomRadiusR }
    }
    var bloomRadiusG: Float = 0 {
        didSet { renderer.shaderOptions.bloomRadiusG = bloomRadiusG }
    }
    var bloomRadiusB: Float = 0 {
        didSet { renderer.shaderOptions.bloomRadiusB = bloomRadiusB }
    }
    var bloomBrightness: Float = 0 {
        didSet { renderer.shaderOptions.bloomBrightness = bloomBrightness }
    }
    var bloomWeight: Float = 0 {
        didSet { renderer.shaderOptions.bloomWeight = bloomWeight }
    }
    var dotMask: Int = 0 {
        didSet {
            renderer.shaderOptions.dotMask = Int32(dotMask)
            ressourceManager.buildDotMasks()
            if !ressourceManager.selectDotMask(dotMask) { dotMask = oldValue }
        }
    }
    var dotMaskBrightness: Float = 0 {
        didSet {
            renderer.shaderOptions.dotMaskBrightness = dotMaskBrightness
            ressourceManager.buildDotMasks()
            ressourceManager.selectDotMask(dotMask)
        }
    }
    var scanlines: Int = 0 {
        didSet {
            renderer.shaderOptions.scanlines = Int32(scanlines)
            if !ressourceManager.selectScanlineFilter(scanlines) { scanlines = oldValue }
        }
    }
    var scanlineBrightness: Float = 0 {
        didSet { renderer.shaderOptions.scanlineBrightness = scanlineBrightness }
    }
    var scanlineWeight: Float = 0 {
        didSet { renderer.shaderOptions.scanlineWeight = scanlineWeight }
    }
    var disalignment: Int = 0 {
        didSet { renderer.shaderOptions.disalignment = Int32(disalignment) }
    }
    var disalignmentH: Float = 0 {
        didSet { renderer.shaderOptions.disalignmentH = disalignmentH }
    }
    var disalignmentV: Float = 0 {
        didSet { renderer.shaderOptions.disalignmentV = disalignmentV }
    }
    
    init(with controller: MyController) { parent = controller }

    //
    // Roms
    //

    func loadRomUserDefaults() {

        func load(_ url: URL?, type: vc64.FileType) {

            if url != nil {
                if let file = try? RomFileProxy.make(with: url!) {
                    if file.type == type { c64.loadRom(file) }
                }
            }
        }

        debug(.defaults)
        
        c64.suspend()
        load(UserDefaults.basicRomUrl, type: .BASIC_ROM)
        load(UserDefaults.charRomUrl, type: .CHAR_ROM)
        load(UserDefaults.kernalRomUrl, type: .KERNAL_ROM)
        load(UserDefaults.vc1541RomUrl, type: .VC1541_ROM)
        c64.resume()
    }

    func saveRomUserDefaults() throws {

        debug(.defaults)

        var url: URL?

        func save(_ type: vc64.RomType) throws {

            if url == nil { throw VC64Error(vc64.ErrorCode.FILE_CANT_WRITE) }
            try? FileManager.default.removeItem(at: url!)
            try c64.saveRom(type, url: url!)
        }

        c64.suspend()

        do {
            url = UserDefaults.basicRomUrl;  try save(.BASIC)
            url = UserDefaults.charRomUrl;   try save(.CHAR)
            url = UserDefaults.kernalRomUrl; try save(.KERNAL)
            url = UserDefaults.vc1541RomUrl; try save(.VC1541)

        } catch {

            c64.resume()
            throw error
        }

        c64.resume()
    }
}
