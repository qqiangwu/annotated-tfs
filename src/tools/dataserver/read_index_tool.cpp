/*
 * (C) 2007-2010 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: read_index_tool.cpp 5 2010-09-29 07:44:56Z duanfei@taobao.com $
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *      - initial release
 *   chuyu <chuyu@taobao.com>
 *      - modify 2010-03-20
 *
 */
#include <iostream>
#include <string>
#include "dataserver/index_handle.h"
#include "dataserver/file_op.h"

using namespace std;
using namespace tfs::dataserver;

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cout << "Usage: " << argv[0] << " filename " << endl;
    return -1;
  }

  cout << "index file: " << argv[1] << endl;

  string file = argv[1];
  FileOperation* file_op = new FileOperation(argv[1]);
  int32_t read_size = sizeof(IndexHeader);
  char buf[read_size];
  memset(buf, 0, read_size);

  int32_t ret = file_op->pread_file(buf, read_size, 0);
  if (ret)
  {
    cout << "pread file error" << ret << endl;
    return ret;
  }

  IndexHeader* idx_header = reinterpret_cast<IndexHeader*>(buf);

  cout << "blockid: " << idx_header->block_info_.block_id_ << endl;
  cout << "version: " << idx_header->block_info_.version_ << endl;
  cout << "filecount: " << idx_header->block_info_.file_count_ << endl;
  cout << "size: " << idx_header->block_info_.size_ << endl;
  cout << "delfilecount: " << idx_header->block_info_.del_file_count_ << endl;
  cout << "delsize: " << idx_header->block_info_.del_size_ << endl;
  cout << "seqno: " << idx_header->block_info_.seq_no_ << endl;

  cout << "flag: " << idx_header->flag_ << endl;
  cout << "bucket size: " << idx_header->bucket_size_ << endl;
  cout << "data file offset: " << idx_header->data_file_offset_ << endl;
  cout << "index file size: " << idx_header->index_file_size_ << endl;
  cout << "free head offset: " << idx_header->free_head_offset_ << endl;
  delete file_op;
  return 0;
}
