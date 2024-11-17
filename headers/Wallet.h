#pragma once

#include <string>
#include <string_view>
#include <pqxx/pqxx>
#include <iostream>

class Wallet {
private:
    std::string wallet_address;
    double wallet_balance;
    pqxx::connection &conn;
public:
    Wallet(const std::string &wallet_address, const double wallet_balance, pqxx::connection &conn);

    ~Wallet();

    void display_wallet_info() const;

    void set_wallet_address();

    void set_wallet_balance(const double wallet_balance_input);

    std::string get_wallet_address() const;

    double get_wallet_balance() const;

    void load_from_db();

    Wallet(const Wallet &) = delete;

    void operator=(const Wallet &) = delete;
};
