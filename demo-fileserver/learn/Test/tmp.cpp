#include <iostream>
using namespace std;
#include <toml.hpp>
void fun(){
    // auto config = toml::parse_file( "config/config.toml" );
    auto config = toml::parse_file( "/home/snow/code/netcpp/config/config.toml" );

    std::string_view ser_ip=config["server"]["ip"].value_or("[]"sv);
    int ser_port=config["server"]["port"].value_or(0);

    cout<< ser_ip<<ser_port<<endl;
}

int main(int argc,char *argv[]){
    fun();
    return 0;
}