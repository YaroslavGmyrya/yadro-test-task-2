#include <spdlog/spdlog.h>

bool CONFIG_LOADER_POSITIVE_TEST();
bool CONFIG_LOADER_NEGATIVE_TEST();

int main(){

    if(CONFIG_LOADER_POSITIVE_TEST()){
        spdlog::error("#1 CONFIG_LOADER_POSITIVE_TEST FAILED!\n");   
    }
    else{
        spdlog::info("#1 CONFIG_LOADER_POSITIVE_TEST PASS!\n");   
    }

    if(CONFIG_LOADER_NEGATIVE_TEST()){
        spdlog::error("#2 CONFIG_LOADER_NEGATIVE_TEST FAILED!\n");   
    }
    else{
        spdlog::info("#2 CONFIG_LOADER_NEGATIVE_TEST PASS!\n");   
    }

    return 0;
}