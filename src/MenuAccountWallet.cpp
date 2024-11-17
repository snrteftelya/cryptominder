#include "headers/MenuAccountWallet.h"

void menu_account_wallet() {
    std::string connectionString = "dbname=crypto user=crypto_owner password=I6XNyohUfBj8 host=ep-yellow-truth-a2dw0siz.eu-central-1.aws.neon.tech sslmode=require";
    pqxx::connection conn(connectionString);
    DatabaseSchema db(connectionString);
    db.createTables();
    auto account = std::make_unique<Account>("Mikalai","test", "123", conn);
    auto transaction = Transaction(conn);
    auto new_wallet = std::make_unique<Wallet>("wallet_address_example", 0.0, conn);
    account->save_to_db();
    while (true) {
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
        std::cout << "Your option: ";
        int choice;
        std::cin >> choice;
        std::cout << std::endl;
        auto option = static_cast<menu_options>(choice);
        switch (option) {
            case menu_options::account_info: {
                account->load_from_db(1);
                account->display_account_info();
                break;
            }
            case menu_options::add_wallet: {
                double wallet_balance;
                std::cout << "Enter wallet balance: ";
                std::cin >> wallet_balance;
                account->add_wallet(wallet_balance);
                break;
            }
            case menu_options::delete_wallet: {
                std::string wallet_address;
                std::cout << "Enter wallet address to delete: ";
                std::cin >> wallet_address;
                account->delete_wallet(wallet_address);
                break;
            }
            case menu_options::set_name: {
                std::string new_name;
                std::cout << "Enter new account name: ";
                std::cin >> new_name;
                std::cout << std::endl;
                account->set_client_name(new_name);
                break;
            }
            case menu_options::set_email: {
                std::string new_email;
                std::cout << "Enter new email: ";
                std::cin >> new_email;
                std::cout << std::endl;
                account->set_account_email(new_email);
                break;
            }
            case menu_options::transfer_wallets: {
                std::string recipient_wallet_address;
                std::string sender_wallet_address;
                double sum;
                std::cout << "Enter recipient wallet address: ";
                std::cin >> recipient_wallet_address;
                std::cout << "Enter sender wallet address: ";
                std::cin >> sender_wallet_address;
                std::cout << "Enter sum to transfer: ";
                std::cin >> sum;
                account->transfer_money(sender_wallet_address, recipient_wallet_address, sum);
                break;
            }
            case menu_options::get_transactions: {
                std::cout << "Here are the transactions:" << std::endl;
                transaction.getTransactions();
                break;
            }
            case menu_options::merge_wallets: {
                std::string wallet_address1;
                std::string wallet_address2;
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
                break;
            }
            case menu_options::compare_wallets: {
                std::string wallet_address1;
                std::string wallet_address2;
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
                break;
            }
            case menu_options::end:
                std::cout << "Exit" << std::endl;
                return;
            default:
                std::cout << "Invalid option. Please try again" << std::endl;
        }
    }
}
