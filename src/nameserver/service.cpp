/*
 * (C) 2007-2010 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: service.cpp 50 2010-11-17 02:04:35Z nayan@taobao.com $
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
#include <exception>
#include <tbsys.h>
#include <Memory.hpp>
#include "service.h"
#include "message/client_pool.h"
#include "common/config_item.h"
#include "common/directory_op.h"
#include "common/error_msg.h"

using namespace tbsys;
using namespace tfs::common;
using namespace tfs::nameserver;

static void signal_handler(int32_t sig);
static Service* g_tfs_ns_service_ = NULL;

// check whether the local ip is in the ip list.
static int in_ip_list(uint64_t local_ip)
{
  const char *ip_list = CONFIG.get_string_value(CONFIG_NAMESERVER, CONF_IP_ADDR_LIST);
  if (ip_list == NULL)
  {
    return 0;
  }
  char buffer[1024];
  strcpy(buffer, ip_list);
  char *token = NULL;
  char *str = buffer;
  while ((token = strsep(&str, "|")) != NULL)
  {
    uint64_t ip = Func::get_addr(token);
    if (ip == local_ip)
    {
      return 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
  int32_t i;
  bool is_daemon = false;
  bool is_help = false;
  std::string conf_file_path;

  // get input argument
  while ((i = getopt(argc, argv, "f:dh")) != EOF)
  {
    switch (i)
    {
    case 'f':
      conf_file_path = optarg;
      break;
    case 'd':
      is_daemon = true;
      break;
    case 'h':
    default:
      is_help = true;
    break;
    }
  }

  if (conf_file_path.empty() || conf_file_path == " " || is_help)
  {
    fprintf(stderr, "\nUsage: %s -f conf_file -d -h\n", argv[0]);
    fprintf(stderr, "  -f conf_file   config file path\n");
    fprintf(stderr, "  -d             run daemon\n");
    fprintf(stderr, "  -h             help\n");
    fprintf(stderr, "\n");
    return EXIT_GENERAL_ERROR;
  }

  // load config file
  int ret = TFS_SUCCESS;
  if ((ret = SysParam::instance().load(conf_file_path.c_str())) != TFS_SUCCESS)
  {
    fprintf(stderr, "SysParam::load failed:%s", conf_file_path.c_str());
    return ret;
  }
  TBSYS_LOGGER.setLogLevel(CONFIG.get_string_value(CONFIG_PUBLIC, CONF_LOG_LEVEL, "debug"));
	TBSYS_LOGGER.setMaxFileSize(CONFIG.get_int_value(CONFIG_PUBLIC, CONF_LOG_SIZE, 0x40000000));
	TBSYS_LOGGER.setMaxFileIndex(CONFIG.get_int_value(CONFIG_PUBLIC, CONF_LOG_NUM, 0x0A));
  const char *ip = CONFIG.get_string_value(CONFIG_NAMESERVER, CONF_IP_ADDR);
  const char *dev_name = CONFIG.get_string_value(CONFIG_NAMESERVER, CONF_DEV_NAME);
  uint64_t local_ip = Func::get_local_addr(dev_name);
  if (Func::get_addr(ip) != local_ip && in_ip_list(local_ip) == 0)
  {
    TBSYS_LOG(ERROR, "ip '%s' is not local ip, local ip: %s", ip, tbsys::CNetUtil::addrToString(local_ip).c_str());
    return EXIT_GENERAL_ERROR;
  }

  const char *work_dir = SYSPARAM_NAMESERVER.work_dir_.c_str();
  if (!DirectoryOp::create_full_path(work_dir))
  {
    TBSYS_LOG(ERROR, "create directory(%s) failed", work_dir);
    return EXIT_GENERAL_ERROR;
  }

  const char *pid_file = SYSPARAM_NAMESERVER.pid_file_.c_str();
  int32_t pid = 0;
  if ((pid = tbsys::CProcess::existPid(pid_file)))
  {
    TBSYS_LOG(ERROR, "nameserver has already run. Pid: %d", pid);
    return EXIT_SYSTEM_ERROR;
  }
  if (!DirectoryOp::create_full_path(pid_file, true))
  {
    TBSYS_LOG(ERROR, "create file(%s)'s directory failed", pid_file);
    return EXIT_GENERAL_ERROR;
  }

  const char *log_file = SYSPARAM_NAMESERVER.log_file_.c_str();
  if (access(log_file, R_OK) == 0)
  {
    TBSYS_LOGGER.rotateLog(log_file);
  }
  else if (!DirectoryOp::create_full_path(log_file, true))
  {
    TBSYS_LOG(ERROR, "create file(%s)'s directory failed", log_file);
    return EXIT_GENERAL_ERROR;
  }

  // start daemon process
  pid = 0;
  if (is_daemon)
  {
    pid = Func::start_daemon(pid_file, log_file);
  }

  // start service
  if (pid == 0)
  {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGUSR1, SIG_IGN);
    signal(40, SIG_IGN);

    try
    {
      ARG_NEW(g_tfs_ns_service_, Service);
      ret = g_tfs_ns_service_->start();
    }
    catch (std::exception& ex)
    {
      TBSYS_LOG(WARN, "Catch execption (%s), must be exit...", ex.what());
      g_tfs_ns_service_->stop();
    }
    catch (...)
    {
      TBSYS_LOG(WARN, "Catch unknow execption, must be exit...");
      g_tfs_ns_service_->stop();
    }
    tbsys::gDelete(g_tfs_ns_service_);
  }
  return ret;
}

static void signal_handler(int32_t sig)
{
  switch (sig)
  {
  case SIGTERM:
  case SIGINT:
    g_tfs_ns_service_->stop();
    break;
  case SIGSEGV:
  case SIGABRT:
    //TBSYS_LOG(DEBUG,sig == SIGSEGV ? "*SIGSEGV*" : "*SIGABRT*");
    //TBSYS_LOG(ERROR, const_cast<char*>(sig == SIGSEGV ? "*SIGSEGV*" : "*SIGABRT*"));
    break;
  case 40:
    TBSYS_LOGGER.checkFile();
    break;
  }
}

namespace tfs
{

  namespace nameserver
  {

    Service::Service()
    {
    }

    Service::~Service()
    {
    }

    int Service::start()
    {
      TBSYS_LOGGER.setMaxFileSize();
      TBSYS_LOGGER.setMaxFileIndex();
      if (fs_name_system_.start() == TFS_SUCCESS)
      {
        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
        signal(SIGUSR1, signal_handler);
        signal(40, signal_handler);
        fs_name_system_.wait();
      }
      const char *pid_file_path = CONFIG.get_string_value(CONFIG_NAMESERVER, CONF_LOCK_FILE);
      unlink(pid_file_path);
      TBSYS_LOG(INFO, "nameserver exit");
      return TFS_SUCCESS;
    }

    void Service::stop()
    {
      fs_name_system_.stop();
    }
  }
}

