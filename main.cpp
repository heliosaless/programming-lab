#include <iostream>
using namespace std;

int leftChild(int i){return 2*i + 1;}
int rightChild(int i){return 2*i + 2;}
int father(int i){return (i-1)/2;}

template <class T>
void realocVector(T *&vector, int &max){
    max = 2*max;
    
    T *temp = new int[max];
    for (int i = 0; i < max/2; ++i) temp[i] = vector[i];
    
    delete[] vector;
    vector = temp;
    //cout << "vector realocada! Novo tamanho: " << max << "\n";
}

template <class T>
void printVector(T *vector, const int len)
{
    for (int i = 0; i < len; ++i)
        cout << vector[i] << " ";
    cout << endl;
}

template <class T>
void changeVector(T *vector, const int pos1, const int pos2){
    if (pos1 == pos2) return;
    T temp = vector[pos1];
    vector[pos1] = vector[pos2];
    vector[pos2] = temp;
}


/* HEAPSORT */

template <class T>
void upHeap(T *heap, const int index, const int len){
    if(index == 0) return;

    int fat = father(index);
    int right = rightChild(fat);
    int left = leftChild(fat);


    int max;
    if(right < len && heap[left] < heap[right]) max = right;
    else max = left;

    if(heap[fat] < heap[max]){
        changeVector(heap, fat, max);
        upHeap(heap, fat, len);
    }
}

template <class T>
void downHeap(T *heap, const int index, const int len){
    int fat = index;
    int right = rightChild(index);
    int left = leftChild(index);

    int max;
    if(right < len && heap[left] < heap[right]) max = right;
    else {
        if(left < len) max = left;
        else return;
    }

    if(heap[fat] < heap[max]){
        changeVector(heap, fat, max);
        downHeap(heap, max, len);
    }
}

template <class T>
void insertHeap(T *&heap, int &max, int &len, const int value){
    if(len == max) realocVector(heap, max);
    
    heap[len] = value;
    if(len > 0) upHeap(heap, len, len);
    len = len + 1;
}

template <class T>
void removeHeap(T *&heap, int &len){
    if(len == 0){
        cout << "Não é possivel remover!" << endl;
        return;
    }

    changeVector(heap, 0, len-1);
    len = len - 1;
    downHeap(heap, 0, len);
}

template<class T>
void heapSort(T* heap, const int len){
    if(len == 1){
        return;
    }

    changeVector(heap, 0, len - 1);
    downHeap(heap, 0, len - 1);
    heapSort(heap, len - 1);    
}

/* QUICKSORT */
#include <stdlib.h>

int randomBetween(const int begin, const int end){
    return rand() % end + begin; 
}

int pivot(const int begin, const int end, bool randomized = false){
    if(randomized) return randomBetween(begin, end);
    return begin;
}

template<class T>
int partition(T* vector, const int begin, const int end, const int pivot){
    int i = begin+1;
    int p = pivot;
    int f = end;
    changeVector(vector, begin, p);

    while(i < f){

        while (i < end && vector[i] <= vector[p]) i++;

        while (f > 0 && vector[f] > vector[p]) f--;
        
        if(i!=f) changeVector(vector, i, f);
        
    }

    changeVector(vector, begin, f);

    return f;
}

template<class T>
void quicksort_(T* vector, const int begin, const int end, bool randomized){
    if(end <= begin) return;
    
    int pivot_ = pivot(begin, end, randomized);
    int pos = partition(vector, begin, end, pivot_);

    quicksort_(vector, begin, pos-1, randomized);
    quicksort_(vector, pos+1, end, randomized);
}

template<class T>
void quicksort(T* vector, const int len, bool randomized = false){
    return quicksort_(vector, 0, len-1, randomized);
}

/* INTROSORT */
template<class T>
void insertsort(T* vector, const int begin, const int end){

}

int lg(int x){
    int c = 0;
    while((x >> 1)) c++;
    return c;
}

template<class T>
void introsort_(T* vector, const int begin, const int end, const int rec_limit = -1, bool randomized = false){
    if (end <= begin) return;

    if(rec_limit == 0) return insertsort(vector, begin, end);
    else{
        int pivot_ = pivot(begin, end, randomized);
        int pos = partition(vector, begin, end, pivot_);

        introsort_(vector, begin, pos - 1, rec_limit - 1, randomized);
        introsort_(vector, pos + 1, end, rec_limit - 1, randomized);
    }
}

template <class T>
void introsort(T *vector, const int len, const int rec_limit=-1, bool randomized = false){
    if(rec_limit == -1) return introsort_(vector, 0, len-1, 0, randomized);
    else return introsort_(vector, 0, len-1, rec_limit, randomized);
}

/* INSTANCES */

int main()
{
    
    int max = 1, len = 0;
    int* heap = new int[max];

    insertHeap(heap, max, len, 10);
    insertHeap(heap, max, len, 5);
    insertHeap(heap, max, len, 3);
    insertHeap(heap, max, len, 2);
    insertHeap(heap, max, len, 4);

    printVector(heap, len);

    //heapSort(heap, len);
    //quicksort(heap, len);
    printVector(heap, len);

    cout << lg(4);

    delete[] heap;
    return 0;
}
