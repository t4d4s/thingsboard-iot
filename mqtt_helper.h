#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mosquitto.h>
#include <cjson/cJSON.h>

// Callbacks
void on_connect(struct mosquitto *mosq, void *obj, int reason_code);
void on_disconnect(struct mosquitto *mosq, void *obj, int rc);
void on_publish(struct mosquitto *mosq, void *obj, int mid);
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);

// Test functions
int get_temperature();

// Function declarations
void publish_data(struct mosquitto *mosq, const char* topic);
void run_mqtt_client(const char* access_token, const char* topic, const char* action_code);

#endif