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


void run_midi(void){

	printf("run_midi!\n");

	Dac::setup();

    synth.begin();                          // Start synth sample/mixing on Timer2 ISR	
	// Midi::begin(31250);                 // Start receiving MIDI messages via USART.
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

	for(int i = 0; i < 40000; i++){
		synth.isr();
	}

}