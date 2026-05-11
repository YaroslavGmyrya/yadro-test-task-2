#include "../include/traffic_models.hpp"
#include <fstream>
#include <random>
#include <iostream>

bool Model::write_to_file(const char* filename, const model_out& data){

    std::string new_filename(filename);

    //slice extenssion (.txt)
    new_filename = new_filename.substr(0, new_filename.size() - 4);

    //add type of model (for python side)
    new_filename = new_filename + "_" + cfg.model_type + ".txt";

    // check sizes 
    if(data.timestamps.size() != data.packet_sizes.size()){
        logger->error("[traffic_models.cpp:write_to_file]: Timestamps and packets must be have same size, but timestamps and packets have ({}{}) elements", data.timestamps.size(), data.packet_sizes.size());
        return false;
    }

    // try open file
    std::ofstream file(new_filename);
    if(!file.is_open()){
        logger->error("[traffic_models.cpp:write_to_file]: Error in opening {}", filename);
        return false;
    }

    // write data to file
    for(int i = 0; i < data.timestamps.size(); ++i){
        file << data.timestamps[i] << "," << data.packet_sizes[i] << "\n";
    }

    file.close();

    return true;
}

model_out equal_model::run_bench(const config& cfg){
    // 1.get model parameters
    double simulation_time = cfg.simulation_time;
    double pause_bw_packets = cfg.eq_parameters.pause_bw_packet;
    int packets_size = cfg.eq_parameters.packet_size;

    int count_of_packets = simulation_time / pause_bw_packets;

    // 2.create output struct
    model_out output_data;
    
    //I use reserve to avoid memmory allocation and copying
    output_data.packet_sizes.reserve(count_of_packets);
    output_data.timestamps.reserve(count_of_packets);
    
    // 3.fill output struct
    for(int time = 0; time < simulation_time; time+=pause_bw_packets){
        output_data.packet_sizes.push_back(packets_size);
        output_data.timestamps.push_back(time);
    }

    return output_data;
}

model_out poisson_model::run_bench(const config& cfg){
    // 1.get model parameters
    double simulation_time = cfg.simulation_time;
    double poisson_lambda = cfg.poisson_parameters.poisson_lambda;
    int exp_lambda = cfg.poisson_parameters.exp_lambda;

    // 2.create generators
    std::random_device rd;
    std::mt19937 gen(rd());

    std::exponential_distribution<double> exp_dist(exp_lambda);
    std::poisson_distribution<int> poisson_dist(poisson_lambda);

    //3. fill output struct
    model_out model_data;

    for(int time = 0; time < simulation_time;){
        model_data.packet_sizes.push_back((int)exp_dist(gen));
        model_data.timestamps.push_back(time);

        time += poisson_dist(gen);
    }

    
    return model_data;
}

void Model::run_model(const char* config_filename, const char* output_filename){
    // 1. Read config
    if(!cl.load_config(config_filename, cfg)){
        logger->error("Error in opening {}", config_filename);
        return;
    }

    // 2. Run bench
    model_out simulation_results;
    if(cfg.model_type == "equal"){
        simulation_results = equal_model::run_bench(cfg);
    } 
    else if(cfg.model_type == "poisson"){
        simulation_results = equal_model::run_bench(cfg);
    }

    // 3. Write bench results to file
    write_to_file(output_filename, simulation_results);
}