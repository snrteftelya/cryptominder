#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "Wallet.h"

class Account {
private:
    std::string client_name_;
    std::vector<std::unique_ptr<Wallet>> wallets_;
    int account_balance_;

public:
    Account(const std::string &client_name, const int account_balance = 0);

    ~Account();

    void display_account_info() const;

    std::string get_client_name() const;

    void set_client_name(const std::string_view &client_name);

    int get_account_balance() const;

    void set_account_balance(const int account_balance);

    Wallet *get_wallet(const std::string_view &wallet_address) const;

    void add_wallet(std::unique_ptr<Wallet> wallet);

    bool delete_wallet(const std::string_view &wallet_address);

    int get_account_available_balance() const;

    void transfer_money(const std::string_view &recipient_wallet_address, const std::string_view &sender_wallet_address,
                        const int sum) const;

    void transfer_money(const std::string_view &wallet_address, const int sum) const;

    Account(const Account &) = delete;

    void operator=(const Account &) = delete;
};
