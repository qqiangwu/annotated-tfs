/*
 * (C) 2007-2010 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: close_file_message.h 5 2010-09-29 07:44:56Z duanfei@taobao.com $
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *      - initial release
 *
 */
#ifndef TFS_MESSAGE_CLOSEFILEMESSAGE_H_
#define TFS_MESSAGE_CLOSEFILEMESSAGE_H_

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
    class CloseFileMessage: public Message
    {
      public:
        CloseFileMessage();
        virtual ~CloseFileMessage();

        inline void set_block_id(const uint32_t block_id)
        {
          close_file_info_.block_id_ = block_id;
        }
        inline void set_file_id(const uint64_t file_id)
        {
          close_file_info_.file_id_ = file_id;
        }
        inline void set_mode(const common::CloseFileServer mode)
        {
          close_file_info_.mode_ = mode;
        }
        inline void set_crc(const uint32_t crc)
        {
          close_file_info_.crc_ = crc;
        }
        inline void set_file_number(const uint64_t file_number)
        {
          close_file_info_.file_number_ = file_number;
        }
        inline void set_ds_list(const common::VUINT64& ds)
        {
          ds_ = ds;
        }
        inline void set_block(common::BlockInfo* const block_info)
        {
          block_ = block_info;
        }
        inline void set_file_info(common::FileInfo* const file_info)
        {
          file_info_ = file_info;
        }
        inline void set_option_flag(const int32_t flag)
        {
          option_flag_ = flag;
        }
        inline void set_block_version(const int32_t version)
        {
          version_ = version;
          has_lease_ = true;
        }
        inline void set_lease_id(const int32_t lease_id)
        {
          lease_id_ = lease_id;
          has_lease_ = true;
        }
        inline uint32_t get_block_id() const
        {
          return close_file_info_.block_id_;
        }
        inline uint64_t get_file_id() const
        {
          return close_file_info_.file_id_;
        }
        inline common::CloseFileServer get_mode() const
        {
          return close_file_info_.mode_;
        }
        inline uint32_t get_crc() const
        {
          return close_file_info_.crc_;
        }
        inline uint64_t get_file_number() const
        {
          return close_file_info_.file_number_;
        }
        inline const common::VUINT64& get_ds_list() const
        {
          return ds_;
        }
        inline common::BlockInfo* get_block() const
        {
          return block_;
        }
        inline common::FileInfo* get_file_info() const
        {
          return file_info_;
        }
        inline int32_t get_option_flag() const
        {
          return option_flag_;
        }
        inline int32_t get_block_version() const
        {
          return version_;
        }
        inline int32_t get_lease_id() const
        {
          return lease_id_;
        }
        inline common::CloseFileInfo get_close_file_info() const
        {
          return close_file_info_;
        }

        virtual int parse(char* data, int32_t len);
        virtual int build(char* data, int32_t len);
        virtual int message_length();
        virtual char* get_name();

        static Message* create(const int32_t type);
      protected:
        common::CloseFileInfo close_file_info_;
        common::BlockInfo* block_;
        common::FileInfo* file_info_;
        common::VUINT64 ds_;
        int32_t option_flag_;
        int32_t version_;
        int32_t lease_id_;
        bool has_lease_;
    };
  }
}
#endif
