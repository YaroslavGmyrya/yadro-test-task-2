#include "../include/sub_funcs.hpp"

// convert string to lower case
void lower_case(std::string& str){
    for(int i = 0; i < str.size(); ++i){
        str[i] = tolower(str[i]);
    }
}