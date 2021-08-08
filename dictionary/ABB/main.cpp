#include <iostream>
using namespace std;
template <typename TC, typename TV>
class DictABB{
    public:
    class Noh{
        public:
        TC key;
        TV value;
        Noh* father;
        Noh* left;
        Noh* right;

        Noh(TC key, TV value):key(key),value(value){
            left = nullptr;
            right = nullptr;
            father = nullptr;
        }
    };

    Noh *root;


    class Iterador {
        private:
            Noh *p;
        public:
        Iterador(Noh* p):p(p){}

        TC key() {return p->key;}
        TV value() {return p->value;}

        void setLeft(Noh *l) { p->left =l; }
        void setRight(Noh *r) { p->right = r; }
        void setFather(Noh *f) { p->father = f; }

        Iterador left(){Iterador i(p->left); return i;}
        Iterador right(){Iterador i(p->right); return i;}
        Iterador father(){Iterador i(p->father); return i;}

        Noh* get(){return p;}

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

    DictABB():root(nullptr){}
    ~DictABB(){} // Desaloca a memória dinamica

    Iterador start(){
        Iterador it(root);
        while(it.left()!=end()) it = it.left();
        return it;
    }      

    Iterador end() { Iterador i(nullptr); return i; }    
    
    Iterador insert(TC c, TV v){
        Noh *n = new Noh(c,v); if(n == nullptr) return end();
        Iterador i(n);
        
        if(root != nullptr){
            Iterador it(root);

            while(it!=end())
            {
                if(c <= it.key()){
                    if(it.left() != nullptr) it = it.left();
                    else break;
                }   
                else{
                    if(it.right() != nullptr) it = it.right();
                    else break;
                }
            }

            if(c <= it.key()) it.setLeft(n); 
            else it.setRight(n);

            i.setFather(it.get());
        }else{
            root = n;
        }

        return i;
    }

    Iterador search(TC c){
        Iterador it(root);

        while(it != end() && it.key() != c){
            if(c <= it.key()) it = it.left();
            else it = it.right();
        }

        return it;
    } 

    void remove(Iterador i){
        Iterador a(root);
        if(i.left() == end() && i.right() == end()){
            if (a == i) root = nullptr;
            else{
                if(i.key() <= i.father().key()) i.father().setLeft(nullptr);
                else i.father().setRight(nullptr);
            }
        }
        else if(i.left() != end() && i.right() == end()){
            if (a == i){
                i.left().setFather(nullptr);
                root = i.left().get();
            }
            else{
                if(i.key() <= i.father().key()) i.father().setLeft(i.left().get());
                else i.father().setRight(i.left().get());
                i.left().setFather(i.father().get());
            }
        }
        
        else if(i.left() == end() && i.right() != end()){
            if (a == i){
                i.right().setFather(nullptr);
                root = i.right().get();
            }
            else{
                if(i.key() <= i.father().key()) i.father().setLeft(i.right().get());
                else i.father().setRight(i.right().get());
                i.right().setFather(i.father().get());
            }
        }
        else{
            Iterador aux = i;
            ++aux;
            
            if(aux.right() != end()){ 
                aux.right().setFather(i.right().get());
                i.right().setLeft(aux.right().get());
            }
            
            if(i.left() != end()){
                i.left().setFather(aux.get());
                aux.setLeft(i.left().get());
            } 

            if(aux != i.right()){
                i.right().setFather(aux.get());
                aux.setRight(i.right().get());
            }

            if (a == i){
                root = aux.get();
                aux.setFather(nullptr);
            }else{
                if(i.key() <= i.father().key()) i.father().setLeft(aux.get());
                else i.father().setRight(aux.get());
                aux.setFather(i.father().get());
            }

        }

    } 
};

int main(int argc, char const *argv[])
{
    DictABB<int,int> dict;

    dict.insert(50, 1);
    dict.insert(30, 1);
    dict.insert(100,1);
    dict.insert(20, 1);
    dict.insert(40, 1);
    dict.insert(35, 1);
    dict.insert(45, 1);
    dict.insert(37, 1);

    for (DictABB<int,int>::Iterador it = dict.start(); it != dict.end(); ++it)
        cout << it.key() << " ";
    cout << endl;

    dict.remove(dict.search(30));

    for (DictABB<int, int>::Iterador it = dict.start(); it != dict.end(); ++it)
        cout << it.key() << " ";
    cout << endl;

    

    return 0;
}
