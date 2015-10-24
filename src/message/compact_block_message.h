/*
 * (C) 2007-2010 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: compact_block_message.h 5 2010-09-29 07:44:56Z duanfei@taobao.com $
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *      - initial release
 *
 */
#ifndef TFS_MESSAGE_COMPACTBLOCKMESSAGE_H_
#define TFS_MESSAGE_COMPACTBLOCKMESSAGE_H_

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
    class CompactBlockMessage: public Message
    {
      public:
        CompactBlockMessage();
        virtual ~CompactBlockMessage();
        virtual int parse(char* data, const int32_t len);
        virtual int build(char* data, const int32_t len);
        virtual int32_t message_length();
        virtual char* get_name();
        static Message* create(const int32_t type);

        inline void set_preserve_time(const int32_t preserve_time)
        {
          preserve_time_ = preserve_time;
        }
        inline void set_block_id(const uint32_t block_id)
        {
          block_id_ = block_id;
        }
        inline void set_owner(const int32_t owner)
        {
          is_owner_ = owner;
        }

        inline int32_t get_preserve_time() const
        {
          return preserve_time_;
        }
        inline uint32_t get_block_id() const
        {
          return block_id_;
        }
        inline int32_t get_owner() const
        {
          return is_owner_;
        }

      protected:
        int32_t preserve_time_;
        uint32_t block_id_;
        int32_t is_owner_;
    };

    class CompactBlockCompleteMessage: public Message
    {
      public:
        CompactBlockCompleteMessage();
        virtual ~CompactBlockCompleteMessage();
        virtual int parse(char* data, int32_t len);
        virtual int build(char* data, int32_t len);
        virtual int32_t message_length();
        virtual char* get_name();
        inline void set_block_id(const uint32_t block_id)
        {
          block_id_ = block_id;
        }

        inline uint32_t get_block_id() const
        {
          return block_id_;
        }

        inline void set_server_id(const uint64_t server_id)
        {
          server_id_ = server_id;
        }

        inline uint64_t get_server_id() const
        {
          return server_id_;
        }

        inline void set_block_info(common::BlockInfo* const block_info)
        {
          block_info_ = block_info;
        }

        inline common::BlockInfo* get_block_info() const
        {
          return block_info_;
        }

        inline void set_success(const common::CompactCompleteStatus success)
        {
          success_ = success;
        }

        inline int32_t get_success() const
        {
          return success_;
        }

        inline void set_flag(const uint8_t flag)
        {
          flag_ = flag;
        }

        inline uint32_t get_flag() const
        {
          return flag_;
        }

        inline void set_ds_list(const common::VUINT64& ds_list)
        {
          ds_list_.assign(ds_list.begin(), ds_list.end());
        }

        inline const common::VUINT64& get_ds_list() const
        {
          return ds_list_;
        }

        static Message* create(const int32_t type);

      protected:
        uint32_t block_id_;
        int32_t success_;
        uint64_t server_id_;
        common::BlockInfo* block_info_;
        uint32_t flag_;
        common::VUINT64 ds_list_;

    };
  }
}
#endif
