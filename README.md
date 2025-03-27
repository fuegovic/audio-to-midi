# MOD Audio to MIDI

An LV2 plugin that converts audio signals to MIDI notes, designed for the MOD platform.

## Description

This plugin analyzes incoming audio signals and converts them to MIDI notes in real-time. It features:
- Audio to MIDI note conversion
- Adjustable threshold for note detection
- Configurable MIDI velocity
- Bypass switch

## Dependencies

To build the plugin, you need:
- LV2 development headers
- GCC or compatible C compiler
- GNU Make

## Building

```bash
make
```

## Installation

```bash
make install
```

By default, the plugin will be installed to `/usr/local/lib/lv2/`.

## Usage

1. Load the plugin in your LV2 host
2. Connect an audio source to the input
3. Connect the MIDI output to a synthesizer or other MIDI-capable device
4. Adjust the threshold to set the note triggering sensitivity
5. Use the velocity control to set the MIDI note velocity

## Controls

- **Threshold**: Sets the audio level threshold for note detection (-60dB to 0dB)
- **Velocity**: Controls the velocity of the generated MIDI notes (0-127)
- **Bypass**: Enables/disables the plugin

## License

This project is licensed under the GPL-2.0-or-later License - see the LICENSE file for details.

## Author

[Your Name]