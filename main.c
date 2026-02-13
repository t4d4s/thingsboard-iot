#include "mqtt_helper.h"
#include "http_helper.h"
#include "signal_handler.h"

// v1/devices/me/telemetry

int main(int argc, char *argv[]) {
    char access_token[100];
    char topic[100];
    char action_code[100];
    int use_mqtt = 0;

    // Setting up the signal handlers to prevent memory leaks
    setup_signal_handler();

    // If we have provided an access token in program's arguments
    if (argc > 1) {
        strncpy(access_token, argv[1], sizeof(access_token) - 1);
        access_token[sizeof(access_token) - 1] = '\0';
    } else {
        printf("[INFO] Enter access token: ");
        if (scanf("%99s", access_token) != 1) {
            fprintf(stderr, "[ERROR] Failed to read access token.\n");
            return 1;
        }
    }

    // If we have provided a mode (mqtt or http) in the arguments
    if (argc > 2) {
        if (strcmp(argv[2], "mqtt") == 0) {
            use_mqtt = 1;
        } else if (strcmp(argv[2], "http") == 0) {
            use_mqtt = 0;
        } else {
            fprintf(stderr, "[ERROR] Invalid mode. Use 'mqtt' or 'http'.\n");
            return 1;
        }
    } else {
        // Default mode is MQTT if not specified otherwise
        printf("[INFO] No mode specified, defaulting to MQTT.\n");
        use_mqtt = 1;
    }

    // If MQTT mode, get the topic and action code as user input
    if (use_mqtt) {
        printf("[INFO] Enter MQTT topic: ");
        if (scanf("%99s", topic) != 1) {
            fprintf(stderr, "[ERROR] Failed to read MQTT topic.\n");
            return 1;
        }
        printf("[RPC] Enter action code: ");
        if (scanf("%99s", action_code) != 1) {
            fprintf(stderr, "[ERROR] Failed to read action code.\n");
            return 1;
        }
        printf("[INFO] Running MQTT client...\n");
        run_mqtt_client(access_token, topic, action_code);
    } else {
        printf("[RPC] Enter action code: ");
        if (scanf("%99s", action_code) != 1) {
            fprintf(stderr, "[ERROR] Failed to read action code.\n");
            return 1;
        }
        printf("[INFO] Running HTTP client...\n");
        run_http_client(access_token, action_code);
    }

    return 0;
}
