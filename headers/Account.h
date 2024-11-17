#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "headers/Wallet.h"
#include <pqxx/pqxx>
#include "Transaction.h"


class Account {
private:
    std::string username;
    std::string email;
    std::string password_hash;
    std::vector<std::unique_ptr<Wallet>> wallets;
    pqxx::connection &conn;
public:
    Account(const std::string &username, const std::string &email, const std::string &password_hash, pqxx::connection &conn);

    ~Account();

    void display_account_info();

    void set_client_name(const std::string_view &client_name_input);

    void set_account_email(const std::string_view &client_email_input);

    std::unique_ptr<Wallet> get_wallet(const std::string_view &wallet_address) const;
    void add_wallet(std::unique_ptr<Wallet> wallet);

    bool delete_wallet(const std::string_view &wallet_address_input);

    void transfer_money(const std::string_view &recipient_wallet_address, const std::string_view &sender_wallet_address,
                        const double sum) const;

    void save_to_db() const;

    void load_from_db();

    Account(const Account &) = delete;

    void operator=(const Account &) = delete;

    void display_account_info() const;
};
