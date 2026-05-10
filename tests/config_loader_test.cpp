#include "../cpp/include/config_loader.hpp"

bool CONFIG_LOADER_POSITIVE_TEST(){
    std::vector<std::string> pos_files = {"../tests/input_files/pos_1.txt", "../tests/input_files/pos_2.txt"};
    config_loader cl;

    bool errors_exist = false;
    for(const std::string& file : pos_files){
        config cfg;

        if(!cl.load_config(file.data(), cfg)){
            errors_exist = true;
        }
    }

    return errors_exist;
}

bool CONFIG_LOADER_NEGATIVE_TEST(){
    std::vector<std::string> neg_files = {
        "../tests/input_files/neg_1.txt", 
        "../tests/input_files/neg_2.txt",
        "../tests/input_files/neg_3.txt", 
        "../tests/input_files/neg_4.txt",
        "../tests/input_files/neg_5.txt", 
        "../tests/input_files/neg_6.txt"
    };
    config_loader cl;

    bool errors_exist = false;
    for(const std::string& file : neg_files){
        config cfg;

        if(cl.load_config(file.data(), cfg)){
            errors_exist = true;
        }
    }

    return errors_exist;
}