#include "../include/traffic_models.hpp"

int main(int argc, char* argv[]){
    
    /*Check count of command line arguments*/
    if(argc != 2){
        spdlog::error("Programm wait 1 command line argument, but get {}! Try again", argc-1);
        return 1;
    }

    std::string input_filename(argv[1]);
    std::string output_filename("./results/sim_result.txt");

    /*Create model object*/
    Model model;

    /*Run model*/
    model.run_model(input_filename.data(), output_filename.data());

    return 0;
}