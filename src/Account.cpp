#include "headers/Account.h"

Account::Account(const std::string &username, const std::string &email, const std::string &password_hash, pqxx::connection &connection) :
        username(username), email(email), password_hash(password_hash), conn(connection) {
}

Account::~Account() {
    wallets.clear();
}

void Account::display_account_info() {
    std::cout << "Client name: " << username << std::endl;
    std::cout << "Client email: " << email << std::endl;
}

void Account::set_client_name(const std::string_view &client_name_input) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(
                "UPDATE account SET username = $1 WHERE account_id = $2",
                client_name_input, 1
        );
        txn.commit();
        std::cout << "Client name updated successfully in the database." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error updating client name in database: " << e.what() << std::endl;
    }
}


void Account::set_account_email(const std::string_view &client_email_input) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(
                "UPDATE account SET email = $1 WHERE account_id = $2",
                client_email_input, 1
        );
        txn.commit();
        std::cout << "Client name updated successfully in the database." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error updating client name in database: " << e.what() << std::endl;
    }
}

std::unique_ptr<Wallet> Account::get_wallet(const std::string_view &wallet_address) const {
    try {
        pqxx::work txn(conn);  // Начинаем транзакцию
        pqxx::result result = txn.exec_params(
            "SELECT wallet_address, balance FROM wallet WHERE wallet_address = $1",
            wallet_address
        );

        if (!result.empty()) {
            return std::make_unique<Wallet>(
                result[0]["wallet_address"].c_str(),
                result[0]["balance"].as<double>(),
                conn
            );
        }
    } catch (const std::exception &e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    return nullptr;
}

void Account::add_wallet(std::unique_ptr<Wallet> wallet) {
    wallet->set_wallet_address();
    if (get_wallet(wallet->get_wallet_address()) == nullptr) {
        try {
            pqxx::work txn(conn);
            txn.exec_params(
                    "INSERT INTO wallet (account_id, wallet_address, currency, balance, created_at) VALUES ($1, $2, $3, $4, CURRENT_TIMESTAMP)",
                    1,
                    wallet->get_wallet_address().c_str(),
                    "TON",
                    wallet->get_wallet_balance()
            );
            txn.commit();
            std::cout << "Wallet saved to database." << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Database error: " << e.what() << std::endl;
        }
    }
    else {
        std::cout << "Wallet is already exists" << std::endl << std::endl;
    }
}

bool Account::delete_wallet(const std::string_view &wallet_address) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(
                "DELETE FROM wallet WHERE wallet_address = $1",
                wallet_address
        );
        txn.commit();
        std::cout << "Wallet deleted successfully." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error deleting wallet: " << e.what() << std::endl;
    }
    return false;
}

void Account::transfer_money(const std::string_view &recipient_wallet_address,
                              const std::string_view &sender_wallet_address,
                              const double sum) const {
    if (sum <= 0) {
        std::cout << "Transfer amount must be greater than zero." << std::endl;
        return;
    }

    std::unique_ptr<Wallet> sender_wallet(get_wallet(sender_wallet_address));
    if (sender_wallet) {
        if (sender_wallet->get_wallet_balance() >= sum) {
            std::unique_ptr<Wallet> recipient_wallet(get_wallet(recipient_wallet_address));
            if (recipient_wallet) {
                recipient_wallet->set_wallet_balance(recipient_wallet->get_wallet_balance() + sum);
                sender_wallet->set_wallet_balance(sender_wallet->get_wallet_balance() - sum);
                auto transaction = std::make_unique<Transaction>(conn);
                transaction->addTransaction(sender_wallet_address, recipient_wallet_address, sum, 0.0);

                std::cout << "Transfer successful!" << std::endl;
            } else {
                std::cout << "There is no such wallet address : " << recipient_wallet_address << std::endl;
            }
        } else {
            std::cout << "Sum is greater than sender's wallet balance." << std::endl;
            std::cout << "Sum: " << sum << std::endl;
            std::cout << "Balance: " << sender_wallet->get_wallet_balance() << std::endl;
        }
    } else {
        std::cout << "There is no such wallet address : " << sender_wallet_address << std::endl;
    }
}

void Account::save_to_db() const {
    try {
        pqxx::work txn(conn);
        txn.exec_params(
                "INSERT INTO account (username, email, password_hash, created_at) VALUES ($1, $2, $3, CURRENT_TIMESTAMP)",
                username,
                email,
                password_hash
        );
        txn.commit();
        std::cout << "Account saved to database." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}

void Account::load_from_db() {
    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec_params(
                "SELECT username, email, created_at FROM account WHERE account_id = 1"
        );
        if (!result.empty()) {
            username = result[0]["username"].c_str();
            email = result[0]["email"].c_str();

            std::cout << "Account loaded from database." << std::endl;
        } else {
            std::cout << "Account not found in database." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}