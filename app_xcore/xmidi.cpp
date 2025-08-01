#include <stdio.h>

#include "midi.h"
#include "midisynth.h"
#include "instruments.h"
#include "instruments_generated.h"


// prevent mangling so we can call from XC
extern "C" {
    void run_midi();
}

MidiSynth synth;

// The below thunks are invoked during Midi::Dispatch() and forwarded to our MidiSynth.
void noteOn(uint8_t channel, uint8_t note, uint8_t velocity)		    { synth.midiNoteOn(channel, note, velocity); }
void noteOff(uint8_t channel, uint8_t note)							            { synth.midiNoteOff(channel, note); }
void sysex(uint8_t cbData, uint8_t data[])							            { /* do nothing */ }
void controlChange(uint8_t channel, uint8_t control, uint8_t value) { synth.midiControlChange(channel, control, value); }
void programChange(uint8_t channel, uint8_t value)					        { synth.midiProgramChange(channel, value); }
void pitchBend(uint8_t channel, int16_t value)						          { synth.midiPitchBend(channel, value); }

const uint8_t doom_riff[] = {
    // Header Chunk
    0x4D, 0x54, 0x68, 0x64,   // "MThd"
    0x00, 0x00, 0x00, 0x06,   // Header length = 6
    0x00, 0x00,               // Format type 0
    0x00, 0x01,               // One track
    0x00, 0x60,               // Division = 96 ticks per quarter note

    // Track Chunk
    0x4D, 0x54, 0x72, 0x6B,   // "MTrk"

    // Track length placeholder (updated below)
    0x00, 0x00, 0x00, 0x2A,   // Chunk length = 42 bytes

    // Events
    0x00, 0xC0, 0x1E,         // Program change: channel 0, instrument 30 (Distortion Guitar)
    0x00, 0xFF, 0x51, 0x03, 0x07, 0xA1, 0x20, // Set tempo: 500000 microseconds per quarter note (120 BPM)
    
    0x00, 0x90, 0x28, 0x60,   // Note On: E2
    0x30, 0x80, 0x28, 0x40,   // Note Off: E2

    0x00, 0x90, 0x2A, 0x60,   // Note On: G2
    0x30, 0x80, 0x2A, 0x40,   // Note Off: G2

    0x00, 0x90, 0x2C, 0x60,   // Note On: A2
    0x30, 0x80, 0x2C, 0x40,   // Note Off: A2

    0x00, 0x90, 0x2E, 0x60,   // Note On: B2
    0x30, 0x80, 0x2E, 0x40,   // Note Off: B2

    0x00, 0xFF, 0x2F, 0x00    // End of Track
};

void run_midi(void){

	printf("run_midi!\n");

	Dac::setup();

    synth.begin();                          // Start synth sample/mixing on Timer2 ISR
    Midi midi = Midi();
	Midi::begin(31250);                 // Start receiving MIDI messages via USART.
    printf("Sample rate: %f\n", Synth::sampleRate);

	// const Instrument& instrument = instruments[1];

	// for (int i = 0; i < Synth::numVoices; i++) {
	for (int i = 0; i < Synth::numVoices; i++) {
	    uint8_t note = 60 + 4 * i; // Use different notes for clarity
	    // synth.noteOn(i, note, 127, instrument);

		noteOn(i, note, 127);
		
		int volume = i < 2 ? 127 : 0;
		synth.setVolume(i, volume);
		// synth.noteOff(i);
	}

	for(int i = 0; i < sizeof(doom_riff); i++){
		midi.enqueue(doom_riff[i]);
	}
	midi.dispatch();

	for(int i = 0; i < 40000; i++){
		synth.isr();
	}

}