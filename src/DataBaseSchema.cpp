#include "headers/DataBaseSchema.h"
#include <iostream>
#include <pqxx/pqxx>

DatabaseSchema::DatabaseSchema(const std::string &connectionString) : C(connectionString) {
    if (!C.is_open()) {
        throw std::runtime_error("Exception opening database connection.");
    }
}

void DatabaseSchema::createTables() {
    try {
        pqxx::work W(C);

        std::string createAccountTable = R"(
            CREATE TABLE IF NOT EXISTS Account (
                account_id SERIAL PRIMARY KEY,
                username VARCHAR(16) NOT NULL,
                email VARCHAR(30) UNIQUE NOT NULL,
                password_hash VARCHAR(255) NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            );
        )";

        std::string createWalletTable = R"(
            CREATE TABLE IF NOT EXISTS Wallet (
                account_id INT REFERENCES Account(account_id) ON DELETE CASCADE,
                wallet_address VARCHAR(30) UNIQUE NOT NULL,
                currency VARCHAR(15) NOT NULL,
                balance DECIMAL(18, 8) DEFAULT 0.0,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            );
        )";

        std::string createTransactionsTable = R"(
            CREATE TABLE IF NOT EXISTS Transactions (
                transaction_id SERIAL PRIMARY KEY,
                sender_wallet_address VARCHAR(30) REFERENCES Wallet(wallet_address) ON DELETE CASCADE,
                receiver_wallet_address VARCHAR(30) REFERENCES Wallet(wallet_address) ON DELETE CASCADE,
                amount DECIMAL(18, 8) NOT NULL,
                transaction_fee DECIMAL(18, 8) NOT NULL,
                status VARCHAR(20) DEFAULT 'pending',
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            );
        )";

        W.exec(createAccountTable);
        W.exec(createWalletTable);
        W.exec(createTransactionsTable);

        W.commit();

        std::cout << "Tables are successfully created or exist." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unluck" << e.what() << std::endl;
    }
}
