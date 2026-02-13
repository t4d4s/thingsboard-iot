#include "http_helper.h"
#include "signal_handler.h"
#include "additions.h"

// Function to subscribe to RPC commands
void subscribe_rpc_commands(const char *access_token, const char *action_code) {
    CURL *curl;
    CURLcode res;
    char url[256];

    snprintf(url, sizeof(url), "http://localhost:8080/api/v1/%s/rpc?timeout=5000", access_token);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");

        struct mqtt_client_context context = {0};
        strncpy(context.action_code, action_code, sizeof(context.action_code) - 1);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rpc_response_callback_http);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &context);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "[ERROR] cURL RPC subscription error: %s\n", curl_easy_strerror(res));
        } else {
            printf("[INFO] Subscribed to RPC commands\n");
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}


// Runs the HTTP client to send telemetry and subscribe to RPC commands
void run_http_client(const char *access_token, const char *action_code) {
    CURL *curl;
    CURLcode res;
    char url[256];
    char payload[256];

    while (!exit_flag) {
        subscribe_rpc_commands(access_token, action_code);
        int hum = get_humidity();
        snprintf(payload, sizeof(payload), "{\"humidity\": %d}", hum);
        printf("[HTTP] Sending message: %s\n", payload);
        snprintf(url, sizeof(url), "http://localhost:8080/api/v1/%s/telemetry", access_token);

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "[ERROR] cURL telemetry error: %s\n", curl_easy_strerror(res));
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
        sleep(5);
    }

    printf("[INFO] Exiting HTTP client loop...\n");
}
