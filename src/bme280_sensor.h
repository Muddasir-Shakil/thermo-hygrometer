#ifndef BME280_SENSOR_H_
#define BME280_SENSOR_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/dsp/print_format.h>

struct bme280_data
{
	double temperature;
	double humidity;
};

const struct device *check_bme280_device(void);
int get_bme280_data(struct bme280_data *data);
#endif