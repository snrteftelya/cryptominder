#include "DataBaseSchema.h"
#include <iostream>
#include <pqxx/pqxx>

DatabaseSchema::DatabaseSchema(const std::string &connection_string) : C(connection_string) {
}

void DatabaseSchema::create_tables() {
    pqxx::work W(C);

    std::string create_account_table = R"(
            CREATE TABLE IF NOT EXISTS Account (
                account_id SERIAL PRIMARY KEY,
                username VARCHAR(16) NOT NULL,
                email VARCHAR(30) UNIQUE NOT NULL,
                password_hash VARCHAR(255) NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            );
        )";

    std::string create_wallet_table = R"(
            CREATE TABLE IF NOT EXISTS Wallet (
                account_id INT REFERENCES Account(account_id) ON DELETE CASCADE,
                wallet_address VARCHAR(30) UNIQUE NOT NULL,
                currency VARCHAR(15) NOT NULL,
                balance DECIMAL(18, 8) DEFAULT 0.0,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            );
        )";

    std::string create_transactions_table = R"(
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

    W.exec(create_account_table);
    W.exec(create_wallet_table);
    W.exec(create_transactions_table);

    W.commit();

    std::cout << "Tables are successfully created or exist." << std::endl;
}
