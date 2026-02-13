#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

// Includes
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>

// Function declarations
int get_humidity();
void subscribe_rpc_commands(const char *access_token, const char* action_code);
void run_http_client(const char *access_token, const char* action_code);

#endif