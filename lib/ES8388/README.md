# ES8388 (project-local copy)

ES8388 audio-codec driver, based on
[vanbwodonk/es8388arduino](https://github.com/vanbwodonk/es8388arduino) at commit `ce15dd0`
(GPL-2.0, see `LICENSE`).

These are the files that were hand-patched inside `.pio/libdeps/` for the pre-v3 build of this fork
(and therefore never were under version control), taken over unchanged except for one point.

## Changes against the library (from the original pre-v3 patch)

| Change | Effect |
| --- | --- |
| Register access via ESPuino's `i2cBusTwo` | The codec shares the external I2C bus with the port-expander (SDA = GPIO13, SCL = GPIO4). |
| `ES8388()` default constructor | Objects can be created without starting a bus (main, AudioPlayer, System). |
| `power_down()` | Mutes ADC and DAC, then powers down DEM/STM, ADC, analog inputs, mic bias, DAC and outputs (user-guide sequence). Called before deep sleep and before the restart on an operation-mode change. |
| `init()`: ADC input differential LIN1/RIN1 (`0xF0`/`0x00`), PGA gain 0 dB (`0x00` instead of `0x77` = +21 dB), mixer source ADC (`0x1B`), mixer DAC + ADC (`0xD0`) | Microphone path as wired in this box. |
| `setALCmode(VOICE)`: PGA gain 0 dB (`0x00` instead of `0x77`) | Otherwise this call would raise the mic gain to +21 dB again, after `setInputGain(0)`. |
| Destructor no longer destroys the bus | |

## Change made during the merge with upstream v3.2

The unused `TwoWire i2c = TwoWire(0)` member was removed (the 3-argument constructor and
`identify()` use `Wire` instead; ESPuino calls neither). Under Arduino-ESP32 3.x, destroying a
`TwoWire` object calls `end()`, which shuts the hardware bus down for every object on that bus. The
local `ES8388` in `setup()` would therefore have switched off I2C bus 0 at the end of `setup()`
whenever something else was using it.

## I2S

`DACCONTROL1 = 0x18` (16-bit word length, I2S format, MCLK = 256 * Fs) stays as it is although
ESP32-audioI2S v4 sends 32-bit slots: the library author's own ES8388 example for v4 uses exactly
this value, with MCLK on GPIO0.
