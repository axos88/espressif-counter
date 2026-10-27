# LVGL on Zephyr RTOS

Zephyr workspace application for [LVGL](https://lvgl.io/) and the
[LVGL Editor](https://lvgl.io/pro). Design your UI in the Editor, export to `ui/`,
and build for your target board or the PC simulator.

## Prerequisites

You'll need a working Zephyr development environment. Follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)
and complete these sections:

1. **Select and Update OS** and **Install dependencies** — the host tools
   (CMake, Python, and the devicetree compiler).
2. **Get Zephyr and install Python dependencies** — a virtual environment,
   `west`, the Zephyr CMake package, and Zephyr's Python dependencies.
3. **Install the Zephyr SDK** — the
   [compiler toolchains](https://docs.zephyrproject.org/latest/develop/toolchains/zephyr_sdk.html).

In section 2 you can skip sub-step 4 ("Get the Zephyr source code"): this
repository is already its own west workspace, so `west init`/`west update` are
covered by the [Quick start](#quick-start) below. Running it anyway is harmless.

To run the simulator on your PC, you also need SDL2
(`sudo apt install libsdl2-dev` on Ubuntu/Debian).

## Quick start

```sh
west init -l manifest
west update
```

## Run on your PC (simulator)

```sh
west build -b native_sim/native/64 -t run
```

## Run on your development board

```sh
west blobs fetch hal_espressif && west build -p -b m5stack_core2/esp32/procpu
```

Then flash and debug:

```sh
west flash
west debug
```

## LVGL Editor UI

Export C code from the LVGL Editor into the `ui/` folder. The application picks it up
automatically when present.

## Support

- [LVGL documentation](https://lvgl.io/docs)
- [LVGL on Zephyr](https://lvgl.io/docs/details/integration/rtos/zephyr)
- [Report issues](https://github.com/lvgl/lvgl_editor/issues)
