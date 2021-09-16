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
        else{ raiz = y; } 
        
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
        else{ raiz = y; } 
        
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
    ~DicioAVL(){               
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

            int a = x->b;
            x->b = succ->b;
            succ->b = a;

            if (x->left != nullptr){          
                x->left->father = succ;
                succ->left = x->left;
            } 

            Noh* succ_father = succ->father;
            if (x->father == nullptr){           
                raiz = succ;
                succ->father = nullptr;
            }else{
                if (x->chave <= x->father->chave)
                    x->father->left = succ;
                else
                    x->father->right = succ;
                succ->father = x->father;
            }

            if (succ != x->right){   
                Noh*succ_rig = succ->right;

                x->right->father = succ;
                succ->right = x->right;

                x->father = succ_father;
                succ_father->left = x;

                x->right = succ_rig;
                x->left = nullptr;
                if(succ_rig!=nullptr) succ_rig->father = x;
                removerAVL_(i, false);
                return;
            }else{
                x->left = nullptr;
                x->right = succ->right;
                if(succ->right != nullptr) succ->right->father = x;

                succ->right = x;
                x->father = succ;
                removerAVL_(i, true);
                return;
            }

            

            
        }
        
        removerAVL_(i, false);
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

    void removerAVL_(Iterador i, bool isChange){
        Noh *del = i.p;
        bool decreased = true;
        Noh *x, *aux = del;

        if (del->left != nullptr && del->right == nullptr)
        {
            if (del->father == nullptr){ 
                raiz = del->left;
                del->left->father = nullptr;
            }else{
                if (!isChange && del->chave <= del->father->chave)
                    del->father->left = del->left;
                else
                    del->father->right = del->left;
                del->left->father = del->father;
            }
            aux = del->left;
        }

        else if (del->left == nullptr && del->right != nullptr){

            if (del->father == nullptr){ 
                raiz = del->right;
                del->right->father = nullptr;
            }else{
                if (!isChange && del->chave <= del->father->chave)
                    del->father->left = del->right;
                else
                    del->father->right = del->right;
                del->right->father = del->father;
            }
            aux = del->right;
        }

        else{
            if (del->father == nullptr) 
                raiz = nullptr;
            else{
                if (!isChange && del->chave <= del->father->chave)
                    del->father->left = nullptr;
                else
                    del->father->right = nullptr;
            }
            aux = del;
        }


        while(decreased && aux != nullptr){
            x = aux->father;
            if(!isChange && x!=nullptr && aux->chave <= x->chave){     // Sub arvore esquerda diminuiu
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
                        aux = x;
                        x->b = 0; y->b = 0; decreased = true;
                    }
                    else if(y->b == -1){
                        Noh* z = y->left;
                        if(z->b == 0) { x->b = 0; y->b = 0; }
                        else if(z->b == 1)  { x->b = -1; y->b = 0; }
                        else { x->b = 0; y->b = 1;}
                        rot_dir(y); rot_esq(x); aux = x; z->b = 0; decreased = true;
                    }
                    else{
                        rot_esq(x);
                        aux = x;
                        x->b = 1; y->b = -1; decreased = false;
                    }
                }
            }
            else if (isChange || (x != nullptr && aux->chave > x->chave))
            { // Sub arvore direita diminui
                isChange = false;
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
                        aux = x;
                        x->b = 0; y->b = 0; decreased = true;
                    }
                    else if(y->b == 1){
                        Noh* z = y->right;
                        if(z->b == 0) { x->b = 0; y->b = 0; }
                        else if(z->b == 1)  { x->b = 0; y->b = -1; }
                        else { x->b = 1; y->b = 0;}
                        rot_esq(y); rot_dir(x); z->b = 0; aux = x; decreased = true;
                    }
                    else{
                        rot_dir(x);
                        aux = x;
                        x->b = -1; y->b = 1; decreased = false;
                    }
                }
            }else{}
            aux = aux->father;
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
                        rot_esq(y); rot_dir(x); z->b = 0; increased = false;
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
                        rot_dir(y); rot_esq(x); z->b = 0; increased = false;
                    }
                    else{}
                }
            }
        }

        return increased;
    }

};

// #include <iostream>
// using namespace std;

// int main()
// {
//         DicioAVL<double, double> dict;
        // dict.inserir(-202.25, -45);
        // dict.inserir(218.5, -1.5);
        // dict.inserir(-21.75, 69.5);
        // dict.inserir(-2.25, -41.5);
        // dict.inserir(-191.5, 36.5);
        // dict.inserir(136.5, -72);
        // dict.inserir(61.75, -1.5);
        // dict.inserir(202.75, 44.5);
        // dict.inserir(-193, -55.5);
        // dict.inserir(-0.75, -75.5);

        // cout << dict.obter_raiz()->chave << " " << dict.obter_raiz()->b << endl;
        // cout << dict.obter_raiz()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->b << endl;
        // cout << dict.obter_raiz()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->b << endl;
        // if(dict.obter_raiz()->obter_dir()->obter_esq()!=nullptr) cout << dict.obter_raiz()->obter_dir()->obter_esq()->chave << " " << dict.obter_raiz()->obter_dir()->obter_esq()->b << endl;
        // if(dict.obter_raiz()->obter_dir()->obter_dir()!= nullptr)cout << dict.obter_raiz()->obter_dir()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->obter_dir()->b << endl;
        // if(dict.obter_raiz()->obter_esq()->obter_esq()!= nullptr)cout << dict.obter_raiz()->obter_esq()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->obter_esq()->b << endl;
        // if(dict.obter_raiz()->obter_esq()->obter_dir()!= nullptr)cout << dict.obter_raiz()->obter_esq()->obter_dir()->chave << " " << dict.obter_raiz()->obter_esq()->obter_dir()->b << endl;

        // cout << "remover\n";
        // dict.remover(dict.buscar(-0.75));

        // cout << dict.obter_raiz()->chave << " " << dict.obter_raiz()->b << endl;
        // cout << dict.obter_raiz()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->b << endl;
        // cout << dict.obter_raiz()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->b << endl;
        // if(dict.obter_raiz()->obter_dir()->obter_esq()!=nullptr) cout << dict.obter_raiz()->obter_dir()->obter_esq()->chave << " " << dict.obter_raiz()->obter_dir()->obter_esq()->b << endl;
        // if(dict.obter_raiz()->obter_dir()->obter_dir()!= nullptr)cout << dict.obter_raiz()->obter_dir()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->obter_dir()->b << endl;
        // if(dict.obter_raiz()->obter_esq()->obter_esq()!= nullptr)cout << dict.obter_raiz()->obter_esq()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->obter_esq()->b << endl;
        // if(dict.obter_raiz()->obter_esq()->obter_dir()!= nullptr)cout << dict.obter_raiz()->obter_esq()->obter_dir()->chave << " " << dict.obter_raiz()->obter_esq()->obter_dir()->b << endl;

        // cout << "remover\n";
        // dict.remover(dict.buscar(-21.75));
        // cout << dict.obter_raiz()->chave << " " << dict.obter_raiz()->b << endl;
        // cout << dict.obter_raiz()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->b << endl;
        // cout << dict.obter_raiz()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->b << endl;
        // if(dict.obter_raiz()->obter_dir()->obter_esq()!=nullptr) cout << dict.obter_raiz()->obter_dir()->obter_esq()->chave << " " << dict.obter_raiz()->obter_dir()->obter_esq()->b << endl;
        // if(dict.obter_raiz()->obter_dir()->obter_dir()!= nullptr)cout << dict.obter_raiz()->obter_dir()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->obter_dir()->b << endl;
        // if(dict.obter_raiz()->obter_esq()->obter_esq()!= nullptr)cout << dict.obter_raiz()->obter_esq()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->obter_esq()->b << endl;
        // if(dict.obter_raiz()->obter_esq()->obter_dir()!= nullptr)cout << dict.obter_raiz()->obter_esq()->obter_dir()->chave << " " << dict.obter_raiz()->obter_esq()->obter_dir()->b << endl;

//         dict.inserir(-217, -16);
//         dict.inserir(-172.5, -22.5);
//         dict.inserir(166.5, -74);
//         dict.inserir(-292, -71);
//         dict.inserir(147, 6);
//         auto i = dict.inserir(14, -50.5);
//         dict.inserir(-165, 8);

//         cout << dict.obter_raiz()->chave << " " << dict.obter_raiz()->b << endl;
//         cout << dict.obter_raiz()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->b << endl;
//         cout << dict.obter_raiz()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->b << endl;
//         if (dict.obter_raiz()->obter_dir()->obter_esq() != nullptr)
//             cout << dict.obter_raiz()->obter_dir()->obter_esq()->chave << " " << dict.obter_raiz()->obter_dir()->obter_esq()->b << endl;
//         if (dict.obter_raiz()->obter_dir()->obter_dir() != nullptr)
//             cout << dict.obter_raiz()->obter_dir()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->obter_dir()->b << endl;
//         if (dict.obter_raiz()->obter_esq()->obter_esq() != nullptr)
//             cout << dict.obter_raiz()->obter_esq()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->obter_esq()->b << endl;
//         if (dict.obter_raiz()->obter_esq()->obter_dir() != nullptr)
//             cout << dict.obter_raiz()->obter_esq()->obter_dir()->chave << " " << dict.obter_raiz()->obter_esq()->obter_dir()->b << endl;

//         cout << "remover\n";
//         dict.remover(dict.buscar(-172.5));

//         cout << dict.obter_raiz()->chave << " " << dict.obter_raiz()->b << endl;
//         cout << dict.obter_raiz()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->b << endl;
//         cout << dict.obter_raiz()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->b << endl;
//         if (dict.obter_raiz()->obter_dir()->obter_esq() != nullptr)
//             cout << dict.obter_raiz()->obter_dir()->obter_esq()->chave << " " << dict.obter_raiz()->obter_dir()->obter_esq()->b << endl;
//         if (dict.obter_raiz()->obter_dir()->obter_dir() != nullptr)
//             cout << dict.obter_raiz()->obter_dir()->obter_dir()->chave << " " << dict.obter_raiz()->obter_dir()->obter_dir()->b << endl;
//         if (dict.obter_raiz()->obter_esq()->obter_esq() != nullptr)
//             cout << dict.obter_raiz()->obter_esq()->obter_esq()->chave << " " << dict.obter_raiz()->obter_esq()->obter_esq()->b << endl;
//         if (dict.obter_raiz()->obter_esq()->obter_dir() != nullptr)
//             cout << dict.obter_raiz()->obter_esq()->obter_dir()->chave << " " << dict.obter_raiz()->obter_esq()->obter_dir()->b << endl;

//         if(i == dict.buscar(14)) cout << "Igual\n"; else cout << "Diferente\n";

//         return 0;
// }
 