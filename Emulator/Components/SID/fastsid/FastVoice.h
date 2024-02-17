/*
 * This file belongs to the FastSID implementation of VirtualC64,
 * an adaption of the code used in VICE 3.1, the Versatile Commodore Emulator.
 *
 * Original code written by
 *  Teemu Rantanen <tvr@cs.hut.fi>
 *
 * Adapted for VirtualC64 by
 *  Dirk Hoffmann
 */
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#pragma once

#include "SIDTypes.h"
#include "SubComponent.h"

using namespace vc64;

// Waveform types
#define FASTSID_TRIANGLE 0x10
#define FASTSID_SAW      0x20
#define FASTSID_PULSE    0x40
#define FASTSID_NOISE    0x80

// Filter types
#define FASTSID_HIGH_PASS 0x40
#define FASTSID_BAND_PASS 0x20
#define FASTSID_LOW_PASS  0x10

// ADSR state (Attack, Decay, Sustain, Release)
#define FASTSID_ATTACK   0
#define FASTSID_DECAY    1
#define FASTSID_SUSTAIN  2
#define FASTSID_RELEASE  3
#define FASTSID_IDLE     4

// Noise magic

// The noise waveform of the SID is generated by a simple 23 bit LFSR.
// On shifting, bit 0 is filled with bit 22 EXOR bit 17.
#define NSHIFT(v, n) \
(((v) << (n))    \
| ((((v) >> (23 - (n))) ^ (v >> (18 - (n)))) & ((1 << (n)) - 1)))

#define NVALUE(v)                                   \
(noiseLSB[v & 0xff] | noiseMID[(v >> 8) & 0xff] \
| noiseMSB[(v >> 16) & 0xff])

#define NSEED 0x7ffff8


class FastVoice : public SubComponent {
    
    friend class FastSID;
    
    // Result of the latest inspection
    mutable SIDInfo info = { };
    mutable VoiceInfo voiceInfo[3] = { };
    
    // Wave tables
    // The first index determines the chip model (0 = old, 1 = new).
    static u16 wavetable10[2][4096];
    static u16 wavetable20[2][4096];
    static u16 wavetable30[2][4096];
    static u16 wavetable40[2][8192];
    static u16 wavetable50[2][8192];
    static u16 wavetable60[2][8192];
    static u16 wavetable70[2][8192];
    
    // Noise tables
    static u8 noiseMSB[256];
    static u8 noiseMID[256];
    static u8 noiseLSB[256];
    
    // The SID voice which is represented by this object (1,2, or 3)
    isize nr;

    // Pointer to parent SID object
    class FastSID *fastsid;
    
    // Pointer to previous voice
    FastVoice *prev;
    
    // Pointer to SID registers controlling this voice
    u8 *sidreg;
    

    //
    // Wave tables
    //
    
    // Pointer to the active wavetable
    u16 *wavetable;
    
    // Wavetable offset
    /* This 32-bit offset is added to the counter before referencing the
     * wavetable. It is used when other waveforms are combined with pulse
     */
    u32 waveTableOffset;
    
    // Counter value
    u32 waveTableCounter;
    
    // Counter steps
    /* After each sample, the counter is incremented by this amount.
     */
    u32 step;
    
    // Set to true if the oscillator should ring modulate
    bool ringmod;
    
    //
    // Waveform generator
    //
    
    // Current envelope phase (ATTACK, DECAY, SUSTAIN, RELEASE, or IDLE)
    u8 adsrm;
    
    // 31-bit adsr counter
    u32 adsr;
    
    // adsr counter step per sample
    i32 adsrInc;
    
    // adsr sustain level comparision value
    u32 adsrCmp;
    
    
    //
    // Noise generator
    //
    
    /* Noise shift register
     * The Noise waveform is created using a 23-bit pseudo-random sequence
     * generator (Linear Feedback Shift Register, LSFR).
     */
    u32 lsfr;
    
    
    //
    // Filter
    //
    
    // Filter related variables
    signed char filterIO;
    u8 filterType;
    double filterLow;
    double filterRef;
    double filterDy;
    double filterResDy;
    
    
    //
    // Initializing
    //
    
public:
 
    using SubComponent::SubComponent;
    
    static void initWaveTables();
    void init(FastSID *owner, isize voiceNr, FastVoice *prevVoice);

    
    //
    // Methods from CoreObject
    //

private:
    
    const char *getDescription() const override { return "FastVoice"; }

    
    //
    // Methods from CoreComponent
    //
    
private:
    
    void _reset(bool hard) override;
    
    template <class T>
    void serialize(T& worker)
    {
        worker
        
        << waveTableOffset
        << waveTableCounter
        << step
        << ringmod
        << adsrm
        << adsr
        << adsrInc
        << adsrCmp
        << lsfr
        << filterIO
        << filterType
        << filterLow
        << filterRef
        << filterDy
        << filterResDy;
    }
    
    void newserialize(util::SerChecker &worker) override { serialize(worker); }
    void newserialize(util::SerCounter &worker) override { serialize(worker); }
    void newserialize(util::SerResetter &worker) override { serialize(worker); }
    void newserialize(util::SerReader &worker) override;
    void newserialize(util::SerWriter &worker) override { serialize(worker); }

    /*
    isize _size() override { COMPUTE_SNAPSHOT_SIZE }
    isize _load(const u8 *buffer) override { LOAD_SNAPSHOT_ITEMS }
    isize _save(u8 *buffer) override { SAVE_SNAPSHOT_ITEMS }
    isize didLoadFromBuffer(const u8 *buffer) override;
    */

    //
    // Analyzing
    //
    
public:
    
    // SIDInfo getInfo() const { return CoreComponent::getInfo(info); }
    VoiceInfo getVoiceInfo(isize nr) const { return voiceInfo[nr]; }
   
    
    //
    // Accessing
    //
    
public:
        
    // Updates the wavetable pointer
    void updateWaveTablePtr();

    // Updates internal data structures
    // This method is called on each voice related register change
    void updateInternals(bool gateBitFlipped);

    // Sets the current filter type
    void setFilterType(u8 type);

    // Change ADSR state and all related variables
    void set_adsr(u8 fm);
    
    // ADSR counter triggered state change
    void trigger_adsr();
    
    // 15-bit oscillator value
    u32 doosc();
    
    // Apply filter effect
    void applyFilter();
    
    //
    // Querying configuration items
    //
        
    // Returns the currently set oscillator frequency
    u16 frequency() { return HI_LO(sidreg[0x01], sidreg[0x00]); }

    /* Returns the pulse width of the pulse waveform
     * The pulse width is a 12-bit number which linearly controls the pulse
     * width (duty cycle) of the pulse waveform.
     */
    u16 pulseWidth() { return (u16)((sidreg[3] & 0x0F) << 8 | sidreg[0x02]); }

    /* Returns the GATE bit for this voice
     * The gate bit controls the Envelope Generator. When this bit is set to a
     * one, the Envelope Generator is Gated (triggered) and the
     * attack/decay/sustain cycle is initiated.  When the bit is reset to a
     * zero, the release cycle begins.
     */
    bool gateBit() { return sidreg[0x04] & 0x01; }
    
    /* Returns the SYNC bit for this voice
     * If this bit is set, hard sync effects are produced. Hard sync is where
     * one waveform plays at its own frequency, but gets reset back to its
     * start every time the second waveform loops. It is responsible for the
     * rising modulating sound in Ben Daglish's Wilderness music from The Last
     * Ninja and the ludicrous intro noise in Martin Galway's Roland's Rat Race
     * music.
     */
    bool syncBit() { return (sidreg[0x04] & 0x02) != 0; }
    
    /* Returns the RING MOD bit of the control register
     * The RING MOD bit, when set to a one, replaces the Triangle waveform
     * output of Oscillator 1 with a “Ring Modulated” combination of
     * Oscillators 1 and 3.
     */
    bool ringModBit() { return (sidreg[0x04] & 0x04) != 0; }
    
    /* Returns the TEST bit of the control register
     * The TEST bit, when set to a one, resets and locks Oscillator 1 at zero
     * until the TEST bit is cleared. The Noise waveform output of Oscillator 1
     * is also reset and the Pulse waveform output is held at a DC level.
     */
    bool testBit() { return (sidreg[0x04] & 0x08) != 0; }

    // Returns the waveform bits of the control register
    u8 waveform() { return sidreg[0x04] & 0xF0; }
    
    /* Returns the attack rate for the envelope generator
     * The attack rate is a 4 bit value which determines how rapidly the output
     * of the voice rises from zero to peak amplitude when the envelope
     * generator is gated.
     */
    u8 attackRate() { return sidreg[0x05] >> 4; }

    /* Returns the decay rate for the envelope generator
     * The decay cycle follows the attack cycle and the decay rate determines
     * how rapidly the output falls from the peak amplitude to the selected
     * sustain level.
     */
    u8 decayRate() { return sidreg[0x05] & 0x0F; }

    /* Returns the decay rate for the envelope generator
     * The sustain cycle follows the decay cycle and the output of the voice
     * will remain at the selected sustain amplitude as long as the gate bit
     * remains set. The sustain levels range from zero to peak amplitude in 16
     * linear steps, with a sustain value of 0 selecting zero amplitude and a
     * sustain value of 15 selecting the peak amplitude. A sustain value of 8
     * would cause the voice to sustain at an amplitude one-half the peak
     * amplitude reached by the attack cycle.
     */
    u8 sustainRate() { return sidreg[0x06] >> 4; }

    /* Returns the release rate for the envelope generator
     * The release cycle follows the sustain cycle when the Gate bit is reset
     * to zero. At this time, the output of Voice 1 will fall from the sustain
     * amplitude to zero amplitude at the selected release rate. The 16 release
     * rates are identical to the decay rates.
     */
    u8 releaseRate() { return sidreg[0x06] & 0x0F; }
};
