#include "Wallet.h"
#include "UpdVector.h"
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

void Wallet::update_balance() {
    pqxx::work txn(conn);
    txn.exec_params("UPDATE wallet SET balance = $1 WHERE wallet_address = $2",
                    wallet_balance,
                    wallet_address);
    txn.commit();
}

bool get_wallet_by_address(const std::string &address, Wallet *&wallet, UpdVector<std::unique_ptr<Wallet>> &wallets) {
    auto wallet_it = std::find_if(wallets.begin(), wallets.end(),
                                  [&address](const std::unique_ptr<Wallet> &wallet) {
                                      return wallet->get_wallet_address() == address;
                                  });

    if (wallet_it != wallets.end()) {
        wallet = wallet_it->get();
        return true;
    } else {
        wallet = nullptr;
        return false;
    }
}

