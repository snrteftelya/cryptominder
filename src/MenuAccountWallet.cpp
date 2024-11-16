#include "headers/MenuAccountWallet.h"

void menu_account_wallet() {
    try {
        pqxx::connection C("dbname=crypto user=crypto_owner password=I6XNyohUfBj8 host=ep-yellow-truth-a2dw0siz.eu-central-1.aws.neon.tech sslmode=require");
        if (C.is_open()) {
            std::cout << "Соединение с базой данных успешно!" << std::endl;
        } else {
            std::cerr << "Не удалось открыть базу данных!" << std::endl;
        }

        pqxx::nontransaction N(C);
        pqxx::result R(N.exec("SELECT * FROM playing_with_neon LIMIT 5"));

        std::cout << "Результаты запроса:" << std::endl;
        for (auto row : R) {
            for (auto field : row) {
                std::cout << field.c_str() << " ";
            }
            std::cout << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}