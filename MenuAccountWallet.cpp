#include "MenuAccountWallet.h"

void menu_account_wallet() {
    auto account = std::make_unique<Account>("Mikalai", 300);
    while (true) {
        std::cout << "1) Display account info" << std::endl;
        std::cout << "2) Add wallet" << std::endl;
        std::cout << "3) Delete wallet" << std::endl;
        std::cout << "4) Get available account balance" << std::endl;
        std::cout << "5) Set account name" << std::endl;
        std::cout << "6) Set account balance" << std::endl;
        std::cout << "7) Transfer money between account and wallet" << std::endl;
        std::cout << "8) Transfer money between two wallets" << std::endl;
        std::cout << "0) Exit" << std::endl;
        std::cout << "Your option : ";
        int choice;
        std::cin >> choice;
        std::cout << std::endl;
        auto option = static_cast<menu_options>(choice);
        switch (option) {
            case menu_options::account_info:
                account->display_account_info();
                break;
            case menu_options::add_wallet: {
                std::string wallet_address;
                int wallet_balance;
                std::cout << "Enter wallet balance: ";
                std::cin >> wallet_balance;
                account->add_wallet(std::make_unique<Wallet>(wallet_address, wallet_balance));
                break;
            }
            case menu_options::delete_wallet: {
                std::string wallet_address;
                std::cout << "Enter wallet address to delete: ";
                std::cin >> wallet_address;

                if (account->delete_wallet(wallet_address)) {
                    std::cout << "Wallet was deleted" << std::endl << std::endl;
                } else {
                    std::cout << "Wallet was not found" << std::endl << std::endl;
                }
                break;
            }
            case menu_options::get_balance:
                std::cout << "Available account balance: " << account->get_account_available_balance() << std::endl
                          << std::endl;
                break;
            case menu_options::set_name: {
                std::string new_name;
                std::cout << "Enter new account name: ";
                std::cin >> new_name;
                std::cout << std::endl;
                account->set_client_name(new_name);
                break;
            }
            case menu_options::set_balance: {
                int new_balance;
                std::cout << "Enter new account balance: ";
                std::cin >> new_balance;
                std::cout << std::endl;
                account->set_account_balance(new_balance);
                break;
            }
            case menu_options::transfer_account: {
                std::string wallet_address;
                int sum;
                std::cout << "Enter wallet address: ";
                std::cin >> wallet_address;
                std::cout << "Enter sum to transfer: ";
                std::cin >> sum;
                account->transfer_money(wallet_address, sum);
                break;
            }
            case menu_options::transfer_wallets: {
                std::string recipient_wallet_address;
                std::string sender_wallet_address;
                int sum;
                std::cout << "Enter recipient wallet address: ";
                std::cin >> recipient_wallet_address;
                std::cout << "Enter sender wallet address: ";
                std::cin >> sender_wallet_address;
                std::cout << "Enter sum to transfer: ";
                std::cin >> sum;
                account->transfer_money(recipient_wallet_address, sender_wallet_address, sum);
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