#define TRANSACTION_H

#include <pqxx/pqxx>

class Transaction {
private:
    pqxx::connection &C;

public:
    Transaction(pqxx::connection &connection);

    void addTransaction(const std::string_view &sender_wallet_address, const std::string_view &receiver_wallet_address, const double &amount, const double &transaction_fee);

    void getTransactions();

    Transaction(const Transaction &) = delete;

    void operator=(const Transaction &) = delete;
};