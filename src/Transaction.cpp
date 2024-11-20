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

void Transaction::get_transactions(std::string_view wallet_address) {
    pqxx::nontransaction N(C);
    std::string query = std::format("SELECT * FROM transactions WHERE sender_wallet_address = '{}' OR receiver_wallet_address = '{}';",
    wallet_address, wallet_address);
    pqxx::result R = N.exec(query);
    for (const auto &row: R) {
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << "Transaction ID: " << row[0].as<int>() << std::endl;
        std::cout << "Sender Wallet Address: " << row[1].as<std::string_view>() << std::endl;
        std::cout << "Receiver Wallet Address: " << row[2].as<std::string_view>() << std::endl;
        std::cout << "Amount: " << row[3].as<double>() << std::endl;
        std::cout << "Transaction Fee: " << row[4].as<double>() << std::endl;
        std::cout << "Status: " << row[5].as<std::string_view>() << std::endl;
        std::cout << "Created At: " << row[6].as<std::string_view>() << std::endl;
        std::cout << "--------------------------------------------" << std::endl;
    }
}
