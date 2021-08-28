// Nome: Helio Matheus Sales Silva
// Matrícula: 400800

#include <new>
template <typename TC, typename TV>
class DicioAVL{
    public:

    struct Noh{
        TC chave;
        TV valor;
        Noh* father;
        Noh* left;
        Noh* right;
        int b;

        Noh* obter_pai() {return father;}
        Noh* obter_dir() {return right;}
        Noh* obter_esq() {return left;}

    };
    void rot_esq(Noh* x){
        Noh* y = x->right; 
        if(y == nullptr) return;

        x->right = y->left;
        if(y->left != nullptr) y->left->father = x;

        y->father = x->father;
        if(x->father != nullptr && x->chave <= x->father->chave) x->father->left = y;
        else if(x->father != nullptr && x->chave > x->father->chave) x->father->right = y;
        else{ raiz = y; } // pai do x é nulo.
        
        y->left = x;
        x->father = y;

    }

    void rot_dir(Noh* x){
        Noh* y = x->left; 
        if(y == nullptr) return;

        x->left = y->right;
        if(y->right != nullptr) y->right->father = x;

        y->father = x->father;
        if(x->father != nullptr && x->chave <= x->father->chave) x->father->left = y;
        else if(x->father != nullptr && x->chave > x->father->chave) x->father->right = y;
        else{ raiz = y; } // pai do x é nulo.
        
        y->right = x;
        x->father = y;
    }

    Noh* raiz;
    Noh *obter_raiz() {return raiz;}

    class Iterador {
        private:
            Noh *p;
            friend DicioAVL;

        public:
        Iterador(Noh* p):p(p){}

        TC chave() {return p->chave;}
        TV valor() {return p->valor;}
 
        void operator++() {
            if( p->right != nullptr ) {
                p = p->right;
                while(p->left != nullptr) p = p->left;
            }else{
                while(p->father!= nullptr && p->father->chave <= p->chave){
                    p = p->father;
                }
                p = p->father;
            }
        }

        bool operator==(Iterador j) { return p==j.p; }
        bool operator!=(Iterador j) { return p!=j.p; }
    };

    DicioAVL():raiz(nullptr){}
    ~DicioAVL(){                // Desaloca a memória dinamica
        if(raiz != nullptr) dealloc(raiz);
    }

    void dealloc(Noh *x){
        if(x->left != nullptr) dealloc(x->left);
        if(x->right != nullptr) dealloc(x->right);
        delete x;
    }

    Iterador inicio(){
        Noh* it = raiz;
        while(it != nullptr && it->left != nullptr) it = it->left;
        
        Iterador i(it);
        return i;
    }      

    Iterador fim() { Iterador i(nullptr); return i; }    
    
    Iterador inserir(TC c, TV v){
        Noh *novo = new (std::nothrow) Noh{c, v, nullptr, nullptr, nullptr, 0};
        if(novo == nullptr) return fim();

        if(raiz == nullptr) raiz = novo;
        else inserirAVL_(raiz, nullptr, novo);

        Iterador it(novo);
        return it;
    }

    void remover(Iterador i){
        Noh* x = i.p;
        if (x->left != nullptr && x->right != nullptr)  
        {
            Iterador aux = i; ++aux;
            Noh* succ = aux.p;

            if (x->left != nullptr){          // Colocar o filho esquerdo do x como filho esquerdo do succ
                x->left->father = succ;
                succ->left = x->left;
            }

            // Essas instruções não fazem sentido para succ == x->right
            if (succ != x->right){    
                if (succ->right != nullptr){         // Colocar o filho direito do sucessor como filho do esquerdo do direito do x
                    succ->right->father = x->right;
                }   
                x->right->left = succ->right;

                x->right->father = succ;
                succ->right = x->right;
            }

            Noh* temp = succ->father;

            if (x->father == nullptr){            // Verifica se estamos removendo a raiz
                raiz = succ;
                succ->father = nullptr;
            }else{
                if (x->chave <= x->father->chave)
                    x->father->left = succ;
                else
                    x->father->right = succ;
                succ->father = x->father;
            }

            x->right = nullptr;
            x->left = nullptr;
            
            x->father = temp;
            temp->left = x;

        }
        removerAVL_(i);

    }

    Iterador buscar(TC c){
        Noh* it = raiz;

        while(it != nullptr && it->chave != c){
            if(c <= it->chave) it = it->left;
            else it = it->right;
        }

        Iterador i(it);
        return i;
    } 

    private:

    void removerAVL_(Iterador i){
        Noh* del = i.p;
        Noh *son;
        
        if (del->left != nullptr && del->right == nullptr)
        {
            if (del->father == nullptr){ // Verifica se estamos removendo raiz
                raiz = del->left;
                del->left->father = nullptr;
            }else{
                if (del->chave <= del->father->chave)
                    del->father->left = del->left;
                else
                    del->father->right = del->left;
                del->left->father = del->father;
            }
            son = del->left;
        }

        else if (del->left == nullptr && del->right != nullptr){

            if (del->father == nullptr){ // Verifica se estamos removendo raiz
                raiz = del->right;
                del->right->father = nullptr;
            }else{
                if (del->chave <= del->father->chave)
                    del->father->left = del->right;
                else
                    del->father->right = del->right;
                del->right->father = del->father;
            }
            son = del->right;
        }

        else{

            if (del->father == nullptr) // Verifica se estamos removendo raiz
                raiz = nullptr;
            else{
                if (del->chave <= del->father->chave)
                    del->father->left = nullptr;
                else
                    del->father->right = nullptr;
            }
            son = del;
        }

        Noh* x;
        bool decreased = true;
        
        while(decreased && son->father != nullptr){
            x = son->father;

            if(son->chave <= x->chave){     // Sub arvore esquerda diminuiu
                if(x->b == -1){
                    x->b = 0;
                    decreased = true;
                }

                else if(x->b == 0){
                    x->b = 1;
                    decreased = false;
                }
                
                else{
                    Noh* y = x->right;
                    if(y->b == 1){
                        rot_esq(x);
                        x->b = 0; y->b = 0; decreased = true;
                    }
                    else if(y->b == -1){
                        Noh* z = y->left;
                        if(z->b == 0) { x->b = 0; y->b = 0; }
                        else if(z->b == 1)  { x->b = -1; y->b = 0; }
                        else { x->b = 0; y->b = 1;}
                        rot_dir(y);
                        rot_esq(x);
                        z->b = 0;
                        decreased = true;
                    }
                    else{
                        rot_esq(x);
                        x->b = 1; y->b = -1; decreased = false;
                    }
                }
            }else{              // Sub arvore direita diminui
                if(x->b == 1){
                    x->b = 0;
                    decreased = true;
                }

                else if(x->b == 0){
                    x->b = -1;
                    decreased = false;
                }
                
                else{
                    Noh* y = x->left;
                    if(y->b == -1){
                        rot_dir(x);
                        x->b = 0; y->b = 0; decreased = true;
                    }
                    else if(y->b == 1){
                        Noh* z = y->right;
                        if(z->b == 0) { x->b = 0; y->b = 0; }
                        else if(z->b == 1)  { x->b = -1; y->b = 0; }
                        else { x->b = 0; y->b = 1;}
                        rot_esq(y);
                        rot_dir(x);
                        z->b = 0;
                        decreased = true;
                    }
                    else{
                        rot_dir(x);
                        x->b = -1; y->b = 1; decreased = false;
                    }
                }

            }
            son = son->father;
        }

        delete del;
    }
    
    bool inserirAVL_(Noh* x, Noh *r_father, Noh *novo){
        bool increased = false;
        
        if(x == nullptr){
            novo->father = r_father;
            if(r_father != nullptr && novo->chave <= r_father->chave) r_father->left = novo;
            else if(r_father != nullptr && novo->chave > r_father->chave) r_father->right = novo;
            else {} 

            increased = true;
        }

        else if(novo->chave <= x->chave){
            increased = inserirAVL_(x->left, x, novo);
            if(increased){

                if( x->b == -1 ){ // Ja era desbalanceado para esquerda, insere a esquerda fica b(x) = -2
                    Noh* y = x->left;

                    if(y->b == -1){ // Mais pesado na ponta, rotaciona x a direita
                        rot_dir(x);
                        x->b = 0; y->b = 0; increased = false;
                    }
                    else if(y->b == 1){ //  Mais pesado no meio, rotacao dupla
                        Noh* z = y->right;
                        if(z->b == 0) { x->b = 0; y->b = 0; }
                        else if(z->b == 1)  { y->b = -1; x->b = 0; }
                        else { y->b = 0; x->b = 1; }
                        rot_esq(y);
                        rot_dir(x);
                        z->b = 0;
                        increased = false;
                    }
                    else{}
                    
                } 

                else if( x->b == 0 ){
                    x->b = -1;
                    increased = true;
                }

                else {
                    x->b = 0;
                    increased = false;
                }
            }
        }

        else{
            increased = inserirAVL_(x->right, x, novo);
            if(increased){

                if( x->b == -1 ){
                    x->b = 0;
                    increased = false;
                } 

                else if( x->b == 0 ){
                    x->b = 1;
                    increased = true;
                }

                else {
                    Noh* y = x->right;
                    if(y->b == 1){
                        rot_esq(x);
                        x->b = 0; y->b = 0; increased = false;
                    }
                    else if(y->b == -1){
                        Noh* z = y->left;
                        if(z->b == 0) { x->b = 0; y->b = 0; }
                        else if(z->b == 1)  { x->b = -1; y->b = 0; }
                        else { x->b = 0; y->b = 1;}
                        rot_dir(y);
                        rot_esq(x);
                        z->b = 0;
                        increased = false;
                    }
                    else{}
                }
            }
        }

        return increased;
    }

};

/*
#include <iostream>
using namespace std;
int main()
{
    DicioAVL<double, double> dict;
    dict.inserir(204,-21);
    auto in2 = dict.inserir(169.75, 59.5);
    auto in3 = dict.inserir(246.75, -60);

    for (auto it = dict.inicio(); it != dict.fim(); ++it)
        cout << "KEY: " <<  it.chave() << " VALUE: " << it.valor() << endl;

    dict.remover(dict.buscar(204));

    for (auto it = dict.inicio(); it != dict.fim(); ++it)
        cout << "KEY: " << it.chave() << " VALUE: " << it.valor() << endl;

    auto it = dict.buscar(246.75);
    if(it != dict.fim()){if(it == in3) cout << "Igual" << endl; else cout << "Diferente" << endl;}

     it = dict.buscar(169.75);
    if(it != dict.fim()){if(it == in2) cout << "Igual" << endl; else cout << "Diferente" << endl;}
    
    return 0;
}
*/