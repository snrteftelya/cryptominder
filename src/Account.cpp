#include "headers/Account.h"

Account::Account(const std::string &client_name, const int account_balance)
        : client_name(client_name), account_balance(account_balance) {
}

Account::~Account() {
    wallets.clear();
}

void Account::display_account_info() const {
    std::cout << "Client name: " << client_name << std::endl;
    std::cout << "Account balance: " << account_balance << std::endl;
    if (!wallets.empty()) {
        std::cout << "Attached wallets: " << std::endl << std::endl;
        for (const auto &wallet: wallets) {
            wallet->display_wallet_info();
            std::cout << std::endl;
        }
    } else {
        std::cout << "No wallets are attached to this account." << std::endl << std::endl;
    }
}

std::string Account::get_client_name() const {
    return client_name;
}

void Account::set_client_name(const std::string_view &client_name) {
    this->client_name = std::string(client_name);
}

int Account::get_account_balance() const {
    return account_balance;
}

void Account::set_account_balance(const int account_balance) {
    this->account_balance = account_balance;
}

Wallet *Account::get_wallet(const std::string_view &wallet_address) const {
    for (const auto &wallet: wallets) {
        if (wallet->get_wallet_address() == wallet_address) {
            return wallet.get();
        }
    }
    return nullptr;
}

void Account::add_wallet(std::unique_ptr<Wallet> wallet) {
    wallet->set_wallet_address();
    if (get_wallet(wallet->get_wallet_address()) == nullptr) {
        wallets.push_back(std::move(wallet));
    } else {
        std::cout << "Wallet is already exists" << std::endl << std::endl;
    }
}

bool Account::delete_wallet(const std::string_view &wallet_address) {
    for (auto iter = wallets.begin(); iter != wallets.end(); ++iter) {
        if ((*iter)->get_wallet_address() == wallet_address) {
            wallets.erase(iter);
            return true;
        }
    }
    return false;
}

int Account::get_account_available_balance() const {
    int walletsbalance = 0;
    for (const auto &wallet: wallets) {
        walletsbalance += wallet->get_wallet_balance();
    }
    return (account_balance - walletsbalance > 0) ? account_balance - walletsbalance : 0;
}

void
Account::transfer_money(const std::string_view &recipient_wallet_address, const std::string_view &sender_wallet_address,
                        const int sum) const {
    Wallet *sender_wallet = get_wallet(sender_wallet_address);
    if (sender_wallet != nullptr) {
        if (sender_wallet->get_wallet_balance() >= sum) {
            Wallet *recipient_wallet = get_wallet(recipient_wallet_address);
            if (recipient_wallet != nullptr) {
                int new_balance_recipient = recipient_wallet->get_wallet_balance() + sum;
                recipient_wallet->set_wallet_balance(new_balance_recipient);

                int new_balance_sender = sender_wallet->get_wallet_balance() - sum;
                sender_wallet->set_wallet_balance(new_balance_sender);
            } else {
                std::cout << "There is no such wallet address : " << recipient_wallet_address << std::endl << std::endl;
            }
        } else {
            std::cout << "Sum is greater than sender's wallet balance " << std::endl;
            std::cout << "Sum: " << sum << std::endl;
            std::cout << "Balance: " << sender_wallet->get_wallet_balance() << std::endl << std::endl;
        }
    } else {
        std::cout << "There is no such wallet address : " << sender_wallet_address << std::endl << std::endl;
    }
}

void Account::transfer_money(const std::string_view &wallet_address, const int sum) const {
    int available_balance = get_account_available_balance();
    if (available_balance >= sum) {
        Wallet *wallet = get_wallet(wallet_address);
        if (wallet != nullptr) {
            int new_balance = wallet->get_wallet_balance() + sum;
            wallet->set_wallet_balance(new_balance);
        } else {
            std::cout << "There is no such wallet address : " << wallet_address << std::endl << std::endl;
        }
    } else {
        std::cout << "There is no available account balance" << std::endl << std::endl;
    }
}