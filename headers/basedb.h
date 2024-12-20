#ifndef BASEDB_H
#define BASEDB_H

#include <pqxx/pqxx>
#include <memory>
#include "Account.h"

class BaseDatabase {
public:
    std::unique_ptr<pqxx::connection> conn;
    std::unique_ptr<Account> account;

    BaseDatabase()
        : conn(std::make_unique<pqxx::connection>("dbname=crypto user=crypto_owner password=I6XNyohUfBj8 host=ep-yellow-truth-a2dw0siz.eu-central-1.aws.neon.tech sslmode=require"))
    {}

    virtual ~BaseDatabase() = default;

    void create_account(const std::string& name, const std::string& username, const std::string& password) {
        account = std::make_unique<Account>(name, username, password, *conn);
    }

    bool is_database_connected()
    {
        try {
            BaseDatabase database;
            pqxx::connection* conn = database.getConnection();
            return conn->is_open();
        } catch (const pqxx::broken_connection &) {
            return false;
        }
    }

    pqxx::connection* getConnection() { return conn.get(); }
    Account* getAccount() { return account.get(); }
};

#endif
