/* TP, 2018/2019
*/
#include <iostream>
#include <string>
#include <limits>


enum class Pravci {Sjever, Istok, Jug, Zapad};
enum class Smjer {Nalijevo, Nadesno};
enum class KodoviGresaka {PogresnaKomanda, NedostajeParametar, SuvisanParametar, NeispravanParametar};
enum class Komande {Aktiviraj, Deaktiviraj, Nalijevo, Nadesno, Idi, Kraj};
enum class Status {Aktivan, Neaktivan};

Status status = Status::Aktivan;

// Funkcije kontrole
void AktivirajRobota() {status = Status::Aktivan;}
void DeaktivirajRobota() {status = Status::Neaktivan;}

void Rotiraj(Pravci &orijentacija, Smjer na_koju_stranu) {
    if (int(na_koju_stranu))
        orijentacija = Pravci((int(orijentacija)+1)%4);
    else 
        orijentacija = Pravci((int(orijentacija)-1+4)%4);
}

void Idi(int &x, int &y, Pravci orijentacija, int korak) {
    
    int int_max=std::numeric_limits<int>::max(), int_min=std::numeric_limits<int>::min(), predznak[] {1,1,-1,-1};
    long long int pomak = 1l * korak * predznak[int(orijentacija)];
    if (orijentacija==Pravci::Sjever || orijentacija==Pravci::Jug) {
        if (y>0 && pomak > 0 && int_max - y < pomak)
            y = int_max;
        else if (y<0 && pomak < 0 && int_min - y > pomak)
            y = int_min;
        else 
            y += pomak;
    }
    else {
        if (x>0 && pomak > 0 && int_max - x < pomak)
            x = int_max;
        else if (x<0 && pomak < 0 && int_min - x > pomak)
            x = int_min;
        else 
            x += pomak;
    }
}

void IspisiPoziciju(int x, int y, Pravci orijentacija) {
    const char *pozicije[] {"sjever","istok","jug","zapad"};
    const char *aktiva[] {"aktivan","neaktivan"};
    std::cout<<"Robot je trenutno "<< aktiva[int(status)]<<", nalazi se na poziciji ("<<x<<","<<y<<") i gleda na "<<pozicije[int(orijentacija)]<<".\n";
}

void PrijaviGresku(KodoviGresaka kod_greske) {
    const char *greske[] {"Nerazumljiva komanda!","Komanda trazi parametar koji nije naveden!","Zadan je suvisan parametar nakon komande!","Parametar komande nije ispravan!"};
    std::cout<<greske[int(kod_greske)]<<'\n';
}

void IzvrsiKomandu(Komande komanda, int parametar, int &x, int &y, Pravci &orijentacija) {
    switch (komanda) {
        case Komande::Aktiviraj   : AktivirajRobota(); break;
        case Komande::Deaktiviraj : if (status==Status::Aktivan) DeaktivirajRobota(); break;
        case Komande::Nalijevo    : if (status==Status::Aktivan) Rotiraj(orijentacija,Smjer::Nalijevo); break;
        case Komande::Nadesno     : if (status==Status::Aktivan) Rotiraj(orijentacija,Smjer::Nadesno); break;
        case Komande::Idi         : if (status==Status::Aktivan) Idi(x,y,orijentacija,parametar); break;
        case Komande::Kraj        : std::cout<<"Dovidjenja!";
    }
}
bool UnosKomande(Komande &komanda, int &parametar, KodoviGresaka &kod_greske) {
    auto Ostatak = [] () {
        char znak;
        znak = std::cin.peek();
        while (znak != '\n') {
            znak = std::cin.get();
            if (znak != ' ')
                break;
        }
        return znak;
    };
    char prvi, drugi='\n', treci='\n';
    bool prvi_dobar=false, drugi_dobar=false, treci_dobar=false;
    std::cout<<"Unesi komandu: ";
    std::cin>>prvi;
    if (prvi == 'A' || prvi == 'L' || prvi == 'D' || prvi == 'K' || prvi == 'I') {
        prvi_dobar = true;
        if (prvi_dobar) {
            if (prvi == 'A')
                drugi = std::cin.get();
            else 
                drugi = Ostatak();
            if (prvi == 'A' && (drugi == '+' || drugi == '-')) drugi_dobar = true;
                else if (drugi == '\n' && (prvi == 'L' || prvi == 'D' || prvi == 'K'  )) drugi_dobar = true, treci_dobar = true;
                    else if (prvi == 'I' && drugi != '\n' ) drugi_dobar=true;
            if (drugi_dobar && !treci_dobar) {
                if (prvi == 'A')
                    treci = Ostatak();
                else {
                  treci = drugi;
                  long long int broj=0, predznak = 1;
                  if (std::cin.peek()>='0' && std::cin.peek()<='9') {
                      if (treci =='-') {
                      predznak = -1;
                      treci = std::cin.get();
                      } else if (treci == '+')
                            treci = std::cin.get();
                  }
                  while (treci>='0' && treci<='9') {
                      if (broj <= std::numeric_limits<int>::max() )
                            broj = 10*broj+treci-'0';
                      treci = std::cin.get();
                  }
                  broj *= predznak;
                  if (treci == ' ') treci = Ostatak();
                  if (treci == '\n')
                    if (broj > std::numeric_limits<int>::max())
                        parametar = std::numeric_limits<int>::max();
                    else if (broj < std::numeric_limits<int>::min())
                        parametar = std::numeric_limits<int>::min();
                    else 
                        parametar = broj;
                }
                if (treci == '\n') treci_dobar = true;
            }
        }
    }
    if ( !prvi_dobar && prvi!='A' || (treci!='\n' && (prvi == 'I' || prvi == 'A')) || (drugi!= '\n' && prvi!='I' && prvi != 'A') )
        std::cin.ignore(1000,'\n');
    if (!prvi_dobar || !drugi_dobar || !treci_dobar) {
        if (!prvi_dobar || (!drugi_dobar && prvi == 'A'))
            kod_greske = KodoviGresaka::PogresnaKomanda;
        else if ((!treci_dobar && prvi == 'A') || (!drugi_dobar && prvi != 'I'))
                kod_greske = KodoviGresaka::SuvisanParametar;
            else if (!drugi_dobar)
                    kod_greske = KodoviGresaka::NedostajeParametar;
                else
                    kod_greske = KodoviGresaka::NeispravanParametar;
        return false;
    }
    switch (prvi) {
        case 'A' : if (drugi == '+') 
                        komanda = Komande::Aktiviraj; 
                    else
                        komanda = Komande::Deaktiviraj;
                    break;
        case 'L' : komanda = Komande::Nalijevo; break;
        case 'D' : komanda = Komande::Nadesno; break;
        case 'I' : komanda = Komande::Idi; break;
        case 'K' : komanda = Komande::Kraj; break;
    }
    return true;
}

int main () {
    int x=0, y=0;
    Pravci orijentacija = Pravci::Sjever;
    Komande komanda;
    IspisiPoziciju(x,y,orijentacija);
    do {
        int parametar;
        KodoviGresaka kod_greske;
        while (!UnosKomande(komanda, parametar, kod_greske)) 
                PrijaviGresku(kod_greske);
        IzvrsiKomandu(komanda,parametar,x,y,orijentacija);
        if (komanda != Komande::Kraj)
            IspisiPoziciju(x,y,orijentacija);
    } while (komanda != Komande::Kraj);
	return 0;
}
