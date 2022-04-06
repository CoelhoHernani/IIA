#include "TProcuraConstrutiva.h"
#include <stdio.h>



// nivel de debug: 0 - nada; 1 - actividade; 2 - passos; 3 - detalhe
int TProcuraConstrutiva::debug=0;
// numero de geracaes de estados
int TProcuraConstrutiva::geracoes=0;
// numero de expansoes de estados
int TProcuraConstrutiva::expansoes=0;
// auxiliar para construcao da arvore de procura
TVector<unsigned char> TProcuraConstrutiva::ramo;
// espacamento entre ramos da arvore de debug
int TProcuraConstrutiva::espacosRamo=1;
// construir ou nao o caminho do estado inicial ate a solucao
bool TProcuraConstrutiva::calcularCaminho=false;
// valor retornado pela procura (tem de ser libertado)
TVector<TProcuraConstrutiva*> TProcuraConstrutiva::caminho;
// valor retornado pela procura (tem de ser libertado)
TProcuraConstrutiva* TProcuraConstrutiva::solucao=NULL;


// metodo interno para libertar objectos nao necessarios
void TProcuraConstrutiva::LibertarVector(TVector<TProcuraConstrutiva*>&vector, int excepto)
{
	for(int i=0;i<vector.Count();i++)
		if(i!=excepto && vector[i]!=NULL)
			delete vector[i];
	vector.Count(0);
}

// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
int TProcuraConstrutiva::LarguraPrimeiro(int limite)
{
	TVector<TProcuraConstrutiva*> lista;
	TVector<int> pais, custoTotal;
	lista.Add(this);
	pais.Add(-1);
	custoTotal.Add(0);
	for (int i = 0; i < lista.Count(); i++) {
		lista[i]->DebugPasso(custoTotal[i]);
		if (lista[i]->SolucaoCompleta()) { // teste da funcao Objectivo
			if (calcularCaminho) { // retornar todo o caminho
				caminho.Add(lista[i]);
				// obter caminho para a solucao
				for (int j = pais[i]; j > 0; j = pais[j]) {
					caminho.Add(lista[j]);
					if (debug > 1) printf(" (%d) ", custoTotal[j]);
					if (debug > 0) lista[j]->Debug();
					lista[j] = NULL;
				}
			}
			solucao = lista[i];
			lista[i]->DebugSolucao();
			lista[i] = NULL;
			LibertarVector(lista, 0);
			return custoTotal[i];
		}
		else { // expansao do no
			TVector<TProcuraConstrutiva*>sucessores;
			TVector<int> custo;
			lista[i]->Sucessores(sucessores, custo);
			if (limite) {
				// remover elementos a mais da lista de sucessores
				if (limite < lista.Count() - i + sucessores.Count()) {
					int maximo = limite - lista.Count() + i;
					if (maximo >= 0 && maximo < sucessores.Count()) {
						for (int i = maximo; i < sucessores.Count(); i++)
							delete sucessores[i];
						sucessores.Count(maximo);
					}
					else if (maximo < 0) {
						LibertarVector(sucessores);
					}
				}
			}
			for (int j = 0; j < sucessores.Count(); j++) {
				pais.Add(i);
				custoTotal.Add(custoTotal[i] + (custo.Count() > 0 ? custo[j] : 1));
			}
			lista += sucessores;
			// Nao se pode libertar estados ja expandidos porque nao se sabe se
			// os pais sao necessarios ou nao.
		}
	}
	LibertarVector(lista, 0);
	return -1; // falhou
}

// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
// 090318: bug ao remover elementos da lista
int TProcuraConstrutiva::CustoUniforme(int limite)
{
	TVector<TProcuraConstrutiva*> lista;
	TVector<int> pais, custoTotal;
	TVector<int> id;
	lista.Add(this);
	pais.Add(-1);
	custoTotal.Add(0);
	id.Add(0); // primeiro elemento e o 0
	for (int i = 0; i < id.Count(); i++) {
		lista[id[i]]->DebugPasso(custoTotal[id[i]]);
		if (lista[id[i]]->SolucaoCompleta()) { // teste da funcao Objectivo
			if (calcularCaminho) { // retornar todo o caminho
				caminho.Add(lista[id[i]]);
				// obter caminho para a solucao
				for (int j = pais[id[i]]; j > 0; j = pais[j]) {
					caminho.Add(lista[j]);
					if (debug > 1) printf(" (%d) ", custoTotal[j]);
					if (debug > 0) lista[j]->Debug();
					lista[j] = NULL;
				}
			}
			solucao = lista[id[i]];
			lista[id[i]]->DebugSolucao();
			lista[id[i]] = NULL;
			LibertarVector(lista, 0);
			return custoTotal[id[i]];
		}
		else { // expansao do no
			TVector<TProcuraConstrutiva*>sucessores;
			TVector<int> custo;
			lista[id[i]]->Sucessores(sucessores, custo);
			for (int j = 0; j < sucessores.Count(); j++) {
				pais.Add(id[i]);
				custoTotal.Add(custoTotal[id[i]] + (custo.Count() > 0 ? custo[j] : 1));
				// procurar o elemento com maior valor para inserir este elemento antes
				int l = i + 1;
				while (l < id.Count() && custoTotal[id[l]] <= custoTotal.Last())
					l++;
				if (l < id.Count())
					id.Insert(custoTotal.Count() - 1, l);
				else id.Add(custoTotal.Count() - 1);
			}
			lista += sucessores; // nao pode apagar nada porque podem ser pais, sempre a somar no fim da lista
			if (limite) {
				// remover elementos a mais da lista de sucessores, com a estimativa mais elevada
				while (lista.Count() - i > limite) {
					// como o ultimo elemento esta apos i, nao foi expandido, pelo que nao tem filhos
					int idRemover = id.Pop();
					delete lista[idRemover];
					lista.Delete(idRemover);
					custoTotal.Delete(idRemover);
					pais.Delete(idRemover);
					// ids com valor superior a idRemover, devem ser decrementados
					for (int j = 0; j < id.Count(); j++)
						if (id[j] > idRemover)
							id[j]--;
				}
			}
		}
	}
	LibertarVector(lista, 0);
	return -1; // falhou
}

// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
// caso o nivel=-1, e feita uma procura em profunidade normal
// caso o nivel>0, e feita uma procura em profundidade limitada
// caso o nivel=0, e feita uma procura em profundidade iterativa, sem limite;
int TProcuraConstrutiva::ProfundidadePrimeiro(int nivel)
{
	DebugChamada();
	if(nivel==0) { // metodo iterativo
		if(debug>=2) printf("\nNivel 1\n");
		int resultado=ProfundidadePrimeiro(++nivel);
		while(resultado==-1) {
			if(debug>=2) printf("\nNivel %d\n",nivel+1);
			resultado=ProfundidadePrimeiro(++nivel);
		}
		return resultado;
	} else { // metodo normal
		if(SolucaoCompleta()) { // um no objectivo!
			DebugSolucao();
			solucao=this;
			return 0;
		}
		if(nivel>1 || nivel<0) { // continuar a procura
			// ainda nao e o no objectivo
			TVector<TProcuraConstrutiva*> sucessores;
			TVector<int> custos;
			Sucessores(sucessores, custos);
			// tentar todos os sucessores, um de cada vez
			for(int i=0;i<sucessores.Count();i++) {
				DebugExpansao(i,sucessores.Count());
				int resultado=sucessores[i]->ProfundidadePrimeiro(nivel-1);
				if(resultado>=0) { // este sucessor resolveu o problema, devolver
					LibertarVector(sucessores,i);
					if(calcularCaminho)
						caminho.Add(sucessores[i]); // adicionar este no ao caminho
					resultado++;
					if(debug>1)
						printf(" (%d) ",resultado);
					DebugSolucao();
					return resultado;
				}
			}
			DebugCorte(sucessores.Count());
			LibertarVector(sucessores);
		} else DebugCorte();
		// percorreram-se todos os sucessores e nada (ou atingiu-se o limite), devolver -1
		return -1;
	}
}

// Escrever informacao de debug sobre o objecto currente (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
void TProcuraConstrutiva::Debug(void)
{
	if(debug>0)
		printf("\nTProcuras::Debug nao definido, nao e assim possivel mostrar informacao relativa a este estado.");
}

// Metodo para ser chamado antes de analisar cada sucessor
void TProcuraConstrutiva::DebugExpansao(int sucessor, int sucessores,bool duplo)
{
	if(debug>=2) {
		if(sucessor>0) {
			NovaLinha(false);
		}
	//**** alterado JM  29/03/2018 - códigos ASCII

		if(sucessor==0 && sucessores==1) { // so um ramo
			printf("%*c%c",espacosRamo,45,(duplo?45:45));//215:197));
			ramo.Add(32); // a ser impresso nesta posicao nas linhas seguintes
		} else if(sucessor==0) { // inicio e continua
			printf("%*c%c",espacosRamo,45,(duplo?43:43));//210:194));
			ramo.Add(duplo?124:124); // a ser impresso nesta posicao nas linhas seguintes
		} else if(sucessor>0 && sucessor<sucessores-1) { // no meio e continua
			printf("%*s%c",espacosRamo," ",(duplo?124:124));//199:195));
			ramo.Last()=(duplo?124:124);
		} else {
			printf("%*s%c",espacosRamo," ",(duplo?39:39));//211:192)); // no fim, vai acabar
			ramo.Last()=32; // a ser impresso nesta posicao nas linhas seguintes

	//**** alterado JM  29/03/2018 - códigos ASCII ******** //
		}
	}
}

// Encontrou uma solucao
void TProcuraConstrutiva::DebugSolucao(void)
{
	if(debug>0 && SolucaoCompleta()) {
		printf(" Solucao encontrada!");
		ramo.Count(0);
		Debug();
	} else {
		if(debug>1) Debug();
		if(debug>=2) ramo.Pop();
	}
}

// Informacao de debug na chamada ao metodo recursivo
void TProcuraConstrutiva::DebugChamada(void)
{
	if(debug==1 && expansoes%1000==0)
		printf("#");
//	else if(debug==2) printf("%c",196);
//	else if(debug>2) Debug();
}

// Metodo para ser chamado quando nao ha sucessores ou ha um corte de profundidade
void TProcuraConstrutiva::DebugCorte(int sucessores, bool duplo)
{
	if (debug >= 2) {
		if (sucessores < 0) {

			//**** alterado JM  29/03/2018 - códigos ASCII ******** //

			printf("%c %d/%d", 45, expansoes, geracoes); // corte de profundidade
			if (debug >= 3)
				Debug();
		}
		else if (sucessores > 0)
			ramo.Pop();
		else { // ramo em que nao e possivel continuar
			printf("%c %d/%d", 63, expansoes, geracoes);
			if (debug >= 3)
				Debug();

			//**** alterado JM  29/03/2018 - códigos ASCII ******** //
		}
	}
}

// Passo no algoritmo em largura
void TProcuraConstrutiva::DebugPasso(int custo)
{
	if (debug == 1 && expansoes % 1000 == 0)
		printf("#");
	if (debug >= 2) {
		printf("\n%02d: %d/%d", custo, expansoes, geracoes);
	}
	if (debug > 2)
		Debug();
}


// Chamar sempre que se quer uma nova linha com a arvore em baixo
void TProcuraConstrutiva::NovaLinha(bool tudo)
{
	printf("\n");
	for(int i=0;i<ramo.Count()-(tudo?0:1);i++)
		printf("%*s%c",espacosRamo," ",ramo[i]);
}

// Chamar antes de iniciar uma procura
void TProcuraConstrutiva::LimparEstatisticas(void)
{
	expansoes = geracoes = 0;
	ramo.Count(0);
	if (calcularCaminho)
		LibertarVector(caminho, 0);
	if (solucao != NULL)
		delete solucao;
	solucao = NULL;
}

// Metodo para teste manual do objecto (chamadas aos algoritmos, construcao de uma solucao manual)
// Este metodo destina-se a testes preliminares, e deve ser redefinido apenas se forem definidos novos algoritmos
void TProcuraConstrutiva::TesteManual(const char *nome, int seed)
{
	clock_t start, end;
	instancia vetor;
	char str[256];
	int limiteNivel=10;
	TRand::srand(seed);
	SolucaoVazia();
	while(true) {
		printf("\n\
%s\n\
(TProcuraConstrutiva)\n\
[Configuracoes] debug %d | calcularCaminho %d | limiteNivel %d\n\
[Estatisticas] expansoes %d | geracoes %d",
			nome,debug,calcularCaminho?1:0,limiteNivel,expansoes,geracoes);
		Debug();
		printf("\n\
_______________________________________________________________________________\n\
1 - Definir Instancia | 2 - Solucao Vazia                       [Inicializacao]\n\
3 - Prof.Primeiro | 4 - Lar. Primeiro | 5 - Custo Uniforme     [Procuras Cegas]\n\
6 - debug | 7 - calcularCaminho | 8 - limiteNivel               [Configuracoes]\n\
9 - Sucessores                                                         [Testes]\n\
10 - Limpar Estatistica                                          [Estatisticas]\n\
0 - Sair\n\
Opcao:");
		fgets(str, 256, stdin);
		if(str[0]==0) return;        // jm em 02/04/2018
		switch(atoi(str)) {
			case 1:
				definirInstancia(vetor);
				break;
			case 2:
				LimparEstatisticas();
				TRand::srand(seed); SolucaoVazia();
				break;
			case 3:
				LimparEstatisticas();
				start = clock();
				printf("\nResultado ProfundidadePrimeiro(%d): %d",limiteNivel,ProfundidadePrimeiro(limiteNivel));
				end = clock();
				printf("\nTempo gasto ate encontrar solucao: %f \n", ((((float)end - start))/CLOCKS_PER_SEC));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
					solucao=NULL;
				}
				break;
			case 6: printf("\nNovo valor para debug: "); fgets(str, 256, stdin); debug=atoi(str); break;
			case 7: printf("\nNovo valor para cacularCaminho: "); fgets(str, 256, stdin); calcularCaminho=(atoi(str)!=0); break;
			case 8: printf("\nNovo valor para limiteNivel: "); fgets(str, 256, stdin); limiteNivel=atoi(str); break;
			case 9: // sucessores
				{
					TVector<TProcuraConstrutiva*> sucessores;
					TVector<int> custos;
					int opcao;
					do {
						Sucessores(sucessores, custos);
						printf("\nEstado actual:");
						Debug();
						printf("\nSucessores: %d",sucessores.Count());
						printf("\nMenu:\n1 a %d - vizualizar sucessor\n-1 a -%d - mover para sucessor\n 0 - sair.\nOpcao:",
							sucessores.Count(),sucessores.Count());
						fgets(str, 256, stdin);
						opcao=atoi(str);
						if(opcao>0 && opcao<=sucessores.Count()) {
							printf("\nSucessor %d",opcao);
							sucessores[opcao-1]->Debug();
						} else if(opcao<0 && opcao>=-sucessores.Count()) {
							Copiar(sucessores[-opcao-1]);
						}
						LibertarVector(sucessores);
					} while(opcao!=0);
				}
				break;
			case 4:
				LimparEstatisticas();
				start = clock();
				printf("\nResultado LarguraPrimeiro(%d): %d", limiteNivel, LarguraPrimeiro(limiteNivel));
				end = clock();
				printf("\nTempo gasto ate encontrar solucao: %f \n", ((((float)end - start)) / CLOCKS_PER_SEC));
				if (solucao != NULL) {
					Copiar(solucao);
					delete solucao;
					solucao = NULL;
				}
				break;
			case 5:
				LimparEstatisticas();
				start = clock();
				printf("\nResultado CustoUniforme(%d): %d", limiteNivel, CustoUniforme(limiteNivel));
				end = clock();
				printf("\nTempo gasto ate encontrar solucao: %f \n", ((((float)end - start)) / CLOCKS_PER_SEC));
				if (solucao != NULL) {
					Copiar(solucao);
					delete solucao;
					solucao = NULL;
				}
				break;
			case 10:
				LimparEstatisticas();
				break;
			case 0:
				return;
			default: printf("\nOpcao nao definida."); break;
		}
	}
}

//define um menu para utilizador escolher a instancia que pretende executar uma procura
void TProcuraConstrutiva:: definirInstancia(instancia &v) {
	char str[256];
	printf("\n\
Instancias:\n\
 1 - N = 4 K = 2  |  2 - N = 4 K = 3 |  3 - N = 5 K = 2 |  4 - N = 5 K = 3 \n\
 5 - N = 6 K = 2  |  6 - N = 6 K = 3 |  7 - N = 7 K = 2 |  8 - N = 7 K = 3 \n\
 9 - N = 8 K = 2  | 10 - N = 8 K = 4 | 11 - N = 9 K = 4 | 12 - N = 10 K = 2 \n\
13 - N = 10 K = 3 | 14 - N = 10 K = 4");
	printf("\nEscolha uma instancia: ");
	fgets(str, 256, stdin);
	if (str[0] == 0 || atoi(str) < 0 || atoi(str) > 14) return;
	
	for (int i = 0; i < v.instancias.size(); i++)
		if (i == atoi(str)-1) {
			n = v.instancias[i][0];
			k = v.instancias[i][1];
		}
}
