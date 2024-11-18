#include <string>
#include <string_view>
#include <pqxx/pqxx>
#include <iostream>

class Wallet {
private:
    std::string wallet_address;
    double wallet_balance;
    pqxx::connection &conn;
    friend bool operator==(const Wallet &lhs, const Wallet &rhs) {
        return (lhs.wallet_address == rhs.wallet_address && lhs.wallet_balance == rhs.wallet_balance);
    }
    friend bool operator>(const Wallet& lhs, const Wallet& rhs) {
        return (lhs.wallet_balance > rhs.wallet_balance);
    }
    friend std::ostream& operator<<(std::ostream& os, const Wallet& wallet) {
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
};

bool get_wallets_by_address(const std::string &address1, const std::string &address2,
                            Wallet*& wallet1, Wallet*& wallet2,
                            std::vector<std::unique_ptr<Wallet>>& wallets);
