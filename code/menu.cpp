//
// Created by Madalena Ye on 11/03/2023.
//

#include "menu.h"

Menu::Menu() {
    printf("\n");
    printf("\033[44m===========================================================\033[0m\t\t");
    cout << "\n\n" << " Bem-vindo!\n (Pressione [0] sempre que quiser voltar atrás)\n\n";
    supervisor = new Supervisor();
}

void Menu::init() {

}

void Menu::end() {
    printf("\n");
    printf("\033[46m===========================================================\033[0m\n");
}