#include "ringbuffer.h"
#include <stdint.h>

void ringbuffer_init(RingBuffer *buf) {
    memset(buf->buffer, 0, RING_BUFFER_LENGTH);
    buf->head = 0;
    buf->tail = 0;
}

int ringbuffer_read(RingBuffer *buf, uint8_t *data, uint8_t length) {
    int i;
    for (i = 0; i < length; i++) {
        if (buf->tail == buf->head) {
            return i;
        }

        data[i] = buf->buffer[buf->tail];
        buf->tail = (buf->tail + 1) % RING_BUFFER_LENGTH;
    }

    return i + 1;
}

int ringbuffer_write(RingBuffer *buf, uint8_t *data, uint8_t length) {
    if (ringbuffer_free_space(buf) < length) {
        return 0;
    }

    for (int i = 0; i < length; i++) {
        buf->buffer[buf->head] = data[i];
        buf->head = (buf->head + 1) % RING_BUFFER_LENGTH;
    }

    return 1;
}

int ringbuffer_free_space(RingBuffer *buf) {
    if (buf->head == buf->tail) {
        return RING_BUFFER_LENGTH - 1;
    }

    if (buf->head > buf->tail) {
        return RING_BUFFER_LENGTH - ((buf->head - buf->tail) + 1);
    }

    return (buf->tail - buf->head) - 1;
}

int ringbuffer_data_space(RingBuffer *buf) {
    return RING_BUFFER_LENGTH - (ringbuffer_free_space(buf) + 1);
}

int ringbuffer_has_data(RingBuffer *buf) {
    int space = ringbuffer_data_space(buf);
    return space > 0;
}
