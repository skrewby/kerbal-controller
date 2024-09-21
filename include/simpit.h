#ifndef _SIMPIT_H
#define _SIMPIT_H

#include <stdint.h>

#define MAX_PAYLOAD_SIZE 32   // Does not include headers if any exist (usually 4 bytes)
uint8_t simpit_read(uint8_t* buf, int len);
int simpit_init();

#endif
