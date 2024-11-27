#include <string>
#include "UpdVector.h"
#include <memory>
#include <pqxx/pqxx>
#include "Wallet.h"
#include "Transaction.h"
#include "IPersistable.h"
#include "ITransferable.h"

class Account : public IPersistable, public ITransferable {
private:
    std::string username;
    std::string email;
    std::string password_hash;
    pqxx::connection &conn;

public:
    UpdVector<std::unique_ptr<Wallet>> wallets;

    Account(const std::string &username, const std::string &email, const std::string &password_hash,
            pqxx::connection &conn);

    ~Account() final;

    void display_account_info() const;

    void set_client_name(const std::string &client_name);

    void set_account_email(const std::string &email);

    void add_wallet(double initial_balance);

    bool delete_wallet(const std::string &wallet_address);

    void save_to_db() const override;

    void load_from_db(int account_id) override;

    void transfer_money(const std::string &from_wallet, const std::string &to_wallet, double amount) override;

    pqxx::result get_wallets_from_db(int account_id);

};
