/*
This file defines system diagnostics and management functions.
*/
#ifndef SYSTEM_H
#define SYSTEM_H
#include "cycloarm.h"

status_comb_t get_status();
status_board_t get_board_status(uint8_t board_ident);

#endif