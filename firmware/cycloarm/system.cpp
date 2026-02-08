#include "cycloarm.h"

status_comb_t get_status()
{
    status_board_t board_a_status = get_board_status(BOARD_A_IDENT);
    status_board_t board_b_status = get_board_status(BOARD_B_IDENT);
    status_comb_t combined_status;
    combined_status.board_a = board_a_status;
    combined_status.board_b = board_b_status;
    return combined_status;
}

status_board_t get_board_status(uint8_t board_ident)
{
    status_board_t board_status;
#ifdef BOARD_A
    board_status.num_motors = sizeof(board_a_steppers) / sizeof(AccelStepper);
    board_status.last_command = *last_command;
#endif
#ifdef BOARD_B
    board_status.num_motors = sizeof(board_b_steppers) / sizeof(AccelStepper);
    board_status.last_command = *last_command;
#endif
    return board_status;
}