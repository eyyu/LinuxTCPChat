/*------------------------------------------------------------------------------
-- Struct: Message
--
-- DATA MEMBERS:
-- int32_t header       -- for messag type
-- char username[32]    -- to store the user name of the Client
-- char msg[128]        -- to store the chat msg
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0 - Mar/22/17 - EY
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- NOTES:
-- This is the message structure that is communicated between server and client
------------------------------------------------------------------------------*/
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
