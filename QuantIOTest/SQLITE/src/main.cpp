#include "connector/include/sqlite.hpp"
#include "connector/include/database.hpp"
#include <iostream>

using namespace QuantIO;

int main() {

    std::string location = "..\\..\\misc\\database\\main.db";
    
    Connection connection(location);
    static std::vector<std::vector<std::string>> tableData = 
        connection.getTableData2("SELECT CODE, NAME FROM CURRENCY");
    
    for (std::vector<std::string> row : tableData) {
        for (std::string cell : row) {
            std::cout << cell << "\n";
        }
    }

    return 0;
}