#ifndef TFS_COMMON_CONFIG_H_
#define TFS_COMMON_CONFIG_H_

#include <string>
#include <map>
#include "interval.h"
#include "func.h"

/* read tfs.conf and provide a global configuration object */
// Shared State: used by SysParam
namespace tfs {
    namespace common {
        /* identify sections in the config file */
        enum {
            CONFIG_PUBLIC = 0,
            CONFIG_NAMESERVER,
            CONFIG_DATASERVER,
            CONFIG_TFSCLIENT,
            CONFIG_ADMINSERVER,
            CONFIG_MYSQLSYNC
        };

        /* simply view it as a property map */
        class Configure {
        public:
            Configure();

            ~Configure();

            int load(const std::string& filename);

            std::string to_string();

            // queries
            char* get_string_value(const int section, const std::string& key, char* d = NULL);

            int get_int_value(const int section, const std::string& key, const int d = 0);

            int parse_value(char* str, char* key, char* val);

            STRING_MAP get_define_key();

            std::string get_config_file_name() const
            {
                return config_file_name_;
            }

            static Configure& get_configure();

            static int32_t get_cluster_id();

            static void set_cluster_id(const int32_t cluster_id);

        protected:
            static const int32_t CFG_KEY_LEN = 128;
            static const int32_t CFG_BUFFER_LEN = 4096;

        protected:
            int check_load();

            void clear_config_map();

        protected:
            STRING_MAP public_config_map_;
            STRING_MAP name_config_map_;
            STRING_MAP data_config_map_;
            STRING_MAP client_config_map_;
            STRING_MAP admin_config_map_;
            bool is_load_;
            std::string config_file_name_;
        };

        // Configure
        /* global access (singleton) */
#define CONFIG Configure::get_configure()
    }
}
#endif //TFS_COMMON_CONFIGURE_H_
