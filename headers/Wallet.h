#include <string>
#include <pqxx/pqxx>
#include <iostream>
#include <memory>
#include "UpdVector.h"

class Wallet {
private:
    std::string wallet_address;
    double wallet_balance;
    pqxx::connection &conn;

    friend Wallet &operator+(Wallet &wallet, double cryptocurrency) {
        wallet.set_wallet_balance(wallet.get_wallet_balance() + cryptocurrency);
        wallet.update_balance();
        return wallet;
    }

    friend Wallet &operator-(Wallet &wallet, double cryptocurrency) {
        wallet.set_wallet_balance(wallet.get_wallet_balance() - cryptocurrency);
        wallet.update_balance();
        return wallet;
    }

    friend std::ostream &operator<<(std::ostream &os, const Wallet &wallet) {
        os << "Address: " << wallet.wallet_address << ", Balance: " << wallet.wallet_balance;
        return os;
    }

public:
    Wallet(const std::string &wallet_address, double wallet_balance, pqxx::connection &conn);

    ~Wallet() = default;

    void set_wallet_address();

    void set_wallet_balance(double wallet_balance_input);

    std::string get_wallet_address() const;

    double get_wallet_balance() const;

    void update_balance();
};

bool get_wallet_by_address(const std::string &address, Wallet *&wallet, UpdVector<std::unique_ptr<Wallet>> &wallets);
