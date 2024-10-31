#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <random>
#include <iomanip>
#include <sstream>

class Wallet {
private:
    std::string wallet_address;
    int wallet_balance;
public:
    Wallet(const std::string &wallet_address, const int wallet_balance_input = 0);

    ~Wallet();

    void display_wallet_info() const;

    void set_wallet_address();

    void set_wallet_balance(const int wallet_balance_input);

    std::string get_wallet_address() const;

    int get_wallet_balance() const;

    Wallet(const Wallet &) = delete;

    void operator=(const Wallet &) = delete;
};
