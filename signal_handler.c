#include <syslog.h>
#include "signal_handler.h"

volatile sig_atomic_t exit_flag = 0;

// Signal handler function
void handle_signal(int signal) {
    switch (signal) {
        case SIGINT:
             printf("[WARN] Caught SIGINT (Interrupt). Exiting...\n");
            break;
        case SIGTERM:
             printf("[WARN] Caught SIGTERM (Terminate). Exiting...\n");
            break;
        case SIGQUIT:
             printf("[WARN] Caught SIGQUIT (Quit). Exiting...\n");
            break;
        default:
             printf("[WARN] Caught unknown signal (%d). Exiting...\n", signal);
            break;
    }
    exit_flag = 1;
}

// Function to set up signal handling
void setup_signal_handler() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    // Register signals
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    printf("[INFO] Signal handler setup complete.\n");
}