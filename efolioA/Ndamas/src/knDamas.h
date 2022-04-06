#pragma once
#include "TProcuraConstrutiva.h"

#include <vector>

///////////////////////////////////////////////////////////////////////////////
//	knDamas class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jose Coelho
//	Last revision: 2007-09-06
//	Description:
//    Implementa o problema das oito damas. Este problema consiste em colocar
//    oito damas de xadrez (movem-se na horizontal, vertical e diagonal), num
//    tabuleiro de xadrez, sem que estas se ataquem umas as outras.
///////////////////////////////////////////////////////////////////////////////
class knDamas : public TProcuraConstrutiva
{

public:
	//Parametros do tabuleiro e numero de damas por linha/coluna/diagonal

	knDamas(void);
	~knDamas(void);

	// posicao de cada dama
	TVector<int> damas;
	// vetor que contem as posi��es do tabuleiro do jogo
	TVector<TVector<int>> tabuleiro;

	// metodos redefinidos de TProcuraConstrutiva

	// Cria um objecto que e uma copia deste
	TProcuraConstrutiva* Duplicar(void);
	//faz uma c�pia do objeto que recebe
	void Copiar(TProcuraConstrutiva* objecto);
	// Coloca o objecto no estado inicial da procura
	void SolucaoVazia(void);
	// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
	// O custo nao necessita de ser preenchido, caso seja sempre unitario
	void Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>& custo);
	// Retorna verdade caso o estado actual seja um estado objectivo
	bool SolucaoCompleta(void) {
		if (damas.Count() == k * n) {
			printf("\nResultado: Solucao");
			return true;
		}
		return false;
	}
	// Escrever informacao de debug sobre o objecto currente
	// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
	void Debug(void);
	//verifica se os estado que se pretende criar � simetrico de algum existente
	bool verificarSimetriaHorizontal(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int linhaCorrente);
	bool verificarSimetriaVertical(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int linhaCorrente);
	bool verificarSimetriaDiagonal(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int linhaCorrente);
	bool verificaDuplicado(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int linhaCorrente);
	//verifica se a regra de k damas em linha coluna ou diagonal foi violada
	bool verificarLinhaColunaDiagonal(knDamas* objeto, int linha, int coluna);

	void contagemLinhaColunaDiagonal();

};
