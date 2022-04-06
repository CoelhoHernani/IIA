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

void knDamas::SolucaoVazia(void) {
	damas.Count(0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			tabuleiro[i][j] = 0;
}

// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
void knDamas::Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>& custo)
{	 
	//definir a linha do tabuleiro na qual estamos a trabalhar
	int coluna = damas.Count() / k;
	// tentar colocar damas em todas as colunas
	for (int j = 0; j < n; j++) {
		// verificar se casa esta ocupada ou se a coluna ou diagonal h� k damas
		if (this->tabuleiro[j][coluna] != 1 && verificarLinhaColunaDiagonal(this, j, coluna)) {
			knDamas* sucessor = (knDamas*)Duplicar();	//cria o sucessor, ou seja, uma copia do estado atual que ira ter x pe�as mais 1
			sucessor->damas.Add(j);					//numero damas no presente tabuleiro
			sucessor->tabuleiro[j][coluna] = 1;		//coloca��o da dama no tabueleiro
			//if(/*verificaDuplicado(sucessores, sucessor, coluna)/* &&*/ verificarSimetriaVertical(sucessores, sucessor, coluna) && verificarSimetriaDiagonal(sucessores, sucessor, coluna) && verificarSimetriaHorizontal(sucessores, sucessor, coluna))
				sucessores.Add(sucessor);				//adiciona sucessor(estado) � lista de sucessores se se verificar que n�o � simetrico 
		}
	}
	TProcuraConstrutiva::Sucessores(sucessores, custo);
}

bool knDamas::verificaDuplicado(TVector<TProcuraConstrutiva*>& sucessores, knDamas* sucessor, int colunaCorrente) {
	int flag = 0, count = 0;


	knDamas* teste = new knDamas;
	teste->damas.Count(sucessor->damas.Count());

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			teste->tabuleiro[n - 1 - i][j] = sucessor->tabuleiro[i][j];

	for (int i = 0; i < sucessores.Count(); i++)
		if (teste->damas.Count() == ((knDamas*)sucessores[i])->damas.Count())
			for (int coluna = 0; coluna <= colunaCorrente; coluna++) {
				for (int linha = 0; linha < n; linha++) {
					if (teste->tabuleiro[linha][coluna] == ((knDamas*)sucessores[i])->tabuleiro[linha][coluna]) {
						if (teste->tabuleiro[linha][coluna] == 1)
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
			
	/*for (int i = 0; i < sucessores.Count(); i++) {
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
	}*/
	return true;
}

//fun��o que pretende otimizar o programa, removendo possiveis estados simetricos aos j� existentes
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

//fun��o que verifica se numa coluna ou numa diagonal h� viola��o da regra k pe�as
bool knDamas::verificarLinhaColunaDiagonal(knDamas* objeto, int linha, int coluna) 
{
	if (coluna >= k) {
		int count = 0;
		//verificar linha
		for (int i = 0; i < coluna; i++) {
			if (objeto->tabuleiro[linha][i] == 1)
				count++;
			if (count == k)
				return false;
		}
		count = 0;
		//verifica diagonais sentido NO e NE
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

//fun��o que verifica se numa coluna ou numa diagonal h� viola��o da regra k pe�as
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
	//verifica diagonais sentido SE
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
	//verifica diagonais sentido SO
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