#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <random>
#include "../include/config_loader.hpp"

/*
    Any traffic model return 2 vectors:

    1. Timestamps: time, when receive packet
    2. Packet_sizes: size of receive packet
*/
struct model_out{
    std::vector<double> timestamps;
    std::vector<int> packet_sizes;
};

/* Interface */
class Imodel{
    public:
        Imodel() : gen(std::random_device{}()) {}
        /*
            Interface define method, but doesn't implement it. This method is implemented by child classes.
            If child class doesn't implement this child, compiler throw error
        */
        virtual model_out run_bench(const config& cfg) = 0;
        std::mt19937 gen;
};

/* This class implemeted equal traffic model and inherited from class "model" */
class equal_model : public Imodel{
    protected:
        model_out run_bench(const config& cfg) override;
};

/* This class implemeted poisson traffic model and inherited from class "model" */
class poisson_model : public Imodel{
    protected:
        model_out run_bench(const config& cfg) override;
};

/*This main class, which run pipeline*/
class Model : public equal_model, poisson_model{
    public:
        Model(){
            logger = spdlog::get("model_logger");

            if(!logger){
                logger = spdlog::basic_logger_mt("model_logger", "../logs/model_logs.txt");
            }
        }

        /*
            This method call model pipeline:
            1. Read, validate, parse config
            2. Run bench
            3. Write result to .csv
        */
        void run_model(const char* config_filename, const char* output_filename);

    private:
        model_out model_data;                       // this struct describe data, which bench return
        std::shared_ptr<spdlog::logger> logger;     // object of spdlog library, which write logs in file
        config_loader cl;                           // object, which read, validate and parse config file
        config cfg;                                 // struct, which storage model parameters        


        /* This method write to file resutls of model work */
        bool write_to_file(const char* filename, const model_out& data);
};
