#include <string>
#include <vector>
#include <memory>
#include <pqxx/pqxx>
#include "Wallet.h"
#include "Transaction.h"

class Account {
private:
    std::string username;
    std::string email;
    std::string password_hash;
    pqxx::connection &conn;
public:
    std::vector<std::unique_ptr<Wallet>> wallets;
    Account(const std::string &username, const std::string &email, const std::string &password_hash, pqxx::connection &conn);
    ~Account();
    void display_account_info() const;
    void set_client_name(const std::string &client_name);
    void set_account_email(const std::string &email);
    void add_wallet(double initial_balance);
    bool delete_wallet(const std::string &wallet_address);
    void transfer_money(const std::string &from_wallet, const std::string &to_wallet, double amount);
    void save_to_db() const;
    void load_from_db(int account_id);
    Wallet* get_wallet(const std::string &wallet_address) const;
};