#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>

class Wallet {
private:
    std::string wallet_address;
    int wallet_balance;
public:
    Wallet(const std::string &wallet_address, const int wallet_balance_input = 0)
            : wallet_address(wallet_address), wallet_balance(wallet_balance_input) {
    }

    ~Wallet() {
        std::cout << "Address is deleted: " << wallet_address;
    }

    void display_wallet_info() const {
        std::cout << "Address: " << wallet_address << std::endl;
        std::cout << "Wallet balance: " << wallet_balance << std::endl;
    }

    void set_wallet_balance(const int wallet_balance_input) {
        wallet_balance = wallet_balance_input;
    }

    std::string get_wallet_address() const {
        return wallet_address;
    }

    int get_wallet_balance() const {
        return wallet_balance;
    }

    Wallet(const Wallet &) = delete;

    void operator=(const Wallet &) = delete;
};

class Account {
private:
    std::string client_name;
    std::vector<std::unique_ptr<Wallet>> wallets;
    int account_balance;
public:
    Account(const std::string &client_name, const int account_balance_input = 0)
            : client_name(client_name), account_balance(account_balance_input) {
    }

    ~Account() {
        wallets.clear();
    }

    void display_account_info() const {
        std::cout << "Client name: " << client_name << std::endl;
        std::cout << "Account balance: " << account_balance << std::endl;

        if (!wallets.empty()) {
            std::cout << "Attached wallets: " << std::endl << std::endl;
            for (auto &wallet: wallets) {
                wallet->display_wallet_info();
                std::cout << std::endl;
            }
        } else {
            std::cout << "No wallets are attached to this account." << std::endl << std::endl;
        }
    }

    std::string get_client_name() const {
        return client_name;
    }

    void set_client_name(const std::string_view &client_name_input) {
        client_name = client_name_input;
    }

    int get_account_balance() const {
        return account_balance;
    }

    void set_account_balance(const int account_balance_input) {
        account_balance = account_balance_input;
    }

    const Wallet *get_wallet(const std::string_view &wallet_address) const {
        for (const auto &wallet: wallets) {
            if (wallet->get_wallet_address() == wallet_address) {
                return wallet.get();
            }
        }
        return nullptr;
    }

    void add_wallet(std::unique_ptr<Wallet> wallet) {
        if (get_wallet(wallet->get_wallet_address()) == nullptr) {
            wallets.push_back(std::move(wallet));
        } else {
            std::cout << "Wallet is already exists" << std::endl << std::endl;
        }
    }

    bool delete_wallet(const std::string_view &wallet_address) {
        for (auto iter = wallets.begin(); iter != wallets.end(); iter++) {
            if ((*iter)->get_wallet_address() == wallet_address) {
                wallets.erase(iter);
                return true;
            }
        }

        return false;
    }

    int get_account_available_balance() const {
        int walletsbalance = 0;
        int balance_available = 0;

        for (auto &wallet: wallets) {
            walletsbalance += wallet->get_wallet_balance();
        }

        if (account_balance - walletsbalance > 0) {
            balance_available = account_balance - walletsbalance;
        }

        return balance_available;
    }

    Account(const Account &) = delete;

    void operator=(const Account &) = delete;
};