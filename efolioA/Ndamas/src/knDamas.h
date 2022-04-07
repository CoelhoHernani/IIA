#pragma once
#include "TProcuraConstrutiva.h"
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////
//	knDamas class
// 
//	Author: Hernani Coelho
//  Created on: 07/04/2022
//      
//	- Reutilizacao dos algoritmos disponibilizados na unidade curricular introdução à inteligencia artificial:
//	- Adaptacao ao problema knDamas, onde objetivo é colocar k*n damas num tabuleiro de dimensao n onde por linha
//	  coluna ou diagonal não pode existir mais de k damas
// 
//	- Alteracao para adaptacao ao problema dos metodos Sucessores, SolucaoVazia, Debug 
//	- Adicionado metodos verificarSimetriaHorizontal, verificarSimetriaVertical, verificarSimetriaDiagonal, 
//	  verificaDuplicado, verificarLinhaColunaDiagonal, contagemLinhaColunaDiagonal
// 
// declaration file
//////////////////////////////////////////////////////////////////////////////////////

class knDamas : public TProcuraConstrutiva
{

public:
	knDamas(void);
	~knDamas(void);

	// posicao de cada dama 
	TVector<int> damas;
	// vetor 2D que contem as posições do tabuleiro do jogo
	TVector<TVector<int>> tabuleiro;

	// metodos redefinidos de TProcuraConstrutiva

	// Cria um objecto que e uma copia deste
	TProcuraConstrutiva* Duplicar(void);
	//faz uma cópia do objeto que recebe
	void Copiar(TProcuraConstrutiva* objecto);
	// Coloca o objecto no estado inicial da procura
	void SolucaoVazia(void);
	// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
	// O custo nao necessita de ser preenchido, caso seja sempre unitario
	void Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>& custo);
	// Retorna verdade caso o estado actual seja um estado objectivo
	bool SolucaoCompleta(void);
	// Escrever informacao de debug sobre o objecto currente
	// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
	void Debug(void);
	//verifica se os estado que se pretende criar é simetrico na horizontal de algum sucessor no vetor
	bool verificarSimetriaHorizontal(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int linhaCorrente);
	//verifica se os estado que se pretende criar é simetrico na vertical de algum sucessor no vetor
	bool verificarSimetriaVertical(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int linhaCorrente);
	//verifica se os estado que se pretende criar é simetrico na diagonal de algum sucessor no vetor
	bool verificarSimetriaDiagonal(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int linhaCorrente);
	//verifica se os estado que se pretende criar é duplicado de algum sucessor no vetor
	bool verificaDuplicado(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int linhaCorrente);
	//verifica se a regra de k damas em linha coluna ou diagonal foi violada
	bool verificarLinhaColunaDiagonal(knDamas* objeto, int linha, int coluna);
	//faz a contagem das damas em linhas colunas e diagonais para fazer print
	void contagemLinhaColunaDiagonal();

};
