cmake_minimum_required(VERSION 3.20.0)

set(BOARD esp32_devkitc_wroom/esp32/procpu)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(thermo-hygrometer)

zephyr_include_directories(src)

target_sources(app PRIVATE src/main.c src/bme280_sensor.h src/bme280_sensor.c src/wifi.h src/wifi.c 
    src/mqtt_publisher.c src/mqtt_publisher.h)
        