#ifndef TFS_NAMESERVER_SERVICE_H_
#define TFS_NAMESERVER_SERVICE_H_

#include <tbsys.h>
#include <tbnet.h>

#include "nameserver.h"
#include "common/func.h"
#include "common/config.h"
#include "message/tfs_packet_streamer.h"
#include "message/message_factory.h"

namespace tfs {
    namespace nameserver {
        // The entrypoint for the server.
        class Service {
        public:
            Service();

            virtual ~Service();

            int start();

            void stop();

        private:
            DISALLOW_COPY_AND_ASSIGN(Service);

            NameServer fs_name_system_;
        };
    }
}

#endif
