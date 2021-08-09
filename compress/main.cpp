#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>

using namespace std;

struct Noh
{
	unsigned char symbol;
	int value;
	int idx_esq;
	int idx_dir;
	int idx;
};

struct NohReduced
{
	unsigned char symbol;
	int idx_esq;
	int idx_dir;
};

/*VECTOR*/
template <class T>
void realocVector(T *&vector, int &max)
{
	max = 2 * max;

	T *temp = new T[max];
	if (temp == NULL)
		cout << "Fail Realoc" << endl;

	for (int i = 0; i < max / 2; ++i)
		temp[i] = vector[i];

	delete[] vector;
	vector = temp;
	//cout << "vector realocada! Novo tamanho: " << max << "\n";
}

template <class T>
void insertVector(T *&vector, int &max, int &len, T value)
{
	if (len == max)
		realocVector(vector, max);

	vector[len] = value;
	len = len + 1;
}

template <class T>
void changeVector(T *vector, const int pos1, const int pos2)
{
	if (pos1 == pos2)
		return;
	T temp = vector[pos1];
	vector[pos1] = vector[pos2];
	vector[pos2] = temp;
}

template <class T>
void printVector(T *vector, const int len){
	for (int i = 0; i < len; ++i){
		cout << vector[i];
	}
	cout << endl;
}


/*HEAP*/
int leftChild(int i) { return 2 * i + 1; }
int rightChild(int i) { return 2 * i + 2; }
int father(int i) { return (i - 1) / 2; }

template <class T>
void downHeap(T *heap, const int len, const int index)
{
	int fat = index;
	int right = rightChild(fat);
	int left = leftChild(fat);
	int min;
	if (right < len && heap[right].value < heap[left].value)
		min = right;
	else
	{
		if (left < len)
			min = left;
		else
			return;
	}

	if (heap[fat].value > heap[min].value)
	{
		changeVector(heap, fat, min);
		downHeap(heap, len, min);
	}
}

template <class T>
void upHeap(T *heap, const int index)
{
	if (index == 0)
		return;
	int fat = father(index);
	if (fat >= 0 and heap[fat].value > heap[index].value)
	{
		changeVector(heap, fat, index);
		upHeap(heap, fat);
	}
}

template <class T>
void addHeap(T *&heap, int &max, int &len, T value)
{
	insertVector(heap, max, len, value);
	upHeap(heap, len - 1);
}

template <class T>
T *removeHeap(T *heap, int &len)
{
	T *root = new T;
	*root = heap[0];
	changeVector(heap, 0, len - 1);
	len = len - 1;
	downHeap(heap, len, 0);
	return root;
}

/*HUFFTREE*/
void constructTree(Noh *&heap, Noh *&writeVec, int &maxHeap, int &lenHeap, int &maxVec, int &lenVec, const long int *v)
{
	int index = 0;
	for (unsigned int i = 0; i < 256; i++)
		if (v[i] > 0)
		{
			Noh n;
			n.symbol = (unsigned char)i;
			n.value = v[i];
			n.idx_esq = -1;
			n.idx_dir = -1;
			n.idx = index;
			index++;
			addHeap(heap, maxHeap, lenHeap, n);
			insertVector(writeVec, maxVec, lenVec, n);
		}

	while (lenHeap > 1)
	{
		Noh *x = removeHeap(heap, lenHeap);
		Noh *y = removeHeap(heap, lenHeap);

		Noh z;
		z.symbol = '?';
		
		z.value = x->value + y->value;



		if (x->value == y->value && x->idx_esq == -1 && x->idx_dir == -1 && (y->idx_esq != -1 || y->idx_dir != -1))
		{
			z.idx_esq = x->idx;
			z.idx_dir = y->idx;
		}
		else
		{
			z.idx_esq = y->idx;
			z.idx_dir = x->idx;
		}


		z.idx = index;
		index++;
		addHeap(heap, maxHeap, lenHeap, z);
		insertVector(writeVec, maxVec, lenVec, z);
	}
}


void constructCodVec(Noh *treeVec, char **codVec, int idx, char *acc)
{
	Noh t = treeVec[idx];

	if (t.idx_esq == -1 && t.idx_dir == -1)
		strcpy(codVec[(int)t.symbol], acc);

	if (t.idx_esq != -1)
	{
		constructCodVec(treeVec, codVec, t.idx_esq, strcat(acc, "1"));
		acc[strlen(acc) - 1] = '\0';
	}

	if (t.idx_dir != -1)
	{
		constructCodVec(treeVec, codVec, t.idx_dir, strcat(acc, "0"));
		acc[strlen(acc) - 1] = '\0';
	}
}

/*FILES*/

bool readTxtV(const char *file, long int *v)
{
	ifstream arq(file);
	if (!arq.is_open())
		return false;
	else
	{
		for (int i = 0; i < 256; i++)
			v[i] = 0;
		for (int i = 0;; i++)
		{
			unsigned int c = arq.get();
			if (arq.eof())
				break;
			v[c]++;
		}
	}

	arq.close();
	return true;
}

bool readTxtT(const char *file, char *text)
{
	ifstream arq(file);
	if (!arq.is_open())
		return false;
	else
	{
		for (int i = 0;; i++)
		{
			unsigned int c = arq.get();
			if (arq.eof())
				break;
			text[i] = c;
		}
	}

	arq.close();
	return true;
}

bool writeCompress(const char *file, NohReduced *writeVec, int len, int symbols, char *text, char **codVec)
{
	ofstream arq(file);
	if (!arq.is_open())
		return false;
	else
	{
		if(symbols == 0){return true;}
		char *v = (char *)&len;
		for (unsigned int i = 0; i < sizeof(int); ++i)
			arq.put(v[i]);


		char *fim = (char *)(writeVec + len);
		for ( char *p = ( char *)writeVec; p < fim; ++p)
		{
			arq.put(*p);
		}


		v = (char *)&symbols;
		for (unsigned int i = 0; i < sizeof(int); ++i)
			arq.put(v[i]);


		int writtenBits = 0;
		unsigned char byte = 0;
		for (int i = 0; i < symbols; i++)
		{
			unsigned char toWrite = text[i];
			char *bitsToWrite = codVec[(int)toWrite];
			
			for (int i = 0; bitsToWrite[i] != '\0'; i++)
			{
				if (writtenBits < 8)
				{
					char byte_aux = bitsToWrite[i] - 48;
					byte <<= 1;
					byte = byte_aux | byte;
					writtenBits++;
					if (writtenBits == 8)
					{
						arq.put(byte);
						byte = 0;
						writtenBits = 0;
					}
				}
			}
		}

		if (writtenBits != 8)
		{
			while (writtenBits++ != 8)
				byte <<= 1;
			arq.put(byte);
			byte = 0;
			writtenBits = 0;
		}
		
		
	}
	return true;
}


bool readCompress(const char *file, NohReduced *&readVec, int &len, int &symbols, char *&decompress)
{
	ifstream arq(file);
	if (!arq.is_open())
		return false;
	else
	{

		char *v = (char *)&len;
		for (unsigned int i = 0; i < sizeof(int); ++i){
			v[i] = arq.get();
			if(arq.eof()) return true; // empty file
		}

		readVec = new NohReduced[len];

		char *fim = (char *)(readVec + len);
		for (char *p = (char *)readVec; p < fim; ++p)
		{
			*p = arq.get();
			if (arq.eof())	break;
			
		}

		v = (char *)&symbols;
		for (unsigned int i = 0; i < sizeof(int); ++i)
			v[i] = arq.get();
	}

	decompress = new char[symbols];
	int readBits = 0;
	int actual_idx = len - 1;
	int i = 0;
	while (true)
	{
		unsigned char b = arq.get();
		if (arq.eof())
			break;
		while (readBits < 8)
		{
			if (b >= 128)
			{
				actual_idx = readVec[actual_idx].idx_esq;
			}
			else
			{
				actual_idx = readVec[actual_idx].idx_dir;
			}
			if (readVec[actual_idx].idx_esq == -1 && readVec[actual_idx].idx_dir == -1)
			{
				decompress[i] = readVec[actual_idx].symbol;
				i++;
				actual_idx = len - 1;
				if (i == symbols)
					return true;
			}
			b <<= 1;
			readBits += 1;
		}
		readBits = 0;
	}
	return true;
}

bool writeDecompress(const char *file, char *text, const int symbols)
{
	ofstream arq(file);
	if (!arq.is_open())
		return false;
	else
	{
		for (int i = 0; i < symbols; ++i)
		{
			arq.put(text[i]);
		}
	}
	return true;
}

int main(int argc, char const *argv[])
{
	if (argc < 4) return 0;
	clock_t time;
	if (strcmp(argv[1], "-c") == 0)
	{
		time = clock();
		long int *v = new long int[256];
		char **codVec = new char *[256];
		readTxtV(argv[2], v); 

		int maxHeap = 1;
		int lenHeap = 0;
		struct Noh *heap = new Noh[maxHeap];

		int maxVector = 0;
		int lenVector = 0;
		int symbols = 0;

		int rem; 
		for (int i = 0; i < 256; ++i)
			if (v[i] > 0){
				rem = i;  
				maxVector++;
				symbols += v[i];
			}

		if(maxVector == 1){
			if(rem < 255) v[rem+1] = 1;
			else v[rem-1] = 1;
			maxVector ++;
		}

		for (int i = 0; i < 256; i++)
		{
			codVec[i] = NULL;
			if (v[i] > 0){
				codVec[i] = new char[symbols];
				strcpy(codVec[i], "");
			}
		}

		char *text = new char[symbols];
		readTxtT(argv[2], text);


		struct Noh *treeVec;

		if (maxVector != 0){
			
			maxVector = maxVector * 2 - 1;  // 2*n - 1
			treeVec = new Noh[maxVector];
		
			constructTree(heap, treeVec, maxHeap, lenHeap, maxVector, lenVector, v);

			
			char *aux = new char[symbols];
			strcpy(aux, "");
		 	constructCodVec(treeVec, codVec, maxVector - 1, aux);
		
		}

		struct NohReduced *treeVecR = new NohReduced[maxVector];

		for (int i = 0; i < maxVector; ++i)
		{
			treeVecR[i].symbol = treeVec[i].symbol;
			treeVecR[i].idx_esq = treeVec[i].idx_esq;
			treeVecR[i].idx_dir = treeVec[i].idx_dir;
		}


		writeCompress(argv[3], treeVecR, lenVector, symbols, text, codVec);
		time = clock() - time;
		cout << "compress time: " << ((double)time) / ((CLOCKS_PER_SEC)) << endl;
	
	}
	else if (strcmp(argv[1], "-d") == 0)
	{
		time = clock();
		
		int len = 0;
		int symbols = 0;
		char *decompress;
		struct NohReduced *readVec;
		
		readCompress(argv[2], readVec, len, symbols, decompress);


		writeDecompress(argv[3], decompress, symbols);

		time = clock() - time;
		cout << "decompress time: " << ((double)time) / ((CLOCKS_PER_SEC)) << endl;
	}
	else{cout << "expected -c for compress or -d for decompress" << endl;}

	return 0;
}