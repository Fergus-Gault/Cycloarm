#include "cycloarm.h"

bool motion_enqueue(const motion_segment_t *seg)
{
    print_string("Adding motion segment to queue.");
    if (motion_q_full())
        return false;

    motion_q.q[motion_q.tail] = *seg;
    motion_q.tail++;
    return true;
}

motion_segment_t *motion_dequeue()
{
    if (motion_q_empty())
        return nullptr;

    // Progress the head
    motion_q.head++;
    // Return the motion seg
    return &motion_q.q[motion_q.head - 1];
}

motion_segment_t *motion_peek()
{
    if (motion_q_empty())
        return nullptr;

    return &motion_q.q[motion_q.head];
}