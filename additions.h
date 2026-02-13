#ifndef ADDITIONS_H
#define ADDITIONS_H

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include <mosquitto.h>

// Structure to hold action code and subscribe state
struct mqtt_client_context {
    char action_code[100];
    int subscribed;
};

// Test functions
int get_temperature(); // for MQTT testing
int get_humidity(); // for HTTP testing

// Function declarations
cJSON* parse_rpc_mqtt(const struct mosquitto_message *message, const char* action_code);
size_t rpc_response_callback_http(void *ptr, size_t size, size_t nmemb, void *userdata);

#endif