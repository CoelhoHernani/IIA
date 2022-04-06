#pragma once
/*
 * menu.hpp
 *
 *  Created on: 24/11/2020
 *      Author: Hernani Coelho
 *
 * declaration file
 */

//Biblioteca padrão
#include <iostream>

namespace std {
    //declaração da classe menu
    class menu {
    private:
        int opcao;
    public:
        menu();
        ~menu();
        void setOpcao();
        int getOpcao() const;
    };
};


