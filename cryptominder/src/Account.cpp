#include "Account.h"
#include <iostream>
#include <pqxx/pqxx>
#include <memory>

Account::Account(const std::string &username, const std::string &email, const std::string &password_hash,
                 pqxx::connection &connection)
    : username(username), email(email), password_hash(password_hash), conn(connection) {
}

Account::~Account() {
    wallets.clear();
}

void Account::load_from_db(int account_id) {
    pqxx::work txn(conn);
    pqxx::result account_result = txn.exec_params(
        "SELECT username, email FROM account WHERE account_id = $1", account_id
    );
    txn.commit();
    if (!account_result.empty()) {
        username = account_result[0]["username"].c_str();
        email = account_result[0]["email"].c_str();
        wallets.clear();
        pqxx::result wallet_result = get_wallets_from_db(account_id);
        for (const auto &row: wallet_result) {
            wallets.add(std::make_unique<std::unique_ptr<Wallet>>(std::make_unique<Wallet>(
                row["wallet_address"].c_str(), row["balance"].as<double>(), conn
                )));
        }
        std::cout << "Account and wallets loaded from database." << std::endl;
    } else {
        std::cerr << "Account not found in the database." << std::endl;
    }
}

void Account::save_to_db() const {
    pqxx::work txn(conn);
    txn.exec_params(
        "INSERT INTO account (username, email, password_hash, created_at) VALUES ($1, $2, $3, CURRENT_TIMESTAMP)",
        username, email, password_hash
    );
    for (const auto &wallet: wallets) {
        txn.exec_params(
            "INSERT INTO wallet (account_id, wallet_address, balance, created_at) VALUES ($1, $2, $3, CURRENT_TIMESTAMP)",
            1, wallet->get()->get_wallet_address(), wallet->get()->get_wallet_balance()
        );
    }
    txn.commit();
    std::cout << "Account and wallets saved to database." << std::endl;
}

void Account::set_client_name(const std::string &client_name) {
    username = client_name;
    pqxx::work txn(conn);
    txn.exec_params("UPDATE account SET username = $1 WHERE account_id = $2", client_name, 1);
    txn.commit();
    std::cout << "Client name updated successfully." << std::endl;
}

void Account::set_account_email(const std::string &client_email) {
    email = client_email;
    pqxx::work txn(conn);
    txn.exec_params("UPDATE account SET email = $1 WHERE account_id = $2", client_email, 1);
    txn.commit();
    std::cout << "Client email updated successfully." << std::endl;
}

void Account::add_wallet(double initial_balance) {
    auto wallet = std::make_unique<Wallet>("address", initial_balance, conn);
    wallet->set_wallet_address();
    for (const auto &existing_wallet : wallets) {
        if (existing_wallet->get()->get_wallet_address() == wallet->get_wallet_address()) {
            std::cerr << "Wallet with this address already exists!" << std::endl;
            return;
        }
    }

    wallet->set_wallet_balance(initial_balance);
    wallets.add(std::make_unique<std::unique_ptr<Wallet>>(std::move(wallet)));
    pqxx::work txn(conn);
    txn.exec_params(
        "INSERT INTO wallet (account_id, wallet_address, currency, balance, created_at) VALUES ($1, $2, $3, $4, CURRENT_TIMESTAMP)",
        1, wallet->get_wallet_address(), "TON", initial_balance
        );
    txn.commit();
    std::cout << "Wallet added successfully with address: " << wallet->get_wallet_address() << std::endl;
}



bool Account::delete_wallet(const std::string &wallet_address) {
    for (auto it = wallets.begin(); it != wallets.end(); ++it) {
        if ((*it)->get()->get_wallet_address() == wallet_address) {
            wallets.remove(it);
            pqxx::work txn(conn);
            txn.exec_params("DELETE FROM wallet WHERE wallet_address = $1", wallet_address);
            txn.commit();
            std::cout << "Wallet deleted successfully." << std::endl;
            return true;
        }
    }
    std::cerr << "Wallet not found." << std::endl;
    return false;
}

void Account::transfer_money(const std::string &from_wallet, const std::string &to_wallet, double amount) {
    if (amount <= 0) {
        std::cerr << "Amount must be positive!" << std::endl;
        return;
    }

    auto sender_it = std::ranges::find_if(wallets.begin(), wallets.end(),
                                          [&from_wallet](const std::unique_ptr<std::unique_ptr<Wallet>> &wallet) {
                                              return (*wallet)->get_wallet_address() == from_wallet;
                                          });


    auto receiver_it = std::ranges::find_if(wallets.begin(), wallets.end(),
                                          [&to_wallet](const std::unique_ptr<std::unique_ptr<Wallet>> &wallet) {
                                              return (*wallet)->get_wallet_address() == to_wallet;
                                          });


    if (sender_it == wallets.end() || receiver_it == wallets.end()) {
        std::cerr << "Sender or receiver wallet not found." << std::endl;
        return;
    }

    Wallet *sender = sender_it->get()->get();
    Wallet *receiver = receiver_it->get()->get();

    if (sender->get_wallet_balance() < amount) {
        std::cerr << "Insufficient balance in sender's wallet." << std::endl;
        return;
    }

    sender->set_wallet_balance(sender->get_wallet_balance() - amount);
    receiver->set_wallet_balance(receiver->get_wallet_balance() + amount);
    sender->update_balance();
    receiver->update_balance();
    Transaction transaction(conn);
    transaction.add_transaction(from_wallet, to_wallet, amount, 0);
    std::cout << "Transfer successful!" << std::endl;
}

void Account::display_account_info() const {
    std::cout << "Client name: " << username << "\nClient email: " << email << "\nWallets:" << std::endl;
    for (auto it = wallets.begin(); it != wallets.end(); ++it) {
        std::cout << " - " << (*it)->get()->get_wallet_address() << ": " << (*it)->get()->get_wallet_balance() << std::endl;
    }
}

pqxx::result Account::get_wallets_from_db(int account_id) {
    pqxx::work txn(conn);
    return txn.exec_params(
        "SELECT wallet_address, balance FROM wallet WHERE account_id = $1", account_id
    );
}

pqxx::row Account::get_wallet_from_db(int account_id, std::string_view wallet_address) {
    pqxx::work txn(conn);
    pqxx::result wallet_result = txn.exec_params(
        "SELECT wallet_address, balance FROM wallet WHERE account_id = $1 AND wallet_address = $2", account_id, wallet_address
    );
    if (wallet_result.empty()) {
        std::cout << "No wallet found for the given account ID.";
    }
    return wallet_result[0];
}

