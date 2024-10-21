#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>

class Wallet {
private:
    std::string wallet_address_;
    int wallet_balance_;
public:
    Wallet(const std::string &wallet_address, const int wallet_balance = 0)
            : wallet_address_(wallet_address), wallet_balance_(wallet_balance) {
    }

    ~Wallet() {
        std::cout << "Address is deleted: " << wallet_address_;
    }

    void display_wallet_info() const {
        std::cout << "Address: " << wallet_address_ << std::endl;
        std::cout << "Wallet balance: " << wallet_balance_ << std::endl;
    }

    void set_wallet_balance(const int wallet_balance) {
        wallet_balance_ = wallet_balance;
    }

    std::string get_wallet_address() const {
        return wallet_address_;
    }

    int get_wallet_balance() const {
        return wallet_balance_;
    }

    Wallet(const Wallet &) = delete;

    void operator=(const Wallet &) = delete;
};

class Account {
private:
    std::string client_name_;
    std::vector<std::unique_ptr<Wallet>> wallets_;
    int account_balance_;
public:
    Account(const std::string &client_name, const int account_balance = 0)
            : client_name_(client_name), account_balance_(account_balance) {
    }

    ~Account() {
        wallets_.clear();
    }

    void display_account_info() const {
        std::cout << "Client name: " << client_name_ << std::endl;
        std::cout << "Account balance: " << account_balance_ << std::endl;

        if (!wallets_.empty()) {
            std::cout << "Attached wallets: " << std::endl << std::endl;
            for (auto &wallet: wallets_) {
                wallet->display_wallet_info();
                std::cout << std::endl;
            }
        } else {
            std::cout << "No wallets are attached to this account." << std::endl << std::endl;
        }
    }

    std::string get_client_name() const {
        return client_name_;
    }

    void set_client_name(const std::string_view &client_name) {
        client_name_ = client_name;
    }

    int get_account_balance() const {
        return account_balance_;
    }

    void set_account_balance(const int account_balance) {
        account_balance_ = account_balance;
    }

    const Wallet *get_wallet(const std::string_view &wallet_address) const {
        for (const auto &wallet: wallets_) {
            if (wallet->get_wallet_address() == wallet_address) {
                return wallet.get();
            }
        }
        return nullptr;
    }

    void add_wallet(std::unique_ptr<Wallet> wallet) {
        if (get_wallet(wallet->get_wallet_address()) == nullptr) {
            wallets_.push_back(std::move(wallet));
        } else {
            std::cout << "Wallet is already exists" << std::endl << std::endl;
        }
    }

    bool delete_wallet(const std::string_view &wallet_address) {
        for (auto iter = wallets_.begin(); iter != wallets_.end(); iter++) {
            if ((*iter)->get_wallet_address() == wallet_address) {
                wallets_.erase(iter);
                return true;
            }
        }

        return false;
    }

    int get_account_available_balance() const {
        int wallets_balance = 0;
        int balance_available = 0;

        for (auto &wallet: wallets_) {
            wallets_balance += wallet->get_wallet_balance();
        }

        if (account_balance_ - wallets_balance > 0) {
            balance_available = account_balance_ - wallets_balance;
        }

        return balance_available;
    }

    Account(const Account &) = delete;

    void operator=(const Account &) = delete;
};