#include "mqtt_helper.h"
#include "signal_handler.h"
#include "additions.h"

// On connect callback
void on_connect(struct mosquitto *mosq, void *obj, int reason_code) {
    struct mqtt_client_context *context = (struct mqtt_client_context *)obj;
    printf("[INFO] Connected to MQTT broker with reason code %d\n", reason_code);

    if (reason_code == 0) {
        // Subscribe to the RPC request topic on successful connection
        int rc = mosquitto_subscribe(mosq, NULL, "v1/devices/me/rpc/request/+", 0);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "[ERROR] Failed to subscribe: %s\n", mosquitto_strerror(rc));
        } else {
            printf("[INFO] Subscribed to topic v1/devices/me/rpc/request/+\n");
            context->subscribed = 1;
        }
    } else {
        printf("[ERROR] Connection failed.\n");
    }
}

// On disconnect callback
void on_disconnect(struct mosquitto *mosq, void *obj, int rc) {
    printf("[DEBUG] Disconnected: %s\n", mosquitto_strerror(rc));

    if (rc != MOSQ_ERR_SUCCESS) {
        printf("[WARN] Unexpected disconnection. Reconnecting...\n");
        mosquitto_reconnect(mosq);
    } else {
        printf("[INFO] Disconnected from the broker.\n");
    }
}

// Callback function that gets called when a message is received
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
    struct mqtt_client_context *context = (struct mqtt_client_context *)obj;

    printf("[INFO] Received message on topic %s\n", message->topic);
    printf("[INFO] Message body: %s\n", (char *)message->payload);   

    // Calling the parse function with the action code from user input
    cJSON* json_root = parse_rpc_mqtt(message, context->action_code);

    // Cleaning up
    if (json_root) {
        cJSON_Delete(json_root);
    }
}

// Publishes the data to our ThingsBoard topic
void publish_data(struct mosquitto *mosq, const char* topic) {
    char payload[50];
    int temp = get_temperature();
    snprintf(payload, sizeof(payload), "{\"temperature\":%d}", temp);

    printf("[MQTT] Publishing message: %s\n", payload);

    int rc = mosquitto_publish(mosq, NULL, topic, strlen(payload), payload, 1, false);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "[ERROR] Failed to publish message: %s\n", mosquitto_strerror(rc));
    }
}

// Runs the MQTT client based on the provided access token by the user
void run_mqtt_client(const char* access_token, const char* topic, const char* action_code) {
    struct mosquitto *mosq;
    int rc;

    // Initialize the client context structure
    struct mqtt_client_context context = {0};
    strncpy(context.action_code, action_code, sizeof(context.action_code) - 1);

    printf("[INFO] Initializing Mosquitto library...\n");
    mosquitto_lib_init();

    printf("[INFO] Creating a new MQTT client instance...\n");
    mosq = mosquitto_new(NULL, true, &context);

    if (!mosq) {
        fprintf(stderr, "[ERROR] Out of memory while creating MQTT client instance.\n");
        return;
    }

    mosquitto_username_pw_set(mosq, access_token, NULL);
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
    mosquitto_message_callback_set(mosq, on_message);
    
    printf("[INFO] Connecting to the MQTT broker...\n");
    rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "[ERROR] Connection failed: %s\n", mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return;
    }

    // Start the loop that will handle connection and messages
    while (!exit_flag) {
        if (context.subscribed) {
            publish_data(mosq, topic);
        } else {
            printf("[INFO] Waiting for subscription to complete...\n");
        }

        // Process the MQTT loop to receive messages and handle events
        rc = mosquitto_loop(mosq, -1, 1); // Calling callback functions
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "[ERROR] Error in MQTT loop: %s\n", mosquitto_strerror(rc));
            break;
        }

        sleep(5);
    }
    
    printf("[INFO] Cleaning up and shutting down...\n");
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}
