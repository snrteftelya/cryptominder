#include <string>
#include <pqxx/pqxx>
#include <iostream>
#include <vector>
#include <memory>

class Wallet {
private:
    std::string wallet_address;
    double wallet_balance;
    pqxx::connection &conn;

public:
    Wallet(const std::string &wallet_address, double wallet_balance, pqxx::connection &conn);

    ~Wallet() = default;

    void set_wallet_address();

    void set_wallet_balance(double wallet_balance_input);

    std::string get_wallet_address() const;

    double get_wallet_balance() const;

    void update_balance();

    friend Wallet &operator+(Wallet &wallet, double cryptocurrency);

    friend Wallet &operator-(Wallet &wallet, double cryptocurrency);

    friend std::ostream &operator<<(std::ostream &os, const Wallet &wallet);
};

bool get_wallet_by_address(const std::string &address, Wallet *&wallet,
                           std::vector<std::unique_ptr<Wallet> > &wallets);
