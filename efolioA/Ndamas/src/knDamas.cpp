#include "knDamas.h"
#include <iostream>

knDamas::knDamas(void)
{
}

knDamas::~knDamas(void)
{
}

// Cria um objecto que e uma copia deste
TProcuraConstrutiva* knDamas::Duplicar(void)
{
	knDamas *clone=new knDamas;
	clone->tabuleiro = tabuleiro;
	clone->damas = damas;
	clone->n = n;
	clone->k = k;
	return clone;
}

void knDamas::Copiar(TProcuraConstrutiva* objecto) {
	tabuleiro = ((knDamas*)objecto)->tabuleiro;
	damas = ((knDamas*)objecto)->damas;
}

//Coloca tabuleiro num estado inicial, vazio
void knDamas::SolucaoVazia(void) {
	damas.Count(0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			tabuleiro[i][j] = 0;
}

// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
void knDamas::Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>& custo)
{	 
	//definir a coluna do tabuleiro na qual estamos a trabalhar
	int coluna = damas.Count() / k;
	// tentar colocar damas em todas as linhas
	for (int j = 0; j < n; j++) {
		// verificar se casa esta ocupada ou se a coluna ou diagonal há k damas
		if (this->tabuleiro[j][coluna] != 1 && verificarLinhaColunaDiagonal(this, j, coluna)) {
			knDamas* sucessor = (knDamas*)Duplicar();	//cria o sucessor, ou seja, uma copia do estado atual que ira ter x peças mais 1
			sucessor->damas.Add(j);
			sucessor->tabuleiro[j][coluna] = 1;			//posição da dama no tabueleiro
			if(verificaDuplicado(sucessores, sucessor, coluna) && verificarSimetriaVertical(sucessores, sucessor, coluna) && verificarSimetriaDiagonal(sucessores, sucessor, coluna) && verificarSimetriaHorizontal(sucessores, sucessor, coluna))
				sucessores.Add(sucessor);				//adiciona sucessor(estado) à lista de sucessores se se verificar que não é simetrico 
		}
	}
	TProcuraConstrutiva::Sucessores(sucessores, custo);
}

//verifica se estado sucessor é igual a algum já existente na lista, se for retorna falso
bool knDamas::verificaDuplicado(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int colunaCorrente) {
	int flag = 0, count = 0;

	for (int i = 0; i < sucessores.Count(); i++) {
		flag = 0;
		count = 0;
		if (sucessor->damas.Count() == ((knDamas*)sucessores[i])->damas.Count())
			for (int coluna = 0; coluna <= colunaCorrente; coluna++) {
				for (int linha = 0; linha < n; linha++) {
					if (sucessor->tabuleiro[linha][coluna] == ((knDamas*)sucessores[i])->tabuleiro[linha][coluna]) {
						if (sucessor->tabuleiro[linha][coluna] == 1)
							count++;
					}
					else {
						flag = 1;
						break;
					}
					if (count == sucessor->damas.Count())
						return false;
				}
				if (flag == 1)
					break;
			}
	}
	return true;
}

//verifica se sucessor é simetrico horizontal de algum na lista
bool knDamas::verificarSimetriaHorizontal(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int colunaCorrente) {

	int flag = 0, count = 0;

	if (sucessores.Count() >= 2) {
		for (int i = 0; i < sucessores.Count(); i++) {
			flag = 0;
			count = 0;
			if (sucessor->damas.Count() == ((knDamas*)sucessores[i])->damas.Count())
				for (int coluna = 0; coluna <= colunaCorrente; coluna++) {
					for (int linha = 0; linha < n; linha++) {
						if (sucessor->tabuleiro[linha][coluna] == ((knDamas*)sucessores[i])->tabuleiro[linha][n - 1 - coluna]) {
							if (sucessor->tabuleiro[linha][coluna] == 1)
								count++;
						}
						else {
							flag = 1;
							break;
						}
						if (count == sucessor->damas.Count())
							return false;
					}
					if (flag == 1)
						break;
				}
		}
	}
	return true;
}

//verifica se sucessor é simetrico vertical de algum existente na lista
bool knDamas::verificarSimetriaVertical(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int colunaCorrente) {
	int flag = 0, count = 0;

	if (sucessores.Count() >= 2) {
		for (int i = 0; i < sucessores.Count(); i++) {
			flag = 0;
			count = 0;
			if (sucessor->damas.Count() == ((knDamas*)sucessores[i])->damas.Count())
				for (int coluna = 0; coluna <= colunaCorrente; coluna++) {
					for (int linha = 0; linha < n; linha++) {
						if (sucessor->tabuleiro[linha][coluna] == ((knDamas*)sucessores[i])->tabuleiro[n - 1 - linha][coluna]) {
							if (sucessor->tabuleiro[linha][coluna] == 1)
								count++;
						}
						else {
							flag = 1;
							break;
						}
						if (count == sucessor->damas.Count())
							return false;
					}
					if (flag == 1)
						break;
				}
		}
	}
	return true;
}

//verifica se sucessor é simetrico diagonal de algum estado na lista
bool knDamas::verificarSimetriaDiagonal(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int colunaCorrente) {
	int flag = 0, count = 0;

	if (sucessores.Count() >= 2) {
		for (int i = 0; i < sucessores.Count(); i++) {
			flag = 0;
			count = 0;
			if (sucessor->damas.Count() == ((knDamas*)sucessores[i])->damas.Count())
				for (int coluna = 0; coluna <= colunaCorrente; coluna++) {
					for (int linha = 0; linha < n; linha++) {
						if (sucessor->tabuleiro[linha][coluna] == ((knDamas*)sucessores[i])->tabuleiro[coluna][linha]) {
							if (sucessor->tabuleiro[linha][coluna] == 1)
								count++;
						}
						else {
							flag = 1;
							break;
						}
						if (count == sucessor->damas.Count())
							return false;
					}
					if (flag == 1)
						break;
				}
		}
	}
	return true;
}

//função que verifica se numa coluna ou numa diagonal há violação da regra k peças
bool knDamas::verificarLinhaColunaDiagonal(knDamas* objeto, int linha, int coluna) 
{
	if (coluna >= k) {
		int count = 0;
		//verificar em linha
		for (int i = 0; i < coluna; i++) {
			if (objeto->tabuleiro[linha][i] == 1)
				count++;
			if (count == k)
				return false;
		}
		count = 0;
		//verifica diagonais sentido NO e NE a partir da posicao que se esta
		for ( int i = linha-1, j = coluna-1; i >= 0 && j >= 0; i--, j--) {
			if (objeto->tabuleiro[i][j] == 1)
				count++;
			if (count == k)
				return false;
		}
		count = 0;
		for (int i = linha+1, j = coluna-1; i <= n && j >= 0; i++, j--) {
			if (objeto->tabuleiro[i][j] == 1)
				count++;
			if (count == k)
				return false;
		}
	}
	return true;
}

// Escrever informacao de debug sobre o objecto currente
// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
void knDamas::Debug(void)
{
	NovaLinha();
	for(int i = 0; i<n; i++){
		NovaLinha();
		for (int j = 0; j < n; j++) {
			if (tabuleiro[i][j] == 1)
				printf("D");
			else printf("+");
		}
	}
	NovaLinha();
	contagemLinhaColunaDiagonal();
}

//função que faz contagem para print do numero de pecas em linha coluna e diagonaiss
void knDamas::contagemLinhaColunaDiagonal()
{
	int count = 0;
	//contagem damas em linha
	printf("\nLinhas: ");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (this->tabuleiro[i][j] == 1)
				count++;
			if (j == n - 1)
				printf("%d ", count);
		}
		count = 0;
	}

	//contagem damas em coluna
	printf(" Colunas: ");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (this->tabuleiro[j][i] == 1)
				count++;
			if (j == n - 1)
				printf("%d ", count);
		}
		count = 0;
	}

	int diagonal = 0;
	//contagem damas sentido SE
	printf(" DiagSEID: ");
	while (diagonal < 2 * n - 1) {
		if (diagonal < n)
			for (int i = 0; i <= diagonal; i++) {
				if (this->tabuleiro[diagonal-i][i] == 1)
					count++;
					if (i == diagonal)
						printf("%d ", count);
			}
		else if (diagonal >= n)
			for (int i = n - 1; i > 0; i--) {
				if (this->tabuleiro[diagonal - i][i] == 1)
					count++;
				if (i == 1)
					printf("%d ", count);
			}
		count = 0;
		diagonal++;
	}
	//contagem damas sentido SO
	diagonal = 0;
	printf(" DiagSDIE: ");
	while (diagonal < 2 * n - 1) {
		if (diagonal < n)
			for (int i = 0; i <= diagonal; i++) {
				if (this->tabuleiro[diagonal-i][n-1-i] == 1)
					count++;
				if (i == diagonal)
					printf("%d ", count);
			}
		else if (diagonal >= n)
			for (int i = n - 1; i > 0; i--) {
				if (this->tabuleiro[diagonal - i][n-1-i] == 1)
					count++;
				if (i == 1)
					printf("%d ", count);
			}
		count = 0;
		diagonal++;
	}
}