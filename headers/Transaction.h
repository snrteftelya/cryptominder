#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

struct TransactionData {
    int transaction_id;
    std::string sender_wallet_address;
    std::string receiver_wallet_address;
    double amount;
    double transaction_fee;
    std::string status;
    std::string created_at;
};

class Transaction {
private:
    pqxx::connection &C;

public:
    explicit Transaction(pqxx::connection &connection);

    void add_transaction(const std::string_view &sender_wallet_address, const std::string_view &receiver_wallet_address,
                         const double &amount, const double &transaction_fee);
    std::vector<TransactionData> get_transactions(std::string_view wallet_address);

    nlohmann::json get_filtered_report(const std::string &sender_wallet_address,
                                                    const std::string &receiver_wallet_address,
                                       const std::string &created_at);
};

#endif // TRANSACTION_H
