#include <pqxx/pqxx>

class DatabaseSchema {
private:
    pqxx::connection C;

public:
    explicit DatabaseSchema(const std::string &connectionString);

    void createTables();
};
