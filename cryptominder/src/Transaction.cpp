#include "Transaction.h"
#include <iostream>
#include <pqxx/pqxx>

Transaction::Transaction(pqxx::connection &connection) : C(connection) {
}

void Transaction::add_transaction(const std::string_view &sender_wallet_address,
                                  const std::string_view &receiver_wallet_address, const double &amount,
                                  const double &transaction_fee) {
    pqxx::work W(C);
    std::string query = std::format(
        "INSERT INTO transactions (sender_wallet_address, receiver_wallet_address, amount, transaction_fee) "
        "VALUES ('{}', '{}', {}, {}) RETURNING transaction_id;",
        sender_wallet_address, receiver_wallet_address, amount, transaction_fee
    );
    pqxx::result R = W.exec(query);
    int transaction_id = R[0][0].as<int>();
    W.commit();
    std::cout << "Transaction w ID: " << transaction_id << std::endl;
}

std::vector<TransactionData> Transaction::get_transactions(std::string_view wallet_address) {
    pqxx::nontransaction N(C);
    std::vector<TransactionData> transactions;
    std::string query = std::format("SELECT * FROM transactions WHERE sender_wallet_address = '{}' OR receiver_wallet_address = '{}';",
    wallet_address, wallet_address);
    pqxx::result R = N.exec(query);
    for (const auto &row : R) {
        TransactionData transaction{
            row["transaction_id"].as<int>(),
            row["sender_wallet_address"].as<std::string>(),
            row["receiver_wallet_address"].as<std::string>(),
            row["amount"].as<double>(),
            row["transaction_fee"].as<double>(),
            row["status"].as<std::string>(),
            row["created_at"].as<std::string>()
        };
        transactions.push_back(transaction);
    }

    return transactions;
}
