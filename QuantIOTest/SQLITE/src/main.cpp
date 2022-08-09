#include "../include/sqlite.hpp"
#include "../include/database.hpp"

using namespace QuantIO;

class Testing {
private:
    std::string m_test;
public:
    Testing() {
        m_test = "Green";
    }
 };

class Testing2 : Testing {
private:
    std::string m_test2;
public:
    Testing2() : Testing() {
        m_test2 = "Blue";
    }
    std::string returnSomething() {
        return m_test2;
    }
};

int main() {

    std::string location = "..\\..\\misc\\database\\main.db";
    
    Connection connection(location);
    Connection::Stmt statement(connection);

    //statement.getTable("SELECT CODE, NAME FROM CURRENCY");

    Testing testing();
    

    return 0;
}