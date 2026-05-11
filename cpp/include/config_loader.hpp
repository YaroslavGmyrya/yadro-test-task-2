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

/* Structure for storage model parameters */
struct config {
    double simulation_time;
    std::string model_type;
    eq_params eq_parameters;
    poisson_params poisson_parameters;

};

/*
    This class:
        1.Read config file
        2.Check file on errors
        3.Parse
*/
class config_loader{
    public:
        /* create logger */
        config_loader(){
            logger = spdlog::get("config_loader_logger");

            if(!logger){
                logger = spdlog::basic_logger_mt("config_loader_logger", "../logs/config_loader_logs.txt");
            }
        }

        /*
            This method call pipeline, which:
                1. Read config
                2. Check errors
                3. Parse values
        */
        bool load_config(const char* filename, config& cfg);

    private:
        std::vector<std::string> allowed_models = {"equal", "poisson"};
        std::shared_ptr<spdlog::logger> logger;                         // object, which write logs in file

        std::vector<std::string> read_config(const char* filename);     // read file and return lines
        bool parse_config(const std::vector<std::string>& lines, const char* filename, config& cfg);    // parse & check errors in file

};

