#ifndef MSG_H
#define MSG_H
#include <stdint.h>

#define MAX_MSG_SIZE         128
#define MAX_USERNAME_SIZE    32

struct Message
{
  int32_t header;
  char username[MAX_USERNAME_SIZE];
  char msg[MAX_MSG_SIZE];
};

#define MSG_SIZE sizeof(Message)

#endif
