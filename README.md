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

## Learning roadmap

- [ ] Show Wi-Fi and MQTT connection status on the display.
- [ ] Track minimum and maximum temperature and humidity.
- [ ] Add comfort states such as dry, comfortable, and humid.
- [ ] Add multiple LVGL screens and a small history graph.
- [ ] Split sensor, display, and MQTT work into separate Zephyr threads.
- [ ] Pass sensor readings between threads with a Zephyr message queue.
- [ ] Reconnect automatically after Wi-Fi or MQTT connection loss.
- [ ] Publish retained status and MQTT Last Will messages.
- [ ] Publish sensor readings as structured JSON.
- [ ] Subscribe to MQTT commands for display mode and sampling interval.
- [ ] Store runtime thresholds and intervals with Zephyr Settings/NVS.
- [ ] Add a devicetree-configured status LED and interrupt-driven button.
- [ ] Add high-temperature and high-humidity alarms.
- [ ] Blank the display when idle and experiment with ESP32 sleep modes.
- [ ] Add a hardware watchdog and subsystem error counters.
- [ ] Reduce memory usage by disabling unused shell, debug, and LVGL features.
- [ ] Add MQTT over TLS after reducing the current DRAM footprint.
- [ ] Add Ztest coverage for thresholds, payload formatting, and error handling.
