#include <iostream>
using namespace std;

/* VECTOR*/

template <class T>
void realocVector(T *&vector, int &max){
    max = 2*max;
    
    T *temp = new int[max];
    if(temp == NULL) cout << "Fail Realoc" << endl;

    for (int i = 0; i < max/2; ++i) temp[i] = vector[i];
    
    delete[] vector;
    vector = temp;
    //cout << "vector realocada! Novo tamanho: " << max << "\n";
}


template<class T>
void deleteVecVec(T** vec, int n_instances){
    for (int i = 0; i < n_instances; i++){
        delete[] vec[i];
    }
    delete[] vec;
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

template <class T>
void insertVector(T *&vector, int &max, int &len, const int value){
    if (len == max)
        realocVector(vector, max);

    vector[len] = value;
    len = len + 1;
}

template <class T>
void copyVector(T* vector1, T* vector2, int len){
    for (int i = 0; i < len; i++) vector2[i] = vector1[i];
}

/* HEAPSORT */

int leftChild(int i) { return 2 * i + 1; }
int rightChild(int i) { return 2 * i + 2; }
int father(int i) { return (i - 1) / 2; }

template <class T>
void downHeap(T *heap, const int index, const int begin, const int end){
    int fat = index-begin;
    int right = rightChild(fat);
    int left = leftChild(fat);

    int max;
    if(right <= end-begin && heap[left+begin] < heap[right+begin]) max = right;
    else {
        if(left <= end-begin) max = left;
        else return;
    }

    if(heap[fat+begin] < heap[max+begin]){
        changeVector(heap, fat+begin, max+begin);
        downHeap(heap, max+begin, begin, end);
    }
}

template<class T>
void doHeap(T* vector, const int begin, const int end){ // Comeca do pai do ultimo elemento. Não é folha.
    for(int i = father(end-begin)+begin; i >= begin; i--) downHeap(vector, i, begin, end);
}


template<class T>
void heapSort_(T* heap, const int begin, const int end){
/*
    if(len == 1){
        return;
    }

    changeVector(heap, 0, len - 1);
    downHeap(heap, 0, len - 1);
    heapSort(heap, len - 1);
*/
    for (int i = end; i > begin; i--){
        changeVector(heap, begin, i);
        downHeap(heap, begin, begin, i-1);

    }

}

template<class T>
void heapSort(T* heap, const int len){
    heapSort_(heap, 0, len-1);
}

/* QUICKSORT */
#include <cstdlib>
#include <ctime>

int randomBetween(const int begin, const int end){
    return begin + rand() % (end-begin+1); 
}

int pivot(const int begin, const int end, bool randomized = false){
    if(randomized) return randomBetween(begin, end);
    return (begin+end)/2;
    //return begin;
}

template<class T>
int partition(T* vector, const int begin, const int end, const int pivot){
    //cout << "pivot: " << vector[pivot] << endl;
    int i = begin+1;
    int f = end;
    changeVector(vector, begin, pivot);
    //printVector(vector, 10);

    while(i <= f){

        while (i <= end && vector[i] <= vector[begin]) i++;

        while (f > begin && vector[f] > vector[begin]) f--;

        if(i < f){
            changeVector(vector, i, f);
            //printVector(vector, 10);
        }
        
    }
    changeVector(vector, begin, f);
    //printVector(vector, 10);
    //cout << "-----------------" << endl;
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
void quickSort(T* vector, const int len, bool randomized = false){
    quicksort_(vector, 0, len-1, randomized);
}

/* INTROSORT */
template<class T>
void insertsort(T* vector, const int begin, const int end){
    int i,j;
    for(i = begin+1; i <= end; i++){
        j = i - 1;
        while(j >= begin && vector[j+1] < vector[j]){
            changeVector(vector, j, j+1);
            j = j - 1;
        }
        //printVector(vector, end - begin + 1);
    }
}

int lg(int x){
    int c = 0;
    while((x = x >> 1) >= 1) c++;
    return c;
}

template<class T>
void introsort_(T* vector, const int begin, const int end, const int rec_limit, bool randomized){
    if (end <= begin) return;

    if(rec_limit == 0){
        doHeap(vector,begin,end);
        return heapSort_(vector, begin, end);
    }else{

        int pivot_ = pivot(begin, end, randomized);
        int pos = partition(vector, begin, end, pivot_);

        introsort_(vector, begin, pos - 1, rec_limit - 1, randomized);
        introsort_(vector, pos + 1, end, rec_limit - 1, randomized);
    }
}

template <class T>
void introSort(T *vector, const int len, const int rec_limit=-1, bool randomized = false, bool insert=false){
    if(insert && len <= 32) insertsort(vector, 0, len-1);
    else{ 
        if(rec_limit == -1) introsort_(vector, 0, len - 1, 2 * lg(len), randomized);
        else introsort_(vector, 0, len-1, rec_limit, randomized);
    }
}

/* INSTANCES */

template<class T>
void worstcase_(T* vector, const int begin, const int end, const int i){
    if(end < begin) return;

    int pivot_ = pivot(begin, end);
    vector[pivot_] = i;
    changeVector(vector, begin, pivot_);
    worstcase_(vector, begin+1, end, i+1);
    changeVector(vector, begin, pivot_);
}

template <class T>
void worst(T* &vector, int &max, int &len, const int n){
    for (int i = 0; i < n; i++) insertVector(vector, max, len, 0);
    worstcase_(vector, 0, len - 1, 1);
}

template <class T>
void crescent(T* &vector, int &max, int &len, const int n){
    for (int i = 1; i <= n; i++) insertVector(vector, max, len, i);
}

template <class T>
void decrescent(T* &vector, int &max, int &len, const int n){
    for (int i = 0; i < n; i++) insertVector(vector, max, len, n-i);
}

template <class T>
void random(T *&vector, int &max, int &len, const int n){
    for (int i = 0; i < n; i++) insertVector(vector, max, len, randomBetween(1, n));
}

template<class T>
bool checkOrder(T* vector, T* orderVec, int n){
    for(int i = 0; i < n; i++) if(orderVec[i]!=vector[i]) return false;

    return true;
}


int main(int argc, char *argv[])
{
    if(argc != 4){ cout << "Entrada invalida\n"; return 1;}
    srand(time(NULL));

    int n = atoi(argv[2]), n_instances = atoi(argv[3]);
    int **vector = new int*[n_instances];

    for (int i = 0; i < n_instances; i++){
        int max = 1, len = 0;
        int *vector_aux = new int[max];


        switch((argv[1])[0]){
            case 'A':
                random(vector_aux, max, len, n);
                break;
            case 'C':
                crescent(vector_aux, max, len, n);
                break;
            case 'D':
                decrescent(vector_aux, max, len, n);
                break;
            case 'P':
                worst(vector_aux, max, len, n);
                break;
            default:
                cout << "Tipo de instancia invalida\n";
                return 1;
        }

        vector[i] = vector_aux;
    }

    int **vectorOrder = new int*[n_instances];
    int **vectorHeap = new int*[n_instances];
    int **vectorQuick = new int*[n_instances];
    int **vectorQuickR = new int*[n_instances];
    int **vectorIntro = new int*[n_instances];
    int **vectorIntroIn = new int*[n_instances];

    for (int i = 0; i < n_instances; i++){
        int *vectorOrder_aux = new int[n];
        copyVector(vector[i], vectorOrder_aux, n);
        vectorOrder[i] = vectorOrder_aux;

        int *vectorHeap_aux = new int[n];
        copyVector(vector[i], vectorHeap_aux, n);
        doHeap(vectorHeap_aux, 0, n-1);
        vectorHeap[i] = vectorHeap_aux;

        int *vectorQuick_aux = new int[n];
        copyVector(vector[i], vectorQuick_aux, n);
        vectorQuick[i] = vectorQuick_aux;

        int *vectorQuickR_aux = new int[n];
        copyVector(vector[i], vectorQuickR_aux, n);
        vectorQuickR[i] = vectorQuickR_aux;

        int *vectorIntro_aux = new int[n];
        copyVector(vector[i], vectorIntro_aux, n);
        vectorIntro[i] = vectorIntro_aux;

        int *vectorIntroIn_aux = new int[n];
        copyVector(vector[i], vectorIntroIn_aux, n);
        vectorIntroIn[i] = vectorIntroIn_aux;
    }

    clock_t time;
    deleteVecVec(vector, n_instances);

    time = clock();
    for (int i = 0; i < n_instances; i++){
        heapSort(vectorHeap[i], n); 
    }
    time = clock() - time;
    cout << "heapSort: " << ((double)time) / ((CLOCKS_PER_SEC / 1000)) << endl;

    time = clock();
    for (int i = 0; i < n_instances; i++){
        quickSort(vectorQuick[i], n);
    }
    time = clock() - time;
    cout << "quickSort: " << ((double)time) / ((CLOCKS_PER_SEC / 1000)) << endl;
    
    time = clock();
    for (int i = 0; i < n_instances; i++){
        quickSort(vectorQuickR[i], n, true);
    }
    time = clock() - time;
    cout << "quickSort random: " << ((double)time) / ((CLOCKS_PER_SEC / 1000)) << endl;

    time = clock();
    for (int i = 0; i < n_instances; i++){
        introSort(vectorIntro[i], n);
    }
    time = clock() - time;
    cout << "introSort: " << ((double)time) / ((CLOCKS_PER_SEC / 1000)) << endl; 

    time = clock();
    for (int i = 0; i < n_instances; i++){
        introSort(vectorIntroIn[i], n, -1, false, true);
    }
    time = clock() - time;
    cout << "introSort c/ insert: " << ((double)time) / ((CLOCKS_PER_SEC / 1000)) << endl;  


    for (int i = 0; i < n_instances; i++){
        insertsort(vectorOrder[i], 0, n-1);
    }

    for (int i = 0; i < n_instances; i++){
        if(!checkOrder(vectorHeap[i], vectorOrder[i], n)){
            cout << "Heap Nao ordenado corretamente\n";
            return 1;
        }
    }

    deleteVecVec(vectorHeap, n_instances);

    for (int i = 0; i < n_instances; i++){
        if(!checkOrder(vectorQuick[i], vectorOrder[i], n)){
            cout << "QuickS Nao ordenado corretamente\n";
            return 1;
        }
    }

    deleteVecVec(vectorQuick, n_instances);


    for (int i = 0; i < n_instances; i++){
        if(!checkOrder(vectorQuickR[i], vectorOrder[i], n)){
            cout << "QuickR Nao ordenado corretamente\n";
            return 1;
        }
    }
    deleteVecVec(vectorQuickR, n_instances);

    for (int i = 0; i < n_instances; i++){
        if(!checkOrder(vectorIntro[i], vectorOrder[i], n)){
            cout << "Intro Nao ordenado corretamente\n";
            return 1;
        }
    }  

    deleteVecVec(vectorIntro, n_instances);

    for (int i = 0; i < n_instances; i++){
        if(!checkOrder(vectorIntroIn[i], vectorOrder[i], n)){
            cout << "IntroIn Nao ordenado corretamente\n";
            return 1;
        }
    }  

    deleteVecVec(vectorIntroIn, n_instances);

    deleteVecVec(vectorOrder, n_instances);


    cout << "Todos os vetores estao ordenados corretamente\n";
    return 0;
}
