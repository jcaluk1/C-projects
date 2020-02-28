#include <iostream>
#include <cmath>

template <typename Tip>
class Iterator;


template <typename Tip>
class Lista {
public:
    Lista () {}
    virtual ~Lista(){};
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
struct cvor {
    Tip info;
    cvor *prethodni, *sljedeci;
};

// Dvostruka lista sa fiktivnim pocetkom i krajem 

template <typename Tip>
class DvostrukaLista : public Lista<Tip> {
    cvor<Tip> *p;
    cvor<Tip> *t; 
    cvor<Tip> *k;        // pocetak, tekuci , kraj
    int lduzina, dduzina;
    
    void alocirajPraznu () {
        dduzina = 0; lduzina = 0;
        p = new cvor<Tip>{ Tip{}, nullptr, nullptr };
        k = new cvor<Tip>{ Tip{}, p, nullptr }; 
         
        p -> sljedeci = k;
        t = p;
    }
    void dealociraj () {
        auto tmp = p;    
        while (tmp != nullptr) {
            tmp = tmp -> sljedeci;
            delete p;
            p = tmp;
        }
        lduzina=0; dduzina=0;
    }
    
    void iskopirajuPraznu (const DvostrukaLista<Tip> &lista) {
        // pocetak i kraj vec su alocirani, lista ima samo dva cvora;
        cvor<Tip> *q1 = lista.p -> sljedeci;
        cvor <Tip> *q2 = p;
        
        for (int i=0; i<lista.lduzina; i++) {               // q2 pok formira listu, q1 pok hoda po postojecoj
            q2 = new cvor<Tip> { q1 -> info, q2, nullptr};
            (q2 -> prethodni) -> sljedeci = q2;
            if (i == lista.lduzina-1)
                t = q2;
            q1 = q1 -> sljedeci;
                
        }
        // Spoji kraj
        q2 -> sljedeci = k;
        k -> prethodni = q2;
        lduzina = lista.lduzina;
        dduzina = lista.dduzina;
    }
public:
  
    DvostrukaLista () {
        alocirajPraznu();
    }
   
    ~DvostrukaLista () {
        dealociraj();
    }
    DvostrukaLista (const DvostrukaLista<Tip> &lista) {
        alocirajPraznu();
        iskopirajuPraznu(lista);
    }
    
    DvostrukaLista &operator= (const DvostrukaLista &lista) {
        if (this == &lista)
            return *this;
        dealociraj();
        alocirajPraznu();
        iskopirajuPraznu(lista);
        return *this;
    }
    
    
    
     
    int brojElemenata() const {
        return lduzina+dduzina;        
    }

    Tip &trenutni () {
        if (lduzina + dduzina == 0)
            throw std::logic_error ("Lista je prazna");
        return t -> info;
    }
    
    Tip trenutni () const {
        if (lduzina + dduzina == 0)
            throw std::logic_error ("Lista je prazna");
        return t -> info;
    }
    
    bool prethodni() {
        if (lduzina + dduzina == 0)
            throw std::logic_error ("Lista je prazna");
        if (t -> prethodni != p) {
            t = t -> prethodni;
            lduzina--; dduzina++;
            return true;
        }
        return false;
    }
    
    bool sljedeci () {
        if (lduzina + dduzina == 0)
            throw std::logic_error ("Lista je prazna");
        if ( t -> sljedeci != k) {
            t = t -> sljedeci;
            dduzina--; lduzina++;
            return true;
        }
        return false;
    }
    
    void pocetak () {
        if (lduzina + dduzina == 0)
            throw std::logic_error ("Lista je prazna");
        t = p -> sljedeci;
    }
    
    void kraj () {
        if (lduzina + dduzina == 0)
            throw std::logic_error ("Lista je prazna");
        t = k -> prethodni;
    }
    
    void obrisi () {
        if (lduzina+dduzina == 0)
            return;
        
        cvor<Tip> *temp;
        if (t -> sljedeci == k) {
            temp = t -> prethodni;
            lduzina--;
        }
        else {
            temp = t -> sljedeci;
            dduzina--;
        }
            
        (t -> prethodni) -> sljedeci = t -> sljedeci;
        (t -> sljedeci) -> prethodni = t -> prethodni;
        delete t;
        t = temp;
    }

     void dodajIspred (const Tip &elem) {
        if (t == p)
            t = k;
        auto novi = new cvor <Tip>{ elem, t -> prethodni, t };
        (t -> prethodni) -> sljedeci = novi;
        t -> prethodni = novi;
        if (t == k)
            t = t -> prethodni;
        lduzina++;
     }
     
     void dodajIza (const Tip &elem) {
         auto novi = new cvor<Tip>{ elem, t, t -> sljedeci };
         (t -> sljedeci) -> prethodni = novi;
         t -> sljedeci = novi;
         if (t != p) 
             dduzina++;
         else {
             lduzina++;
             t = t -> sljedeci;
         }
     }
     
     Tip &operator[] (int i) {
        if ( i<0 || i >= lduzina+dduzina)
            throw std::logic_error("Indeks van okvira");
        
        cvor<Tip> *temp = nullptr;
        int brKoraka = 0;
        
        //Potreban broj koraka
        if (i+1 <= std::abs(i+1-lduzina)) { // od pocetka
            temp = p;
            brKoraka = i+1;
        }
        else if (std::abs(i+1-lduzina) <= std::abs(i-lduzina-dduzina)) { // od tekuceg
            temp = t;
            brKoraka = i+1-lduzina;
        }
        else {                             // od kraja
            temp = k;
            brKoraka = i-lduzina-dduzina;
        }
        
        if (brKoraka>=0) 
            for (int j=0; j<brKoraka; j++)
                temp = temp -> sljedeci;
        
        else
            for (int j=0; j<(-brKoraka); j++)
                temp = temp -> prethodni;
        
        return temp -> info;
    }

     Tip operator[] (int i) const {
        if ( i<0 || i >= lduzina+dduzina)
            throw std::logic_error("Indeks van okvira");
        
        cvor<Tip> *temp = nullptr;
        int brKoraka = 0;
        
        //Potreban broj koraka
        if (i+1 <= std::abs(i+1-lduzina)) { // od pocetka
            temp = p;
            brKoraka = i+1;
        }
        else if (std::abs(i+1-lduzina) <= std::abs(i-lduzina-dduzina)) { // od tekuceg
            temp = t;
            brKoraka = i+1-lduzina;
        }
        else {                             // od kraja
            temp = k;
            brKoraka = i-lduzina-dduzina;
        }
        
        if (brKoraka>=0) 
            for (int j=0; j<brKoraka; j++)
                temp = temp -> sljedeci;
        
        else
            for (int j=0; j<(-brKoraka); j++)
                temp = temp -> prethodni;
        
        return temp -> info;
    }    
    
    friend class Iterator<Tip>;
 
};

template <typename Tip>
class Iterator {
    
    const cvor<Tip> *it_t;
    const cvor<Tip> *it_p;
    const cvor<Tip> *it_k;
    const DvostrukaLista<Tip> *lista;
    
        
public:
    Iterator (const Lista<Tip> &l) {
        lista = dynamic_cast<const DvostrukaLista<Tip>*>(&l);
        it_p = lista -> p;
        it_k = lista -> k;
        it_t = it_p -> sljedeci;
    }
    const cvor<Tip>* kraj_pok () {return it_k;};
    const cvor<Tip>* trenutni_pok () {return it_t;};
    
    Tip trenutni() const {
        return it_t -> info;
    }
    
    void sljedeci () {
        it_t = it_t -> sljedeci;
    }
    
    
};

template <typename Tip>
Tip dajMaksimum (const Lista<Tip> &n) {
    Iterator<Tip> it(n);
    if (n.brojElemenata() == 0)
        throw std::logic_error("Lista prazna");
        
    Tip max = it.trenutni();
    it.sljedeci();
    while (it.trenutni_pok() != it.kraj_pok()) {
        if (it.trenutni() > max)
            max = it.trenutni();
        it.sljedeci();
    }
    return max;
}

bool test_brojElemenata () {
    DvostrukaLista<int> l1;
   
    // 20 elemenata
    for (int i=0; i<20; i++) {
        l1.dodajIspred(i);
    }
    return l1.brojElemenata() == 20;
}

bool test_dodajIspred_dodajIza_pocetak_kraj () {
    DvostrukaLista<int> l1;
        // 20
    for (int i=0; i<10; i++) {
        l1.dodajIspred(i);
        l1.dodajIza(i);
    }
    l1.pocetak();
    bool t1,t2; 
    t1 = (l1.trenutni()==1) ? true : false;
    
    l1.kraj();
    t2 = (l1.trenutni()==0) ? true : false;
    
    return t1 && t2;
}

bool test_obrisi () {
    DvostrukaLista<int> l1;
    for (int i=0; i<10 ;i++) {
        l1.dodajIza(i);
    }
    
    l1.obrisi();
    l1.kraj();
    
    for (int i=0; i<8; i++) {
        l1.obrisi();
    }
    return l1.trenutni() == 9;
    
}

bool test_indeksiranja () {
    DvostrukaLista<int> l1;
    for (int i=0; i<10 ;i++) {
        l1.dodajIspred(i);
    }
    return l1[8] == 9;
}

bool test_dajMax() {
    DvostrukaLista<int> l1;
    for (int i=0; i<1000; i++)
        l1.dodajIspred(1);
    l1[562] = 15;
    return dajMaksimum(l1) == 15;
    
}

int main() {
    if (test_brojElemenata() && test_dodajIspred_dodajIza_pocetak_kraj() && test_indeksiranja() && test_obrisi() && test_dajMax())
        std::cout << "SVE OK";
    else 
        std::cout << "NOT OK";
    return 0;
}
