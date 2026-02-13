#include "additions.h"

// Generates random temperature for sending (testing purposes)
int get_temperature() {
    sleep(1);
    int temp = rand() % 100;
    printf("[DEBUG] Generated temperature value: %d\n", temp);
    return temp;
}

// Generates random humidity value for sending (testing purposes)
int get_humidity() {
    sleep(1);
    int hum = rand() % 100;
    printf("[DEBUG] Generated humidity value: %d\n", hum);
    return hum;
}

// Parses the RPC from MQTT message
cJSON* parse_rpc_mqtt(const struct mosquitto_message *message, const char* action_code) {
    printf("[RPC] JSON payload received\n");
    cJSON* root = cJSON_Parse((char *)message->payload);

    if (!root) {
        printf("[ERROR] Failed to parse JSON\n");
        return NULL;
    }

    cJSON *method = cJSON_GetObjectItem(root, "method");
    if (cJSON_IsString(method) && method->valuestring) {
        if (strcmp(method->valuestring, action_code) == 0) {
            printf("[RPC] Action %s received\n", action_code);
            printf("[ACTION] Performing action!!!\n");
        } else {
            printf("[RPC] Different action code received, skipping..\n");
        }
    }

    return root;
}

// Callback function to handle RPC responses in HTTP
size_t rpc_response_callback_http(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    char *response = (char *)ptr;

    // Cast userdata to the context structure (mqtt_client_context)
    struct mqtt_client_context *context = (struct mqtt_client_context *)userdata;

    // Print the raw response
    printf("[INFO] RPC Response: %.*s\n", (int)total_size, response);

    // Parse JSON response
    cJSON *json = cJSON_Parse(response);
    if (json) {
        cJSON *method = cJSON_GetObjectItem(json, "method");
        if (method && cJSON_IsString(method)) {
            printf("[INFO] RPC Method: %s\n", method->valuestring);
            if (strcmp(method->valuestring, context->action_code) == 0) {
                printf("[ACTION-HTTP] Performing action\n");
            }
            else
            {
                printf("[ACTION-HTTP] Different action received, skipping..\n");
            }
        }
        cJSON_Delete(json);
    } else {
        printf("[ERROR] Failed to parse JSON response.\n");
    }

    return total_size;
}
