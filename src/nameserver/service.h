/*
 * (C) 2007-2010 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: service.h 5 2010-09-29 07:44:56Z duanfei@taobao.com $
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *      - initial release
 *   qushan<qushan@taobao.com> 
 *      - modify 2009-03-27
 *   duanfei <duanfei@taobao.com> 
 *      - modify 2010-04-23
 *
 */
#ifndef TFS_NAMESERVER_SERVICE_H_
#define TFS_NAMESERVER_SERVICE_H_

#include <tbsys.h>
#include <tbnet.h>

#include "nameserver.h"
#include "common/func.h"
#include "common/config.h"
#include "message/tfs_packet_streamer.h"
#include "message/message_factory.h"

namespace tfs
{
  namespace nameserver
  {
    class Service
    {
    public:
      Service();
      virtual ~Service();
      int start();
      void stop();
    private:
      DISALLOW_COPY_AND_ASSIGN( Service);
      NameServer fs_name_system_;
    };
  }
}

#endif
