#include "headers/Wallet.h"

#include <random>

Wallet::Wallet(const std::string &wallet_address, const double wallet_balance, pqxx::connection &conn)
        : wallet_address(wallet_address), wallet_balance(wallet_balance), conn(conn) {
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
    wallet_balance = wallet_balance_input;
}

std::string Wallet::get_wallet_address() const {
    return wallet_address;
}

double Wallet::get_wallet_balance() const {
    return wallet_balance;
}

bool get_wallets_by_address(const std::string &address1, const std::string &address2,
                            Wallet*& wallet1, Wallet*& wallet2,
                            std::vector<std::unique_ptr<Wallet>>& wallets) {
    auto wallet1_it = std::ranges::find_if(wallets.begin(), wallets.end(),
        [&address1](const std::unique_ptr<Wallet> &wallet) {
            return wallet->get_wallet_address() == address1;
        });
    auto wallet2_it = std::ranges::find_if(wallets.begin(), wallets.end(),
        [&address2](const std::unique_ptr<Wallet> &wallet) {
            return wallet->get_wallet_address() == address2;
        });
    if (wallet1_it != wallets.end() && wallet2_it != wallets.end()) {
        wallet1 = wallet1_it->get();
        wallet2 = wallet2_it->get();
        return true;
    } else {
        return false;
    }
}
