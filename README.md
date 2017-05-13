# Intro
TFS is mainly comprised of 3 components: nameserver, dataserver, client.

# NameServer
## Startup
+ Application start
    + command line argument parsing: conf file and daemon or not
    + load config file
    + logging setup
    + create working directories
    + mutex: pid file
    + fork if daemon: lock and write the pid file
    + ignore some signals
    + service start
        + start NameServer
        + setup signals
        + wait to stop
        + remove pid files
+ NameServer start
    + init NameServer Stat Information
    + start Network Server
    + start 

## Signal handling
+ SIGTERM/SIGINT: NameServer::stop()
    + ???: where is the synchronization

## Shard state
+ CONFIG: configuration module, read config files
    + init on setup
    + global
+ System Parameters: singleton/read only, depends on CONFIG module
    + content
        + NameServer config
        + DataServer config
        + FileSystemParam config
    + init on setup
    + global
+ NameServer Stat Information
    + owned by NameServer
    
## Actors
+ Network Server: accepts requests and handles it asynchronously
    + Transport
    + MessageFactory
    + IServerAdaptor
+ Proactor/MasterAndSlaveHeartManager: handle peer nameserver heartbeats (TODO)
+ 


## Data Model
+ LayoutManager: all blocks and dataservers info are retained here.
+ MetaManager: LayoutManager + LeaseClerk

## Thread Model
+ Main thread with queue: 
    + accepts requests and handles it asynchronously
    + timer: send heartbeat to master/slave
    + timer: issue a block owner check request
    + timer: ...
+ background process pipeline: tasks are pushed by other threads, handled asynchronously
    + Compacting
    + Replication
        + rebalance
        + replication
    + Redundant: the code is inconsistent, this job should have a pipeline, but it doesn't. Redundancy is checked in the calling thread
    + Heartbeat from DS: handle heartbeat from DS
    + Heartbeat from Master/Slave
+ worker thread:
    + Check DS: check dataservers
    + Check Block: perform block scanning, send new tasks to pipeline(optionally)
    + Rebalancing: check send requests to rebalancing pipeline
    + TimeoutCheck