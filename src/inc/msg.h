#ifndef MSG_H
#define MSG_H
#include <stdint.h>
#include <wchar.h>

#define MAX_MSG_SIZE 128

struct Message
{
  int32_t header;
  wchar_t msg[MAX_MSG_SIZE];
};

#endif
