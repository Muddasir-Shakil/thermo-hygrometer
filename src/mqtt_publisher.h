#ifndef MQTT_PUBLISHER_H_
#define MQTT_PUBLISHER_H_
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>

#include <string.h>
#include <errno.h>

#include "config.h"

int start_publisher(struct mqtt_client *client);
int process_mqtt_and_sleep(struct mqtt_client *client);
int publish(struct mqtt_client *client, enum mqtt_qos qos, const char* topic, const char* message);
#endif