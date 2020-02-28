//TP 2018/2019: Zadaća 5, Zadatak 2
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <iomanip>
#include <iterator>

class Berza {
  std::vector<int> cijene;
  int min_dozvoljena_cijena, max_dozvoljena_cijena;
  
public:
    Berza (int min, int max);
    explicit Berza (int max);
    
    void RegistrirajCijenu (int cijena);
    
    int DajBrojRegistriranihCijena() {return cijene.size();}
    void BrisiSve () {cijene.clear();}
    int DajMinimalnuCijenu () const;
    int DajMaksimalnuCijenu () const;
    bool operator ! () const;
    int DajBrojCijenaVecihOd (int cijena) const;
    void Ispisi () const;
    int operator[] (int indeks) const;
    
    Berza &operator++ ();
    Berza &operator-- ();
    
    Berza operator++ (int);
    Berza operator-- (int);
    
    Berza operator- () const;
    
    Berza &operator += (int cijena);
    Berza &operator -= (int cijena);
    Berza &operator -= (const Berza &berza);
    
    friend Berza operator+(const Berza &b, int cijena);
    friend Berza operator+(int cijena, const Berza &b);
    
    friend Berza operator-(const Berza &b, int cijena);
    friend Berza operator-(int cijena, const Berza &b);
    friend Berza operator-(const Berza &b1, const Berza &b2);
    
    friend bool operator == (const Berza &b1, const Berza &b2);
    friend bool operator != (const Berza &b1, const Berza &b2);
};

Berza::Berza(int min, int max) {
    if (min < 0 || max < 0 || min > max)
        throw std::range_error("Ilegalne granice cijene");
    min_dozvoljena_cijena = min; max_dozvoljena_cijena = max;
}

Berza::Berza(int max) {
    if (max < 0)
        throw std::range_error("Ilegalne granice cijene");
    min_dozvoljena_cijena = 0; max_dozvoljena_cijena = max;
}

void Berza::RegistrirajCijenu(int cijena) {
    if (cijena < min_dozvoljena_cijena || cijena > max_dozvoljena_cijena)
        throw std::range_error("Ilegalna cijena");
    cijene.push_back(cijena);
}

int Berza::DajMaksimalnuCijenu () const {
    if (cijene.size() == 0)
        throw std::range_error("Nema registriranih cijena");
    return *std::max_element(cijene.begin(), cijene.end());
}

int Berza::DajMinimalnuCijenu () const {
    if (cijene.size() == 0)
        throw std::range_error("Nema registriranih cijena");
    return *std::min_element(cijene.begin(), cijene.end());
}

bool Berza::operator ! () const {
    return cijene.size() == 0;
}

int Berza::DajBrojCijenaVecihOd (int cijena) const {
    if (cijene.size() == 0)
        throw std::range_error("Nema registriranih cijena");
    return std::count_if(cijene.begin(), cijene.end(), std::bind(std::greater<int> (), std::placeholders::_1, cijena));
}

void Berza::Ispisi () const {
    std::vector<int> cijene2 (cijene);
    std::sort(cijene2.begin(), cijene2.end(), std::greater<int> ());
    std::cout << std::fixed << std::setprecision(2);
    std::transform(cijene2.begin(), cijene2.end(), std::ostream_iterator<double>(std::cout,"\n"), 
        std::bind(std::divides<double>(), std::placeholders::_1, 100.));
}

int Berza::operator[] (int indeks) const {
    if (indeks < 1 || indeks > cijene.size())
        throw std::range_error("Neispravan indeks");
    return cijene[indeks-1];
}

Berza &Berza::operator++ () {
        if (DajMaksimalnuCijenu() + 100 > max_dozvoljena_cijena)
            throw std::range_error("Prekoracen dozvoljeni opseg cijena");
        std::transform(cijene.begin(), cijene.end(), cijene.begin(), std::bind(std::plus<int>(), std::placeholders::_1, 100));
        return *this;
}

Berza &Berza::operator-- () {
        if (DajMinimalnuCijenu() - 100 < min_dozvoljena_cijena)
            throw std::range_error("Prekoracen dozvoljeni opseg cijena");
        std::transform(cijene.begin(), cijene.end(), cijene.begin(), std::bind(std::minus<int>(), std::placeholders::_1, 100));
        return *this;
}

Berza Berza::operator++ (int) {
        if (DajMaksimalnuCijenu() + 100 > max_dozvoljena_cijena)
            throw std::range_error("Prekoracen dozvoljeni opseg cijena");
        Berza pomocni(*this);
        std::transform(cijene.begin(), cijene.end(), cijene.begin(), std::bind(std::plus<int>(), std::placeholders::_1, 100));
        return pomocni;
}

Berza Berza::operator-- (int) {
        if (DajMinimalnuCijenu() - 100 < min_dozvoljena_cijena)
            throw std::range_error("Prekoracen dozvoljeni opseg cijena");
        Berza pomocni(*this);
        std::transform(cijene.begin(), cijene.end(), cijene.begin(), std::bind(std::minus<int>(), std::placeholders::_1, 100));
        return pomocni;
}

Berza Berza:: operator - () const {
    Berza vrati (*this);
    std::transform(vrati.cijene.begin(), vrati.cijene.end(), vrati.cijene.begin() , 
            std::bind(std::minus<int>(), (max_dozvoljena_cijena+min_dozvoljena_cijena), std::placeholders::_1));
    return vrati;
}

Berza &Berza::operator += (int cijena) {
    if (DajMaksimalnuCijenu() + cijena > max_dozvoljena_cijena || DajMinimalnuCijenu() + cijena < min_dozvoljena_cijena)
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    std::transform(cijene.begin(), cijene.end(), cijene.begin(), std::bind(std::plus<int>(), std::placeholders::_1, cijena));
    return *this;
}

Berza &Berza::operator -= (int cijena) {
    if (DajMaksimalnuCijenu() - cijena > max_dozvoljena_cijena || DajMinimalnuCijenu() - cijena < min_dozvoljena_cijena)
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    std::transform(cijene.begin(), cijene.end(), cijene.begin(), std::bind(std::minus<int>(), std::placeholders::_1, cijena));
    return *this;
}

Berza &Berza::operator -= (const Berza &b) {
    if (max_dozvoljena_cijena != b.max_dozvoljena_cijena || min_dozvoljena_cijena != b.min_dozvoljena_cijena || cijene.size() != b.cijene.size())
        throw std::domain_error("Nesaglasni operandi");
    
    std::vector<int> razlike_cijena (cijene.size());
    std::transform(cijene.begin(), cijene.end(), b.cijene.begin(), razlike_cijena.begin(), std::minus<int> ());
    
    if (*std::min_element(razlike_cijena.begin(), razlike_cijena.end()) < min_dozvoljena_cijena)
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    cijene = std::move(razlike_cijena);
    return *this;
}

inline Berza operator+(const Berza &b, int cijena) {
     Berza b1 = b; b1 += cijena; return b1;
 }

inline Berza operator+(int cijena, const Berza &b) {
    return b+cijena;   
} 

inline Berza operator-(const Berza &b, int cijena) {
    Berza b1 = b; b1 -= cijena; return b1;
}

Berza operator-(int cijena, const Berza &b) {
    Berza b2 = b;
    std::fill(b2.cijene.begin(), b2.cijene.end(), cijena);
    return b2 -= b;
}

inline Berza operator-(const Berza &b1, const Berza &b2) {
    Berza b3 = b1; b3 -= b2; return b3;
}

bool operator == (const Berza &b1, const Berza &b2) {
    if (b1.max_dozvoljena_cijena != b2.max_dozvoljena_cijena || b1.min_dozvoljena_cijena != b2.min_dozvoljena_cijena || b1.cijene.size() != b2.cijene.size())
        return false;
    return b1.cijene == b2.cijene;
}

inline bool operator != (const Berza &b1, const Berza &b2) {
    return !(b1 == b2);
}

int main ()
{
    //AT 4 - [javni] konstruktor, metode RegistrirajCijenu, Ispisi, BrisiSve
    Berza b1 = Berza(1000,4000);
    b1.RegistrirajCijenu(3000);
    b1.RegistrirajCijenu(1500);
    b1.RegistrirajCijenu(2000);
    std::cout << "Broj registrovanih cijena: " << b1.DajBrojRegistriranihCijena() << std::endl;
    b1.Ispisi();
    b1.BrisiSve();
    std::cout << "Broj registrovanih cijena: " << b1.DajBrojRegistriranihCijena();
	return 0;
}