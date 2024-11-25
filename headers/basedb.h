#ifndef BASEDB_H
#define BASEDB_H

#include <pqxx/pqxx>
#include <memory>
#include "Account.h"

class BaseDatabase {
protected:
    pqxx::connection *conn;
    std::unique_ptr<Account> account;

public:
    BaseDatabase() {
        conn = new pqxx::connection("dbname=crypto user=crypto_owner password=I6XNyohUfBj8 host=ep-yellow-truth-a2dw0siz.eu-central-1.aws.neon.tech sslmode=require");
    }

    virtual ~BaseDatabase() = default;

    void createAccount(const std::string& name, const std::string& username, const std::string& password) {
        account = std::make_unique<Account>(name, username, password, *conn);
    }

    pqxx::connection* getConnection() { return conn; }
    Account* getAccount() { return account.get(); }
};

#endif
