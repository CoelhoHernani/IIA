#include "menu.h"


/*
 * menu.cpp
 *
 *  Created on: 24/11/2020
 *      Author: Hernani Coelho
 *
 * Implementation file
 */

 //Biblioteca pr�pria
#include "menu.hpp"

namespace std {
    /**********************************M�todos classe menu*******************************/

    //Apresenta��o do menu ao utilizador
    menu::menu(cursos curso) {
        do {
            clearScreen();
            cout << "**********Licenciatura Engenharia Inform�tica**********\n\nEscolha uma das seguintes opc�es:" << endl;
            cout << "\t1 - Adicionar Aluno" << endl;
            cout << "\t2 - Consultar lista Alunos" << endl;
            cout << "\t3 - Adicionar UC" << endl;
            cout << "\t4 - Consultar lista UC" << endl;
            cout << "\t5 - Adicionar Avalia��o" << endl;
            cout << "\t6 - Consultar lista Avalia��es" << endl;
            cout << "\t7 - Consultar situa��o por Aluno" << endl;
            cout << "\t8 - Consultar situa��o por UC" << endl;
            cout << "\t0 - Sair" << endl;
            setOpcao();
            switch (getOpcao()) {
            case 1:
                curso.adicinarAluno();
                break;
            case 2:
                curso.imprimeAlunos();
                pressioneENTER();
                break;
            case 3:
                curso.adicionarUC();
                break;
            case 4:
                curso.imprimeUCs();
                pressioneENTER();
                break;
            case 5:
                curso.adicionarAvaliacao();
                break;
            case 6:
                curso.imprimeAvaliacoes();
                pressioneENTER();
                break;
            case 7:
                curso.progressoAluno();
                pressioneENTER();
                break;
            case 8:
                curso.progressoAluno(1);
                pressioneENTER();
                break;
            case 0:
                curso.gravarDadosAlunos();
                curso.gravarDadosUC();
                curso.gravarDadosAvaliacoes();
                break;
            default:
                cout << "Opc�o inv�lida!" << endl;
            }
        } while (opcao != 0);
    }
    //Destrutor
    menu::~menu() {

    }
    //defini��o metodo get
    int menu::getOpcao() const {
        return opcao;
    }
    //defini��o metodo set
    void menu::setOpcao() {
        cin >> opcao;
    }
}