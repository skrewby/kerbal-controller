#ifndef _SIMPIT_H
#define _SIMPIT_H

#include <stdint.h>

#define MAX_PAYLOAD_SIZE 32   // Does not include headers if any exist (usually 4 bytes)
uint8_t simpit_read(uint8_t* buf, int len);
void simpit_print(char* str, uint8_t options);
int simpit_init();
void simpit_send(uint8_t type, char *msg, int len);
void simpit_update();
void message_handler(uint8_t type, uint8_t *msg, uint8_t length);

#endif
