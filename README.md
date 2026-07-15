# Smart thermo-hygrometer

Zephyr firmware for the ESP32-WROOM DevKitC. The board target is
`esp32_devkitc_wroom/esp32/procpu`.

## First-time setup

Zephyr 3.7.0, its Python environment, modules, and SDK 0.16.8 are installed
inside the workspace. Nothing is registered or installed globally.

From the workspace root (`thermo-hygrometer-dev`), run:

```sh
app/scripts/setup-zephyr.sh
```

The setup script installs only the ESP32 Xtensa toolchain and starts the local
Mosquitto MQTT broker through the project's `broker-start` Taskfile command.
Generated files live in `.venv`, `.west`, `.cache`, and `deps` at the workspace
root. Run it once after cloning the project.

The firmware connects to the broker address configured by
`MQTT_BROKER_ADDRESS` in `Kconfig`. Update it if this computer's LAN address
changes.

## Start a development shell

Run this once in every new terminal:

```sh
source app/zephyr-env.sh
```

This selects the project-local Python, West, CMake, Zephyr SDK, and ccache. It
does not require a separate virtual environment activation.

## Build

```sh
west build -d app/build -p auto app
```

Clean rebuild:

```sh
west build -d app/build -p always app
```

## Flash

```sh
west flash -d app/build
```

If automatic port detection fails, find the board's serial port and specify it:

```sh
ls /dev/cu.*
west flash -d app/build --esp-device <PORT>
```

## Monitor the console

Use the Espressif monitor with the built ELF file:

```sh
west espressif -p <PORT> -b 115200 -e app/build/zephyr/zephyr.elf monitor
```

Exit the Espressif monitor with `Ctrl-]`.
