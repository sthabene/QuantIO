#include "Source.hpp"
#include <string>
#include <sstream>

int main() {

    std::vector<std::string> result;
    std::vector<std::string> result2 = {"ABC", "DEF", "GHJ"};

    for (std::size_t i = 0; i < result2.size(); i++) {
        result.push_back(result2[i]);
    }
    
    for (std::string str : result)     
        std::cout << str << std::endl;

	return 0;
}