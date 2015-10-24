/*
 * (C) 2007-2010 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: crc_error_message.h 5 2010-09-29 07:44:56Z duanfei@taobao.com $
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *      - initial release
 *
 */
#ifndef TFS_MESSAGE_CRCERRORMESSAGE_H_
#define TFS_MESSAGE_CRCERRORMESSAGE_H_

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
    class CrcErrorMessage: public Message
    {
      public:
        CrcErrorMessage();
        virtual ~CrcErrorMessage();
        virtual int parse(char* data, int32_t len);
        virtual int build(char* data, int32_t len);
        virtual int32_t message_length();
        virtual char* get_name();
        static Message* create(const int32_t type);

        inline void set_block_id(const uint32_t block_id)
        {
          block_id_ = block_id;
        }

        inline uint32_t get_block_id() const
        {
          return block_id_;
        }

        inline void set_file_id(const uint64_t file_id)
        {
          file_id_ = file_id;
        }

        inline uint64_t get_file_id() const
        {
          return file_id_;
        }
        inline void set_crc(const uint32_t crc)
        {
          crc_ = crc;
        }

        inline uint32_t get_crc() const
        {
          return crc_;
        }
        inline void set_error_flag(const common::CheckDsBlockType flag)
        {
          error_flag_ = flag;
        }

        inline common::CheckDsBlockType get_error_flag() const
        {
          return error_flag_;
        }

        inline void add_fail_server(const uint64_t server_id)
        {
          fail_server_.push_back(server_id);
        }

        inline const common::VUINT64* get_fail_server() const
        {
          return &fail_server_;
        }
      protected:
        uint32_t block_id_;
        uint64_t file_id_;
        uint32_t crc_; // client report the correct crc
        common::CheckDsBlockType error_flag_;
        common::VUINT64 fail_server_;
    };
  }
}

#endif
