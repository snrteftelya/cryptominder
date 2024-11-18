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
    transfer_money,
    get_transactions,
    add_cryptocurrency_to_wallet,
    delete_cryptocurrency_from_wallet
};

void menu_account_wallet();
