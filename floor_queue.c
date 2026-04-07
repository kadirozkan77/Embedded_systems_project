#include "floor_queue.h"

static int16_t queue[MAX_QUEUE_SIZE];
static uint8_t front = 0;
static uint8_t rear = 0;
static uint8_t count = 0;

void queue_init(void)
{
    front = 0;
    rear = 0;
    count = 0;
}

bool enqueue_floor(int16_t floor)
{
    if (count >= MAX_QUEUE_SIZE) {
        return false;
    }

    queue[rear] = floor;
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    count++;

    return true;
}

bool dequeue_floor(int16_t *floor)
{
    if (count == 0) {
        return false;
    }

    *floor = queue[front];
    front = (front + 1) % MAX_QUEUE_SIZE;
    count--;

    return true;
}

bool is_queue_empty(void)
{
    return (count == 0);
}

void clear_queue(void)
{
    front = 0;
    rear = 0;
    count = 0;
}