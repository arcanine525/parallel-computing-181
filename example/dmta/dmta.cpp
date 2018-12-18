/* Algoritmo DMTA (Distributed Multithread Apriori), desenvolvido na UFLA */
/* (Universidade Federal de Lavras) por Andre' Camilo Bolina, sob orientacao */
/* de Prof. Dr. Marluce Pereira Rodrigues, Prof. Dr. Ahmed Ali Abdalla Esmin */
/* e Prof. Dr. Denilson ALves Pereira. */
/* Data da implementação: 18/09/2012 */
/* Publicação disponível em http://www.fsma.edu.br/si/edicao11/FSMA_SI_2013_1_Principal_1.html */

//Bibliotecas
#include "mpi.h"
#include "omp.h"
#include <sys/time.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>

//Definicao do ambiente C++
using namespace std;

//Definicoes de novos tipos de variaveis
typedef vector < vector < int > > matint;
typedef vector < matint > mattri;
typedef vector < int > vetint;

//Declaracoes das funcoes
mattri le_dados(char entrada []);
mattri inverte(mattri matriz);
mattri ordena(mattri matriz, int sup);
mattri gera2items(mattri matriz, int sup, int numthreads);
mattri gerakitems(mattri matriz, int sup, int iteracao, int numthreads);
vetint intersecao(vetint vet2, vetint vet1);
void quicksort(int matriz[][2], int inicio, int fim);
int iguais(vetint vet1, vetint vet2);
bool pertence(int item, vetint vet);
int maior(mattri matriz);
int itensvet(mattri matriz);
void escreve(mattri matriz);
void msginicial(int prosize);

//Funcao principal
int main(int argc, char **argv) {
  //Inicializao do MPI
  MPI::Init(argc,argv);
  MPI_Status status;
  MPI_Request request;
  int prosize = MPI::COMM_WORLD.Get_size();
  int prorank = MPI::COMM_WORLD.Get_rank();

  //Declaracao das variaveis
  int sup;
  int encerra;
  int auxsr;
  int *vetorsr;
  int numitensvet;
  int iteracao = 1;
  int numthreads = atoi(argv[2]);
  double minsup = atof(argv[3]);
  mattri tabelak;

  //Leitura da entrada e definicao do suporte minimo real
  mattri tabela1 = le_dados(argv[1]);
  sup = (minsup * tabela1.size()) + 1;

  //Variaveis de tempo
  timeval tim;
  double t1,t2,t3,b1,b2,b3,c1,c2,c3;
  if (prorank == 0) {
	gettimeofday(&tim, NULL);
	t1=tim.tv_sec+(tim.tv_usec/1000000.0);
  }

  //Inversao da tabela e eliminacao de candidatos
  tabela1 = inverte(tabela1);
  tabela1 = ordena(tabela1, sup);

  //Apenas o processo mestre executa
  if (prorank == 0) {
	//Escrita dos resultados para 1-itemsets
	msginicial(prosize);
	encerra = 0;
	if (tabela1.size() > 0) {
  		escreve(tabela1);
	} else {
		cout << "Não houve itemsets acima do suporte mínimo." << endl;
		encerra = 1;
	}
  }

  MPI_Bcast(&encerra, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (encerra == 1) {
	MPI::Finalize();
	exit(0);
  }

  tabelak = gera2items(tabela1, sup, numthreads);

  if(prorank != 0) {
	if (tabelak.size() > 0) {
  		numitensvet = itensvet(tabelak);
  		vetorsr = new int[numitensvet];
		auxsr = 0;
  		for (int i = 0; i < tabelak.size(); ++i) {
			for (int j = 0; j < tabelak[0][0].size(); j++) {
				vetorsr[auxsr] = tabelak[i][0][j];
				auxsr++;
			}
			for (int k = 0; k < tabelak[i][1].size(); k++) {
				vetorsr[auxsr] = tabelak[i][1][k];
				auxsr++;
			}
			vetorsr[auxsr] = -1;
			auxsr++;
  		}
	} else {
		numitensvet = 1;
		vetorsr = new int[numitensvet];
	}
	MPI_Send(&numitensvet,1,MPI_INT,0,NULL,MPI_COMM_WORLD);
	MPI_Send(vetorsr,numitensvet,MPI_INT,0,NULL,MPI_COMM_WORLD);
	delete [] vetorsr;
  }

  if (prorank == 0) {
	matint matpar;
	vetint vet;
	for (int i=1; i < prosize; i++) {
		MPI_Recv(&numitensvet,1,MPI_INT,MPI_ANY_SOURCE,NULL,MPI_COMM_WORLD,&status);
		vetorsr = new int[numitensvet];	
		MPI_Recv(vetorsr,numitensvet,MPI_INT,status.MPI_SOURCE,NULL,MPI_COMM_WORLD,&status);
		auxsr = 0;
		if (numitensvet != 1) {
			while (auxsr < numitensvet) {
  				for (int j = 0; j < (iteracao+1); ++j) {
					vet.push_back(vetorsr[auxsr]);
					auxsr++;
				}
				matpar.push_back(vet);
				vet.erase(vet.begin(), vet.end());
				for (int k = 0; k < numitensvet; ++k) {
					if (vetorsr[auxsr] != -1) {
						vet.push_back(vetorsr[auxsr]);
						auxsr++;
					} else {
						k = numitensvet;
						auxsr++;
					}
				}	
				matpar.push_back(vet);
				tabelak.push_back(matpar);
				matpar.erase(matpar.begin(), matpar.end());
				vet.erase(vet.begin(), vet.end());
			}
		}
		delete [] vetorsr;	
	}
  }

  if (prorank == 0) { 
	if (tabelak.size() > 1) {
		tabelak = ordena(tabelak, sup);
	}
	if (tabelak.size() > 0) {
		escreve(tabelak);
	}
  	if (tabelak.size() > 1) {
		encerra = 0;
		numitensvet = itensvet(tabelak);
	  	vetorsr = new int[numitensvet];
		auxsr = 0;
	  	for (int i = 0; i < tabelak.size(); ++i) {
			for (int j = 0; j < tabelak[0][0].size(); j++) {
				vetorsr[auxsr] = tabelak[i][0][j];
				auxsr++;
			}
			for (int k = 0; k < tabelak[i][1].size(); k++) {
				vetorsr[auxsr] = tabelak[i][1][k];
				auxsr++;
			}
			vetorsr[auxsr] = -1;
			auxsr++;
	  	}
	} else {
		cout << "Algoritmo encerrado." << endl;
		encerra = 1;
	}
  }

  MPI_Bcast(&encerra, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (encerra == 1) {
	MPI::Finalize();
	exit(0);
  }

  MPI_Bcast(&numitensvet, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (prorank != 0) vetorsr = new int[numitensvet];
  MPI_Bcast(vetorsr, numitensvet, MPI_INT, 0, MPI_COMM_WORLD);

  if (prorank != 0) {
	matint matpar;
	vetint vet;
	tabelak.erase(tabelak.begin(), tabelak.end());
	auxsr = 0;
	while (auxsr < numitensvet) {
	  	for (int j = 0; j < 2; ++j) {
			vet.push_back(vetorsr[auxsr]);
			auxsr++;
		}
		matpar.push_back(vet);
		vet.erase(vet.begin(), vet.end());
		for (int k = 0; k < numitensvet; ++k) {
			if (vetorsr[auxsr] != -1) {
				vet.push_back(vetorsr[auxsr]);
				auxsr++;
			} else {
				k = numitensvet;
				auxsr++;
			}
		}	
		matpar.push_back(vet);
		tabelak.push_back(matpar);
		matpar.erase(matpar.begin(), matpar.end());
		vet.erase(vet.begin(), vet.end());
	}
  }

  iteracao = 2;
  while (tabelak.size() > 3) {
  	tabelak = gerakitems(tabelak, sup, iteracao, numthreads);

	if (prorank != 0) {
		if (tabelak.size() > 0) {
  			numitensvet = itensvet(tabelak);
  			vetorsr = new int[numitensvet];
			auxsr = 0;
  			for (int i = 0; i < tabelak.size(); ++i) {
				for (int j = 0; j < tabelak[0][0].size(); j++) {
					vetorsr[auxsr] = tabelak[i][0][j];
					auxsr++;
				}
				for (int k = 0; k < tabelak[i][1].size(); k++) {
					vetorsr[auxsr] = tabelak[i][1][k];
					auxsr++;
				}
				vetorsr[auxsr] = -1;
				auxsr++;
  			}
		} else {
			numitensvet = 1;
			vetorsr = new int[numitensvet];
		}
		MPI_Send(&numitensvet,1,MPI_INT,0,NULL,MPI_COMM_WORLD);
		MPI_Send(vetorsr,numitensvet,MPI_INT,0,NULL,MPI_COMM_WORLD);
		delete [] vetorsr;
	}

  	if (prorank == 0) {
		matint matpar;
		vetint vet;
		for (int i=1; i < prosize; i++) {
			MPI_Recv(&numitensvet,1,MPI_INT,MPI_ANY_SOURCE,NULL,MPI_COMM_WORLD,&status);
			vetorsr = new int[numitensvet];
			MPI_Recv(vetorsr,numitensvet,MPI_INT,status.MPI_SOURCE,NULL,MPI_COMM_WORLD,&status);

			auxsr = 0;
			if (numitensvet != 1) {
				while (auxsr < numitensvet) {
	  				for (int j = 0; j < (iteracao+1); ++j) {
						vet.push_back(vetorsr[auxsr]);
						auxsr++;
					}
					matpar.push_back(vet);
					vet.erase(vet.begin(), vet.end());
					for (int k = 0; k < numitensvet; ++k) {
						if (vetorsr[auxsr] != -1) {
							vet.push_back(vetorsr[auxsr]);
							auxsr++;
						} else {
							k = numitensvet;
							auxsr++;
						}
					}	
					matpar.push_back(vet);
					tabelak.push_back(matpar);
					matpar.erase(matpar.begin(), matpar.end());
					vet.erase(vet.begin(), vet.end());
				}
			}
			delete [] vetorsr;	
		}
	}

  	if (prorank == 0) {
		if (tabelak.size() > 1) {
			tabelak = ordena(tabelak, sup);
		}
		if (tabelak.size() > 0) {
			escreve(tabelak);
  			numitensvet = itensvet(tabelak);
  			vetorsr = new int[numitensvet];
			auxsr = 0;
  			for (int i = 0; i < tabelak.size(); ++i) {
				for (int j = 0; j < tabelak[0][0].size(); j++) {
					vetorsr[auxsr] = tabelak[i][0][j];
					auxsr++;
				}
				for (int k = 0; k < tabelak[i][1].size(); k++) {
					vetorsr[auxsr] = tabelak[i][1][k];
					auxsr++;
				}
				vetorsr[auxsr] = -1;
				auxsr++;
  			}
			encerra = 0;	
		} else {
			encerra = 1;
		}
 	}

	MPI_Bcast(&encerra, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (encerra != 1) {
		MPI_Bcast(&numitensvet, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (prorank != 0) vetorsr = new int[numitensvet];
		MPI_Bcast(vetorsr, numitensvet, MPI_INT, 0, MPI_COMM_WORLD);

	  	if (prorank != 0) {
			matint matpar;
			vetint vet;
			tabelak.erase(tabelak.begin(), tabelak.end());
			auxsr = 0;
			while (auxsr < numitensvet) {
			  	for (int j = 0; j < (iteracao+1); ++j) {
					vet.push_back(vetorsr[auxsr]);
					auxsr++;
				}
				matpar.push_back(vet);
				vet.erase(vet.begin(), vet.end());
				for (int k = 0; k < numitensvet; ++k) {
					if (vetorsr[auxsr] != -1) {
						vet.push_back(vetorsr[auxsr]);
						auxsr++;
					} else {
						k = numitensvet;
						auxsr++;
					}
				}	
				matpar.push_back(vet);
				tabelak.push_back(matpar);
				matpar.erase(matpar.begin(), matpar.end());
				vet.erase(vet.begin(), vet.end());
			}
		}
		iteracao = iteracao+1;
	}
  }

  if (prorank == 0) {
	gettimeofday(&tim, NULL);
	t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	t3 = t2-t1;
	cout << "Tempo total: " << t3 << endl;
	cout << "Algoritmo encerrado." << endl;
  }

  MPI::Finalize();
  return 0;
}

/* Verifica e cria o arquivo de saida, escreve as primeiras linhas no mesmo e a
mensagem inicial de execucao do algoritmo. */
void msginicial(int prosize) {
  ifstream arq("data/saida.txt");
	ofstream saida("data/saida.txt");
	saida << "Arquivo gerado pelo algoritmo DMTA (Distributed Multithread Apriori), ";
	saida << "disponível em https://sourceforge.net/projects/dmta.";
	saida << endl;
	saida.close();

	//cout << "Algoritmo iniciado com " << prosize << " processos..." << endl;
}

/* Le os dados do arquivo de entrada. */
mattri le_dados(char entrada []) {
  int item, cont = -1;
  mattri matriz;
  matint matpar;
  vetint vet;
  ifstream file(entrada);
  if (file.good()) {
	string line;
	while (getline(file, line)) {
		cont = cont + 1;
		vet.push_back(cont);
		matpar.push_back(vet);
		vet.erase(vet.begin());
		istringstream iss(line);
		while (iss >> item) {
			vet.push_back(item);
		}
		matpar.push_back(vet);
		matriz.push_back(matpar);
		vet.erase(vet.begin(), vet.end());
		matpar.erase(matpar.begin(), matpar.end());
	}
  	return matriz;
  } else {
	cout << "Arquivo de entrada nao encontrado!" << endl;
	exit(0);
  }
}

/* Inverte a matriz inicial onde cada linha representa uma transacao
e as colunas representam items, para uma matriz em que cada item fica
em uma linha e as transacoes em que ele esta presente ficam em colunas */
mattri inverte(mattri matriz) {
  mattri mataux, mataux2;
  matint matpar;
  vetint vet;
  int item, totalitems = maior(matriz);
  for (int i = 0; i <= totalitems; ++i) {
	vet.push_back(i);
	matpar.push_back(vet);
	vet.erase(vet.begin());
	vet.push_back(-1);
	matpar.push_back(vet);
	mataux.push_back(matpar);
	vet.erase(vet.begin());
	matpar.erase(matpar.begin(), matpar.end());
  }
  for (int i = 0; i < matriz.size(); ++i) {
	for (int j = 0; j < matriz[i][1].size(); ++j) {
		item = matriz[i][1][j];
		if (mataux[item][1][0] != -1) {
			mataux[item][1].push_back(i);
		} else {
			mataux[item][1][0] = i;
		}
	}
  }
  for (int i = 0; i < mataux.size(); ++i) {
	if (mataux[i][1][0] !=-1) {
		mataux2.push_back(mataux[i]);
	}
  }
  return mataux2;
}

/* Organiza a matriz de forma decrescente de acordo com o numero
de transacoes em que o 'k-itemset' esta presente, e elimina os
'k-itemsets' que estao abaixo do suporte minimo utilizando o metodo
de ordenacao quicksort. */
mattri ordena(mattri matriz, int sup) {
  int tam = matriz.size();
  int tabaux[tam][2];
  mattri mataux;
  if (matriz.size() > 0) {
  	for (int i = 0; i < tam; ++i) {
		tabaux[i][0] = i;
		tabaux[i][1] = matriz[i][1].size();
  	}
	quicksort(tabaux, 0, (tam-1));
	for (int i = (tam-1); i >= 0; i--) {
		if (tabaux[i][1] >= sup) {
			mataux.push_back(matriz[tabaux[i][0]]);
		} else {
			i = -1;
		}
	}
  }
  return mataux;
}

/* Retorna o maior numero representativo de item. */
int maior(mattri matriz) {
  int limite, aux = 0;
  for (int i = 0; i < matriz.size(); ++i) {
	limite = matriz[i][1].size()-1;
	if (aux < matriz[i][1][limite]) {
		aux = matriz[i][1][limite];
	}
  }
  return aux;
}

/* Escreve os resultados no arquivo de saida. */
void escreve(mattri matriz) {
  ofstream saida("data/saida.txt", ios_base :: app);
  saida << endl << endl;
  saida << "Conjuntos de " << matriz[0][0].size() << "-itemsets frequentes: " << "  (" << matriz.size() << ")" << endl;
  for (int i = 0; i < matriz.size(); ++i) {
	for (int j = 0; j < 1; ++j) {
		saida << "[ ";
		for (int k = 0; k < matriz[i][j].size(); ++k) {
			saida << matriz[i][j][k] << " ";
		}
		saida << "]  ";
	}
	saida << "  (" << matriz[i][1].size() << ")" << endl;
	saida << "---------------------------------" << endl;
  }
  saida.close();
  //cout << "Gerado conjuntos de " << matriz[0][0].size() << "-itemsets frequentes..." << endl;
}

/* Verifica a quantidade de items em comum entre dois conjuntos. */
int iguais(vetint vet1, vetint vet2) {
  int aux = 0;
  int i = 0;
  while (vet1[i] == vet2[i]) {
	i++;
	aux++;
  }
  return aux;
}

/* Verifica a quantidade de items total da matriz. */
int itensvet(mattri matriz) {
  int aux = 0;
  int k = matriz[0][0].size();
  for (int i=0; i < matriz.size(); ++i) {
	aux = aux + matriz[i][1].size() + 1;
  }
  aux = aux + (k*matriz.size());
  return aux;
}

/* Realiza a intesercao de dois conjuntos de transacoes */
vetint intersecao(vetint vet2, vetint vet1) {
  vetint aux;
  int pvet1 = 0;
  int pvet2 = 0;
  while ((pvet1 < vet1.size()) && (pvet2 < vet2.size())) {
	if (vet2[pvet2] > vet1[pvet1])
		pvet1++;
	else if (vet2[pvet2] < vet1[pvet1])
		pvet2++; 
	else if (vet2[pvet2] == vet1[pvet1]) {
		aux.push_back(vet2[pvet2]);
		pvet2++;
		pvet1++;
	}
  }
  return aux;
}

/* Verifica se o item pertence ao conjunto. Nao aplica binaria, so para o conjunto de itens e nao transacoes */
bool pertence(int item, vetint vet) {
  for (int i = 0; i < vet.size(); ++i) {
	if (item == vet[i]) {
		return true;
	}
  }
  return false;
}

/* Gera os conjuntos de 2-itemsets frequentes. */
mattri gera2items(mattri matriz, int sup, int numthreads) {
  timeval tim;
  double t1,t2,t3,ti,tf,tt;
  mattri mataux;
  matint matpar;
  vetint vet;
  int prosize = MPI::COMM_WORLD.Get_size();
  int prorank = MPI::COMM_WORLD.Get_rank();
  int vezes = (matriz.size()/prosize) + 1;
  #pragma omp parallel for private(matpar,vet) shared(mataux) num_threads(numthreads) schedule(static,1)
  for (int x = 0; x < vezes; ++x) {
	int i = prorank + (x*prosize);
	if (i < matriz.size()) {
		for (int j = (i+1); j < matriz.size(); ++j) {
			if (matriz[i][0][0] < matriz[j][0][0]) {
				vet.push_back(matriz[i][0][0]);
				vet.push_back(matriz[j][0][0]);
			} else {
				vet.push_back(matriz[j][0][0]);
				vet.push_back(matriz[i][0][0]);
			}
			matpar.push_back(vet);
			vet.erase(vet.begin(), vet.end());
			vet = intersecao(matriz[j][1], matriz[i][1]);
			matpar.push_back(vet);
			#pragma omp critical
			mataux.push_back(matpar);
			vet.erase(vet.begin(), vet.end());
			matpar.erase(matpar.begin(), matpar.end());
		}
	}
  }
  mataux = ordena(mataux, sup);
  return mataux;
}

/* Gera os conjuntos de k-temsets frequentes, para todo iteracao > 2. */
mattri gerakitems(mattri matriz, int sup, int iteracao, int numthreads) {
  mattri mataux;
  matint matpar;
  vetint vet1;
  vetint vet2;
  int prorank = MPI::COMM_WORLD.Get_rank();
  int prosize = MPI::COMM_WORLD.Get_size();
  int vezes = (matriz.size()/prosize) + 1;
  #pragma omp parallel for private(matpar,vet1,vet2) shared(mataux) num_threads(numthreads) schedule(static,1)
  for (int x = 0; x < vezes; ++x) {
	int i = prorank + (x*prosize);
	if (i < matriz.size()) {
		for (int j = (i+1); j < matriz.size(); ++j) {
			if (iguais(matriz[i][0], matriz[j][0]) == (iteracao-1)) {
				for (int l = 0; l < iteracao-1; ++l) {
					vet1.push_back(matriz[i][0][l]);
				}
				if (matriz[i][0][iteracao-1] < matriz[j][0][iteracao-1]) {
					vet1.push_back(matriz[i][0][iteracao-1]);
					vet1.push_back(matriz[j][0][iteracao-1]);
				} else {
					vet1.push_back(matriz[j][0][iteracao-1]);
					vet1.push_back(matriz[i][0][iteracao-1]);
				}
				vet2 = intersecao(matriz[j][1], matriz[i][1]);
				matpar.push_back(vet1);
				matpar.push_back(vet2);
				#pragma omp critical
				mataux.push_back(matpar);
			}
			vet1.erase(vet1.begin(), vet1.end());
			vet2.erase(vet2.begin(), vet2.end());
			matpar.erase(matpar.begin(), matpar.end());
		}
	}
  }

  mataux = ordena(mataux, sup);

  return mataux;
}

/* Metodo Quick Sort para ordenacao da matriz de itemsets e transacoes */
void quicksort(int matriz[][2], int inicio, int fim) {
  int pivo, aux1, aux2, i, j, meio;
  i = inicio;
  j = fim;
  meio = (int) ((i + j) / 2);
  pivo = matriz[meio][1];
  do {
	while (matriz[i][1] < pivo) i = i + 1;
	while (matriz[j][1] > pivo) j = j - 1;
	if(i <= j) {
		aux1 = matriz[i][0];
		aux2 = matriz[i][1];
		matriz[i][0] = matriz[j][0];
		matriz[i][1] = matriz[j][1];
		matriz[j][0] = aux1;
		matriz[j][1] = aux2;
		i = i + 1;
		j = j - 1;
	}
  } while(j > i);
  if(inicio < j) quicksort(matriz, inicio, j);
  if(i < fim) quicksort(matriz, i, fim);
}


