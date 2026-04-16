#include "floor_queue.h"

static int16_t queue_data[FLOOR_QUEUE_SIZE]; //This creates the actual array that stores floor numbers.
//static means these variables are private to this file
static uint8_t queue_front = 0;//This points to the first item in the queue.
static uint8_t queue_rear = 0;//This points to the next free place where a new floor will be added

static uint8_t queue_count = 0;//This tells how many floors are currently stored.

void floor_queue_init(void)
{
    queue_front = 0;
    queue_rear = 0;
    queue_count = 0;
}

uint8_t floor_queue_is_empty(void)
{
    return (queue_count == 0);//if there is 0 items, return true values
}

uint8_t floor_queue_is_full(void)
{
    return (queue_count >= FLOOR_QUEUE_SIZE); //ifqueue already has the maximum number of requests
}

uint8_t floor_queue_contains(int16_t floor)
{
    uint8_t i;//i is loop counter
    uint8_t index;// index is position inside array

    for (i = 0; i < queue_count; i++)// starts from first item, runs as many times mentioned in count
    {
        index = (queue_front + i) % FLOOR_QUEUE_SIZE;//starts from front and move forward by 1; come backt to 0 if the system reaches the end of  array
        //floor_queue_size reuse old empty spaces.
        if (queue_data[index] == floor)//this compare stored floor with wanted floor
        {
            return 1;
        }
    }
    return 0;
}

uint8_t floor_queue_enqueue(int16_t floor) //add a floor to queue
{
    if (floor_queue_is_full())
    {
        return 0;
    }

    queue_data[queue_rear] = floor;//put new floor in current rear position
    queue_rear = (queue_rear + 1) % FLOOR_QUEUE_SIZE;//same logic as before
    queue_count++;

    return 1;
}

uint8_t floor_queue_dequeue(int16_t *floor)//creating FIFO
    if (floor_queue_is_empty())//if nothing is stored it wont remove anything
    {
        return 0;
    }

    *floor = queue_data[queue_front];//next floor
    queue_front = (queue_front + 1) % FLOOR_QUEUE_SIZE;//next item becomes the first item
    queue_count--;//count goes down

    return 1;
}

void floor_queue_clear(void)
{
    queue_front = 0;
    queue_rear = 0;
    queue_count = 0;
}
