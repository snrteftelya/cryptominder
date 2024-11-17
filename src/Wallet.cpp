#include "headers/Wallet.h"

#include <random>

Wallet::Wallet(const std::string &wallet_address, const double wallet_balance, pqxx::connection &conn)
        : wallet_address(wallet_address), wallet_balance(wallet_balance), conn(conn) {
}

Wallet::~Wallet() {
}

void Wallet::display_wallet_info() const {
    std::cout << "Wallet address: " << wallet_address << std::endl;
    std::cout << "Wallet balance: " << wallet_balance << std::endl;
}

void Wallet::set_wallet_address() {
    const char hex_chars[] = "0123456789abcdef";
    int length = 26;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(0, 15);
    std::string hex_string;
    for (size_t i = 0; i < length; ++i) {
        hex_string += hex_chars[dis(gen)];
    }
    wallet_address = std::string(hex_string);
}

void Wallet::set_wallet_balance(const double wallet_balance_input) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(
                "UPDATE wallet SET balance = $1 WHERE wallet_address = $2",
                wallet_balance_input, wallet_address
        );
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "Error updating wallet balance in database: " << e.what() << std::endl;
    }
}

std::string Wallet::get_wallet_address() const {
    return wallet_address;
}

double Wallet::get_wallet_balance() const {
    return wallet_balance;
}

void Wallet::load_from_db() {
    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec_params(
            "SELECT wallet_address, balance FROM wallet WHERE account_id = 1"
        );
        if (!result.empty()) {
            for (const auto& row : result) {
                std::string wallet_address = row["wallet_address"].c_str();
                double wallet_balance = row["balance"].as<double>();
                std::cout << "Wallet address: " << wallet_address << ", Balance: " << wallet_balance << std::endl;
            }
        } else {
            std::cout << "No wallets found in database for this account." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}