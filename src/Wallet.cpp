#include "headers/Wallet.h"

Wallet::Wallet(const std::string &wallet_address, const int wallet_balance_input)
        : wallet_address(wallet_address), wallet_balance(wallet_balance_input) {
}

Wallet::~Wallet() {
    std::cout << "Address is deleted: " << wallet_address << std::endl;
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

void Wallet::set_wallet_balance(const int wallet_balance_input) {
    wallet_balance = wallet_balance_input;
}

std::string Wallet::get_wallet_address() const {
    return wallet_address;
}

int Wallet::get_wallet_balance() const {
    return wallet_balance;
}
