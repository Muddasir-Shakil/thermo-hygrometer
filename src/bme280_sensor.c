#include "bme280_sensor.h"
/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/dsp/print_format.h>

const struct device *dev;

const struct device *check_bme280_device(void)
{

    dev = DEVICE_DT_GET_ANY(bosch_bme280);
    if (dev == NULL)
    {
        /* No such node, or the node does not have status "okay". */
        printk("\nError: no device found.\n");
        return NULL;
    }

    if (!device_is_ready(dev))
    {
        printk("\nError: Device \"%s\" is not ready; "
               "check the driver initialization logs for errors.\n",
               dev->name);
        return NULL;
    }

    printk("Found device \"%s\", getting sensor data\n", dev->name);
    return dev;
}

int get_bme280_data(struct bme280_data *data)
{
    struct sensor_value temp, hum;
    double temp_db, hum_db;
    if (sensor_sample_fetch(dev) < 0)
    {
        printf("Sensor sample update error\n");
        return -1;
    }
    if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0)
    {
        printf("Cannot read BME280 temperature channel\n");
        return -1;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum) < 0)
    {
        printf("Cannot read BME280 Humidity channel\n");
        return -1;
    }

    temp_db = sensor_value_to_double(&temp);
    hum_db = sensor_value_to_double(&hum);
    if (data != NULL)
    {
        data->humidity = hum_db;
        data->temperature = temp_db;
    }
    else
    {

        printf("Temperature:%.1f C Humidity :%.1f %\n", temp_db, hum_db);
    }
    return 0;
}
