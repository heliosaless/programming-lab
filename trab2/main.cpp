#include <iostream>
#include <string.h>
using namespace std;


/* vector */
template<class T>
void printVec(T* A, int len, const char* msg=""){
	cout << msg;
	for (int i = 0; i < len; i++)
		cout << *(A+i) << " ";
	cout << endl;
}

int sizeCharVec(const char* T){
	int i = 0;
	while(*(T+(i++)) != '\0'){}
	return i-1;
}


/* Auxiliar vector A */
int nextA(int i, const char* P, int* A){
	int j = *(A+i);//A[i];
	while(true){
		if(*(P+i+1) == *(P+j+1)) return j+1; //A[i+1] = j + 1
		else
			if(j==-1) return -1;
			j = *(A+j); 	
	}

	cout << "Error: Invalid next A! \n";
	return -2; //error flag
}


void setA(const char* P, int* A){
	*A = -1;
	for (int i = 1; *(P+i) != '\0'; i++){
		*(A+i) = nextA(i-1, P, A);
	}
}



void brute_force(const char *T, const char *P, int *O, int& size){
	bool t = true;
	for (int i = 0; T[i]!='\0'; i++){
		for (int j = 0; P[j]!='\0' && t; j++){
			if(T[i+j]!=P[j]) t=false;
		}

		if(t == true) O[size++] = i;
		else t = true;

	}
}

void knp(const char* T, const char* P, int* O, int &size){
	int n = sizeCharVec(T);
	int m = sizeCharVec(P);
	int i = 0, j = 0;
	
	int *A = new int[m];
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

}


int main()
{
	const char *T = "o sol nao e sol";
	const char *P = "sol";
	int len = sizeCharVec(P);
	int *O1 = new int[5];
	int *O2 = new int[5];
	
	int size1 = 0, size2=0; 
	brute_force(T,P,O1, size1);
	printVec(O1, size1, "vector O brute_force: ");

	knp(T,P,O2, size2);
	printVec(O2, size2, "vector O knp: ");


	return 0;
}

