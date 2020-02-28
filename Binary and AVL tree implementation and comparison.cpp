#include <iostream>
#include <queue> 


template <typename tipKljuc, typename tipVrijednost>
class Mapa {
public:
  
    Mapa () {};
    virtual~Mapa() {};
    
    virtual tipVrijednost operator [] (tipKljuc kljuc) const = 0;
    virtual tipVrijednost &operator [] (tipKljuc kljuc) = 0;
    
    virtual int brojElemenata() const =  0;
    virtual void obrisi() = 0;
    virtual void obrisi( const tipKljuc &kljuc) = 0;
    
};


template <typename Tip1, typename Tip2>
struct cvorA {
    Tip1 kljuc;
    Tip2 vrijednost;
    cvorA<Tip1,Tip2> *left;
    cvorA<Tip1,Tip2> *right;
    int visina;
};


template <typename Tip1, typename Tip2>
class AVLStabloMapa : public Mapa<Tip1, Tip2> { 
    int vel;
    cvorA<Tip1, Tip2> *root;
    
   
    void dealociraj(cvorA<Tip1, Tip2>*& root);
    void kopiraj (const AVLStabloMapa<Tip1,Tip2> &m);
   
   
    cvorA<Tip1, Tip2>* findKey (const Tip1 &key) const;
    cvorA<Tip1,Tip2>* insert (cvorA<Tip1,Tip2>*, const Tip1&, cvorA<Tip1,Tip2>*&);
    
    cvorA<Tip1, Tip2>* findMin (cvorA<Tip1, Tip2>*) const;
    cvorA<Tip1, Tip2>* obrisi (cvorA<Tip1, Tip2>*, const Tip1&);
   
    int dajVisinu (cvorA<Tip1, Tip2>* node) {
        if (node == nullptr)
            return 0;
        return node -> visina;
   }
   
   int max (int a, int b) {
       if (a>b) return a;
       return b;
   }
   
    int dajBalans(cvorA<Tip1, Tip2>* node) {
        if (node == nullptr)
            return 0;
        return dajVisinu(node -> left) - dajVisinu(node->right);    
    }
    
    cvorA<Tip1, Tip2>* desnaRotacija (cvorA<Tip1, Tip2>* y);
    cvorA<Tip1, Tip2>* lijevaRotacija (cvorA<Tip1, Tip2>* x);
    
    
    
    
public:
    AVLStabloMapa () : vel(0), root(nullptr) {};
    
    ~AVLStabloMapa () override {
        obrisi();
    };
   
    void obrisi () override {
        dealociraj(root);
        root = nullptr;
        vel = 0;
    }
    
    AVLStabloMapa (const AVLStabloMapa<Tip1,Tip2> &m);
    AVLStabloMapa<Tip1,Tip2> &operator = (const AVLStabloMapa<Tip1,Tip2> &m);
   
    
    Tip2 &operator [] (Tip1 key) override;    
    Tip2 operator [] (const Tip1 key) const override;
    

    
    void obrisi(const Tip1 &key) override {
        root = obrisi (root, key);
        vel--;
    }
    
    int brojElemenata() const override {
        return vel;
    }
    
};


template <typename Tip1, typename Tip2>
void AVLStabloMapa<Tip1, Tip2>::kopiraj (const AVLStabloMapa<Tip1,Tip2> &m) {
    std::queue<cvorA<Tip1,Tip2>*> red;
    red.push(m.root);
    while (!red.empty()) {
        auto trenutni = red.front();
        (*this)[trenutni -> kljuc] = trenutni -> vrijednost;
        if (trenutni -> left != nullptr)
            red.push(trenutni -> left);
        if (trenutni -> right != nullptr)
            red.push(trenutni -> right);
        red.pop();
    }    
}

template <typename Tip1, typename Tip2>
void AVLStabloMapa<Tip1, Tip2>::dealociraj(cvorA<Tip1, Tip2>*& root) {
    // PostOrder Traversal
    if (root == nullptr)
        return;
    dealociraj(root -> left);
    dealociraj(root -> right);
    delete root;
}

template <typename Tip1, typename Tip2>
AVLStabloMapa<Tip1, Tip2>::AVLStabloMapa (const AVLStabloMapa<Tip1,Tip2> &m) : vel(0), root(nullptr) {
    if (m.root == nullptr)
        return;
    kopiraj(m);
}

template <typename Tip1, typename Tip2>
AVLStabloMapa<Tip1,Tip2> &AVLStabloMapa<Tip1, Tip2>::operator = (const AVLStabloMapa<Tip1,Tip2> &m) {
    if (this != &m) {
        obrisi();
        kopiraj(m);
    }
    return *this;
}

template <typename Tip1, typename Tip2>
cvorA<Tip1, Tip2>* AVLStabloMapa<Tip1, Tip2>::findKey (const Tip1 &key) const {
    if (root == nullptr)
        return root;
    auto trenutni = root;
    auto prethodni = root;
    
    while (trenutni != nullptr && trenutni -> kljuc != key) {
        prethodni = trenutni;
        if (key < trenutni -> kljuc)
            trenutni = trenutni -> left;
        else 
            trenutni = trenutni -> right;
    }
    if (trenutni == nullptr)    // key nije pronadjen u stablu;
        return prethodni;
    return trenutni;            // key je pronadjen u stablu
}


template <typename Tip1, typename Tip2>
Tip2 &AVLStabloMapa<Tip1, Tip2>::operator [] (Tip1 key) {
    
    cvorA<Tip1,Tip2>* place;     // Mjesto na kojem se nalazi dodani element ili vec postojeci
    root = insert (root, key, place);
    return place -> vrijednost;
}

template <typename Tip1, typename Tip2>
Tip2 AVLStabloMapa<Tip1, Tip2>::operator [] (const Tip1 key) const {
    auto place = findKey(key);
    if (place != nullptr && place -> kljuc == key)
        return place -> vrijednost;
    return Tip2();
}

// ROTACIJE
template <typename Tip1, typename Tip2>
cvorA<Tip1,Tip2>* AVLStabloMapa<Tip1, Tip2>::desnaRotacija (cvorA<Tip1,Tip2>* y) {
    cvorA<Tip1,Tip2> *x = y->left;
    cvorA<Tip1,Tip2> *t2 = x -> right;
    
    x -> right = y;
    y -> left = t2;
    
    y -> visina = max(dajVisinu(y->left), dajVisinu(y->right)) + 1;
    x -> visina = max(dajVisinu(x->left), dajVisinu(x->right)) + 1;
    
    return x;
}

template <typename Tip1, typename Tip2>
cvorA<Tip1,Tip2>* AVLStabloMapa<Tip1, Tip2>::lijevaRotacija (cvorA<Tip1,Tip2>* x) {
    cvorA<Tip1,Tip2> *y = x -> right;
    cvorA<Tip1,Tip2> *t2 = y -> left;
    
    y -> left = x;
    x -> right = t2;
    
    x -> visina = max(dajVisinu(x->left), dajVisinu(x->right)) + 1;
    y -> visina = max(dajVisinu(y->left), dajVisinu(y->right)) + 1;
    
    return y;
}

template <typename Tip1, typename Tip2>
cvorA<Tip1,Tip2>* AVLStabloMapa<Tip1, Tip2>::insert (cvorA<Tip1,Tip2>* root,const Tip1 &key, cvorA<Tip1,Tip2>*& place) {
    if (root == nullptr) {
        root = new cvorA<Tip1,Tip2> {key, Tip2(), nullptr, nullptr, 1};
        place = root;
        (this -> vel)++;
        return root;
    }
    
    if (key < root -> kljuc)
        root -> left = insert (root->left, key, place);
    else if (key > root -> kljuc)
        root -> right = insert (root->right, key, place);
    else {
        // Ako kljuc vec postoji, samo vracamo pokazivac na njega, place
        place = root;
        return root;
    }
    
    // Visina i balans
    root -> visina = max(dajVisinu(root->left), dajVisinu(root->right)) + 1;
    int balans = dajBalans(root);

    // Slucajevi za rotaciju
    if (balans > 1 && key < root -> left -> kljuc)
        return desnaRotacija(root);
    if (balans < -1 && key > root -> right -> kljuc)
        return lijevaRotacija(root);
        
    if (balans > 1 && key > root -> left -> kljuc) {
        root -> left = lijevaRotacija(root -> left);
        return desnaRotacija(root);
    }
    if (balans < -1 && key < root -> right -> kljuc) {
        root -> right = desnaRotacija(root -> right);
        return lijevaRotacija(root);
    }
    
    return root;
}

template <typename Tip1, typename Tip2>
cvorA<Tip1, Tip2>* AVLStabloMapa<Tip1, Tip2>::findMin (cvorA<Tip1, Tip2>* root) const { 
    auto trenutni = root;
    cvorA<Tip1, Tip2>* prethodni = nullptr;
    while (trenutni != nullptr) {
        prethodni = trenutni;
        trenutni = trenutni -> left;
    }
    return prethodni;
}
 
template <typename Tip1, typename Tip2>
cvorA<Tip1, Tip2>* AVLStabloMapa<Tip1, Tip2>::obrisi(cvorA<Tip1, Tip2>* root, const Tip1 &key) {
    if (root == nullptr)
        return root;
    else if (key < root -> kljuc)
        root -> left = obrisi (root -> left, key);
    else if (key > root -> kljuc)
        root -> right = obrisi (root -> right, key);
    else { // Nadjen je element za brisanje
        // Nema djece 
        if (root -> left == nullptr && root -> right == nullptr) {
            delete root;
            root = nullptr;
        }
        // Ima samo lijevo dijete
        else if (root -> right == nullptr) {
            auto temp = root;
            root = root -> left;
            delete temp;
        }
        // Ima samo desno dijete
        else if (root -> left == nullptr) {
            auto temp = root;
            root = root -> right;
            delete temp;
        }
        // Ima dva dijeteta
        else {
            auto temp = findMin(root -> right);
            root -> kljuc = temp -> kljuc;
            root -> vrijednost = temp -> vrijednost;
            root -> right = obrisi (root -> right, temp -> kljuc);
        }

    }

    // Visina i balans
    if (root == NULL)  
        return root; 
    root -> visina = max(dajVisinu(root->left), dajVisinu(root->right)) + 1;
    int balans = dajBalans(root);
    
    // Slucajevi za rotaciju
    
    if (balans > 1 && dajBalans(root -> left) >=0)
        return desnaRotacija(root);
    if (balans < -1 && dajBalans(root -> right) <=0)
        return lijevaRotacija(root);
        
    if (balans > 1 && dajBalans(root -> left) <=0) {
        root -> left = lijevaRotacija(root -> left);
        return desnaRotacija(root);
    }
    if (balans < -1 && dajBalans(root -> right) >=0) {
        root -> right = desnaRotacija(root -> right);
        return lijevaRotacija(root);
    }    
    
    return root;
}

template <typename Tip1, typename Tip2>
struct cvor {
    Tip1 kljuc;
    Tip2 vrijednost;
    cvor<Tip1,Tip2> *left;
    cvor<Tip1,Tip2> *right;
    cvor<Tip1,Tip2> *parent;
};

template <typename Tip1, typename Tip2>
class BinStabloMapa : public Mapa<Tip1, Tip2> { 
    int vel;
    cvor<Tip1, Tip2> *root;
    
    cvor<Tip1, Tip2>* findKey (const Tip1 &key) const;
    void dealociraj(cvor<Tip1, Tip2>*& root);
    cvor<Tip1, Tip2>* findMin (cvor<Tip1, Tip2>*& root) const;
    cvor<Tip1, Tip2>* obrisi (cvor<Tip1, Tip2>*& root, const Tip1 &key);
    void kopiraj (const BinStabloMapa<Tip1,Tip2> &m);
    
public:
    BinStabloMapa () : vel(0), root(nullptr) {};
    
    ~BinStabloMapa () override {
        obrisi();
    };
   
    BinStabloMapa (const BinStabloMapa<Tip1,Tip2> &m);
    BinStabloMapa<Tip1,Tip2> &operator = (const BinStabloMapa<Tip1,Tip2> &m);
   
    
    Tip2 &operator [] (const Tip1 key) override;    
    Tip2 operator [] (const Tip1 key) const override;
    
    void obrisi () override {
        dealociraj(root);
        root = nullptr;
        vel = 0;
    }
    
    void obrisi(const Tip1 &key) override {
        root = obrisi (root, key);
        vel--;
    }
    
    int brojElemenata() const override {
        return vel;
    }
    
};


template <typename Tip1, typename Tip2>
void BinStabloMapa<Tip1, Tip2>::kopiraj (const BinStabloMapa<Tip1,Tip2> &m) {
    std::queue<cvor<Tip1,Tip2>*> red;
    red.push(m.root);
    while (!red.empty()) {
        auto trenutni = red.front();
        (*this)[trenutni -> kljuc] = trenutni -> vrijednost;
        if (trenutni -> left != nullptr)
            red.push(trenutni -> left);
        if (trenutni -> right != nullptr)
            red.push(trenutni -> right);
        red.pop();
    }    
}

template <typename Tip1, typename Tip2>
BinStabloMapa<Tip1, Tip2>::BinStabloMapa (const BinStabloMapa<Tip1,Tip2> &m) : vel(0), root(nullptr) {
    if (m.root == nullptr)
        return;
    kopiraj(m);
}
template <typename Tip1, typename Tip2>
BinStabloMapa<Tip1,Tip2> &BinStabloMapa<Tip1, Tip2>::operator = (const BinStabloMapa<Tip1,Tip2> &m) {
    if (this != &m) {
        obrisi();
        kopiraj(m);
    }
    return *this;
}

template <typename Tip1, typename Tip2>
cvor<Tip1, Tip2>* BinStabloMapa<Tip1, Tip2>::findKey (const Tip1 &key) const {
    if (root == nullptr)
        return root;
    auto trenutni = root;
    auto prethodni = root;
    
    while (trenutni != nullptr && trenutni -> kljuc != key) {
        prethodni = trenutni;
        if (key < trenutni -> kljuc)
            trenutni = trenutni -> left;
        else 
            trenutni = trenutni -> right;
    }
    if (trenutni == nullptr)    // key nije pronadjen u stablu;
        return prethodni;
    return trenutni;            // key je pronadjen u stablu
}

template <typename Tip1, typename Tip2>
void BinStabloMapa<Tip1, Tip2>::dealociraj(cvor<Tip1, Tip2>*& root) {
    // PostOrder Traversal
    if (root == nullptr)
        return;
    dealociraj(root -> left);
    dealociraj(root -> right);
    delete root;
}

template <typename Tip1, typename Tip2>
Tip2 &BinStabloMapa<Tip1, Tip2>::operator [] (const Tip1 key) {
    auto place = findKey(key);
    if (place == nullptr) {  // Stablo prazno , dodajemo prvi element
        root = new cvor<Tip1,Tip2> {key, Tip2(), nullptr, nullptr, nullptr};
        place = root;
        vel++;
    }
    else if (place -> kljuc != key) {  // key se ne nalazi u stablu, dodajemo novi cvor
        if (key < place -> kljuc) {
            place -> left = new cvor<Tip1,Tip2> {key, Tip2(), nullptr, nullptr, place};
            place = place -> left;
        }
        else {
            place -> right = new cvor<Tip1,Tip2> {key, Tip2(), nullptr, nullptr, place};
            place = place -> right;
        }
        vel++;
    }
    return place -> vrijednost;
}   

template <typename Tip1, typename Tip2>
Tip2 BinStabloMapa<Tip1, Tip2>::operator [] (const Tip1 key) const {
    auto place = findKey(key);
    if (place != nullptr && place -> kljuc == key)
        return place -> vrijednost;
    return Tip2();
}

template <typename Tip1, typename Tip2>
cvor<Tip1, Tip2>* BinStabloMapa<Tip1, Tip2>::findMin (cvor<Tip1, Tip2>*& root) const { 
    auto trenutni = root;
    cvor<Tip1, Tip2>* prethodni = nullptr;
    while (trenutni != nullptr) {
        prethodni = trenutni;
        trenutni = trenutni -> left;
    }
    return prethodni;
}
 
template <typename Tip1, typename Tip2>
cvor<Tip1, Tip2>* BinStabloMapa<Tip1, Tip2>::obrisi(cvor<Tip1, Tip2>*& root, const Tip1 &key) {
    if (root == nullptr)
        return root;
    else if (key < root -> kljuc)
        root -> left = obrisi (root -> left, key);
    else if (key > root -> kljuc)
        root -> right = obrisi (root -> right, key);
    else { // Nadjen je element za brisanje
        // Nema djece 
        if (root -> left == nullptr && root -> right == nullptr) {
            delete root;
            root = nullptr;
        }
        // Ima samo lijevo dijete
        else if (root -> right == nullptr) {
            auto temp = root;
            root = root -> left;
            delete temp;
        }
        // Ima samo desno dijete
        else if (root -> left == nullptr) {
            auto temp = root;
            root = root -> right;
            delete temp;
        }
        // Ima dva dijeteta
        else {
            auto temp = findMin(root -> right);
            root -> kljuc = temp -> kljuc;
            root -> vrijednost = temp -> vrijednost;
            root -> right = obrisi (root -> right, temp -> kljuc);
        }
    }
    return root;
}

int main () {
    // TEST PERFORMANSI
    int vel(2000);

    AVLStabloMapa <int, int> m;
    clock_t vrijeme1 = clock();
    for (int i(0); i<vel; i++) 
        m[i] = i;
    for (int i(0); i>=-vel; i--)
        m[i] = i;
    for (int i(-vel); i<vel; i++)
        m.obrisi(i);
    clock_t vrijeme2  = clock();
    double AVLtime = (double)(vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);

    BinStabloMapa <int, int> n;
    clock_t vrijeme3 = clock();
    for (int i(0); i<vel; i++) 
        n[i] = i;
    for (int i(0); i>=-vel; i--)
        n[i] = i;
    for (int i(-vel); i<vel; i++)
        n.obrisi(i);
    clock_t vrijeme4  = clock();
    double Bintime = (double)(vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);

    std::cout << "Rezultati testa:\nAvlStablo: " <<AVLtime<<" s\n" << "BinStablo: " << Bintime << " s";

return 0;

}
