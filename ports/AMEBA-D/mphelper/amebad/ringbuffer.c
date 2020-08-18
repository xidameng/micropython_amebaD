#include "ringbuffer.h"
#include <string.h>


void ringbuf_init(void)
{
    ringbuf_t self;
    memset((void *)self._aucBuffer, 0, SERIAL_BUFFER_SIZE);
    self._iHead=0;
    self._iTail=0;
}

void ringbuf_store_char(uint8_t c)
{
    ringbuf_t self;
    int i = (uint32_t)(self._iHead + 1) % SERIAL_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != self._iTail) {
        self._aucBuffer[self._iHead] = c;
        self._iHead = i;
    }
}
