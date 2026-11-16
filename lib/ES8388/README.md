# ES8388 (project-local copy)

ES8388 audio-codec driver, taken from
[vanbwodonk/es8388arduino](https://github.com/vanbwodonk/es8388arduino) at commit `ce15dd0`
(GPL-2.0, see `LICENSE`).

The ESPuino code of this fork needs three things the upstream library does not offer, which is why a
copy lives here instead of a `lib_deps` entry:

| Addition | Why |
| --- | --- |
| `ES8388()` default constructor + `ES8388::setBus(TwoWire&)` | The codec is used from several source files and may share its I2C bus with the port-expander. All instances use the one bus selected with `setBus()`. |
| `power_down()` | Standby before deep sleep / restart (no pops, lower quiescent current). |
| Destructor no longer shuts the I2C bus down | The bus is owned by the application. |

The original constructor `ES8388(sda, scl, speed)` is still there and starts `Wire` on those pins.

**Note:** before the merge with upstream v3.2 these additions only existed as hand edits inside
`.pio/libdeps/`, which is not under version control. The versions here are a reconstruction. If you
still have the original patched `ES8388.h` / `ES8388.cpp`, compare them against `src/` — in
particular the register sequence in `power_down()` and the I2C pins.
