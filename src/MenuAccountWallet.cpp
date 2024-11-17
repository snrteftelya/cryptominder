#include "headers/MenuAccountWallet.h"

void display_menu() {
    std::cout << "1) Display account info" << std::endl;
    std::cout << "2) Add wallet" << std::endl;
    std::cout << "3) Delete wallet" << std::endl;
    std::cout << "4) Set account name" << std::endl;
    std::cout << "5) Set account email" << std::endl;
    std::cout << "6) Transfer money between two wallets" << std::endl;
    std::cout << "7) Get all transactions" << std::endl;
    std::cout << "8) Merge two wallets" << std::endl;
    std::cout << "9) Compare two wallets" << std::endl;
    std::cout << "0) Exit" << std::endl;
}

menu_options get_user_choice() {
    int choice;
    std::cout << "Your option: ";
    std::cin >> choice;
    return static_cast<menu_options>(choice);
}

void handle_account_info(Account* account) {
    account->load_from_db(1);
    account->display_account_info();
}

void handle_add_wallet(Account* account) {
    double wallet_balance;
    std::cout << "Enter wallet balance: ";
    std::cin >> wallet_balance;
    account->add_wallet(wallet_balance);
}

void handle_delete_wallet(Account* account) {
    std::string wallet_address;
    std::cout << "Enter wallet address to delete: ";
    std::cin >> wallet_address;
    account->delete_wallet(wallet_address);
}

void handle_set_name(Account* account) {
    std::string new_name;
    std::cout << "Enter new account name: ";
    std::cin >> new_name;
    std::cout << std::endl;
    account->set_client_name(new_name);
}

void handle_set_email(Account* account) {
    std::string new_email;
    std::cout << "Enter new email: ";
    std::cin >> new_email;
    std::cout << std::endl;
    account->set_account_email(new_email);
}

void handle_transfer_money(Account* account) {
    std::string recipient_wallet_address, sender_wallet_address;
    double sum;
    std::cout << "Enter recipient wallet address: ";
    std::cin >> recipient_wallet_address;
    std::cout << "Enter sender wallet address: ";
    std::cin >> sender_wallet_address;
    std::cout << "Enter sum to transfer: ";
    std::cin >> sum;
    account->transfer_money(sender_wallet_address, recipient_wallet_address, sum);
}

void handle_get_transactions(Transaction* transaction) {
    std::cout << "Here are the transactions:" << std::endl;
    transaction->getTransactions();
}

void handle_merge_wallets(Account* account) {
    std::string wallet_address1, wallet_address2;
    std::cout << "Enter first wallet address to merge: ";
    std::cin >> wallet_address1;
    std::cout << "Enter second wallet address to merge: ";
    std::cin >> wallet_address2;
    Wallet* wallet1 = nullptr;
    Wallet* wallet2 = nullptr;
    if (get_wallets_by_address(wallet_address1, wallet_address2, wallet1, wallet2, account->wallets)) {
        Wallet merged_wallet = *wallet1 + *wallet2;
        std::cout << "Merged wallet: " << merged_wallet << std::endl;
    } else {
        std::cout << "One or both wallets not found!" << std::endl;
    }
}

void handle_compare_wallets(Account* account) {
    std::string wallet_address1, wallet_address2;
    std::cout << "Enter first wallet address to compare: ";
    std::cin >> wallet_address1;
    std::cout << "Enter second wallet address to compare: ";
    std::cin >> wallet_address2;
    Wallet* wallet1 = nullptr;
    Wallet* wallet2 = nullptr;
    if (get_wallets_by_address(wallet_address1, wallet_address2, wallet1, wallet2, account->wallets)) {
        if (*wallet1 == *wallet2) {
            std::cout << "The wallets are the same!" << std::endl;
        } else {
            std::cout << "The wallets are different!" << std::endl;
        }
    } else {
        std::cout << "One or both wallets not found!" << std::endl;
    }
}

void menu_account_wallet() {
    std::string connectionString = "dbname=crypto user=crypto_owner password=I6XNyohUfBj8 host=ep-yellow-truth-a2dw0siz.eu-central-1.aws.neon.tech sslmode=require";
    pqxx::connection conn(connectionString);
    DatabaseSchema db(connectionString);
    db.createTables();
    auto account = std::make_unique<Account>("Mikalai", "test", "123", conn);
    auto transaction = Transaction(conn);
    auto new_wallet = std::make_unique<Wallet>("wallet_address_example", 0.0, conn);
    account->save_to_db();

    while (true) {
        display_menu();
        menu_options option = get_user_choice();

        switch (option) {
            case menu_options::account_info:
                handle_account_info(account.get());
                break;
            case menu_options::add_wallet:
                handle_add_wallet(account.get());
                break;
            case menu_options::delete_wallet:
                handle_delete_wallet(account.get());
                break;
            case menu_options::set_name:
                handle_set_name(account.get());
                break;
            case menu_options::set_email:
                handle_set_email(account.get());
                break;
            case menu_options::transfer_wallets:
                handle_transfer_money(account.get());
                break;
            case menu_options::get_transactions:
                handle_get_transactions(&transaction);
                break;
            case menu_options::merge_wallets:
                handle_merge_wallets(account.get());
                break;
            case menu_options::compare_wallets:
                handle_compare_wallets(account.get());
                break;
            case menu_options::end:
                std::cout << "Exit" << std::endl;
                return;
            default:
                std::cout << "Invalid option. Please try again" << std::endl;
        }
    }
}
