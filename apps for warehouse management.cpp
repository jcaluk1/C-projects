//TP 2018/2019: Zadaća 5, Zadatak 3
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <numeric>
#include <algorithm>
#include <fstream>

class Spremnik {
    double tezina_spremnika;
    std::string naziv_materije;
public:
    
    Spremnik (double tezina_spremnika, std::string naziv_materije) : tezina_spremnika(tezina_spremnika), naziv_materije(naziv_materije) {}
    
    virtual ~Spremnik () {};
    
    double DajTezinu () const { return tezina_spremnika;}
    std::string DajNazivMaterije () const {return naziv_materije;}
    
    virtual double DajUkupnuTezinu () const = 0;
    virtual void Ispisi () const = 0;
    
    virtual std::shared_ptr<Spremnik> Kloniraj () const = 0;    
};

class Sanduk : public Spremnik {
    std::vector<double> tezine_predmeta;
public:
    Sanduk (double tezina_spremnika, std::string naziv_materije, std::vector<double> tezine_predmeta) : Spremnik(tezina_spremnika,naziv_materije),
        tezine_predmeta(tezine_predmeta) {}
    
    double DajUkupnuTezinu () const override {
        return DajTezinu() + std::accumulate(tezine_predmeta.begin(), tezine_predmeta.end(), 0.);
    }
    
    void Ispisi () const override;
    
    std::shared_ptr<Spremnik> Kloniraj () const override {
        return std::make_shared<Sanduk> (*this);
    }
};
void Sanduk::Ispisi() const {
    std::cout << "Vrsta spremnika: Sanduk" << std::endl << "Sadrzaj: " << DajNazivMaterije() << std::endl
              << "Tezine predmeta: ";
    for (const double &tezina : tezine_predmeta)
        std::cout << tezina << " ";
    std::cout << "(kg)" << std::endl << "Vlastita tezina: " << DajTezinu() << " (kg)"  << std::endl
              << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)" << std::endl;    
}


class Vreca : public Spremnik {
    double tezina_materije;
public:
    Vreca (double tezina_spremnika, std::string naziv_materije, double tezina_materije) : Spremnik(tezina_spremnika,naziv_materije),
        tezina_materije(tezina_materije) {}
    
    double DajUkupnuTezinu () const override {
        return DajTezinu() + tezina_materije;
    }
    
    void Ispisi () const override;

    std::shared_ptr<Spremnik> Kloniraj () const override {
        return std::make_shared<Vreca> (*this);
    }
};
void Vreca::Ispisi () const {
    std::cout << "Vrsta spremnika: Vreca" << std::endl << "Sadrzaj: " << DajNazivMaterije() << std::endl
              << "Vlastita tezina: " << DajTezinu() << " (kg)" << std::endl 
              << "Tezina pohranjene materije: " << tezina_materije << " (kg)" << std::endl
              << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)" << std::endl;
}

class Bure : public Spremnik {
    double rho, zapremina;
public:
    Bure (double tezina_spremnika, std::string naziv_materije, double rho, double zapremina) : Spremnik(tezina_spremnika,naziv_materije),
        rho(rho), zapremina(zapremina) {}
    
    double DajUkupnuTezinu () const override {
        return DajTezinu() + rho / 1000 * zapremina;
    }
    
    void Ispisi () const override;
    
    std::shared_ptr<Spremnik> Kloniraj () const override {
        return std::make_shared<Bure> (*this);
    }
};
void Bure::Ispisi () const {
    std::cout << "Vrsta spremnika: Bure" << std::endl << "Sadrzaj: " << DajNazivMaterije() << std::endl
              << "Vlastita tezina: " << DajTezinu() << " (kg)" << std::endl 
              << "Specificna tezina tecnosti: " << rho << " (kg/m^3)" << std::endl
              << "Zapremina tecnosti: " << zapremina << " (l)" << std::endl
              << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)" << std::endl;
}

class Skladiste {
    std::vector<std::shared_ptr<Spremnik>> skladiste;
public:
    Skladiste () {};
    
    Skladiste (const Skladiste &sk);
    Skladiste (Skladiste &&sk);
    
    Skladiste &operator = (const Skladiste &sk);
    Skladiste &operator = (Skladiste &&sk);
    
    
    
    
    Spremnik* DodajSpremnik (Spremnik *spremnik, bool skladiste_je_vlasnik);
    
    Spremnik* DodajSanduk (double tezina_spremnika, std::string naziv_materije, std::vector<double> tezine_predmeta) {
        return DodajSpremnik(new Sanduk (tezina_spremnika,naziv_materije,tezine_predmeta) , true);
    }
    
    Spremnik* DodajVrecu  (double tezina_spremnika, std::string naziv_materije, double tezina_materije) {
        return DodajSpremnik(new Vreca (tezina_spremnika, naziv_materije, tezina_materije) , true);
    }
    
    Spremnik* DodajBure  (double tezina_spremnika, std::string naziv_materije, double rho, double zapremina) {
        return DodajSpremnik(new Bure (tezina_spremnika, naziv_materije, rho, zapremina) , true);
    }
    
    void BrisiSpremnik (Spremnik *spremnik);
    
    Spremnik &DajNajlaksi () ;
    
    Spremnik &DajNajtezi () ;
    
    int BrojPreteskih (double kilaza) const ;
    
    void IzlistajSkladiste () const ;
    
    void UcitajIzDatoteke (const std::string &ime_datoteke);
};
// --------------------------------------------------------------------------------
    Skladiste::Skladiste (const Skladiste &sk) : skladiste(sk.skladiste.size()) {
        try {
            for (int i=0; i < skladiste.size(); i++)
                skladiste[i] = sk.skladiste[i] -> Kloniraj();
        } catch (...) {
            skladiste.clear();
            throw;
        }
    }
    
    
    Skladiste::Skladiste (Skladiste &&sk) {
        skladiste = std::move(sk.skladiste);
        sk.skladiste.clear();
    }
    
Skladiste &Skladiste::operator = (const Skladiste &sk) {
    if (this != &sk) { 
        std::vector<std::shared_ptr<Spremnik>> novo_skladiste(sk.skladiste.size());
        try {
            for (int i=0; i < sk.skladiste.size(); i++)
            novo_skladiste[i] = sk.skladiste[i] -> Kloniraj();
       } catch (...) {
           novo_skladiste.clear();
           throw;
       }
       skladiste = std::move(novo_skladiste);
    }  
    return *this;
} 
Skladiste &Skladiste::operator = (Skladiste &&sk) {
    if (&sk != this) {
        skladiste = std::move(sk.skladiste);
        sk.skladiste.clear();
    }
    return *this;
} 
// --------------------------------------------------------------------------------

    Spremnik* Skladiste::DodajSpremnik (Spremnik *spremnik, bool skladiste_je_vlasnik) {
        if (skladiste_je_vlasnik)
            skladiste.push_back( std::shared_ptr<Spremnik> (spremnik) );
        else 
            skladiste.push_back ( spremnik -> Kloniraj() );
        return (skladiste.back()).get();
    }
    
    void Skladiste::BrisiSpremnik (Spremnik *spremnik) {
        for (auto it=skladiste.begin(); it<skladiste.end(); it++)
            if (spremnik == (*it).get() ) {
                skladiste.erase(it);
                break;
            }
    }
    
    Spremnik &Skladiste::DajNajlaksi () {
        if (skladiste.size() == 0)
            throw std::range_error("Skladiste je prazno");
        return   **(std::min_element(skladiste.begin(), skladiste.end(), [] (const std::shared_ptr<Spremnik> &s1, const std::shared_ptr<Spremnik> &s2)  
                {return s1 -> DajTezinu() < s2 -> DajTezinu();})); 
    }
    
    Spremnik &Skladiste::DajNajtezi () {
        if (skladiste.size() == 0)
            throw std::range_error("Skladiste je prazno");
        return   **(std::max_element(skladiste.begin(), skladiste.end(), [] (const std::shared_ptr<Spremnik> &s1, const std::shared_ptr<Spremnik> &s2)  
                {return s1 -> DajTezinu() < s2 -> DajTezinu();})); 
    }
    
    int Skladiste::BrojPreteskih (double kilaza) const {
        if (skladiste.size() == 0)
            throw std::range_error("Skladiste je prazno");
        return std::count_if(skladiste.begin(), skladiste.end(), [kilaza] (const std::shared_ptr<Spremnik> &s) {
            return s -> DajUkupnuTezinu() > kilaza;});
    }
    
    void Skladiste::IzlistajSkladiste () const {
        auto temp_skladiste = skladiste;
        std::sort(temp_skladiste.begin(), temp_skladiste.end() , [] (const std::shared_ptr<Spremnik> &s1, const std::shared_ptr<Spremnik> &s2) {
            return s1 -> DajUkupnuTezinu() > s2 -> DajUkupnuTezinu();});
        for (auto pok : temp_skladiste) {
            pok -> Ispisi();
        }
    }
    void Skladiste::UcitajIzDatoteke (const std::string &ime_datoteke) {
        std::ifstream ulaz(ime_datoteke);
        if (!ulaz)
            throw std::logic_error("Trazena datoteka ne postoji");
        (*this).skladiste.clear();
        char znak;
        std::string naziv_materije;
        double tezina_spremnika;
        bool dobar_unos = false;
        while (ulaz) {
            dobar_unos = false;
            ulaz >> znak >> std::ws;
            std::getline(ulaz, naziv_materije);
            ulaz >> tezina_spremnika;
            if (ulaz) {
                if (znak == 'S' ) {
                    int broj_predmeta;
                    ulaz >> broj_predmeta;
                    std::vector<double> predmeti (broj_predmeta);
                    for (int i=0; ulaz && i<broj_predmeta; ++i)
                        ulaz >> predmeti[i];
                    if (ulaz && ( ulaz.peek() == '\n' || ulaz.eof()) ) {
                        DodajSanduk(tezina_spremnika,naziv_materije,predmeti); 
                        dobar_unos = true;
                    }
                    
                }
                else if (znak == 'V') {
                    double tezina_materije;
                    ulaz >> tezina_materije;
                    if (ulaz && ( ulaz.peek() == '\n' || ulaz.eof())) {
                        DodajVrecu(tezina_spremnika,naziv_materije,tezina_materije);
                        dobar_unos = true;
                    }
                }
                else if (znak == 'B' ) {
                    double rho, zapremina;
                    ulaz >> rho >> zapremina;
                    if (ulaz && ( ulaz.peek() == '\n' || ulaz.eof())) {
                        DodajBure(tezina_spremnika,naziv_materije,rho,zapremina); 
                        dobar_unos = true;
                    }
                }
                ulaz.ignore(1000,'\n');
            }
            if (!dobar_unos)
                break;
        }
        if (ulaz.eof() & dobar_unos) 
            return;
        (*this).skladiste.clear();
        if (ulaz.bad())
            throw std::logic_error("Problemi pri citanju datoteke");
        throw std::logic_error("Datoteka sadrzi besmislene podatke");
    }



int main () {
    Skladiste pijaca;
    try {
        pijaca.UcitajIzDatoteke("ROBA.TXT");
        pijaca.IzlistajSkladiste();
    } catch (std::logic_error &e) {
        std::cout << e.what();
    }
    
	return 0;
}