#ifndef TFS_NAMESERVER_HEART_MANAGEMENT_
#define TFS_NAMESERVER_HEART_MANAGEMENT_

#include <Timer.h>
#include "replicate.h"
#include "meta_manager.h"
#include "proactor_data_pipe.h"
#include "message/message_factory.h"

//
//  HeartManagement:
//      a class for handling the heartbeat from ds using a pipeline
//      When the server received a heartbeat from a ds, the msg will be push into this pipeline.
//
//  Tasks:
//      MasterHeartTimerTask: check whether self is the master
//      MasterAndSlaveHeartManager: slave nameserver sends heartbeat to master
//
//  MasterAndSlaveHeartManager:
//      pipeline
//      process heartbeat msg from other nameservers.
namespace tfs
{
  namespace nameserver
  {
    class HeartManagement: public ProactorDataPipe<std::deque<message::Message*>, HeartManagement>
    {
    public:
      typedef ProactorDataPipe<std::deque<message::Message*>, HeartManagement> base_type;

    public:
      HeartManagement(MetaManager& m, ReplicateLauncher& l);
      virtual ~HeartManagement();

      int initialize(const int32_t thread_count, const int32_t max_queue_size);

      int execute(message::Message* msg, void* args);
      int push(message::Message* msg);

    private:
      DISALLOW_COPY_AND_ASSIGN( HeartManagement);
      int join_ds(message::Message* msg);

    private:
      MetaManager& meta_mgr_;
      ReplicateLauncher & replicate_lancher_;
      int32_t max_queue_size_;
    };

    // a class for checking whether it is master.
    class CheckOwnerIsMasterTimerTask: public tbutil::TimerTask
    {
    public:
      CheckOwnerIsMasterTimerTask(MetaManager* mm);
      virtual ~CheckOwnerIsMasterTimerTask()
      {
      }
      virtual void runTimerTask();
    private:
      DISALLOW_COPY_AND_ASSIGN( CheckOwnerIsMasterTimerTask);
      MetaManager* meta_mgr_;
    };
    typedef tbutil::Handle<CheckOwnerIsMasterTimerTask> CheckOwnerIsMasterTimerTaskPtr;

    class MasterHeartTimerTask: public tbutil::TimerTask
    {
    public:
      MasterHeartTimerTask(MetaManager* mm);
      virtual ~MasterHeartTimerTask()
      {
      }
      virtual void runTimerTask();
    private:
      DISALLOW_COPY_AND_ASSIGN( MasterHeartTimerTask);
      MetaManager* meta_mgr_;
    };
    typedef tbutil::Handle<MasterHeartTimerTask> MasterHeartTimerTaskPtr;

    class SlaveHeartTimerTask: public tbutil::TimerTask
    {
    public:
      SlaveHeartTimerTask(MetaManager* mm, tbutil::TimerPtr& timer);
      virtual ~SlaveHeartTimerTask()
      {
      }
      virtual void runTimerTask();
    private:
      DISALLOW_COPY_AND_ASSIGN( SlaveHeartTimerTask);
      MetaManager* meta_mgr_;
      tbutil::TimerPtr& timer_;
    };
    typedef tbutil::Handle<SlaveHeartTimerTask> SlaveHeartTimerTaskPtr;

    // a class for handling the heartbeat from the other ns.
    class MasterAndSlaveHeartManager: public ProactorDataPipe<std::deque<message::Message*>, MasterAndSlaveHeartManager>
    {
      template<typename Container, typename Executor> friend class ProactorDataPipe;
    public:
      MasterAndSlaveHeartManager(MetaManager* mm, tbutil::TimerPtr& timer);
      ~MasterAndSlaveHeartManager();

    public:
      int initialize();
      int wait_for_shut_down();
      int destroy();
    private:
      int execute(message::Message* message, void* args);
      int do_master_msg(message::Message* message, void* args);
      int do_slave_msg(message::Message* message, void* args);
      int do_heartbeat_and_ns_msg(message::Message* message, void* args);
    private:
      DISALLOW_COPY_AND_ASSIGN( MasterAndSlaveHeartManager);
      MetaManager* meta_mgr_;
      tbutil::TimerPtr& timer_;
    };
  }
}

#endif
