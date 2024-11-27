#include "Transaction.h"
#include <iostream>
#include <pqxx/pqxx>
#include <qDebug>

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


nlohmann::json Transaction::get_filtered_report(const std::string &sender_wallet_address,
                                                const std::string &receiver_wallet_address,
                                                const std::string &created_at) {
    pqxx::nontransaction N(C);
    std::vector<TransactionData> transactions;

    std::string query = "SELECT * FROM transactions WHERE 1=1";
    std::vector<std::string> params;

    if (!sender_wallet_address.empty() || !receiver_wallet_address.empty()) {
        query += " AND (";

        if (!sender_wallet_address.empty()) {
            query += std::format("sender_wallet_address = ${}", params.size() + 1);
            params.push_back(sender_wallet_address);
        }

        if (!receiver_wallet_address.empty()) {
            if (!sender_wallet_address.empty()) {
                query += " OR ";
            }
            query += std::format("receiver_wallet_address = ${}", params.size() + 1);
            params.push_back(receiver_wallet_address);
        }

        query += ")";
    }

    if (!created_at.empty()) {
        query += std::format(" AND created_at::date = ${}", params.size() + 1);
        params.push_back(created_at);
    }


    pqxx::result R;
    try {
        switch (params.size()) {
        case 0:
            R = N.exec(query);
            break;
        case 1:
            R = N.exec_params(query, params[0].c_str());
            break;
        case 2:
            R = N.exec_params(query, params[0].c_str(), params[1].c_str());
            break;
        case 3:
            R = N.exec_params(query, params[0].c_str(), params[1].c_str(), params[2].c_str());
            break;
        }
    } catch (const pqxx::sql_error &e) {
        qDebug() << "SQL error: " << e.what();
        return {};
    }

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

    nlohmann::json j_report = nlohmann::json::array();
    for (const auto &transaction : transactions) {
        j_report.push_back({
            {"transaction_id", transaction.transaction_id},
            {"sender_wallet_address", transaction.sender_wallet_address},
            {"receiver_wallet_address", transaction.receiver_wallet_address},
            {"amount", transaction.amount},
            {"transaction_fee", transaction.transaction_fee},
            {"status", transaction.status},
            {"created_at", transaction.created_at}
        });
    }

    return j_report;
}
