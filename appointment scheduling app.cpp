//TP 2018/2019: Zadaća 4, Zadatak 3
#include <iostream>
#include <tuple>
#include <iomanip>
#include <string>
#include <algorithm>

class Datum {
    int dan, mjesec, godina;
    static void TestDatum(int d, int m, int g);
public :
    Datum(int dan, int mjesec, int godina);
    void Postavi(int dan, int mjesec, int godina);
    std::tuple<int, int, int> Ocitaj() const;
    void Ispisi() const;
};

void Datum::TestDatum(int d, int m, int g) {
    int broj_dana[12] {31,28,31,30,31,30,31,31,30,31,30,31};
    if (g % 4 == 0 && g % 100 != 0 || g % 400 == 0 )
            broj_dana[1]++;
    if (g < 0  || d < 1 || m < 1 || m > 12  || d > broj_dana[m-1])
        throw std::domain_error("Neispravan datum");
}
// Javne metode - Datum
Datum::Datum (int dan, int mjesec, int godina) {
    Postavi(dan,mjesec,godina);
}
void Datum::Postavi (int dan, int mjesec, int godina) {
    TestDatum(dan,mjesec,godina);                                               // Baca izuzetak za datum
    Datum::dan = dan;
    Datum::mjesec = mjesec;
    Datum::godina = godina;
}
std::tuple<int, int, int> Datum::Ocitaj() const {
    return {dan,mjesec,godina};
}
void Datum::Ispisi() const {
    std::cout<<dan<<"/"<<mjesec<<"/"<<godina;
}

class Vrijeme {
    int hh, mm;
    static void TestVrijeme (int sati, int minute);
public :
    Vrijeme(int sati, int minute);
    void Postavi(int sati, int minute);
    std::pair<int, int> Ocitaj () const;
    void Ispisi() const;
};

void Vrijeme::TestVrijeme (int sati, int minute) {
    if (sati < 0 || sati > 23 || minute < 0 || minute > 59)
        throw std::domain_error("Neispravno vrijeme");
}
Vrijeme::Vrijeme(int sati, int minute) {
    Postavi (sati, minute);
}

void Vrijeme::Postavi(int sati, int minute) {
    TestVrijeme(sati, minute);                                                  // Vrijeme baca izuzetak;
    hh = sati; 
    mm = minute;
}
std::pair<int, int> Vrijeme::Ocitaj () const {
    return {hh,mm};
}
void Vrijeme::Ispisi() const {
    std::cout<<std::setprecision(2)<<std::setw(2)<<std::setfill('0')<<hh<<":"
             <<std::setw(2)<<mm;
}

class Pregled {
    std::string ime;
    Datum datum;
    Vrijeme vrijeme;
public :
    Pregled(const std::string &ime_pacijenta, const Datum &datum_pregleda, const Vrijeme &vrijeme_pregleda);
    Pregled(const std::string &ime_pacijenta, int dan_pregleda, int mjesec_pregleda, int godina_pregleda, int sati_pregleda, int minute_pregleda);
    void PromijeniPacijenta(const std::string &ime_pacijenta);
    void PromijeniDatum(const Datum &novi_datum);
    void PromijeniVrijeme(const Vrijeme &novo_vrijeme);
    void PomjeriDanUnaprijed();
    void PomjeriDanUnazad();
    std::string DajImePacijenta() const {
        return ime;
    }
    Datum DajDatumPregleda() const {
        return datum;
    }
    Vrijeme DajVrijemePregleda() const {
        return vrijeme;
    }
    static bool DolaziPrije(const Pregled &p1, const Pregled &p2);

    void Ispisi() const {
        std::cout<<std::left<<std::setw(30)<<std::setfill(' ')<<ime<<std::right; 
        datum.Ispisi(); std::cout<<" "; vrijeme.Ispisi(); std::cout<<std::endl;
    }
};
    Pregled::Pregled (const std::string &ime_pacijenta, const Datum &datum_pregleda, const Vrijeme &vrijeme_pregleda) : ime(ime_pacijenta), 
        datum(datum_pregleda), vrijeme(vrijeme_pregleda) {}                     // Default coppy konstruktor
    
    Pregled::Pregled (const std::string &ime_pacijenta, int dan_pregleda, int mjesec_pregleda, int godina_pregleda, int sati_pregleda, int minute_pregleda) :
        ime(ime_pacijenta), datum(dan_pregleda,mjesec_pregleda,godina_pregleda), vrijeme(sati_pregleda, minute_pregleda) {}
    
    void Pregled::PromijeniPacijenta(const std::string &ime_pacijenta) {
        ime = ime_pacijenta;
    }
    
    void Pregled::PromijeniDatum (const Datum &novi_datum) {
        int dan, mjesec, godina;
        std::tie(dan,mjesec,godina) = novi_datum.Ocitaj();
        datum.Postavi(dan,mjesec,godina);
    }
    
    void Pregled::PromijeniVrijeme (const Vrijeme &novo_vrijeme) {
        int sati, minute;
        std::tie(sati,minute) = novo_vrijeme.Ocitaj();
        vrijeme.Postavi(sati,minute);
    }   
    void Pregled::PomjeriDanUnaprijed() {
        int d, m, g;
        std::tie(d,m,g) = datum.Ocitaj();
        int broj_dana[12] {31,28,31,30,31,30,31,31,30,31,30,31};
        if (g % 4 == 0 && g % 100 != 0 || g % 400 == 0 )
            broj_dana[1]++;
        d++;
        if (d > broj_dana[m-1]) {
            d = 1; m++;
            if (m > 12) {
                m = 1; g++;
            }
        }
        datum.Postavi(d,m,g);
    }
    void Pregled::PomjeriDanUnazad() {
        int d, m, g;
        std::tie(d,m,g) = datum.Ocitaj();
        d--;
        if (d == 0 ) {
            m--;
            if (m == 0) {
                g--; m = 12;
            }
            int broj_dana[12] {31,28,31,30,31,30,31,31,30,31,30,31};
            if (g % 4 == 0 && g % 100 != 0 || g % 400 == 0 )
            broj_dana[1]++;        
            d = broj_dana[m-1];
        }
        datum.Postavi(d,m,g);
    }
bool Pregled::DolaziPrije(const Pregled &p1, const Pregled &p2) {
    int g1,g2,m1,m2,d1,d2,h1,h2,mm1,mm2;
    std::tie(d1,m1,g1) = p1.datum.Ocitaj();
    std::tie(d2,m2,g2) = p2.datum.Ocitaj();
    std::tie(h1,mm1) = p1.vrijeme.Ocitaj();
    std::tie(h2,mm2) = p2.vrijeme.Ocitaj();
    if (g1 != g2)
        return g1 < g2;
    if (m1 != m2)
        return m1 < m2;
    if (d1 != d2)
        return d1 < d2;
    if (h1 != h2)
        return h1 < h2;
    if (mm1 != mm2)
        return mm1 < mm2;
    return false;
}
class Pregledi {
    int broj_evidentiranih, max_broj;
    Pregled **pok_pregledi = nullptr;
public:
    explicit Pregledi(int max_broj_pregleda);
    Pregledi(std::initializer_list<Pregled> spisak_pregleda);
    ~Pregledi();
    Pregledi(const Pregledi &pregledi);
    Pregledi(Pregledi &&pregledi);
    Pregledi &operator =(const Pregledi &pregledi);
    Pregledi &operator =(Pregledi &&pregledi);
    void RegistrirajPregled(const std::string &ime_pacijenta, const Datum &datum_pregleda, const Vrijeme &vrijeme_pregleda);
    void RegistrirajPregled(const std::string &ime_pacijenta, int dan_pregleda,int mjesec_pregleda, int godina_pregleda, int sati_pregleda, int minute_pregleda);
    void RegistrirajPregled(Pregled *pregled);
    int DajBrojPregleda() const;
    int DajBrojPregledaNaDatum(const Datum &datum) const;
    Pregled &DajNajranijiPregled();
    Pregled DajNajranijiPregled() const;
    void IsprazniKolekciju();
    void ObrisiNajranijiPregled();
    void ObrisiPregledePacijenta(const std::string &ime_pacijenta);
    void IspisiPregledeNaDatum(const Datum &datum) const;
    void IspisiSvePreglede() const;
};

Pregledi::Pregledi(int max_broj_pregleda) : broj_evidentiranih(0), max_broj(max_broj_pregleda), pok_pregledi (new Pregled*[max_broj]{}) {}
Pregledi::Pregledi(std::initializer_list<Pregled> spisak_pregleda) : broj_evidentiranih(spisak_pregleda.size()), max_broj(broj_evidentiranih), 
    pok_pregledi (new Pregled*[max_broj]{}) {
        int i=0;
        try {
            for (auto it = spisak_pregleda.begin(); it != spisak_pregleda.end(); it++) 
                pok_pregledi[i++] = new Pregled (*it);
        } catch (...) {
            if (pok_pregledi != nullptr)
                for (int k = 0; k<i; k++)
                    delete pok_pregledi[k];
            delete[] pok_pregledi; 
            broj_evidentiranih = 0; pok_pregledi = nullptr; 
            throw;
        }
}
Pregledi::~Pregledi() {
    for (int i=0; i<broj_evidentiranih; i++)
        delete pok_pregledi[i];
    delete[] pok_pregledi;
}
Pregledi::Pregledi(const Pregledi &pregledi) : broj_evidentiranih(pregledi.broj_evidentiranih), max_broj(pregledi.max_broj), 
    pok_pregledi(new Pregled*[max_broj]{}) {
    try {
        for (int i=0; i<broj_evidentiranih; i++) 
                pok_pregledi[i] = new Pregled (*pregledi.pok_pregledi[i]);
        } catch (...) {
            if (pok_pregledi != nullptr)
                for (int i = 0; i<broj_evidentiranih; i++)
                    delete pok_pregledi[i];
            delete[] pok_pregledi; 
            broj_evidentiranih = 0; pok_pregledi = nullptr; 
            throw;
        }    
}
Pregledi::Pregledi(Pregledi &&pregledi) : broj_evidentiranih(pregledi.broj_evidentiranih), max_broj(pregledi.max_broj),
    pok_pregledi(pregledi.pok_pregledi) {
        pregledi.broj_evidentiranih = 0; pregledi.pok_pregledi = nullptr;
    }
Pregledi &Pregledi::operator = (const Pregledi &pregledi) {
    if (&pregledi != this) {
        if (max_broj < pregledi.max_broj) {
            Pregled **nova_lokacija = new Pregled*[pregledi.max_broj]{};
            try {
                for(int i=0; i<pregledi.broj_evidentiranih; i++)
                    nova_lokacija[i] = new Pregled ( *pregledi.pok_pregledi[i] );
            } catch (...) {
                for(int i=0; i<pregledi.broj_evidentiranih; i++)
                    delete nova_lokacija[i];
                delete[] nova_lokacija;
                throw;
            }
            for (int i=0; i<broj_evidentiranih; i++)                            // Oslobadjanje starih resursa
                delete pok_pregledi[i];
            delete[] pok_pregledi;
            pok_pregledi = nova_lokacija;
        }
        else {
            if (broj_evidentiranih <= pregledi.broj_evidentiranih) {
                try {
                    for (int i=broj_evidentiranih; i<pregledi.broj_evidentiranih; i++)
                        pok_pregledi[i] = new Pregled (*pregledi.pok_pregledi[i]); // Sa ovom implementacijom bezveze se inicijalizuje
                } catch (...) {
                    for (int i=broj_evidentiranih; i<pregledi.broj_evidentiranih; i++) {
                            delete pok_pregledi[i]; pok_pregledi[i] = nullptr;
                    }
                    throw;
                }
            }
            else {
                for (int i=pregledi.broj_evidentiranih; i<broj_evidentiranih; i++) {
                    delete pok_pregledi[i], pok_pregledi[i] = nullptr;
                }
            }
        }
        if (!(max_broj < pregledi.max_broj)) {
            for (int i=0; i<std::min(broj_evidentiranih, pregledi.broj_evidentiranih); i++)
                *pok_pregledi[i] = *pregledi.pok_pregledi[i];
        }
        max_broj = pregledi.max_broj; broj_evidentiranih = pregledi.broj_evidentiranih;
    }
return *this;       
}

Pregledi &Pregledi::operator =(Pregledi &&pregledi)  {
    broj_evidentiranih = pregledi.broj_evidentiranih;
    max_broj = pregledi.max_broj;
    pok_pregledi = pregledi.pok_pregledi;
    pregledi.pok_pregledi = nullptr; pregledi.broj_evidentiranih = 0;
    return *this;
}
 // Ostale metode
void Pregledi::RegistrirajPregled (const std::string &ime_pacijenta, const Datum &datum_pregleda, const Vrijeme &vrijeme_pregleda) {
    if (broj_evidentiranih == max_broj)
        throw std::range_error("Dostignut maksimalni broj pregleda");
    try {
        pok_pregledi[broj_evidentiranih++] = new Pregled (ime_pacijenta,datum_pregleda,vrijeme_pregleda);
    } catch (...) {
        broj_evidentiranih--;
        throw;
    }
}
void Pregledi::RegistrirajPregled(const std::string &ime_pacijenta, int dan_pregleda, int mjesec_pregleda, int godina_pregleda, int sati_pregleda, 
        int minute_pregleda) {
            if (broj_evidentiranih == max_broj)
                throw std::range_error("Dostignut maksimalni broj pregleda");
            try {
                pok_pregledi[broj_evidentiranih++] = new Pregled (ime_pacijenta, dan_pregleda, mjesec_pregleda, godina_pregleda,
                                                                   sati_pregleda, minute_pregleda);
            } catch (...) {
                broj_evidentiranih--;
                throw;
            }
}
void Pregledi::RegistrirajPregled(Pregled *pregled) {
    if (broj_evidentiranih == max_broj)
        throw std::range_error("Dostignut maksimalni broj pregleda");
    if (pregled != nullptr)
        pok_pregledi[broj_evidentiranih++] = pregled;
}
int Pregledi::DajBrojPregleda() const {
    return broj_evidentiranih;
}
int Pregledi::DajBrojPregledaNaDatum (const Datum &datum) const {
    return std::count_if(pok_pregledi, pok_pregledi+broj_evidentiranih, [datum] (Pregled *p)  {
        int d1,d2,m1,m2,g1,g2;
        std::tie(d1,m1,g1) = datum.Ocitaj();
        std::tie(d2,m2,g2) = p -> DajDatumPregleda().Ocitaj();
        return d1==d2 && m1==m2 && g1==g2;
    });    
}

Pregled &Pregledi::DajNajranijiPregled() {
    if (broj_evidentiranih == 0)
        throw std::domain_error("Nema registriranih pregleda");
    auto it = std::min_element(pok_pregledi, pok_pregledi+broj_evidentiranih, [] (const Pregled *p1, const Pregled *p2) {return Pregled::DolaziPrije(*p1,*p2);});
    return **it; // prvo dereferenciramo iterator pa onda pokazivac
}
Pregled Pregledi::DajNajranijiPregled() const {
    if (broj_evidentiranih == 0)
        throw std::domain_error("Nema registriranih pregleda");
    auto it = std::min_element(pok_pregledi, pok_pregledi+broj_evidentiranih, [] (const Pregled *p1, const Pregled *p2) {return Pregled::DolaziPrije(*p1,*p2);});
    return **it; // prvo dereferenciramo iterator pa onda pokazivac
}

void Pregledi::IsprazniKolekciju() {
    for (int i=0; i<broj_evidentiranih; i++) {
        delete pok_pregledi[i]; pok_pregledi[i] = nullptr;
    }
    broj_evidentiranih = 0;
}

void Pregledi::ObrisiNajranijiPregled() {
    if (broj_evidentiranih==0)
        throw std::range_error("Prazna kolekcija");
    Pregled prvi_pregled = DajNajranijiPregled();
    int d2,m2,g2,h2,mm2;
    std::string ime1(prvi_pregled.DajImePacijenta());
    std::tie(d2,m2,g2) = prvi_pregled.DajDatumPregleda().Ocitaj();
    std::tie(h2,mm2) = prvi_pregled.DajVrijemePregleda().Ocitaj();
    for (int i=0; i<broj_evidentiranih; i++) {
        int d1,m1,g1,h1,mm1;
        std::string ime2(pok_pregledi[i]->DajImePacijenta());
        std::tie(d1,m1,g1) = pok_pregledi[i] -> DajDatumPregleda().Ocitaj();
        std::tie(h1,mm1) = pok_pregledi[i] -> DajVrijemePregleda().Ocitaj();
        if (ime1 == ime2 && d1 == d2 && m1 == m2 && g1 == g2 && h1 == h2 && mm1 == mm2) {
            delete pok_pregledi[i];
            pok_pregledi[i] = nullptr;
            break;
        }
    }
    std::remove_if (pok_pregledi, pok_pregledi+broj_evidentiranih, [] (Pregled *p) {return p==nullptr;});
    broj_evidentiranih--;
}
void Pregledi::ObrisiPregledePacijenta(const std::string &ime_pacijenta) {
    int broj_obrisanih = 0;
    for (int i=0; i<broj_evidentiranih; i++)
        if ( pok_pregledi[i] -> DajImePacijenta() == ime_pacijenta ) {
            delete pok_pregledi[i]; pok_pregledi[i] = nullptr;
            broj_obrisanih++;
        }
    std::remove_if (pok_pregledi, pok_pregledi+broj_evidentiranih, [] (Pregled *p) {return p==nullptr;});
    broj_evidentiranih -= broj_obrisanih;
}

void Pregledi::IspisiPregledeNaDatum (const Datum &datum) const {
    int broj_pregleda = DajBrojPregledaNaDatum(datum);
    Pregled** kopija_pregleda = new Pregled*[broj_pregleda] {};
    for (int i=0, j=0; i<broj_evidentiranih; i++) {
        int d1,d2,m1,m2,g1,g2;
        std::tie(d1,m1,g1) = datum.Ocitaj();
        std::tie(d2,m2,g2) = pok_pregledi[i] -> DajDatumPregleda().Ocitaj();
        if ( d1==d2 && m1==m2 && g1==g2 )
            kopija_pregleda[j++] = pok_pregledi[i]; 
    }
    std::sort(kopija_pregleda, kopija_pregleda + broj_pregleda, [](const Pregled *p1, const Pregled *p2) {
        return Pregled::DolaziPrije(*p1,*p2);});
    for (int i=0; i<broj_pregleda; i++)
        kopija_pregleda[i] -> Ispisi();
    // Brisanje zauzetih resursa tj samo niza pokazivaca
    delete[] kopija_pregleda;
}
void Pregledi::IspisiSvePreglede() const {
    Pregled** kopija_pregleda = new Pregled*[broj_evidentiranih];
    std::copy(pok_pregledi, pok_pregledi+broj_evidentiranih, kopija_pregleda);
    std::sort(kopija_pregleda, kopija_pregleda + broj_evidentiranih, [](const Pregled *p1, const Pregled *p2) {
        return Pregled::DolaziPrije(*p1,*p2);});
    for (int i=0; i<broj_evidentiranih; i++)
    kopija_pregleda[i] -> Ispisi();
    delete[] kopija_pregleda;
}



int main () {
 
try {    
    // Unos
    std::cout<<"Unesite maksimalan broj pregleda: ";
    int broj_pregleda;
    std::cin>>broj_pregleda;
    
    Pregledi pregledi(broj_pregleda);
    for (int i=0; i<broj_pregleda; i++) {
        std::cin.ignore(1000,'\n');
        std::cout<<"Unesite ime novog pacijenta ili 'kraj' za zavrsetak unosa: ";
        std::string ime;
        std::getline(std::cin,ime);
        if (ime == "kraj")
            break;
        std::cout<<"Unesite dan mjesec i godinu pregleda: ";
        int dan, mjesec, godina, sati, minute;
        std::cin>>dan>>mjesec>>godina;
        std::cout<<"Unesite sate i minute pregleda: ";
        std::cin>>sati>>minute;
        pregledi.RegistrirajPregled(ime,dan,mjesec,godina,sati,minute);
    }
    std::cout<<"Trenutno su registrovani sljedeci pregledi"<<std::endl;
    pregledi.IspisiSvePreglede();
    int izbor;
    do {
        std::cout<<"Odaberite jednu od sljedecih opcija\n1 - Broj pregleda na datum\n2 - Najraniji pregled\n3 - Obrisi pregled pacijenta"
                 <<"\n4 - Obrisi najraniji pregled\n5 - Ispisi sve pregleda na datum\n6 - Ispisi sve preglede\n7 - Kraj programa\n";
        std::cin>>izbor;
        std::cin.ignore(1000,'\n');
        switch (izbor) {
            case 1 : {
                std::cout<<"Unesite dan mjesec i godinu : ";
                int dan,mjesec,godina;
                std::cin>>dan>>mjesec>>godina;
                std::cout<< "Broj pregleda na datum " << dan << " " << mjesec << " " << godina << " je: " 
                         << pregledi.DajBrojPregledaNaDatum({dan,mjesec,godina}) << std::endl;
                break;
            }
            case 2 : {
                std::cout<< "Najraniji pregled je:" << std::endl; pregledi.DajNajranijiPregled().Ispisi();
                break;
            }
            case 3 : {
                std::cout<<"Unesite ime pacijenta: ";
                std::string ime;
                std::getline(std::cin,ime);
                int prije = pregledi.DajBrojPregleda();
                pregledi.ObrisiPregledePacijenta(ime);
                std::cout<<"Uspjesno obrisano " << prije - pregledi.DajBrojPregleda() << " pregleda\n";
                break;
            }
            case 4 : {
                std::cout<<"Najraniji pregled uspjesno obrisan\n";
                pregledi.ObrisiNajranijiPregled();
                break;
            }
            case 5 : {
                std::cout<<"Unesite dan mjesec i godinu : ";
                int dan,mjesec,godina;
                std::cin>>dan>>mjesec>>godina;
                std::cout << "Pregledi na datum " << dan << " " << mjesec << " " << godina << " su:\n";
                pregledi.IspisiPregledeNaDatum({dan,mjesec,godina});
                break;
            }
            case 6 : {
                pregledi.IspisiSvePreglede();
                break;
            }
        }        
    } while (izbor > 0 && izbor < 7);
} catch (std::range_error &e) {
    std::cout<<e.what()<<std::endl;
} catch (std::domain_error &e) {
    std::cout<<e.what()<<std::endl;
}    

	return 0;
}