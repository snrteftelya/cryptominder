#include <pqxx/pqxx>

class Transaction {
private:
    pqxx::connection &C;
public:
    explicit Transaction(pqxx::connection &connection);
    void addTransaction(const std::string_view &sender_wallet_address, const std::string_view &receiver_wallet_address, const double &amount, const double &transaction_fee);
    void getTransactions();
};