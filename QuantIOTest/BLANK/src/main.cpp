#include "Source.hpp"
#include <string>
#include <sstream>

int main() {
    std::string query = "SELECT CODE, NAME, NUM_CODE, SYMBOL FROM CURRENCY";
    std::vector<std::string> result;

    std::string query2 = query.substr(7, query.find("FROM") - 7);
    //std::cout << query2 << std::endl;
    std::stringstream ss(query2);

    while (ss.good()) {
        std::string substr;
        std::getline(ss, substr, ',');
        result.push_back(substr);
    }

    for (std::string out : result)
        std::cout << out << std::endl;
	return 0;
}