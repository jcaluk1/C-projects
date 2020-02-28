//TP 2018/2019: Zadaća 4, Zadatak 1
#include <iostream>
#include <utility>
#include <cmath>
#include <vector>
#include <set>
#include <memory>
#include <algorithm>

typedef std::pair<double, double> Tacka;
const double EPS = 1e-4;
const double PI = 4*std::atan(1);

class Trougao {
    Tacka tacke[3];
    
    static void TestIndeksa(int indeks) {
        if (indeks < 1 || indeks > 3)
            throw std::range_error("Nekorektan indeks");
    }

    static double orijentacija (const Tacka &t1, const Tacka &t2, const Tacka &t3) {
        return t1.first * (t2.second - t3.second) - t2.first * (t1.second - t3.second) + t3.first * (t1.second - t2.second);    
    }
public:
    Trougao(const Tacka &t1, const Tacka &t2, const Tacka &t3);
    void Postavi(const Tacka &t1, const Tacka &t2, const Tacka &t3);
    void Postavi(int indeks, const Tacka &t);
    static int Orijentacija(const Tacka &t1, const Tacka &t2, const Tacka &t3);
    Tacka DajTjeme(int indeks) const;
    double DajStranicu(int indeks) const;
    double DajUgao(int indeks) const;
    Tacka DajCentar() const;
    double DajObim() const;
    double DajPovrsinu() const;
    bool DaLiJePozitivnoOrijentiran() const;
    bool DaLiJeUnutra(const Tacka &t) const;
    void Ispisi() const;
    void Transliraj(double delta_x, double delta_y);
    void Centriraj(const Tacka &t);
    void Rotiraj(const Tacka &t, double ugao);
    void Skaliraj(const Tacka &t, double faktor);
    void Rotiraj(double ugao);
    void Skaliraj(double faktor);
    friend bool DaLiSuIdenticni(const Trougao &t1, const Trougao &t2);
    friend bool DaLiSuPodudarni(const Trougao &t1, const Trougao &t2);
    friend bool DaLiSuSlicni(const Trougao &t1, const Trougao &t2);
    
};
    Trougao::Trougao(const Tacka &t1, const Tacka &t2, const Tacka &t3) {
        if ( Orijentacija(t1,t2,t3) == 0 )
            throw std::domain_error("Nekorektne pozicije tjemena");
        tacke[0] = t1; tacke[1] = t2; tacke[2] = t3;
        
    }
    void Trougao::Postavi(const Tacka &t1, const Tacka &t2, const Tacka &t3) {
        if ( Orijentacija(t1,t2,t3) == 0 )
            throw std::domain_error("Nekorektne pozicije tjemena");
        tacke[0] = t1; tacke[1] = t2; tacke[2] = t3;
    }
    void Trougao::Postavi(int indeks, const Tacka &t) {
        TestIndeksa(indeks);
        if (indeks == 1) {
                if ( Orijentacija(t,tacke[1],tacke[2]) == 0 )
                    throw std::domain_error("Nekorektne pozicije tjemena");
        }
        else if (indeks == 2) {
                if ( Orijentacija(tacke[0],t,tacke[2]) == 0 )
                    throw std::domain_error("Nekorektne pozicije tjemena");
        }
        else
            if ( Orijentacija(tacke[0],tacke[1],t) == 0 )
                throw std::domain_error("Nekorektne pozicije tjemena");
        tacke[indeks-1] = t;
    }
    
    int Trougao::Orijentacija(const Tacka &t1, const Tacka &t2, const Tacka &t3){
        if  ( std::fabs(orijentacija(t1,t2,t3)) < EPS )
            return 0;
        else if (orijentacija(t1,t2,t3) > 0)
            return 1;
        return -1;
    }
    Tacka Trougao::DajTjeme(int indeks) const {
        TestIndeksa(indeks);
        return tacke[indeks-1];
    }
    
    double Trougao::DajStranicu(int indeks) const {
        TestIndeksa(indeks);
        int prvi = indeks % 3 , drugi = (prvi + 1) % 3; 
        return std::sqrt( (tacke[drugi].first - tacke[prvi].first) * (tacke[drugi].first - tacke[prvi].first)
                        + (tacke[drugi].second - tacke[prvi].second) * (tacke[drugi].second - tacke[prvi].second));
    }
    
    double Trougao::DajUgao(int indeks) const {
        TestIndeksa(indeks);
        double a = DajStranicu(indeks);
        double b = DajStranicu( indeks%3 + 1);
        double c = DajStranicu((indeks+1) % 3 + 1);
        return std::acos( (b*b + c*c - a*a) / (2 * b * c));
    }

    Tacka Trougao::DajCentar() const {
        return { (tacke[0].first+tacke[1].first+tacke[2].first)/3 , (tacke[0].second+tacke[1].second+tacke[2].second)/3 }; 
    }
    double Trougao::DajObim() const{
        return DajStranicu(1) + DajStranicu(2) + DajStranicu(3);  
    }
    double Trougao::DajPovrsinu() const {
        return std::fabs(orijentacija(tacke[0], tacke[1], tacke[2])) / 2;
        
    }
    bool Trougao::DaLiJePozitivnoOrijentiran() const {
        return Orijentacija(tacke[0], tacke[1], tacke[2]) == 1;
        
    }
    bool Trougao::DaLiJeUnutra(const Tacka &t) const {
        int trougao_orijentacija = Orijentacija(tacke[0], tacke[1], tacke[2]);
        for (int i=0; i<3; i++)
            if ( Orijentacija( tacke[i], tacke[(i+1)%3], t) != trougao_orijentacija )
                return false;
        return true;
        
        
    }
    void Trougao::Ispisi() const{
        std::cout<<"(";
        for (int i=0; i<3; i++)
            if (i!=0) 
                std::cout <<",(" << tacke[i].first << "," << tacke[i].second << ")";
            else       
                std::cout <<"(" << tacke[i].first << "," << tacke[i].second << ")";
        std::cout<<")";
        
    }
    void Trougao::Transliraj(double delta_x, double delta_y) {
        for (int i=0; i<3; i++) {
            tacke[i].first += delta_x; tacke[i].second += delta_y; 
        }
    }
    
    void Trougao::Centriraj(const Tacka &t) {
        Tacka trougao_centar = DajCentar();
        double delta_x = t.first - trougao_centar.first, delta_y = t.second - trougao_centar.second; 
        Transliraj(delta_x,delta_y);
    
        
    }
    void Trougao::Rotiraj(const Tacka &t, double ugao) {
        for (int i=0; i<3; i++) {
            double x, y;
            x = t.first + (tacke[i].first - t.first)*std::cos(ugao) - (tacke[i].second - t.second)*std::sin(ugao);
            y = t.second + (tacke[i].first - t.first)*std::sin(ugao) + (tacke[i].second - t.second)*std::cos(ugao);
            tacke[i].first = x;
            tacke[i].second = y;
        }
        
    }
    
    void Trougao::Skaliraj(const Tacka &t, double faktor) {
        if (std::fabs (faktor) < EPS)
            throw std::domain_error("Nekorektan faktor skaliranja");
        for (int i=0; i<3; i++) {
            tacke[i].first = t.first + faktor * (tacke[i].first - t.first);
            tacke[i].second = t.second + faktor * (tacke[i].second - t.second);
        }    
    }
    void Trougao::Rotiraj(double ugao) {
        Rotiraj(DajCentar(), ugao);
    }
    
    void Trougao::Skaliraj(double faktor) {
        Skaliraj(DajCentar(), faktor);    
    }
    
    bool DaLiSuIdenticni(const Trougao &t1, const Trougao &t2) {
        if ( Trougao::Orijentacija(t1.tacke[0],t1.tacke[1],t1.tacke[2]) == 0 || Trougao::Orijentacija(t2.tacke[0],t2.tacke[1],t2.tacke[2]) == 0 )
            throw std::domain_error("Nekorektne pozicije tjemena");
        // Zaokruzivanje na 4 decimale
        std::set<std::pair<int, int>> skup_tacaka;
        for (int i=0; i<3; i++) {
            skup_tacaka.insert ({EPS + std::round(t1.tacke[i].first*1e4), EPS + std::round(t1.tacke[i].second*1e4)}); 
            skup_tacaka.insert ({EPS + std::round(t2.tacke[i].first*1e4), EPS + std::round(t2.tacke[i].second*1e4)}); 
        }
        return skup_tacaka.size() == 3;
    }
    
    bool DaLiSuPodudarni(const Trougao &t1, const Trougao &t2) {
        if ( Trougao::Orijentacija(t1.tacke[0],t1.tacke[1],t1.tacke[2]) == 0 || Trougao::Orijentacija(t2.tacke[0],t2.tacke[1],t2.tacke[2]) == 0 )
            throw std::domain_error("Nekorektne pozicije tjemena");
        int k = 1;
        for (; k<=3; k++)
            if (std::fabs( t1.DajStranicu(1) - t2.DajStranicu(k)) < EPS)
                break;
        if (k-- == 4)
            return false;
        int pomak = (t1.DaLiJePozitivnoOrijentiran() == t2.DaLiJePozitivnoOrijentiran() ) ? 1 : -1 ;
        for (int i=2; i<=3; i++) {
            k = (k + pomak >= 0) ? (k+pomak)%3 : 2;
            if (std::fabs(t1.DajStranicu(i) - t2.DajStranicu(k+1)) > EPS )
                return false;
        }
        return true;
    }
    bool DaLiSuSlicni(const Trougao &t1, const Trougao &t2) {
        if ( Trougao::Orijentacija(t1.tacke[0],t1.tacke[1],t1.tacke[2]) == 0 || Trougao::Orijentacija(t2.tacke[0],t2.tacke[1],t2.tacke[2]) == 0 )
            throw std::domain_error("Nekorektne pozicije tjemena");
        std::set<double> stranice1, stranice2;
        for (int i=1;i<=3;i++) {
            stranice1.insert(t1.DajStranicu(i));
            stranice2.insert(t2.DajStranicu(i));
        }
        double koef = *stranice1.begin() / *stranice2.begin();
        Trougao t3 (t2);  t3.Skaliraj(t3.DajCentar(), koef); 
        return DaLiSuPodudarni (t1,t3);
    }
int main ()
{
   /
    int n;
    std::cout<<"Koliko zelite kreirati trouglova: ";
    std::cin>>n;
    std::vector<std::shared_ptr<Trougao>> trouglovi(n);
    for (int i=1; i<=n; ++i) {
        std::cout<<"Unesite podatke za "<<i<<". trougao (x1 y1 x2 y2 x3 y3): ";
        double x1,y1,x2,y2,x3,y3;
        std::cin>>x1>>y1>>x2>>y2>>x3>>y3;
        try {
            trouglovi[i-1] = std::make_shared<Trougao> (Trougao({x1,y1},{x2,y2},{x3,y3}));
        } catch (std::domain_error e) {
            std::cout<<e.what()<<", ponovite unos!\n";
            --i;
        }
    }
    double dx, dy, alpha, koef;
    std::cout<<"Unesite vektor translacije (dx dy): ";
    std::cin>>dx>>dy;
    std::cout<<"Unesite ugao rotacije: ";
    std::cin>>alpha;
    std::cout<<"Unesite faktor skaliranja: ";
    std::cin>>koef;
    std::transform(trouglovi.begin(), trouglovi.end(), trouglovi.begin(), [dx,dy,alpha,koef](const std::shared_ptr<Trougao> &p) {
       p -> Transliraj(dx,dy);
       p -> Rotiraj(alpha);
       p -> Skaliraj(p->DajTjeme(1), koef);
     return p;
    });           
    
    std::sort (trouglovi.begin(), trouglovi.end(), [](const std::shared_ptr<Trougao> &p1, const std::shared_ptr<Trougao> &p2) {return p1->DajPovrsinu() < p2->DajPovrsinu();});
    std::cout<<"Trouglovi nakon obavljenih transformacija: \n";
    std::for_each(trouglovi.begin(), trouglovi.end(), [](const std::shared_ptr<Trougao> &p){ p->Ispisi();std::cout<<'\n';});
    auto p_min = std::min_element(trouglovi.begin(), trouglovi.end(), [](const std::shared_ptr<Trougao> &p1, const std::shared_ptr<Trougao> &p2) {return p1->DajObim() < p2->DajObim();}); 
    std::cout<<"Trougao sa najmanjim obimom: "; (*p_min) -> Ispisi(); std::cout<<'\n'; 
    // Vektori parova pokazivaca na identicne ,podudarne i slicne
    std::vector<std::pair<Trougao*, Trougao*>> identicni_trouglovi;
    std::vector<std::pair<Trougao*, Trougao*>> podudarni_trouglovi;
    std::vector<std::pair<Trougao*, Trougao*>> slicni_trouglovi;
    for (int i=0; i<trouglovi.size()-1; i++)
        for (int j=i+1; j<trouglovi.size(); j++ ) {
            if (DaLiSuIdenticni(*trouglovi[i], *trouglovi[j]))
                identicni_trouglovi.push_back({trouglovi[i].get(), trouglovi[j].get()});
            if (DaLiSuPodudarni(*trouglovi[i], *trouglovi[j]))
                podudarni_trouglovi.push_back({trouglovi[i].get(), trouglovi[j].get()});
            if (DaLiSuSlicni(*trouglovi[i], *trouglovi[j]))
                slicni_trouglovi.push_back({trouglovi[i].get(), trouglovi[j].get()});
        }

    if (identicni_trouglovi.size() != 0) {
        std::cout<<"Parovi identicnih trouglova: \n";
        for (const auto &par : identicni_trouglovi) {
            par.first -> Ispisi(); std::cout<<" i "; par.second -> Ispisi(); std::cout<<std::endl;
        }
    }
    else 
        std::cout<<"Nema identicnih trouglova!\n";
    
    if (podudarni_trouglovi.size() != 0) {
        std::cout<<"Parovi podudarnih trouglova: \n";
        for (const auto &par : podudarni_trouglovi) {
            par.first -> Ispisi(); std::cout<<" i "; par.second -> Ispisi(); std::cout<<std::endl;
        }    
    }
    else 
        std::cout<<"Nema podudarnih trouglova!\n";
    
    if (slicni_trouglovi.size() != 0) {
        std::cout<<"Parovi slicnih trouglova: \n";
        for (const auto &par : slicni_trouglovi) {
            par.first -> Ispisi(); std::cout<<" i "; par.second -> Ispisi(); std::cout<<std::endl;
        }
    }
    else 
        std::cout<<"Nema slicnih trouglova!\n";
        
           
return 0;
}