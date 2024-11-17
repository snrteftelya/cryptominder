#include <string>
#include <string_view>
#include <pqxx/pqxx>

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

    friend bool operator==(const Wallet &lhs, const Wallet &rhs);
    friend Wallet operator+(const Wallet &lhs, const Wallet &rhs);
    friend std::ostream& operator<<(std::ostream& os, const Wallet& wallet);


};

bool get_wallets_by_address(const std::string &address1, const std::string &address2,
                            Wallet*& wallet1, Wallet*& wallet2,
                            std::vector<std::unique_ptr<Wallet>>& wallets);