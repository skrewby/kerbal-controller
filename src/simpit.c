#include "simpit.h"
#include "simpit_message_types.h"
#include "stm32l433xx.h"
#include "timers.h"
#include "uart.h"
#include <stdint.h>

const char KERBALSIMPIT_VERSION[] =
    "2.4.0"; /**< Library version sent to the plugin for compatibility checking.
              */

static int received_index = 0;

// Taken directly from
// https://github.com/Simpit-team/KerbalSimpitRevamped-Arduino/blob/main/src/KerbalSimpit.cpp
// and modified slightly
int cobsEncode(const void *data, int length, uint8_t *buffer) {
  uint8_t *encode = buffer;  // Encoded byte pointer
  uint8_t *codep = encode++; // Output code pointer
  uint8_t code = 1;          // Code value

  for (const uint8_t *byte = (const uint8_t *)data; length--; ++byte) {
    if (*byte) // Byte not zero, write it
      *encode++ = *byte, ++code;

    if (!*byte || code == 0xff) // Input is zero or block completed, restart
    {
      *codep = code, code = 1, codep = encode;
      if (!*byte || length)
        ++encode;
    }
  }
  *codep = code; // Write final code value

  return (int)(encode - buffer);
}

int cobsDecode(const uint8_t *buffer, int length, void *data) {
  const uint8_t *byte = buffer;      // Encoded input byte pointer
  uint8_t *decode = (uint8_t *)data; // Decoded output byte pointer

  for (uint8_t code = 0xff, block = 0; byte < buffer + length; --block) {
    if (block) // Decode block byte
      *decode++ = *byte++;
    else {
      if (code != 0xff) // Encoded zero, write it
        *decode++ = 0;
      block = code = *byte++; // Next block length
      if (!code)              // Delimiter code found
        break;
    }
  }

  return (int)(decode - (uint8_t *)data);
}

void simpit_send(uint8_t type, char *msg, int len) {
  uint8_t buffer[MAX_PAYLOAD_SIZE + 2];

  buffer[0] = type;
  uint8_t checksum = type;

  for (uint8_t i = 0; i < len; i++) {
    buffer[i + 1] = msg[i];
    checksum ^= msg[i];
  }
  buffer[len + 1] = checksum;

  uint8_t payload[MAX_PAYLOAD_SIZE + 4];
  uint8_t encoded_size = cobsEncode(buffer, len + 2, payload);
  for (uint8_t i = 0; i < encoded_size; i++) {
    uart_write(payload[i]);
  }
  uart_write(0x00);
}

// We expect the buffer to be of size MAX_MESSAGE_SIZE + HEADER_SIZE
uint8_t simpit_read(uint8_t *buf, int len) {
  uint8_t count = 0;

  while (uart_read_available()) {
    if (count >= len)
      return count;

    buf[count] = uart_read();
    count++;
  }

  return count;
}

int simpit_init() {
  uart_empty_buffer();

  uint8_t payload[MAX_PAYLOAD_SIZE + 4];
  payload[0] = 0x00;
  uint32_t i = 0;
  for (i = 0; i < sizeof(KERBALSIMPIT_VERSION); i++) {
    payload[i + 1] = KERBALSIMPIT_VERSION[i];
  }
  i += 1;
  simpit_send(0x00, (char *)payload, i); // Send SYN

  // Wait for response
  int count = 0;
  while (!uart_read_available()) {
    count += 1;
    delay_ms(100);
    if (count > 100)
      return 0;
  }

  int index = 0;
  uint8_t buffer[MAX_PAYLOAD_SIZE + 4];
  uint8_t decoded_buffer[MAX_PAYLOAD_SIZE + 4];
  GPIOB->ODR = 0 << 13;
  while (uart_read_available()) {
    if (index >= MAX_PAYLOAD_SIZE + 4)
      return 0;

    buffer[index] = uart_read();
    if (buffer[index] == 0x00) {
      cobsDecode(buffer, index + 1, decoded_buffer);
      GPIOB->ODR = 0 << 13;

      // Test if the message received is a SYNACK
      if (decoded_buffer[0] == SYNC_MESSAGE && decoded_buffer[1] == 0x01) {
        payload[0] = 0x02;
        simpit_send(0x00, (char *)payload, i); // Send ACK
        return 1;
      } else {
        return 0;
      }
    } else {
      GPIOB->ODR = 1 << 13;
    }
    index++;
  }

  return 0;
}

void simpit_print(char *str, uint8_t options) {
  int max_length = sizeof(str);
  uint8_t payload[32];
  payload[0] = options;

  for (uint8_t i = 0; i < 31; i++) {
    if (str[i] == '\0') {
      payload[i + 1] = '\0';
      break;
    }

    payload[i + 1] = str[i];
  }

  simpit_send(CUSTOM_LOG, (char *)payload, 32);
}

void simpit_update() {
  uint8_t buffer[MAX_PAYLOAD_SIZE + 4];
  uint8_t decoded_buffer[MAX_PAYLOAD_SIZE + 4];

  while (uart_read_available()) {
    buffer[received_index] = uart_read();
    received_index++;

    if (!buffer[received_index - 1]) {
      uint8_t decoded_size = cobsDecode(buffer, received_index, decoded_buffer);

      // Account for the overhead of 1 byte of COBS
      if (decoded_size == received_index - 1) {
        uint8_t checksum = 0;
        for (uint8_t i = 0; i < decoded_size - 2; i++) {
          checksum ^= decoded_buffer[i];
        }

        if (checksum == decoded_buffer[decoded_size - 2]) {
          message_handler(decoded_buffer[0], decoded_buffer + 1,
                          decoded_size - 3);
        }
      }
      received_index = 0;
    }
  }
}

void activate_action(uint8_t action) {
  simpit_send(AGACTIVATE_MESSAGE, (char *)&action, 1);
}
