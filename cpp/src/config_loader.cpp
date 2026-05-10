#include "../include/config_loader.hpp"
#include "../include/sub_funcs.hpp"

// load config pipeline
bool config_loader::load_config(const char* filename, config& cfg){

    // 1. Read lines from file
    std::vector<std::string> lines = read_config(filename);

    // 2. Parse lines
    if(!parse_config(lines, filename, cfg)){
        cfg = config{};
        return false;
    }

    return true;
}

// read file and return vector of lines
std::vector<std::string> config_loader::read_config(const char* filename){

    // try open file
    std::ifstream file(filename);

    // check error
    if(!file.is_open()){
        logger->error("[config_loader.cpp:read_config]: <{}> Error in opening file!", filename);
        return {};
    }

    std::string line;
    std::vector<std::string> lines;

    // read all lines in file
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return lines;
}

// Check lines of file on error
bool config_loader::parse_config(const std::vector<std::string>& lines, const char* filename, config &cfg){
    // 1. Check count of lines. File must be have 2 lines
    if(lines.size() != 2){
        logger->error("[config_loader.cpp:validate_config]: <{}> File must be have 2 lines, but have {}!", filename, lines.size());
        return false;
    }


    // 2.Split lines on tokens
    std::vector<std::vector<std::string>> tokens(lines.size());
    std::string token;

    for(int i = 0; i < lines.size(); ++i){
        // get tokens
        std::stringstream ss(lines[i]);
        while(ss >> token){
            tokens[i].push_back(token);
        }
    }


    // 3. Check first line. She must be have one float number
    
    // 3.1 Check size
    if(tokens[0].size() != 1){
        logger->error("[config_loader.cpp:validate_config]: <{}> First line of file must be have one token, but have {}!", filename, tokens.size());
        return false;
    }

    // 3.2 Try convert token to double
    double time = 0;
    try{
        time = std::stod(tokens[0][0]);
    } catch(...){
        logger->error("[config_loader.cpp:validate_config]: <{}> First line must be have float number, but have trash: {}!",filename, tokens[0][0]);
        return false;
    }

    if(time <= 0){
        logger->error("[config_loader.cpp:validate_config]: <{}> Simulation time have negative value!", filename, tokens[0][0]);
        return false;
    }

    // 4. Check second line. She must be have type of model and N parameters

    // 4.1 Check model existing
    std::string model_type = tokens[1][0];
    lower_case(model_type);

    bool model_exist = false;

    for(const std::string& el : allowed_models){
        if(el == model_type){
            model_exist = true;
        }
    }

    if(!model_exist){
        logger->error("[config_loader.cpp:validate_config]: <{}> Second line contains invalid model type: {}!. You can see allowed models in /include/config_loader.hpp!", filename, model_type);
        return false;
    }

    // 4.2 Check parameters

    /*
        4.2.1 if model type is "equal", then she have 2 parameters: 
        1. Double, pause between packets
        2. Int, packet size
    */

    double pause_bw_packets = 0;
    int packet_size = 0;

    if(model_type == "equal"){
        // check count of parameters
        if(tokens[1].size() - 1 != 2){
            logger->error("[config_loader.cpp:validate_config]: <{}> Second line must be have 2 parameters, but have {}!", filename, tokens[1].size() - 1);
            return false;
        }

        // check first parametr
        try{
            pause_bw_packets = stod(tokens[1][1]);
        } catch(...){
            logger->error("[config_loader.cpp:validate_config]: <{}> First parametr is not double! {}", filename, tokens[1][1]);
            return false;
        }

        if(pause_bw_packets <= 0){
            logger->error("[config_loader.cpp:validate_config]: <{}> Pause between packets have negative value!", filename, tokens[1][1]);
            return false;
        }

        // check second parametr
        try{
            packet_size = stoi(tokens[1][2]);
        } catch(...){
            logger->error("[config_loader.cpp:validate_config]: <{}> Second parametr is not int! {}", filename, tokens[1][2]);
            return false;
        }

        // if second parameter is negative
        if(packet_size <= 0){
            logger->error("[config_loader.cpp:validate_config]: <{}> Packet size have negative value!", filename, tokens[1][2]);
            return false;
        }

        // fill eq_model parameters
        cfg.eq_parameters.packet_size = packet_size;
        cfg.eq_parameters.pause_bw_packet = pause_bw_packets;
    }

    /*
        4.2.2 if model type is "poisson", then she have 1 parameters: 
        1. Double, lambda (intensity) for poisson distribution
        2. Double, lambda (intensity) for exp distribution

    */

    else if(model_type == "poisson"){
        double poisson_lambda = 0;
        double exp_lambda = 0;

        // check count of parameters
        if(tokens[1].size() - 1 != 2){
            logger->error("[config_loader.cpp:validate_config]: <{}> Second line must be have 2 parameters, but have {}!", filename, tokens[1].size() - 1);
            return false;
        }

        // check first parametr
        try{
            poisson_lambda = stod(tokens[1][1]);
        } catch(...){
            logger->error("[config_loader.cpp:validate_config]: <{}> First parametr is not double! {}", filename, tokens[1][1]);
            return false;
        }

        if(time <= 0){
            logger->error("[config_loader.cpp:validate_config]: <{}> Poisson lambda have negative value!", filename, tokens[1][1]);
            return false;
        }

        // check second parametr
        try{
            exp_lambda = stod(tokens[1][2]);
        } catch(...){
            logger->error("[config_loader.cpp:validate_config]: <{}> Second parametr is not double! {}", filename, tokens[1][2]);
            return false;
        }

        if(time <= 0){
            logger->error("[config_loader.cpp:validate_config]: <{}> Exp lambda have negative value!", filename, tokens[1][1]);
            return false;
        }

        // fill poisson parameters
        cfg.poisson_parameters.poisson_lambda = poisson_lambda;
        cfg.poisson_parameters.exp_lambda = exp_lambda;
    }

    // fill general parameters
    cfg.simulation_time = time;
    cfg.model_type = model_type;

    logger->info("\n\n\n");


    return true;

}