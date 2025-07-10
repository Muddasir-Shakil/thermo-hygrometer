/*
 * Copyright (c) 2018 Jan Van Winkel <jan.van_winkel@dxplore.eu>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl_input_device.h>
#include "bme280_sensor.h"
#include "wifi.h"
#include "mqtt_publisher.h"
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASSWORD CONFIG_WIFI_PASSWORD
#define MQTT_HOST CONFIG_MQTT_HOST
#define MQTT_USERNAME CONFIG_MQTT_PASSWORD
#define MQTT_PASSWORD CONFIG_MQTT_PASSWORD
// /* The mqtt client struct */
static struct mqtt_client client_ctx;
int main(void)
{   
#ifdef DISPLAY_ON
	const struct device *display_dev;
	// struct mqtt_client client_ctx;
	lv_obj_t *bme280_data_label;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}
	
    bme280_data_label = lv_label_create(lv_scr_act());
	lv_obj_align(bme280_data_label, LV_ALIGN_CENTER, 0, 0);
 	lv_task_handler();	
 	display_blanking_off(display_dev);
#endif
	if (check_bme280_device() == NULL)
    {	
		return -1;
    }

	wifi_connect(WIFI_SSID, WIFI_PASSWORD);
	start_publisher(&client_ctx);
	char data_str[100] = {0};
	char temperature_str[100] = {0};
	char humidity_str[100] = {0}; 
	
	while (1) {
		struct bme280_data data;
		if(get_bme280_data(&data) == -1){
#ifdef DISPLAY_ON
			lv_label_set_text(bme280_data_label, "Data Error");
#else
			LOG_ERR("Data Error");
#endif	
		} else {
			sprintf(temperature_str,"%.1f", data.temperature);
			sprintf(humidity_str,"%.1f", data.humidity);
			sprintf(data_str, "%.1f C \n%.1f %%", data.temperature, data.humidity);
#ifdef DISPLAY_ON
			lv_label_set_text(bme280_data_label, data_str);
#endif
			publish(&client_ctx, MQTT_QOS_0_AT_MOST_ONCE, "Temperature", temperature_str);
			publish(&client_ctx, MQTT_QOS_0_AT_MOST_ONCE, "Humidity", humidity_str);
		}
#ifdef DISPLAY_ON
		lv_task_handler();
#endif
		process_mqtt_and_sleep(&client_ctx);
		k_sleep(K_MSEC(1000));
	}
	return 0;
}
