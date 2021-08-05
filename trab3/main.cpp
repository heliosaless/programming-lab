#include <iostream>
#include <fstream>

using namespace std;

struct Noh
{
	char symbol;
	int value;
	int idx_esq;
	int idx_dir;
	int idx;
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
void printVector(T *vector, const int len)
{
	for (int i = 0; i < len; ++i)
		cout << vector[i].symbol << "\t";
	cout << endl;
	for (int i = 0; i < len; ++i)
		cout << vector[i].idx << "\t";
	cout << endl;
	for (int i = 0; i < len; ++i)
		cout << vector[i].value << "\t";
	cout << endl;
	for (int i = 0; i < len; ++i)
		cout << vector[i].idx_esq << "\t";
	cout << endl;
	for (int i = 0; i < len; ++i)
		cout << vector[i].idx_dir << "\t";
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
template <class T>
void constructTree(T *&heap, T *&writeVec, int &maxHeap, int &lenHeap, int &maxVec, int &lenVec, const long int *v)
{

	int index = 0;
	for (int i = 0; i < 256; i++)
		if (v[i] > 0)
		{
			T n;
			n.symbol = (char)i;
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
		T *x = removeHeap(heap, lenHeap);
		T *y = removeHeap(heap, lenHeap);

		T z;
		z.symbol = '?';
		z.value = x->value + y->value;

		if (x->idx_esq == -1 && x->idx_dir == -1 && (y->idx_esq != -1 || y->idx_esq != -1))
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

#include <string.h>
template <class T>
void constructCodVec(T *treeVec, char **codVec, int idx, char *acc)
{
	T t = treeVec[idx];
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

template <class T>
bool writePreamble(const char *file, T *writeVec, int len, int symbols, char *text, char **codVec)
{
	ofstream arq(file);
	if (!arq.is_open())
		return false;
	else
	{
		char *v = (char *)&len;
		for (int i = 0; i < sizeof(int); ++i)
			arq.put(v[i]);

		 char *fim = ( char *)(writeVec + len);
		for ( char *p = ( char *)writeVec; p < fim; ++p)
		{
			arq.put(*p);
		}


		v = (char *)&symbols;
		for (int i = 0; i < sizeof(int); ++i)
			arq.put(v[i]);

		int writtenBits = 0;
		unsigned char byte = 0;
		for (int i = 0; i < symbols; i++)
		{
			char toWrite = text[i];
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
						cout << "writen: "<< (int)byte << endl;
						arq.put(byte);
						byte = 0;
						writtenBits = 0;
					}
				}
			}
		}

		if (writtenBits != 0)
		{
			while (writtenBits++ != 8)
				byte <<= 1;
			cout << "writen: " << (int)byte << endl;
			arq.put(byte);
			byte = 0;
			writtenBits = 0;
		}
	}
	return true;
}

bool writeCompressedFile(const char *file, char *text, char **codVec, int symbols)
{
	fstream arq(file);
	if (!arq.is_open())
		return false;
	else
	{
		arq.seekg(0, ios::end);
		int writtenBits = 0;
		unsigned char byte = 0;
		for (int i = 0; i < symbols; i++)
		{
			char toWrite = text[i];
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
						//cout << "writen: "<< (int)byte << endl;
						arq.put(byte);
						byte = 0;
						writtenBits = 0;
					}
				}
			}
		}

		if (writtenBits != 0)
		{
			while (writtenBits++ != 8)
				byte <<= 1;
			//cout << "writen: " << (int)byte << endl;
			arq.put(byte);
			byte = 0;
			writtenBits = 0;
		}
	}
	return true;
}

template <class T>
bool readPreamble(const char *file, T *&readVec, int &len, int &symbols, char *&decompress)
{
	ifstream arq(file);
	if (!arq.is_open())
		return false;
	else
	{

		char *v = (char *)&len;
		for (unsigned int i = 0; i < sizeof(int); ++i)
			v[i] = arq.get();

		readVec = new Noh[len];

		char *fim = (char *)(readVec + len);
		for (char *p = (char *)readVec; p < fim; ++p)
		{
			*p = arq.get();
			if (arq.eof())
			{
				cout << "eof read" << endl;
				break;
			}
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

/*
bool readTextFile(const char *file, Noh* vectorRead, const int len, const int symbols){
  ifstream arq(file);
	if (!arq.is_open())
		return false;
	else
	{
    arq.seekg()
    int readBits = 0;
    int numBytes = symbols;
    int actual_idx = len-1;
    while(numBytes-- > 0){
    unsigned char b = arq.get();
    if(arq.eof()) break;
    while(readBits < 8){
      if(b>128){actual_idx = vectorRead[actual_idx].idx_esq;}
      else{actual_idx = vectorRead[actual_idx].idx_dir;}
      if(vectorRead[actual_idx].idx_esq == -1 && vectorRead[actual_idx].idx_dir == -1 )
        {cout << vectorRead[actual_idx].symbol; actual_idx = len-1;}
      b <<= 1;
      readBits += 1;
    }
    readBits = 0;
	}
}
	return true;

}*/

#include <cstdlib>
#include <ctime>

int main(int argc, char const *argv[])
{
	if (argc < 4)
		return 0;
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

		for (int i = 0; i < 256; ++i)
			if (v[i] > 0)
			{
				maxVector++;
				symbols += v[i];
			}

		for (int i = 0; i < 256; i++)
		{
			codVec[i] = NULL;
			if (v[i] > 0)
			{
				codVec[i] = new char[symbols];
				strcpy(codVec[i], "");
			}
		}

		char *text = new char[symbols];
		readTxtT(argv[2], text);

		maxVector = maxVector * 2 - 1;
		struct Noh *treeVec = new Noh[maxVector];
		constructTree(heap, treeVec, maxHeap, lenHeap, maxVector, lenVector, v);

		char *aux = new char[symbols];
		strcpy(aux, "");

		constructCodVec(treeVec, codVec, maxVector - 1, aux);

		for (int i = 0; i < 256; i++)
			if (codVec[i] != NULL && strcmp(codVec[i], "") != 0)
				cout << i << " " << codVec[i] << endl;

		cout << lenVector << " " << symbols << endl;
		printVector(treeVec, lenVector);
		writePreamble(argv[3], treeVec, lenVector, symbols, text, codVec);
		time = clock() - time;
		cout << "compress time: " << ((double)time) / ((CLOCKS_PER_SEC)) << endl;
	}
	else if (strcmp(argv[1], "-d") == 0)
	{
		time = clock();
		int len = 0;
		int symbols = 0;
		struct Noh *readVec;
		char *decompress;
		readPreamble(argv[2], readVec, len, symbols, decompress);
		//cout << lenRead << " " << symbolsRead <<  endl;
		//printVector(readVec, lenRead);

		writeDecompress(argv[3], decompress, symbols);

		time = clock() - time;
		cout << "decompress time: " << ((double)time) / ((CLOCKS_PER_SEC)) << endl;
	}
	else
	{
	}

	return 0;
}