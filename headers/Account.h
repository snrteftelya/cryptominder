#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "headers/Wallet.h"

class Account {
private:
    std::string client_name;
    std::vector<std::unique_ptr<Wallet>> wallets;
    int account_balance;
public:
    Account(const std::string &client_name, const int account_balance_input = 0);

    ~Account();

    void display_account_info() const;

    std::string get_client_name() const;

    void set_client_name(const std::string_view &client_name_input);

    int get_account_balance() const;

    void set_account_balance(const int account_balance_input);

    Wallet *get_wallet(const std::string_view &wallet_address_input) const;

    void add_wallet(std::unique_ptr<Wallet> wallet);

    bool delete_wallet(const std::string_view &wallet_address_input);

    int get_account_available_balance() const;

    void transfer_money(const std::string_view &recipient_wallet_address, const std::string_view &sender_wallet_address,
                        const int sum) const;

    void transfer_money(const std::string_view &wallet_address_input, const int sum) const;

    Account(const Account &) = delete;

    void operator=(const Account &) = delete;
};
