#include <iostream>
#include <fstream>

using namespace std;


struct Noh{
	int value;
	//struct Noh* esq;
	//struct Noh* dir;
	int idx_esq;
	int idx_dir;
	int idx;
};


/*VECTOR*/
template <class T>
void realocVector(T *&vector, int &max){
    max = 2*max;
    
    T *temp = new T[max];
    if(temp == NULL) cout << "Fail Realoc" << endl;

    for (int i = 0; i < max/2; ++i) temp[i] = vector[i];
    
    delete[] vector;
    vector = temp;
    //cout << "vector realocada! Novo tamanho: " << max << "\n";
}

template <class T>
void insertVector(T *&vector, int &max, int &len, T value){
    if (len == max)
        realocVector(vector, max);

    vector[len] = value;
    len = len + 1;
}

template <class T>
void changeVector(T *vector, const int pos1, const int pos2){
    if (pos1 == pos2) return;
    T temp = vector[pos1];
    vector[pos1] = vector[pos2];
    vector[pos2] = temp;
}

template <class T>
void printVector(T *vector, const int len)
{
    for (int i = 0; i < len; ++i)
    	cout << vector[i].value << " ";
    cout << endl;
}

/*HEAP*/
int leftChild(int i) { return 2 * i + 1; }
int rightChild(int i) { return 2 * i + 2; }
int father(int i) { return (i - 1) / 2; }

template <class T>
void downHeap(T *heap, const int len, const int index){
    int fat = index;
    int right = rightChild(fat);
    int left = leftChild(fat);
    int min;
    if(right < len && heap[right].value < heap[left].value) min = right;
    else {
        if(left < len) min = left;
        else return;
    }

    if(heap[fat].value > heap[min].value){
        changeVector(heap, fat, min);
        downHeap(heap, len, min);
    }
}

template <class T>
void upHeap(T *heap, const int index){
	if(index == 0) return;
	int fat = father(index);
	if(fat >= 0 and heap[fat].value > heap[index].value){
		changeVector(heap, fat, index);
		upHeap(heap, fat);
	}
}

template <class T>
void addHeap(T* &heap, int &max, int &len, T value){
	insertVector(heap, max, len, value);
	upHeap(heap, len-1);
}

template <class T>
T* removeHeap(T* heap, int &len){
	T* root = new T;
	*root = heap[0];
	changeVector(heap, 0, len-1);
	len = len - 1;
	downHeap(heap, len, 0);
	return root;
}

/*HUFFTREE*/
template <class T>
void constructTree(T* &heap, T* &writeVec, int& max, int& len, int& max2, int& len2, const long int* v){

	int index = 0;
	for (int i = 0; i < 256; i++)
		if(v[i] > 0){ 
			T n;
			n.value = v[i];
			//n.esq = NULL;
			//n.dir = NULL;
			n.idx_esq = -2;
			n.idx_dir = -2;
			n.idx = index;
			index++;
			addHeap(heap, max, len, n);
			insertVector(writeVec, max2, len2, n);
		}
	
	while(len > 1){
		T* x = removeHeap(heap, len);
		T* y = removeHeap(heap, len);

		T z;
		//z.dir = x;
		//z.esq = y;
		z.value = x->value + y->value;
		z.idx_esq = x->idx;
		z.idx_dir = y->idx;
		z.idx = index;
		index++;
		addHeap(heap, max, len, z);
		insertVector(writeVec, max2, len2, z);

	}

}

template<class T>
void printTree(T* root){
	if(root[0].esq != NULL) printTree(root[0].esq);
	if(root[0].dir != NULL) printTree(root[0].dir);
	cout << root[0].value << endl;
}

/*FILES*/

bool readTxt(const char* file, long int* v){
	ifstream arq(file);
	if(!arq.is_open()) return false;
	else{
		for (int i = 0; i < 255; i++)	v[i] = 0;
		for(int i = 0;;i++){
			if(arq.eof()) break;
			unsigned int c = arq.get(); 
			v[c]++;
		}
	}

	arq.close();
	return true;
}

template <class T>
bool writeVector(const char* file, T* writeVec, int len2){
	ofstream arq(file);
	if(!arq.is_open()) return false;
	else{
		arq.put(len2);
		char *fim = (char*)(writeVec+len2);
		for (char* p = (char*)writeVec; p < fim; ++p)
			arq.put(*p);
	}
	return true;
}

template <class T>
bool readVector(const char* file, T* readVec, int &len){
	ifstream arq(file);
	if(!arq.is_open()) return false;
	else{
		len = arq.get();
		char *fim = (char*)(readVec+len);
		for (char* p = (char*)readVec; p < fim; ++p){
			*p = arq.get();
			//if(arq.eof()) return true;
		}
	}
	return true;
}

int main(int argc, char const *argv[])
{
	
	if(argc < 3) return 0;
	long int* v = new long int[256];
	readTxt(argv[1], v);

	int max = 1;
	int len = 0;
	struct Noh* heap = new Noh[max];	

	int max2 = 0;
	int len2 = 0;
	for (int i = 0; i < 256; ++i)
		if(v[i] > 0)	max2++;
	
	cout << max2 << endl << endl;
	max2 = max2 *2 - 1;
	struct Noh* writeVec = new Noh[max2];
	constructTree(heap, writeVec, max, len, max2, len2, v);

	cout << len2 << endl;
	writeVector(argv[2], writeVec, len2);
	printVector(writeVec, len2);
	
	int len3 = 0;
	struct Noh* readVec = new Noh[max2];
	readVector(argv[2], readVec, len3);
	cout << len3 << endl;
	printVector(readVec, len3);

	return 0;
}