#ifndef UTILS_RING_BUFFER_H
#define UTILS_RING_BUFFER_H

#include <stdint.h>
#include <string.h>

#define RING_BUFFER_LENGTH 0xFF

typedef struct {
  uint8_t buffer[RING_BUFFER_LENGTH];
  uint8_t head;
  uint8_t tail;
} RingBuffer;

void ringbuffer_init(RingBuffer *buf);
// Returns number of bytes read
int ringbuffer_read(RingBuffer *buf, uint8_t* data, uint8_t length);
int ringbuffer_write(RingBuffer *buf, uint8_t* data, uint8_t length);
int ringbuffer_free_space(RingBuffer *buf);
int ringbuffer_data_space(RingBuffer *buf);
int ringbuffer_has_data(RingBuffer *buf);

#endif // UTILS_RING_BUFFER_H
