#ifndef WIFI_H_
#define WIFI_H_
#include <zephyr/logging/log.h>

#include <zephyr/net/wifi_mgmt.h>
void wifi_connect(const char* ssid, const char* password);

#endif