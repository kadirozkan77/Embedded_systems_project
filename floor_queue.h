#ifndef FLOOR_QUEUE_H
#define FLOOR_QUEUE_H

#include <stdint.h>

#define FLOOR_QUEUE_SIZE 10

void floor_queue_init(void);
uint8_t floor_queue_is_empty(void);
uint8_t floor_queue_is_full(void);
uint8_t floor_queue_enqueue(int16_t floor);
uint8_t floor_queue_dequeue(int16_t *floor);
uint8_t floor_queue_contains(int16_t floor);
void floor_queue_clear(void);

#endif
