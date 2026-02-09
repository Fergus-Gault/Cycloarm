#ifndef MOTION_H
#define MOTION_H
#include "cycloarm.h"

typedef struct
{
    motion_segment_t q[MOTION_BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} motion_q_t;

bool motion_enqueue(const motion_segment_t *seg);
motion_segment_t *motion_dequeue();
motion_segment_t *motion_peek();

static motion_q_t motion_q;

static inline bool motion_q_empty(void) { return motion_q.head == motion_q.tail; }
static inline bool motion_q_full(void) { return (uint8_t)(motion_q.head + 1) % MOTION_BUFFER_SIZE == motion_q.tail; }

#endif