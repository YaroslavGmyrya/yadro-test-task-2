#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

struct eq_params {
    double pause_bw_packet;
    int packet_size;
};

struct poisson_params {
    double poisson_lambda;
    double exp_lambda;
};

struct config{
    double simulation_time;
    std::string model_type;
    eq_params eq_parameters;
    poisson_params poisson_parameters;

};

class config_loader{
    public:
        config_loader(){
            logger = spdlog::get("file_logger");

            if(!logger){
                logger = spdlog::basic_logger_mt("file_logger", "../cpp/logs/logs.txt");
            }
        }

        bool load_config(const char* filename, config& cfg);

    private:
        std::vector<std::string> allowed_models = {"equal", "poisson"};
        std::shared_ptr<spdlog::logger> logger;

        std::vector<std::string> read_config(const char* filename);
        bool parse_config(const std::vector<std::string>& lines, const char* filename, config& cfg);

};

