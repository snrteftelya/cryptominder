#include <pqxx/pqxx>

class DatabaseSchema {
private:
    pqxx::connection C;

public:
    DatabaseSchema(const std::string &connectionString);

    void createTables();
};
