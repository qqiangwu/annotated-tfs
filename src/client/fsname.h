/*
 * (C) 2007-2010 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: fsname.h 5 2010-09-29 07:44:56Z duanfei@taobao.com $
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *      - initial release
 *
 */
#ifndef TFS_CLIENT_FSNAME_H_
#define TFS_CLIENT_FSNAME_H_

#include <string>
#include "common/define.h"

/* A simply value class for file names */
namespace tfs {
    namespace client {
        struct FileBits {
            uint32_t block_id_;
            int32_t seq_id_;      //< file id
            int32_t prefix_;
        };

        class FSName {
        public:
            FSName();

            FSName(uint32_t block_id, int32_t seq_id, int32_t prefix = 0, int32_t cluster_id = 0);

            FSName(const char* file_name, const char* prefix = NULL, int32_t cluster_id = 0);

            virtual ~FSName();

            /* get string rep for network transport */
            const char* get_name();

            int set_name(const char* file_name, const char* prefix = NULL, const int32_t cluster_id = 0);

            void set_prefix(const char* prefix);

            /* get readable name for displaying */
            std::string to_string();

            inline void set_block_id(const uint32_t id)
            {
                file_.block_id_ = id;
            }

            inline void set_seq_id(const int32_t id)
            {
                file_.seq_id_ = id;
            }

            inline void set_prefix(const int32_t id)
            {
                file_.prefix_ = id;
            }

            /* [file.seq_id_, file.prefix_] = id */
            inline void set_file_id(const uint64_t id)
            {
                *(reinterpret_cast<uint64_t*> (const_cast<int32_t*> (&file_.seq_id_))) = id;
            }

            inline void set_cluster_id(const int32_t cluster_id)
            {
                cluster_id_ = cluster_id;
            }

            inline int32_t get_cluster_id() const
            {
                return cluster_id_;
            }

            inline uint32_t get_block_id() const
            {
                return file_.block_id_;
            }

            inline int32_t get_seq_id() const
            {
                return file_.seq_id_;
            }

            inline int32_t get_prefix() const
            {
                return file_.prefix_;
            }

            inline uint64_t get_file_id() const
            {
                return *(reinterpret_cast<uint64_t*> (const_cast<int32_t*> (&file_.seq_id_)));
            }

        private:
            FileBits file_;
            int32_t cluster_id_;
            char file_name_[common::TFS_FILE_LEN];

            /* encoding and decoding details */
            void encode(const char* input, char* output);

            void decode(const char* input, char* output);
        };
    }
}
#endif
