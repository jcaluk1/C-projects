//TP 2018/2019: Zadaća 4, Zadatak 2
#include <iostream>
#include <string>

class GradjaninBiH {
    std::string naziv;
    long long int jmbg;
    int dan, mjesec, godina, regija, kod;
    GradjaninBiH *prethodni;
    static GradjaninBiH *posljednji;
    // Privatne metode
    static bool TestDatum(int d, int m, int g);
    static long long int DajBrojIzJMBG (const char *niz, int pocetak, int kraj);
    static void BrojToNiz (char *pocetak, char* kraj, long long int broj);
    void Postavi (const std::string &naziv, long long int jmbg, int dan, int mjesec, int godina, int regija, int kod);
    
public:
    enum Pol {Musko, Zensko} pol;
    GradjaninBiH(std::string ime_i_prezime, long long int jmbg);
    GradjaninBiH(std::string ime_i_prezime, int dan_rodjenja, int mjesec_rodjenja, int godina_rodjenja, int sifra_regije, Pol pol);
    GradjaninBiH (const GradjaninBiH &g) = delete;
    GradjaninBiH &operator = (const GradjaninBiH &g) = delete;
    ~GradjaninBiH();
    std::string DajImeIPrezime() const {
        return naziv;
    }
    long long int DajJMBG() const {
        return jmbg;
    }
    int DajDanRodjenja() const {
        return dan;
    }
    int DajMjesecRodjenja() const {
        return mjesec;
    }
    int DajGodinuRodjenja() const {
        return godina;
    }
    int DajSifruRegije() const {
        return regija;
    }
    Pol DajPol() const {
        return pol;
    }
    void PromijeniImeIPrezime(std::string novo_ime) {
        naziv = novo_ime;
    }
};
// Privatne metode 
bool GradjaninBiH::TestDatum(int d, int m, int g) {
    int broj_dana[12] {31,28,31,30,31,30,31,31,30,31,30,31};
    if (g % 4 == 0 && g % 100 != 0 || g % 400 == 0 )
            broj_dana[1]++;
    return !(g < 1917 || g > 2017 || d < 1 || d > broj_dana[m-1] || m < 1 || m > 12);   
}
long long int GradjaninBiH::DajBrojIzJMBG (const char *niz, int pocetak, int kraj) {
    // vrati broj na poziciji [pocetak, kraj] , indekiranje poceinje od 0
    long long int rezultat = 0;
    for (int i = pocetak; i <= kraj; i++)
        rezultat = 10*rezultat + niz[i-1];
    return rezultat;
}
void GradjaninBiH::BrojToNiz (char *pocetak,char *kraj, long long int broj) {
    // Za upis broj u niz
    while (kraj >= pocetak) {
        *kraj = broj%10;
        broj /= 10;
        kraj--;
    }
}
void GradjaninBiH::Postavi (const std::string &naziv, long long int jmbg, int dan, int mjesec, int godina, int regija, int kod) {
    GradjaninBiH::naziv = naziv;
    GradjaninBiH::jmbg = jmbg;
    GradjaninBiH::dan = dan;
    GradjaninBiH::mjesec = mjesec;
    GradjaninBiH::godina = godina;
    GradjaninBiH::regija = regija;
    GradjaninBiH::kod = kod;
    pol = (kod <= 499) ? Pol::Musko : Pol::Zensko;
    // Postavljanje pokazivaca
    if (posljednji != nullptr) {
        prethodni = posljednji;
        posljednji = this;
    } 
    else {
        posljednji = this;
        prethodni = nullptr;
    }
}
// Javne metode
GradjaninBiH* GradjaninBiH::posljednji = nullptr;   // Incijalizacija statickog pokazivaca
GradjaninBiH::GradjaninBiH(std::string ime_i_prezime, long long int jmbg) {
    if (jmbg >= 1e13 || jmbg < 1e11 )
        throw std::logic_error("JMBG nije validan");
    char temp[13]{};
    BrojToNiz(temp,temp+12,jmbg);
    // Ispravnost podataka
    int dan = DajBrojIzJMBG(temp,1,2),  mjesec=DajBrojIzJMBG(temp,3,4), godina = DajBrojIzJMBG(temp,5,7),
        regija = DajBrojIzJMBG(temp,8,9), kod = DajBrojIzJMBG(temp,10,12); 
    godina = (godina <=17) ? godina+2000 : godina+1000; 
    int zadnja_cifra = 11 - (  7*(temp[0]+temp[6]) + 6*(temp[1]+temp[7]) + 5*(temp[2]+temp[8]) + 4*(temp[3]+temp[9]) + 
                               3*(temp[4]+temp[10]) + 2*(temp[5]+temp[11]) ) % 11;  
    if (zadnja_cifra == 11) zadnja_cifra = 0;
    if (!TestDatum(dan,mjesec,godina) || zadnja_cifra == 10 || zadnja_cifra != temp[12])
        throw std::logic_error("JMBG nije validan");
        
    // Provjera da li postoji identican jmbg u uvezanoj listi građana 
    GradjaninBiH *pok  = GradjaninBiH::posljednji;
    while (pok != nullptr) {
        if (jmbg == pok -> jmbg)
            throw std::logic_error("Vec postoji gradjanin sa istim JMBG");
        pok = pok -> prethodni;
    }
    Postavi(ime_i_prezime, jmbg, dan, mjesec, godina, regija, kod);
}
GradjaninBiH::GradjaninBiH(std::string ime_i_prezime, int dan_rodjenja, int mjesec_rodjenja, int godina_rodjenja, int sifra_regije, Pol pol) {
    if (!TestDatum(dan_rodjenja,mjesec_rodjenja,godina_rodjenja) || sifra_regije<0 || sifra_regije>99)
        throw std::logic_error("Neispravni podaci");
    // Generisanje koda prema 000 - 499 za muske, 500 - 999 za zene;
    int potecijalni_kod = (pol == Pol::Musko) ? 0 : 500;
    GradjaninBiH *pok  = GradjaninBiH::posljednji;
    long long int jmbg;
    
   while (true) {
        while (pok != nullptr) {
            if (dan_rodjenja == pok->dan && mjesec_rodjenja == pok->mjesec && godina_rodjenja == pok -> godina && sifra_regije == pok->regija && 
                pol == pok->pol && potecijalni_kod == pok->kod) 
            {
                potecijalni_kod++;
                pok = posljednji;
                continue;
            }
            pok = pok->prethodni;
        }
        // Provjera zadnje cifre - Formiranje maticnog broja u pomocnom nizu
        char temp[13]{};
        BrojToNiz(temp, temp+1, dan_rodjenja);                                                             // dan
        BrojToNiz(temp+2, temp+3, mjesec_rodjenja);                                                        // mjesec
        BrojToNiz(temp+4, temp+6, (godina_rodjenja < 2000) ? godina_rodjenja%1000 : godina_rodjenja%2000); // godina
        BrojToNiz(temp+7, temp+8, sifra_regije);                                                           // regija
        BrojToNiz(temp+9, temp+11, potecijalni_kod);                                                       // kod
        temp[12] = 11 - (  7*(temp[0]+temp[6]) + 6*(temp[1]+temp[7]) + 5*(temp[2]+temp[8]) + 4*(temp[3]+temp[9]) + 
                           3*(temp[4]+temp[10]) + 2*(temp[5]+temp[11]) ) % 11; 
        // Ako je zadnja cifra 10 trocifreni kod ne valja i treba drugi
        if (temp[12] != 10) {
            temp[12] = (temp[12] != 11 ) ? temp[12] : 0;
            jmbg = DajBrojIzJMBG(temp,1,13);  // Formiranje jmbg
            break;                            // Break vanjske petlje
        }
        else {
            pok = posljednji;
            potecijalni_kod++;
        }
    }
    Postavi(ime_i_prezime, jmbg, dan_rodjenja, mjesec_rodjenja, godina_rodjenja, sifra_regije, potecijalni_kod);
}
GradjaninBiH::~GradjaninBiH() {
    if (this == posljednji) {
        posljednji = this -> prethodni;
    }
    else {
        GradjaninBiH *prije_trenutnog = posljednji;
        while (prije_trenutnog -> prethodni != this)
            prije_trenutnog = prije_trenutnog -> prethodni;
        prije_trenutnog -> prethodni = this -> prethodni;
    }
}

int main ()
{
    std::cout<<"Koliko gradjana zelite unijeti po JMBG? \n";
    int n;
    std::cin>>n;
    std::cin.ignore(1000,'\n');
    for (int i=0; i<n; i++) {
        std::cout<<"Unesite ime i prezime (u istom redu): \n";
        std::string ime;
        std::getline(std::cin,ime);
        
        long long int jmbg;
        std::cout<<"Unesite JMBG: \n";
        std::cin>>jmbg;
        std::cin.ignore(1000,'\n');
        try {
            GradjaninBiH g (ime,jmbg);
            std::cout << "Unijeli ste gradjanina " << g.DajImeIPrezime() <<" rodjenog " << g.DajDanRodjenja() << "."  
                      << g.DajMjesecRodjenja() << "." << g.DajGodinuRodjenja() <<" u regiji " << g.DajSifruRegije() << ", "
                      << ((g.DajPol() == GradjaninBiH::Pol::Musko) ? "musko" : "zensko" ) << "." << std::endl; 
            
        }
        catch (std::logic_error &e) {
            std::cout<<e.what()<<std::endl;
            i--;
        }
    }
    std::cout<<"Koliko gradjana zelite unijeti po datumu rodjenja, regiji i polu? \n";
    std::cin>>n;
    std::cin.ignore(1000,'\n');
    for (int i=0; i<n; i++) {
        std::cout<<"Unesite ime i prezime (u istom redu): \n";
        std::string ime;
        std::getline(std::cin, ime);
        std::cout <<"Unesite datum rodjenja (format dd/mm/gggg): \n";
        int d,m,g;
        char znak;
        std::cin>>d>>znak>>m>>znak>>g;
        std::cout<<"Unesite sifru regije: \n";
        int sifra;
        std::cin>>sifra;
        std::cin.ignore(1000,'\n');
        std::cout << "Unesite M za musko, Z za zensko: \n";
        std::cin>>znak;
        std::cin.ignore(1000,'\n');
        
        
        try {
            GradjaninBiH gg(ime, d, m, g, sifra, ((znak == 'M') ? GradjaninBiH::Pol::Musko : GradjaninBiH::Pol::Zensko));
            std::cout<<"Unijeli ste gradjanina "<< gg.DajImeIPrezime() << " JMBG: "<< gg.DajJMBG() << ".\n";
            
        } catch (std::logic_error &e) {
            std::cout << e.what() << std::endl;
            i--;
        }
    }
	return 0;
}