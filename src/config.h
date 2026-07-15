/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define SERVER_ADDR		CONFIG_MQTT_BROKER_ADDRESS
#define SERVER_PORT		CONFIG_MQTT_BROKER_PORT

#define APP_CONNECT_TIMEOUT_MS	2000
#define APP_MQTT_POLL_TIMEOUT_MS	50
#define APP_RECONNECT_DELAY_MS	1000

#define APP_CONNECT_TRIES	10

#define APP_MQTT_BUFFER_SIZE	128

#define MQTT_CLIENTID		"zephyr_publisher"

/* Set the following to 1 to enable the Bluemix topic format */
#define APP_BLUEMIX_TOPIC	0

/* These are the parameters for the Bluemix topic format */
#if APP_BLUEMIX_TOPIC
#define BLUEMIX_DEVTYPE		"sensor"
#define BLUEMIX_DEVID		"carbon"
#define BLUEMIX_EVENT		"status"
#define BLUEMIX_FORMAT		"json"
#endif

#endif
