#include <iostream>

template <typename Tip>
class Lista {
  
public:
    Lista () {};
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    
    virtual Tip &trenutni () = 0;
    virtual Tip trenutni () const = 0;
    
    
    virtual bool prethodni() = 0;
    virtual bool sljedeci () = 0;
    
    virtual void pocetak () = 0;
    virtual void kraj () = 0;
    
    virtual void obrisi () = 0;
    
    virtual void dodajIspred (const Tip &elem) = 0;
    virtual void dodajIza (const Tip &elem) = 0;
    
    virtual Tip &operator[](int i) = 0;
    virtual Tip operator[](int i) const = 0;
    
    
};

template <typename Tip>
class NizLista : public Lista<Tip> {
    int kapacitet, velicina, tekuci;
    Tip *L;
    void listaPrazna () const {
        if (velicina==0)
            throw std::logic_error("Lista je prazna!");
    }
    void alocirajDuplo() {
        Tip* novi = new Tip[2*kapacitet];
        for (int i=0; i<velicina; i++)
            novi[i] = L[i];
        // Brisanje starog
        delete[] L;
        kapacitet = 2*kapacitet;
        L = novi;
    }
    
public:
    
    
    // Konstruktori
    NizLista() : kapacitet(10), velicina(0), tekuci(0), L(new Tip[10])  {}

    NizLista(const NizLista<int> &lista) {
        Tip* novi = nullptr;
        try {
            novi = new Tip[lista.kapacitet];
            for (int i=0; i<lista.velicina; i++)
                novi[i] = lista.L[i];
        } catch (std::bad_alloc) {
            throw "Nema dovoljno memorije";
        }
        L = novi; velicina = lista.velicina; kapacitet = lista.kapacitet; tekuci = lista.tekuci;
        
    }
    NizLista &operator= (const NizLista<int> lista) {
        Tip* novi = nullptr;
        try {
            novi = new Tip[lista.kapacitet];
            for (int i=0; i<lista.velicina; i++)
                novi[i] = lista.L[i];
        } catch (std::bad_alloc) {
            throw "Nema dovoljno memorije";
        }
        delete[] L;
        L = novi; velicina = lista.velicina; kapacitet = lista.kapacitet; tekuci = lista.tekuci;
        return *this;
    }
    // Destruktor
    ~NizLista() {delete[] L; L = nullptr;}
    
    // brojElemenata()
    int brojElemenata() const {return velicina;}
    
    // trenutni(), prethodni(), sljedeci(), pocetak(), kraj()
    Tip &trenutni() {
        listaPrazna();
        return L[tekuci];
    }
    
    Tip trenutni() const {
        listaPrazna();
        return L[tekuci];
    }
    

    bool prethodni() {
        listaPrazna();
        if (tekuci == 0)
            return false;
        tekuci--;
        return true;
    }
    
    bool sljedeci () {
        listaPrazna();
        if (tekuci == velicina-1)
            return false;
        tekuci++;
        return true;
    }
    
    void pocetak () {
        listaPrazna();
        tekuci = 0;
    }

    void kraj () {
        listaPrazna();
        tekuci = velicina - 1;
    }
    
    // obrisi()
    void obrisi () {
        if (velicina <= 0) return;
        
        for (int i=tekuci; i < velicina -1 ; ++i)
            L[i] = L[i+1];
        if (tekuci != 0 && tekuci == velicina -1)
            tekuci--;
        velicina--;
    }
    
    // dodajISpred(), dodajIza()
    
    void dodajIza(const Tip &elem) {
            if (velicina == kapacitet) 
                alocirajDuplo();
            
            for (int i=velicina-1; i > tekuci; --i)  // ako je velicina nula petlja se nece izvrsiti, tekuci se ne pomijera
                L[i+1] = L[i];
            if (velicina == 0)
                L[tekuci] = elem;
            else 
                L[tekuci+1] = elem;
            velicina++;
        
    }

    void dodajIspred(const Tip &elem) {
            if (velicina == kapacitet) 
                alocirajDuplo();
            
            for (int i=velicina-1; i >= tekuci; --i)  // ako je velicina nula petlja se nece izvrsiti , tekuci se pomijera
                L[i+1] = L[i];
            L[tekuci] = elem;
            if (velicina != 0)
                tekuci++;
            velicina++;
        
    }
    
    Tip &operator[] (int i) {
        if (i<0 || i >= velicina)
            throw std::range_error("Indeks van okvira");
        return L[i];
    }
    
    Tip operator[] (int i) const {
        if (i<0 || i >= velicina)
        throw std::range_error("Indeks van okvira");
        return L[i];
    }
  
};


template <typename Tip>
struct cvor {
    Tip info;
    cvor* sljedeci;
};

template <typename Tip>
class JednostrukaLista : public Lista<Tip> {
    cvor<Tip>* begin;
    cvor<Tip>* tekuci;
    cvor<Tip>* end;
    int lduzina, dduzina;
    
    void dealociraj () {
        auto p = begin;
        while (p != nullptr) {
            p = p -> sljedeci;
            delete begin;
            begin = p;
        }
        begin = tekuci = end = nullptr;
    }

    void listaPrazna () const {
        if (lduzina + dduzina == 0)
            throw std::logic_error("Lista je prazna!");
    } 
    
public:

    JednostrukaLista () {
        begin = end = tekuci = nullptr;
        lduzina = dduzina = 0;
    }

    ~JednostrukaLista() {
        dealociraj();
    }
    
    JednostrukaLista(const JednostrukaLista &lista);
    JednostrukaLista &operator = (const JednostrukaLista &lista);
    
    cvor<Tip>* getBegin () const {return begin;}
    cvor<Tip>* getTekuci () const {return tekuci;}
    int getLduzina () const {return lduzina;}
    int getDduzina() const {return dduzina;}
    
    
    
    int brojElemenata() const {return lduzina + dduzina;}
    
    Tip &trenutni () {
        listaPrazna();
        return tekuci -> info;
    }
    
    Tip trenutni() const {
        listaPrazna();
        return tekuci -> info;
    }
        
    bool prethodni() {
        listaPrazna();
        if (lduzina > 1) {
            auto p = begin;
            while (p -> sljedeci != tekuci)
                p = p -> sljedeci;
            tekuci = p;
            lduzina--; dduzina++;
            return true;
        }
        return false;
    }
   
    bool sljedeci () {
        listaPrazna();
        if (dduzina > 0) {
            tekuci = tekuci -> sljedeci;
            dduzina--; lduzina++;
            return true;
        }
        return false;
    }

    void pocetak () {
        listaPrazna();
        tekuci = begin;
        dduzina = lduzina + dduzina - 1;
        lduzina = 1;
    }
    
    void kraj () {
        listaPrazna();
        tekuci = end;
        lduzina = lduzina+dduzina;
        dduzina = 0;
    }
    
     void obrisi () {
         listaPrazna();
         auto p = begin;
         
         while (p != tekuci && (p -> sljedeci != tekuci)) 
         
            p = p->sljedeci;
             
         
            
         if (lduzina + dduzina == 1) {    // samo jedan element
             delete tekuci;
             begin = tekuci = end = nullptr;
             lduzina --;
         }
         else if (lduzina == 1) {   // element na pocetku
             tekuci = tekuci -> sljedeci;
             delete begin;
             begin = tekuci;
             dduzina--;
         }
         else if (dduzina == 0) { // element na kraju
            delete end;
            tekuci = end = p;
            end -> sljedeci = nullptr;
            lduzina--;
             
         }
         else {
             p -> sljedeci = tekuci -> sljedeci;
             delete tekuci;
             tekuci = p -> sljedeci;
             dduzina--;
         }
         
         
     }
    
    void dodajIza (const Tip &elem) {
        auto p = new cvor<Tip>{elem, nullptr};
        if (lduzina + dduzina == 0) { // prvi element
            begin = end = tekuci = p;
            lduzina++;
        }
        else if (tekuci == end) { //  tekuci na kraju liste 
            end = p;
            tekuci -> sljedeci = p; 
            dduzina++;
        }
        else {
            p -> sljedeci = tekuci -> sljedeci;
            tekuci -> sljedeci = p;
            dduzina++;
        }

    }
     void dodajIspred (const Tip &elem) {
        auto p = new cvor<Tip>{elem, nullptr};
        if (lduzina + dduzina == 0) { // prvi element
            begin = end = tekuci = p;
            lduzina++;
        }
        else if (lduzina == 1) { // tekuci na pocetku, pomjeramo begin
            begin = p;
            begin -> sljedeci = tekuci;
            lduzina++;
        }
        else {
            auto temp = begin;
            while (temp -> sljedeci != tekuci)
                temp = temp->sljedeci;
            temp -> sljedeci = p;
            p -> sljedeci = tekuci;
            lduzina++;
        }
    }
    
    Tip &operator[](int i) {
        if (i<0 || i >= lduzina+dduzina)
            throw std::range_error("Indeks van okvira");
        
        auto p = begin;
        int j = 0;
        for (; j < i; ++j)
            p = p->sljedeci;
        return p -> info;
    }
   
    Tip operator[](int i) const {
        if (i<0 || i >= lduzina+dduzina)
            throw std::range_error("Indeks van okvira");
        
        auto p = begin;
        int j = 0;
        for (; j < i; ++j)
            p = p->sljedeci;
        return p -> info;
    }
    // ------------------------------
    template<typename Tip2>
    friend void izbaciNajveciBroj(JednostrukaLista<Tip2> &lista);
};

// Kopy konstruktor i asigment operator
template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista (const JednostrukaLista<Tip> &lista) {
    auto p = lista.getBegin();
    cvor<int>* tTekuci = nullptr;
    begin = end = tekuci = nullptr;
    
    while (p != nullptr) {
        auto novi = new cvor<Tip>{p->info, nullptr};
        if (begin == nullptr) 
            begin =  novi;
        else 
            tekuci -> sljedeci = novi;
        tekuci = novi;
        p = p->sljedeci;
        
        if (p == lista.getTekuci())    // za kopiranje tekuceg elementa
            tTekuci = tekuci;
    }
    end = tekuci;
    tekuci = tTekuci;
    lduzina = lista.getLduzina();
    dduzina = lista.getDduzina();
}

template <typename Tip>
JednostrukaLista<Tip> &JednostrukaLista<Tip>::operator = (const JednostrukaLista<Tip> &lista) {
    // Dealociranje
    auto p = begin;    
    while (p != nullptr) {
        p = p -> sljedeci;
        delete begin;
        begin = p;
    }
    
    // Kopiranje
    p = lista.getBegin();
    cvor<int>* tTekuci = nullptr;
    begin = end = tekuci = nullptr;
    
    while (p != nullptr) {
        auto novi = new cvor<Tip>{p->info, nullptr};
        if (begin == nullptr) 
            begin =  novi;
        else 
            tekuci -> sljedeci = novi;
        
        tekuci = novi;
        p = p->sljedeci;
        
        if (p == lista.getTekuci())    // za kopiranje tekuceg elementa
            tTekuci = tekuci;
    }
    end = tekuci;
    tekuci = tTekuci;
    lduzina = lista.getLduzina();
    dduzina = lista.getDduzina();
    
    return *this;
}

bool test_brojElemenata () {
    NizLista<int> l1;
    JednostrukaLista <int> l2;
    // 20 elemenata
    for (int i=0; i<20; i++) {
        l1.dodajIspred(i);
        l2.dodajIspred(i);
    }
    return l1.brojElemenata() == 20 && l2.brojElemenata() == 20;
}

bool test_dodajIspred_dodajIza_pocetak_kraj () {
    NizLista<int> l1;
    JednostrukaLista <int> l2;
        // 20
    for (int i=0; i<10; i++) {
        l1.dodajIspred(i);
        l1.dodajIza(i);
        l2.dodajIspred(i);
        l2.dodajIza(i);
    }
    l1.pocetak();
    l2.pocetak();
    bool t1,t2; 
    t1 = (l1.trenutni()==1 && l2.trenutni()==1) ? true : false;
    
    l1.kraj();
    l2.kraj();
    t2 = (l1.trenutni()==0 && l2.trenutni()==0) ? true : false;
    
    return t1 && t2;
}

bool test_obrisi () {
    NizLista<int> l1;
    JednostrukaLista <int> l2;
    for (int i=0; i<10 ;i++) {
        l1.dodajIza(i);
        l2.dodajIza(i);
    }
    
    l1.obrisi(); l2.obrisi();
    l1.kraj(); l2.kraj();
    
    for (int i=0; i<8; i++) {
        l1.obrisi(); l2.obrisi();
    }
    return l1.trenutni() == 9 && l2.trenutni() == 9;
    
}

bool test_indeksiranja () {
    NizLista<int> l1;
    JednostrukaLista <int> l2;
    for (int i=0; i<10 ;i++) {
        l1.dodajIspred(i);
        l2.dodajIspred(i);
    }
    return l1[8] == 9 && l2[8] == 9;
}
/*
template <typename Tip>
bool cvorJeCifra (struct cvor<Tip> *cvorPok) {
    return (cvorPok -> info >= '0' && cvorPok -> info <= '9');
}

template<typename Tip>
void izbaciNajveciBroj(JednostrukaLista<Tip> &lista) {
    
    if (lista.begin == lista.end)
        return;

    cvor<Tip> *trenutni = lista.begin;
    cvor<Tip> *prethodni = nullptr;
    
    cvor<Tip> *pokPrijeBroj = nullptr, 
         *pokPrijeMax = nullptr;
         
    bool imaBaremJedanBroj = false;
    bool jePocetakBroja = true;
    
    int broj = 0, max = 0;
   
    while (trenutni != nullptr) {
            
        if (cvorJeCifra(trenutni)) {
            imaBaremJedanBroj = true;
            
            if (jePocetakBroja)
                pokPrijeBroj = prethodni;
            jePocetakBroja = false;
            broj = 10*broj + trenutni -> info - '0';
        }
        
        else {
            if (imaBaremJedanBroj) {
                 if (broj >= max) {
                     max = broj;
                     pokPrijeMax = pokPrijeBroj;
                 }
            }
            jePocetakBroja = true;
            broj = 0;
        }
        
        
        prethodni = trenutni;
        trenutni = trenutni -> sljedeci;
    }
    
    if (imaBaremJedanBroj) 
        if (broj >= max) {
             max = broj;
             pokPrijeMax = pokPrijeBroj;
        }
    
    if (imaBaremJedanBroj) {
        cvor<Tip> *p = (pokPrijeMax == nullptr) ? lista.begin : pokPrijeMax->sljedeci;
        cvor<Tip> *q = p;
        
        while (p != nullptr && cvorJeCifra(p)) {
            p = p -> sljedeci;
            delete q;
            q = p;
        }
        
        // Spajanje liste
        if (pokPrijeMax == nullptr && q == nullptr)        // samo jedan broj
            lista.begin = lista.end = nullptr;
        else if (pokPrijeMax == nullptr) {      // broj na pocetku
            lista.begin = q;
            lista.begin -> sljedeci = q -> sljedeci;
        }    
        else if (q == nullptr) {                // broj je bio na kraju
            lista.end = pokPrijeMax;
            lista.end -> sljedeci = nullptr;
        }
        else {                                  // svi ostali slucajevi
            pokPrijeMax -> sljedeci = q;
        }
    }
}
*/

int main() {
/*
    if (test_brojElemenata() && test_dodajIspred_dodajIza_pocetak_kraj() && test_indeksiranja() && test_obrisi())
        std::cout << "SVE OK";
    else 
        std::cout << "NOT OK";
        
*/

    JednostrukaLista<char> lista;
    
    lista.dodajIza('2');
    lista.sljedeci();
    
    lista.dodajIza('5');
    lista.sljedeci();
    
    lista.dodajIza('5');
    lista.sljedeci();
        
    lista.dodajIza('2');
    lista.sljedeci();

    lista.dodajIza('2');
    lista.sljedeci();

    lista.dodajIza('2');
    lista.sljedeci();

    lista.dodajIza('3');
    lista.sljedeci();

    lista.dodajIza('3');
    lista.sljedeci();

    lista.dodajIza('4');
    lista.sljedeci();

    lista.dodajIza('g');
    lista.sljedeci();
        
    for (int i=0; i< lista.brojElemenata(); i++)
        std::cout<<lista[i];
    
    std::cout<<"\n----------------\n";
    
    izbaciNajveciBroj(lista);
    for (int i=0; i< lista.brojElemenata()-9; i++)
        std::cout<<lista[i];
    
    
    return 0;
}

