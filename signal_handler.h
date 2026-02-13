#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handle_signal(int signal);
void setup_signal_handler();

extern volatile sig_atomic_t exit_flag;

#endif