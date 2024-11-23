#include <MenuAccountWallet.h>

void display_menu() {
    std::cout << "1) Display account info" << std::endl;
    std::cout << "2) Add wallet" << std::endl;
    std::cout << "3) Delete wallet" << std::endl;
    std::cout << "4) Set account name" << std::endl;
    std::cout << "5) Set account email" << std::endl;
    std::cout << "6) Transfer money between two wallets" << std::endl;
    std::cout << "7) Get all transactions" << std::endl;
    std::cout << "8) Top up wallet with money" << std::endl;
    std::cout << "9) Withdraw money from wallet" << std::endl;
    std::cout << "0) Exit" << std::endl;
}

menu_options get_user_choice() {
    int choice;
    std::cout << "Your option: ";
    std::cin >> choice;
    return static_cast<menu_options>(choice);
}

void handler_account_info(Account *account) {
    account->load_from_db(1);
    account->display_account_info();
}

void handler_add_wallet(Account *account) {
    double wallet_balance;
    std::cout << "Enter wallet balance: ";
    std::cin >> wallet_balance;
    account->add_wallet(wallet_balance);
}

void handler_delete_wallet(Account *account) {
    std::string wallet_address;
    std::cout << "Enter wallet address to delete: ";
    std::cin >> wallet_address;
    account->delete_wallet(wallet_address);
}

void handler_set_name(Account *account) {
    std::string new_name;
    std::cout << "Enter new account name: ";
    std::cin >> new_name;
    account->set_client_name(new_name);
}

void handler_set_email(Account *account) {
    std::string new_email;
    std::cout << "Enter new email: ";
    std::cin >> new_email;
    account->set_account_email(new_email);
}

void handler_transfer_money(Account *account) {
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
}

void handler_get_transactions(Transaction *transaction) {
    std::string wallet_address;
    std::cout << "Enter wallet address: ";
    std::cin >> wallet_address;
    std::cout << "Here are the transactions for your wallet:" << std::endl;
    transaction->get_transactions(wallet_address);
}

void handler_add_cryptocurrency_to_wallet(Account *account) {
    std::string wallet_address;
    std::cout << "Enter wallet address: ";
    std::cin >> wallet_address;
    double balance_to_add;
    std::cout << "How much money you need to top up:" << std::endl;
    std::cin >> balance_to_add;
    Wallet *wallet = nullptr;
    if (get_wallet_by_address(wallet_address, wallet, account->wallets)) {
        *wallet + balance_to_add;
        std::cout << *wallet << std::endl;
    } else {
        std::cout << "Wallet not found!" << std::endl;
    }
}

void handler_delete_cryptocurrency_from_wallet(Account *account) {
    std::string wallet_address;
    std::cout << "Enter wallet address: ";
    std::cin >> wallet_address;
    double balance_to_delete;
    std::cout << "How much money you need to withdraw:" << std::endl;
    std::cin >> balance_to_delete;
    Wallet *wallet = nullptr;
    if (get_wallet_by_address(wallet_address, wallet, account->wallets)) {
        *wallet - balance_to_delete;
        std::cout << *wallet << std::endl;
    } else {
        std::cout << "Wallet not found!" << std::endl;
    }
}

void menu_account_wallet() {
    std::string connectionString =
            "dbname=crypto user=crypto_owner password=I6XNyohUfBj8 host=ep-yellow-truth-a2dw0siz.eu-central-1.aws.neon.tech sslmode=require";
    pqxx::connection conn(connectionString);
    DatabaseSchema db(connectionString);
    db.create_tables();
    auto account = std::make_unique<Account>("Mikalai", "test", "123", conn);
    auto transaction = Transaction(conn);
    auto new_wallet = std::make_unique<Wallet>("wallet_address_example", 0.0, conn);
    account->save_to_db();
    using enum menu_options;
    while (true) {
        display_menu();
        menu_options option = get_user_choice();
        switch (option) {
            case account_info:
                handler_account_info(account.get());
                break;
            case add_wallet:
                handler_add_wallet(account.get());
                break;
            case delete_wallet:
                handler_delete_wallet(account.get());
                break;
            case set_name:
                handler_set_name(account.get());
                break;
            case set_email:
                handler_set_email(account.get());
                break;
            case transfer_money:
                handler_transfer_money(account.get());
                break;
            case get_transactions:
                handler_get_transactions(&transaction);
                break;
            case add_cryptocurrency_to_wallet:
                handler_add_cryptocurrency_to_wallet(account.get());
                break;
            case delete_cryptocurrency_from_wallet:
                handler_delete_cryptocurrency_from_wallet(account.get());
                break;
            case end:
                std::cout << "Exit" << std::endl;
                return;
            default:
                std::cout << "Invalid option. Please try again" << std::endl;
        }
    }
}
