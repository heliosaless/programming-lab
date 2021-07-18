#include <iostream>
#include <string.h>
using namespace std;


/* vector */
template<class T>
void printVec(T* A, int len, const char* msg=""){
	cout << msg;
	for (int i = 0; i < len; i++)
		cout << A[i] << " ";
	cout << endl;
}

int sizeCharVec(const char* T){
	int i = 0;
	while(T[i++] != '\0'){}
	return i-1;
}


/* Auxiliar vector A */
int nextA(int i, const char* P, int* A){
	int j = A[i];
	while(true){
		if(P[i+1] == P[j+1]) return j+1; 
		else{
			if(j==-1) return -1;
			j = A[j];
		} 	
	}

	cout << "Error: Invalid next A! \n";
	return -2; //error flag
}


void setA(const char* P, int* A){
	*A = -1;
	for (int i = 1; P[i] != '\0'; i++){
		A[i] = nextA(i-1, P, A);
	}
}



bool brute_force(const char *T, const char *P, int *O){
	bool t = true;
	int size = 0;
	for (int i = 0; T[i]!='\0'; i++){
		for (int j = 0; P[j]!='\0' && t; j++){
			if(T[i+j]!=P[j]) t=false;
		}

		if(t == true) O[size++] = i;
		else t = true;

	}
	O[size++] = -1;
	return true;
}

bool kmp(const char* T, const char* P, int* O){
	int n = sizeCharVec(T);
	int m = sizeCharVec(P);
	int i = 0, j = 0, size = 0;
	
	int *A = new int[m];
	if(A==NULL) return false;

	setA(P,A);

	while(i < n){

		if(T[i] == P[j]){ 
			if(j == m - 1){  		// Padrão inteiro casou com T
				O[size++] = i - j; 	// Casamento de T e P comecando em i-j;
				j = A[j];			
				// A[j] é o tamanho do maior prefixo que tambem e sufixo de 0 à j.
				// Logo fazer j = A[j] é o salto inteligente pois essa é a prox
				// região que pode haver casamento.	
			}
			++i; ++j; // Se j < m - 1, verificar prox letra. Caso j = m-1, atualizamos o j anterior
			// e devemos incrementa-los para continuar os casamentos.
		}
		else{
			if (j==0) ++i; // nenhuma substring anterior casou e nem T[i]=P[j], testamos prox i;
			else j = A[j-1] + 1; 
		}
	}
	O[size++] = -1;
	return true;
}

bool equals(int *a, int *b){
	for (int i = 0; a[i]!=-1; ++i)
		if(a[i] != b[i]) return false;

	return true;
}

#include <cstdlib>
#include <ctime>
#include "instancias_Reais_Trabalho_2.hpp"	

int main(int argc, char* argv[])
{
	if(argc < 2){ cout << "Entrada invalida\n"; return 1;}

	if(argv[1][0] == 'A'){
		if(argc != 6){ cout << "Entrada invalida\n"; return 1;}
			int l = argv[2][0];
			int m = atoi(argv[3]), n= atoi(argv[4]), inst = atoi(argv[5]);

			char *P = new char[m+1];
			char *T = new char[n+1];
			int  *Obrute = new int[n+1];
			int  *Okmp = new int[n+1];
			
			clock_t time;
			double time1 = 0;
			double time2 = 0;

			while(inst-- > 0){
				for (int i = 0; i < m; ++i)
					P[i] = (char)(97 + rand()%(l-97+1)); 
				P[m] = '\0';

				for (int i = 0; i < n; ++i)
					T[i] = (char)(97 + rand()%(l-97+1)); 
				T[n] = '\0';

				time = clock();
				if(!kmp(T,P,Okmp)){cout << "Error - memory alloc\n"; return 1;}
				time = clock() - time;
				time1 += ((double)time) / ((CLOCKS_PER_SEC)); 

				time = clock();
				if(!brute_force(T, P, Obrute)){cout << "Error - memory alloc\n"; return 1;}
				time = clock() - time;
				time2 += ((double)time) / ((CLOCKS_PER_SEC)); 

				if(!equals(Okmp, Obrute)){cout << "Error - Outputs dont match\n"; return 1;}
			}

			cout << "Kmp: " << time1 << endl; 
			cout << "BruteForce: " << time2 << endl; 

	}else if(argv[1][0] == 'R'){
		if(argc != 4){ cout << "Entrada invalida\n"; return 1;}
		int x = atoi(argv[2]), y = atoi(argv[3]);	if(x < 0 || y < 0 || x > 35129 || y > 35129) { cout << "Entrada invalida\n"; return 1;}

		int inst = y-x+1;

		clock_t time;
		double time1 = 0;
		double time2 = 0;

		const char *T = Texto_Livros;
		const char *P;
	
		int n = sizeCharVec(T);
		int  *Obrute = new int[n+1];
		int  *Okmp = new int[n+1];

		while(inst-- > 0){
			P = Padroes_Palavras[x+inst];

			time = clock();
			if(!kmp(T,P,Okmp)){cout << "Error - memory alloc\n"; return 1;}
			time = clock() - time;
			time1 += ((double)time) / ((CLOCKS_PER_SEC)); 
			
			time = clock();
			if(!brute_force(T, P, Obrute)){cout << "Error - memory alloc\n"; return 1;}
			time = clock() - time;
			time2 += ((double)time) / ((CLOCKS_PER_SEC));

			if(!equals(Okmp, Obrute)){cout << "Outputs dont match\n"; return 1;}
		}

		cout << "Kmp: " << time1 << endl; 
		cout << "BruteForce: " << time2 << endl; 
	}

	return 0;
}

