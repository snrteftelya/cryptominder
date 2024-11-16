#pragma once

#include "pqxx/pqxx"
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include "Account.h"


enum class menu_options {
    end = 0,
    account_info,
    add_wallet,
    delete_wallet,
    get_balance,
    set_name,
    set_balance,
    transfer_account,
    transfer_wallets
};

void menu_account_wallet();