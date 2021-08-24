// Nome: Helio Matheus Sales Silva
// Matrícula: 400800

#include <iostream>
using namespace std;
template <typename TC, typename TV>
class DicioAVL{
    public:

    struct Noh{
        TC key;
        TV value;
        Noh* father;
        Noh* left;
        Noh* right;
        int b;

        Noh* obter_pai() {return father;}
        Noh* obter_dir() {return right;}
        Noh* obter_esq() {return left;}

    };
    void rot_esq(Noh* x){
        Noh* f = x->father;
        Noh* y = x->right; 
        if(y == nullptr) return;

        x->right = y->left;
        if(y->left != nullptr) y->left->father = x;

        y->father = x->father;
        if(x->father != nullptr && x->key <= x->father->key) x->father->left = y;
        else if(x->father != nullptr && x->key > x->father->key) x->father->right = y;
        else{ raiz = y; } // pai do x é nulo.
        
        y->left = x;
        x->father = y;

    }

    void rot_dir(Noh* x){
        Noh* f = x->father;
        Noh* y = x->left; 
        if(y == nullptr) return;

        x->left = y->right;
        if(y->right != nullptr) y->right->father = x;

        y->father = x->father;
        if(x->father != nullptr && x->key <= x->father->key) x->father->left = y;
        else if(x->father != nullptr && x->key > x->father->key) x->father->right = y;
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

        TC chave() {return p->key;}
        TV valor() {return p->value;}
 
        void operator++() {
            if( p->right != nullptr ) {
                p = p->right;
                while(p->left != nullptr) p = p->left;
            }else{
                while(p->father!= nullptr && p->father->key <= p->key){
                    p = p->father;
                }
                p = p->father;
            }
        }

        bool operator==(Iterador j) { return p==j.p; }
        bool operator!=(Iterador j) { return p!=j.p; }
    };

    DicioAVL():raiz(nullptr){}
    ~DicioAVL(){} // Desaloca a memória dinamica

    Iterador inicio(){
        Noh* it = raiz;
        while(it->left != nullptr) it = it->left;
        
        Iterador i(it);
        return i;
    }      

    Iterador fim() { Iterador i(nullptr); return i; }    
    
    Iterador inserir(TC c, TV v){
        Noh* novo = new Noh{c, v, nullptr, nullptr, nullptr, 0};

        if(raiz == nullptr) raiz = novo;
        else inserirAVL_(raiz, nullptr, novo);

        Iterador it(novo);
        return it;
    }

    void remover(Iterador i){
        Noh* x = i.p;
        if (x->left != nullptr && x->right != nullptr)   //2Filhos
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
                    x->right->left = succ->right;
                }   

                x->right->father = succ;
                succ->right = x->right;
            }

            if (x->father == nullptr){            // Verifica se estamos removendo a raiz
                raiz = succ;
                succ->father = nullptr;
            }else{
                if (x->key <= x->father->key)
                    x->father->left = succ;
                else
                    x->father->right = succ;
                succ->father = x->father;
            }
            removerAVL_(raiz, succ->father);
        }
        else{

            if (x->left != nullptr && x->right == nullptr){
                if (x->father == nullptr){               // Verifica se estamos removendo raiz
                    raiz = x->left;
                    x->left->father = nullptr;
                }else{
                    if (x->key <= x->father->key)
                        x->father->left = x->left;
                    else
                        x->father->right = x->left;
                    x->left->father = x->father;
                }
            }

            else if (x->left == nullptr && x->right != nullptr)
            {
                if (x->father == nullptr){               // Verifica se estamos removendo raiz
                    raiz = x->right;
                    x->right->father = nullptr;
                }else{
                    if (x->key <= x->father->key)
                        x->father->left = x->right;
                    else
                        x->father->right = x->right;
                    x->right->father = x->father;
                }
            }

            else {
                 if (x->father == nullptr)              // Verifica se estamos removendo raiz
                    raiz = nullptr;
                else
                {
                    if (x->key <= x->father->key)
                        x->father->left = nullptr;
                    else
                        x->father->right = nullptr;
                }
            }
            removerAVL_(raiz, x->father);
        }
    } 

    Iterador buscar(TC c){
        Noh* it = raiz;

        while(it != nullptr && it->key != c){
            if(c <= it->key) it = it->left;
            else it = it->right;
        }

        Iterador i(it);
        return i;
    } 
    private:
    
    bool inserirAVL_(Noh* x, Noh *x_father, Noh *novo){
        bool increased;
        
        if(x == nullptr){

            novo->father = x_father;
            if(x_father != nullptr && novo->key <= x_father->key) x_father->left = novo;
            else if(x_father != nullptr && novo->key > x_father->key) x_father->right = novo;
            else {} 

            increased = 1;
        }

        else if(novo->key <= x->key){
            increased = inserirAVL_(x->left, x, novo);
            if(increased){

                if( x->b == -1 ){ // Ja era desbalanceado para esquerda, insere a esquerda fica b(x) = -2
                    Noh* y = x->right;
                    if(y->b == -1){ // Mais pesado na ponta, rotaciona x a direita
                        rot_dir(x);
                        x->b = 0; y->b = 0; increased = 0;
                    }
                    else if(y->b == 1){ //  Mais pesado no meio, rotacao dupla
                        Noh* z = y->right;
                        if(z->b == 0) { x->b = 0; y->b = 0; }
                        else if(z->b == 1)  { y->b = -1; x->b = 0; }
                        else { y->b = 0; x->b = 1; }
                        rot_esq(y);
                        rot_dir(x);
                        z->b = 0;
                        increased = 0;
                    }
                    else{cout << "Teoricamente isso nunca acontece\n";}
                    
                } 

                else if( x->b == 0 ){
                    x->b = -1;
                    increased = 1;
                }

                else {
                    x->b = 0;
                    increased = 0;
                }
            }
        }

        else{
            increased = inserirAVL_(x->right, x, novo);
            if(increased){

                if( x->b == -1 ){
                    x->b = 0;
                    increased = 0;
                } 

                else if( x->b == 0 ){
                    x->b = 1;
                    increased = 1;
                }

                else {
                    Noh* y = x->right;
                    if(y->b == 1){
                        rot_esq(x);
                        x->b = 0; y->b = 0; increased = 0;
                    }
                    else if(y->b == -1){
                        Noh* z = y->left;
                        if(z->b == 0) { x->b = 0; y->b = 0; }
                        else if(z->b == 1)  { x->b = -1; y->b = 0; }
                        else { x->b = 0; y->b = 1;}
                        rot_dir(y);
                        rot_esq(x);
                        z->b = 0;
                        increased = 0;
                    }
                    else{cout << "Teoricamente isso nunca acontece\n";}
                }
            }
        }

        return increased;
    }



    bool removerAVL_(Noh* r, Noh* x){  
        bool decreased;
        
        if(r->key == x->key){
            decreased = 1;
        }
        else if(x->key < r->key) {
            decreased = removerAVL_(r->left, x);
            if(decreased){

            }
        }
        else {
            decreased = removerAVL_(r->right, x);
            if(decreased){
                
            }
        }

        return decreased;
    }


};

int main(int argc, char const *argv[])
{

    DicioAVL<int,char> D; int i;

    for (i = 48; i < 58; ++i) if (D.inserir(i, (char) i) == D.fim()) return 1;

    cout << D.obter_raiz()->key << endl;
   
    for (auto it = D.inicio(); it != D.fim(); ++it)
        cout << "O codigo de '" << it.valor() << "' eh " << it.chave() << '\n';

    /*for (i = 48; i < 58; ++i){
        auto it = D.buscar(i); if (it == D.fim()) return 1; else D.remover(it);
    }*/

    return 0;
}
