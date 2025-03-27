#include "lv2/core/lv2.h"
#include "lv2/atom/atom.h"
#include "lv2/atom/forge.h"
#include "lv2/midi/midi.h"
#include "lv2/urid/urid.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Port indices
typedef enum {
    AUDIO_IN = 0,
    MIDI_OUT,
    THRESHOLD,
    VELOCITY,
    BYPASS
} PortIndex;

typedef struct {
    // Port buffers
    const float* audio_in;
    LV2_Atom_Sequence* midi_out;
    const float* threshold;
    const float* velocity;
    const float* bypass;

    // Features
    LV2_URID_Map* map;
    LV2_Atom_Forge forge;
    
    // URIs
    struct {
        LV2_URID midi_MidiEvent;
    } uris;

    // Plugin state
    double rate;
    float prev_sample;
    uint8_t last_midi_note;
    bool note_on;
} AudioToMidi;

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
           double                     rate,
           const char*                bundle_path,
           const LV2_Feature* const* features)
{
    AudioToMidi* self = (AudioToMidi*)calloc(1, sizeof(AudioToMidi));
    if (!self) return NULL;

    // Get host features
    for (int i = 0; features[i]; ++i) {
        if (!strcmp(features[i]->URI, LV2_URID__map)) {
            self->map = (LV2_URID_Map*)features[i]->data;
        }
    }

    if (!self->map) {
        free(self);
        return NULL;
    }

    // Map URIs
    self->uris.midi_MidiEvent = self->map->map(self->map->handle, LV2_MIDI__MidiEvent);

    // Initialize forge
    lv2_atom_forge_init(&self->forge, self->map);

    self->rate = rate;
    self->prev_sample = 0.0f;
    self->last_midi_note = 0;
    self->note_on = false;

    return (LV2_Handle)self;
}

static void
connect_port(LV2_Handle instance,
            uint32_t   port,
            void*      data)
{
    AudioToMidi* self = (AudioToMidi*)instance;

    switch ((PortIndex)port) {
    case AUDIO_IN:
        self->audio_in = (const float*)data;
        break;
    case MIDI_OUT:
        self->midi_out = (LV2_Atom_Sequence*)data;
        break;
    case THRESHOLD:
        self->threshold = (const float*)data;
        break;
    case VELOCITY:
        self->velocity = (const float*)data;
        break;
    case BYPASS:
        self->bypass = (const float*)data;
        break;
    }
}

static void
activate(LV2_Handle instance)
{
    AudioToMidi* self = (AudioToMidi*)instance;
    self->prev_sample = 0.0f;
    self->last_midi_note = 0;
    self->note_on = false;
}

// Simple zero-crossing pitch detection
static uint8_t
detect_pitch(float sample, float prev_sample, float threshold)
{
    if (sample > threshold && prev_sample <= threshold) {
        // TODO: Implement actual pitch detection
        // For now, just return a constant note for testing
        return 60; // Middle C
    }
    return 0;
}

static void
run(LV2_Handle instance,
    uint32_t   n_samples)
{
    AudioToMidi* self = (AudioToMidi*)instance;

    const uint32_t out_capacity = self->midi_out->atom.size;
    
    // Set up forge to write to MIDI output port
    lv2_atom_forge_set_buffer(&self->forge,
                            (uint8_t*)self->midi_out,
                            out_capacity);
    
    // Start MIDI sequence
    LV2_Atom_Forge_Frame seq_frame;
    lv2_atom_forge_sequence_head(&self->forge, &seq_frame, 0);

    // Process audio and generate MIDI
    for (uint32_t i = 0; i < n_samples; ++i) {
        if (*self->bypass > 0.5f) continue;

        float sample = self->audio_in[i];
        uint8_t midi_note = detect_pitch(sample, self->prev_sample, *self->threshold);

        if (midi_note > 0 && !self->note_on) {
            uint8_t velocity = (uint8_t)(*self->velocity * 127.0f);
            uint8_t note_on[3] = { 0x90, midi_note, velocity }; // Note On, channel 1
            lv2_atom_forge_frame_time(&self->forge, i);
            lv2_atom_forge_atom(&self->forge, 3, self->uris.midi_MidiEvent);
            lv2_atom_forge_raw(&self->forge, note_on, 3);
            
            self->note_on = true;
            self->last_midi_note = midi_note;
        }
        else if (midi_note == 0 && self->note_on) {
            uint8_t note_off[3] = { 0x80, self->last_midi_note, 0 }; // Note Off, channel 1
            lv2_atom_forge_frame_time(&self->forge, i);
            lv2_atom_forge_atom(&self->forge, 3, self->uris.midi_MidiEvent);
            lv2_atom_forge_raw(&self->forge, note_off, 3);
            
            self->note_on = false;
        }

        self->prev_sample = sample;
    }

    // Close sequence
    lv2_atom_forge_pop(&self->forge, &seq_frame);
}

static void
deactivate(LV2_Handle instance)
{
    // Nothing to do
}

static void
cleanup(LV2_Handle instance)
{
    free(instance);
}

static const void*
extension_data(const char* uri)
{
    return NULL;
}

static const LV2_Descriptor descriptor = {
    "http://moddevices.com/plugins/mod-devel/mod-audio-to-midi",
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    return index == 0 ? &descriptor : NULL;
}