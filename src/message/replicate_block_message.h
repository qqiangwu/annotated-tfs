/*
 * (C) 2007-2010 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: replicate_block_message.h 5 2010-09-29 07:44:56Z duanfei@taobao.com $
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *      - initial release
 *
 */
#ifndef TFS_MESSAGE_REPLICATEBLOCKMESSAGE_H_
#define TFS_MESSAGE_REPLICATEBLOCKMESSAGE_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <errno.h>

#include "common/interval.h"
#include "message.h"

namespace tfs
{
  namespace message
  {
    // ReplicateBlock
    enum CommandStatus
    {
      COMMAND_REPLICATE = 1,
      COMMAND_REPL_COMPLETE,
      COMMAND_REPL_FAILURE,
      COMMAND_CLIENT_REPL = 100
    };

    class ReplicateBlockMessage: public Message
    {
    public:
      ReplicateBlockMessage();
      virtual ~ReplicateBlockMessage();
      virtual int parse(char* data, int32_t len);
      virtual int build(char* data, int32_t len);
      virtual int32_t message_length();
      virtual char* get_name();
      static Message* create(const int32_t type);

      inline void set_command(const int32_t command)
      {
        command_ = command;
      }

      inline int32_t get_command() const
      {
        return command_;
      }

      inline void set_expire(const int32_t expire)
      {
        expire_ = expire;
      }

      inline int32_t get_expire() const
      {
        return expire_;
      }

      inline void set_repl_block(const common::ReplBlock* repl_block)
      {
        memcpy(&repl_block_, repl_block, sizeof(common::ReplBlock));
      }

      inline const common::ReplBlock* get_repl_block() const
      {
        return &repl_block_;
      }

    protected:
      int32_t command_;
      int32_t expire_;
      common::ReplBlock repl_block_;
    };
  }
}
#endif
