/*
 * (C) 2007-2010 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: directory_op.h 18 2010-10-12 09:45:55Z nayan@taobao.com $
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *      - initial release
 *
 */
#ifndef TFS_COMMON_DIRECTORYOP_H_
#define TFS_COMMON_DIRECTORYOP_H_

#include "define.h"
namespace tfs
{
  namespace common
  {
#ifndef S_IRWXUGO
#define S_IRWXUGO (S_IRWXU | S_IRWXG | S_IRWXO)
#endif

    class DirectoryOp
    {
    public:
      static bool exists(const char* filename);
      static bool delete_file(const char* filename);
      static bool is_directory(const char* dirname);
      static bool delete_directory(const char *dirname);
      static bool delete_directory_recursively(const char* directory, bool delete_flag = false);
      static bool create_directory(const char* dirname);
      static bool create_full_path(const char* fullpath, bool with_file = false);
      static bool rename(const char* srcfilename, const char* destfilename);
      static int64_t get_size(const char* filename);
    };

  }
}

#endif //TFS_COMMON_DIRECTORYOP_H_
