#include <iostream>
#include <string.h>
using namespace std;


void brute_force(const char *T, const char *P, int *O, int& sizeO){
	bool t = true;
	for (int i = 0; T[i]!='\0'; i++){
		for (int j = 0; P[j]!='\0' && t; j++){
			if(T[i+j]!=P[j]) t=false;
		}

		if(t == true) O[sizeO++] = i;
		else t = true;

	}
}

int nextA(int i, const char* P, int* A){
	int j;
	while(true){
		j = *(A+i);//A[i];
		if(j == -1){
			if(*(P+i+1) == *(P)) return 0;//A[i+1] = 0
				else return -1; //A[i-1]=-1
		}else{
			if(*(P+i+1) == *(P+j+1)) return j+1; //A[i+1] = j + 1;
				else j = *(A+j);
		}		
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

int main()
{
	const char *T = "o sol nao e sol";
	const char *P = "sol";
	int len = sizeCharVec(P);
	int *A = new int[len];
	int *O = new int[5];
	
	setA(P, A);
	printVec(A, len, "vector A: ");

	int sizeO = 0; 
	brute_force(T,P,O, sizeO);
	printVec(O, sizeO, "vector O: ");


	return 0;
}