# Intro
TFS is mainly comprised of 3 components: nameserver, dataserver, client.

# NameServer
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

# DataServer

# Client
Not very complex.