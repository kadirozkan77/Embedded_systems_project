#include "floor_queue.h"

static int16_t queue_data[FLOOR_QUEUE_SIZE];
static uint8_t queue_front = 0;
static uint8_t queue_rear = 0;
static uint8_t queue_count = 0;

void floor_queue_init(void)
{
    queue_front = 0;
    queue_rear = 0;
    queue_count = 0;
}

uint8_t floor_queue_is_empty(void)
{
    return (queue_count == 0);
}

uint8_t floor_queue_is_full(void)
{
    return (queue_count >= FLOOR_QUEUE_SIZE);
}

uint8_t floor_queue_contains(int16_t floor)
{
    uint8_t i;
    uint8_t index;

    for (i = 0; i < queue_count; i++)
    {
        index = (queue_front + i) % FLOOR_QUEUE_SIZE;
        if (queue_data[index] == floor)
        {
            return 1;
        }
    }
    return 0;
}

uint8_t floor_queue_enqueue(int16_t floor)
{
    if (floor_queue_is_full())
    {
        return 0;
    }

    queue_data[queue_rear] = floor;
    queue_rear = (queue_rear + 1) % FLOOR_QUEUE_SIZE;
    queue_count++;

    return 1;
}

uint8_t floor_queue_dequeue(int16_t *floor)
{
    if (floor_queue_is_empty())
    {
        return 0;
    }

    *floor = queue_data[queue_front];
    queue_front = (queue_front + 1) % FLOOR_QUEUE_SIZE;
    queue_count--;

    return 1;
}

void floor_queue_clear(void)
{
    queue_front = 0;
    queue_rear = 0;
    queue_count = 0;
}
