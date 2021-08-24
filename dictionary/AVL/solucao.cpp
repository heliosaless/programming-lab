// Nome: Helio Matheus Sales Silva
// Matrícula: 400800

//#include <iostream>
#include <new>
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
                }   
                x->right->left = succ->right;

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

            if(succ->right != nullptr){
                Noh* it = succ->right;
                while(it->left != nullptr) it = it->left;
                removerAVL_(raiz, it);
            }
            else{
                removerAVL_(raiz,succ);
            }
            
        }
        else{
            if (x->left != nullptr && x->right == nullptr){
                if (x->father == nullptr){ // Verifica se estamos removendo raiz
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

            else if (x->left == nullptr && x->right != nullptr){
                if (x->father == nullptr){ // Verifica se estamos removendo raiz
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

            else{
                if (x->father == nullptr) // Verifica se estamos removendo raiz
                    raiz = nullptr;
                else{
                    if (x->key <= x->father->key)
                        x->father->left = nullptr;
                    else
                        x->father->right = nullptr;
                }
            }
            removerAVL_(raiz, x);
        }

        delete x;
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
    
    bool inserirAVL_(Noh* x, Noh *r_father, Noh *novo){
        bool increased = false;
        
        if(x == nullptr){

            novo->father = r_father;
            if(r_father != nullptr && novo->key <= r_father->key) r_father->left = novo;
            else if(r_father != nullptr && novo->key > r_father->key) r_father->right = novo;
            else {} 

            increased = true;
        }

        else if(novo->key <= x->key){
            increased = inserirAVL_(x->left, x, novo);
            if(increased){

                if( x->b == -1 ){ // Ja era desbalanceado para esquerda, insere a esquerda fica b(x) = -2
                    Noh* y = x->right;
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



    bool removerAVL_(Noh* x, Noh* i){  
        bool decreased = false;
        if(x == nullptr){
            decreased = true;
        }
        else if(i->key < x->key) {
            decreased = removerAVL_(x->left, i);
            if(decreased){

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
            }
        }
        else {
            decreased = removerAVL_(x->right, i);
            if(decreased){
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
        }

        return decreased;
    }


};
/*
int main()
{

    DicioAVL<int,char> D; int i;

    for (i = 48; i < 58; ++i) if (D.inserir(i, (char) i) == D.fim()) return 1;

    for (auto it = D.inicio(); it != D.fim(); ++it)
        std::cout << "O codigo de '" << it.valor() << "' eh " << it.chave() << '\n';

    for (i = 48; i < 58; ++i){
        auto it = D.buscar(i); if (it == D.fim()) return 1; else D.remover(it);
    }

    for (auto it = D.inicio(); it != D.fim(); ++it)
        std::cout << "O codigo de '" << it.valor() << "' eh " << it.chave() << '\n';

    return 0;
}
*/