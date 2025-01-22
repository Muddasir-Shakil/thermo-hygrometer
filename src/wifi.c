#include "wifi.h"

LOG_MODULE_REGISTER(wifi);
#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASSWORD CONFIG_WIFI_PASSWORD

static int connected;
static struct net_mgmt_event_callback wifi_shell_mgmt_cb;

static void handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
	const struct wifi_status *status = (const struct wifi_status *)
					   cb->info;

	if (status->status) {
		LOG_ERR("Connection request failed (%d)", status->status);
	} else {
		LOG_INF("WIFI Connected");
		connected = 1;
	}
}

static void handle_wifi_disconnect_result(struct net_mgmt_event_callback *cb)
{
	const struct wifi_status *status = (const struct wifi_status *)
					   cb->info;

	if (status->status) {
		LOG_ERR("Disconnection request failed (%d)", status->status);
	} else {
		LOG_INF("WIFI Disconnect");
		connected = 0;
	}
}

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
				    uint32_t mgmt_event, struct net_if *iface)
{
	switch (mgmt_event) {
	case NET_EVENT_WIFI_CONNECT_RESULT:
		handle_wifi_connect_result(cb);
		break;
    case NET_EVENT_WIFI_DISCONNECT_RESULT:
        handle_wifi_disconnect_result(cb);
		break;
	default:
		break;
	}
}

void wifi_connect(const char* ssid, const char* password)
{
	int nr_tries = 10;
	int ret = 0;

	net_mgmt_init_event_callback(&wifi_shell_mgmt_cb,
				     wifi_mgmt_event_handler,
				     NET_EVENT_WIFI_CONNECT_RESULT);

	net_mgmt_add_event_callback(&wifi_shell_mgmt_cb);

	struct net_if *iface = net_if_get_default();
	struct wifi_connect_req_params cnx_params = {
		.ssid = ssid,
		.ssid_length = 0,
		.psk = password,
		.psk_length = 0,
		.channel = 0,
		.security = WIFI_SECURITY_TYPE_PSK,
	};

	cnx_params.ssid_length = strlen(ssid);
	cnx_params.psk_length = strlen(password);

	connected = 0;

	LOG_INF("WIFI try connecting to %s...", ssid);

	/* Let's wait few seconds to allow wifi device be on-line */
	while (nr_tries-- > 0) {
		ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &cnx_params,
			       sizeof(struct wifi_connect_req_params));
		if (ret == 0) {
			break;
		}

		LOG_INF("Connect request failed %d. Waiting iface be up...", ret);
		k_msleep(500);
	}

	while (connected == 0) {
		k_msleep(100);
	}
}
