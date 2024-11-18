#include "pqxx/pqxx"
#include "Account.h"
#include "DataBaseSchema.h"
#include <iostream>

enum class menu_options {
    end = 0,
    account_info,
    add_wallet,
    delete_wallet,
    set_name,
    set_email,
    transfer_wallets,
    get_transactions,
    comp_balance_wallets,
    compare_wallets
};

void menu_account_wallet();