// -----------------------------------------------------------------------------
// This file is part of VirtualC64
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v2
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

class PreferencesController: DialogController {
    
    var pref: Preferences { return parent.pref }
    var gamePadManager: GamePadManager { return parent.gamePadManager! }
    var myAppDelegate: MyAppDelegate { return NSApp.delegate as! MyAppDelegate }
    
    @IBOutlet weak var tabView: NSTabView!
    
    //
    // General preferences
    //
        
    // Drive
    @IBOutlet weak var emuDriveBlankDiskFormat: NSPopUpButton!
    @IBOutlet weak var emuEjectUnasked: NSButton!
    @IBOutlet weak var emuDriveSounds: NSButton!
    @IBOutlet weak var emuDriveSoundPan: NSPopUpButton!
    @IBOutlet weak var emuDriveInsertSound: NSButton!
    @IBOutlet weak var emuDriveEjectSound: NSButton!
    @IBOutlet weak var emuDriveHeadSound: NSButton!
    @IBOutlet weak var emuDriveConnectSound: NSButton!

    // Fullscreen
    @IBOutlet weak var emuAspectRatioButton: NSButton!
    @IBOutlet weak var emuExitOnEscButton: NSButton!

    // Snapshots and Screenshots
    @IBOutlet weak var emuAutoSnapshots: NSButton!
    @IBOutlet weak var emuSnapshotInterval: NSTextField!
    @IBOutlet weak var emuScreenshotSourcePopup: NSPopUpButton!
    @IBOutlet weak var emuScreenshotTargetPopup: NSPopUpButton!
        
    // Warp mode
    @IBOutlet weak var emuWarpMode: NSPopUpButton!

    // Misc
    @IBOutlet weak var emuPauseInBackground: NSButton!
    @IBOutlet weak var emuCloseWithoutAskingButton: NSButton!
    
    //
    // Controls
    //
    
    // Tag of the button that is currently being recorded
    var conRecordedKey: Int?
    
    // Joystick emulation keys
    @IBOutlet weak var conLeft1: NSTextField!
    @IBOutlet weak var conLeft1button: NSButton!
    @IBOutlet weak var conRight1: NSTextField!
    @IBOutlet weak var conRight1button: NSButton!
    @IBOutlet weak var conUp1: NSTextField!
    @IBOutlet weak var conUp1button: NSButton!
    @IBOutlet weak var conDown1: NSTextField!
    @IBOutlet weak var conDown1button: NSButton!
    @IBOutlet weak var conFire1: NSTextField!
    @IBOutlet weak var conFire1button: NSButton!
    @IBOutlet weak var conLeft2: NSTextField!
    @IBOutlet weak var conLeft2button: NSButton!
    @IBOutlet weak var conRight2: NSTextField!
    @IBOutlet weak var conRight2button: NSButton!
    @IBOutlet weak var conUp2: NSTextField!
    @IBOutlet weak var conUp2button: NSButton!
    @IBOutlet weak var conDown2: NSTextField!
    @IBOutlet weak var conDown2button: NSButton!
    @IBOutlet weak var conFire2: NSTextField!
    @IBOutlet weak var conFire2button: NSButton!
    @IBOutlet weak var conDisconnectKeys: NSButton!
    
    // Joystick buttons
    @IBOutlet weak var conAutofire: NSButton!
    @IBOutlet weak var conAutofireCease: NSButton!
    @IBOutlet weak var conAutofireCeaseText: NSTextField!
    @IBOutlet weak var conAutofireBullets: NSTextField!
    @IBOutlet weak var conAutofireFrequency: NSSlider!
    
    // Mouse
    @IBOutlet weak var conMouseModel: NSPopUpButton!
    @IBOutlet weak var conMouseInfo: NSTextField!
    
    @IBOutlet weak var conOkButton: NSButton!
    @IBOutlet weak var conPowerButton: NSButton!
    
    //
    // Devices
    //
    
    @IBOutlet weak var devSelector: NSSegmentedControl!
    
    // Property Box
    @IBOutlet weak var devInfoBox: NSBox!
    @IBOutlet weak var devInfoBoxTitle: NSTextField!
    @IBOutlet weak var devManufacturer: NSTextField!
    @IBOutlet weak var devProduct: NSTextField!
    @IBOutlet weak var devVersion: NSTextField!
    @IBOutlet weak var devVendorID: NSTextField!
    @IBOutlet weak var devProductID: NSTextField!
    @IBOutlet weak var devTransport: NSTextField!
    @IBOutlet weak var devUsage: NSTextField!
    @IBOutlet weak var devUsagePage: NSTextField!
    @IBOutlet weak var devLocationID: NSTextField!
    @IBOutlet weak var devUniqueID: NSTextField!
    @IBOutlet weak var devActivity: NSTextField!
    @IBOutlet weak var devActivity2: NSTextField!

    // Controller
    @IBOutlet weak var devImage: NSImageView!
    @IBOutlet weak var devLeftText: NSTextField!
    @IBOutlet weak var devRightText: NSTextField!
    @IBOutlet weak var devHatText: NSTextField!
    @IBOutlet weak var devLeftScheme: NSPopUpButton!
    @IBOutlet weak var devRightScheme: NSPopUpButton!
    @IBOutlet weak var devHatScheme: NSPopUpButton!
    
    //
    // Keymap
    //
    
    @IBOutlet weak var keyMappingText: NSTextField!
    @IBOutlet weak var keyMappingPopup: NSPopUpButton!
    @IBOutlet weak var keyMapText: NSTextField!
    @IBOutlet weak var keyBox: NSBox!

    // Array holding a reference to the view of each key button
    var keyView = Array(repeating: nil as RecordButton?, count: 66)

    // Arrays holding the key caps for each record key
    var keyImage = Array(repeating: nil as NSImage?, count: 66)
    var pressedKeyImage = Array(repeating: nil as NSImage?, count: 66)
    var mappedKeyImage = Array(repeating: nil as NSImage?, count: 66)
    
    // The C64 key that has been selected to be mapped
    var selectedKey: C64Key?
    
    @IBOutlet weak var keyOkButton: NSButton!
    @IBOutlet weak var keyPowerButton: NSButton!
    
    // The tab to open first
    var firstTab: String?

    func showSheet(tab: String) {

        firstTab = tab
        showSheet()
    }

    override func awakeFromNib() {

        super.awakeFromNib()
        
        if let id = firstTab { tabView.selectTabViewItem(withIdentifier: id) }
        refresh()
    }

    override func sheetDidShow() {

        if let id = firstTab { tabView.selectTabViewItem(withIdentifier: id) }
    }

    func refresh() {
        
        if let id = tabView.selectedTabViewItem?.identifier as? String {
            
            switch id {
            case "Emulator": refreshEmulatorTab()
            case "Controls": refreshControlsTab()
            case "Keyboard": refreshKeyboardTab()
            default: break
            }
        }
    }
    
    @discardableResult
    func keyDown(with key: MacKey) -> Bool {
        
        if let id = tabView.selectedTabViewItem?.identifier as? String {
            
            switch id {
            case "Controls": return conKeyDown(with: key)
            case "Keyboard": return mapSelectedKey(to: key)
            default: break
            }
        }
        return false
    }
    
    @IBAction override func okAction(_ sender: Any!) {
        
        track()
        pref.saveGeneralUserDefaults()
        pref.saveDevicesUserDefaults()
        hideSheet()
    }
}

extension PreferencesController: NSTabViewDelegate {
    
    func tabView(_ tabView: NSTabView, didSelect tabViewItem: NSTabViewItem?) {
        
        track()
        refresh()
    }
}

extension PreferencesController: NSTextFieldDelegate {
    
    func controlTextDidChange(_ obj: Notification) {
        
        track()
        
        if let view = obj.object as? NSTextField {
            
            let formatter = view.formatter as? NumberFormatter
            
            switch view {
                
            case emuSnapshotInterval:
                
                if formatter?.number(from: view.stringValue) != nil {
                    emuSnapshotIntervalAction(view)
                }

            case conAutofireBullets:
                
                if formatter?.number(from: view.stringValue) != nil {
                    conAutofireBulletsAction(view)
                }
                
            default:
                break
            }
        }
    }
}
