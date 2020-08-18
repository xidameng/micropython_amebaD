#ifndef _RING_BUFFER_
#define _RING_BUFFER_

#include <stdint.h>

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
#define SERIAL_BUFFER_SIZE 128

typedef struct RingBuffer
{
    volatile uint8_t _aucBuffer[SERIAL_BUFFER_SIZE];
    volatile int _iHead;
    volatile int _iTail;

} ringbuf_t;

void ringbuf_init(void);
void ringbuf_store_char(uint8_t c);

#endif /* _RING_BUFFER_ */
