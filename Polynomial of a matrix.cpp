//TP 2018/2019: Zadaća 3, Zadatak 3
#include <iostream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <new>
#include <vector>

template <typename TipElemenata>
struct Matrica {
    int br_redova, br_kolona;
    TipElemenata **elementi = nullptr; // VEOMA BITNA INICIJALIZACIJA!
};

template <typename TipElemenata>
void UnistiMatricu(Matrica<TipElemenata> &mat){
    if(!mat.elementi) return;
    for(int i = 0; i < mat.br_redova; i++) delete[] mat.elementi[i];
    delete[] mat.elementi;
    mat.elementi = nullptr;
}

template <typename TipElemenata>
Matrica<TipElemenata> StvoriMatricu(int br_redova, int br_kolona) {
    Matrica<TipElemenata> mat;
    mat.br_redova = br_redova;
    mat.br_kolona = br_kolona;
    try {
        mat.elementi = new TipElemenata*[br_redova] {};      // Inicijalizacija na nullptr
        for(int i = 0; i < br_redova; i++)
            mat.elementi[i] = new TipElemenata[br_kolona]{}; // Inicijalizacija svakog clana na podrazumijevanu vrijednost 0
    } catch(...) {
        UnistiMatricu(mat);
        throw;
    }
    return mat;
}

template <typename TipElemenata>
void UnesiMatricu(char ime_matrice, Matrica<TipElemenata> &mat) {
    for(int i = 0; i < mat.br_redova; i++)
        for(int j = 0; j < mat.br_kolona; j++) {
            std::cout << ime_matrice
                      << "(" << i + 1 << "," << j + 1 << ") = ";
            std::cin >> mat.elementi[i][j];
        }
}

template <typename TipElemenata>
void IspisiMatricu(Matrica<TipElemenata> mat, int sirina_ispisa, int preciznost = 6, bool brisi = false) {
    for(int i = 0; i < mat.br_redova; i++) {
        for(int j = 0; j < mat.br_kolona; j++)
            std::cout << std::setprecision(preciznost) << std::setw(sirina_ispisa) << mat.elementi[i][j];
        std::cout << std::endl;
    }
    if (brisi)
        UnistiMatricu(mat);
}

template <typename TipElemenata>
Matrica<TipElemenata> ZbirMatrica(const Matrica<TipElemenata> &m1, const Matrica<TipElemenata> &m2) {
    if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
        throw std::domain_error("Matrice nemaju jednake dimenzije!");
    auto m3 = StvoriMatricu<TipElemenata>(m1.br_redova, m1.br_kolona);
    for(int i = 0; i < m1.br_redova; i++)
        for(int j = 0; j < m1.br_kolona; j++)
            m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
    return m3;
}

template <typename TipElemenata>
Matrica<TipElemenata> ProduktMatrica(const Matrica<TipElemenata> &m1, const Matrica<TipElemenata> &m2) {
    if (m1.br_kolona != m2.br_redova) 
        throw std::domain_error("Matrice nisu saglasne za mnozenje");
    auto m3 = StvoriMatricu<TipElemenata>(m1.br_redova, m2.br_kolona);
    for (int i=0; i<m3.br_redova; i++)
        for (int j=0; j<m3.br_kolona; j++) 
            for (int k=0; k<m1.br_kolona; k++)
                m3.elementi[i][j] += m1.elementi[i][k] * m2.elementi[k][j];
    return m3;    
}

template <typename TipElemenata>
Matrica<TipElemenata> MatricniPolinom(const Matrica<TipElemenata> &A, const std::vector<double> &koeficijenti) {
    if (A.br_redova != A.br_kolona) 
        throw std::domain_error("Matrica mora biti kvadratna");
    // Inicijalizacija 
    int vel = koeficijenti.size(), brojac = 0;                                            // brojac - broj uspjesnih alokacija
    Matrica<TipElemenata> rezultat_matrica;                                               // matrica koju vracamo iz funkcije
    std::vector<Matrica<TipElemenata>> pomocne_matrice;                                   // Vektor matrica u koje cemo smjestiti I, A, A^2, ... A^n    
    // U slucaju da nema memorije ...
    try {
        rezultat_matrica = StvoriMatricu<TipElemenata>(A.br_redova, A.br_redova);        
        if (vel == 0) 
            return rezultat_matrica;                                                      // Odmah vracamo jer su elementi inicijalizovani na nulu
    
        pomocne_matrice.resize (vel);                                                     // Vektor matrica u koje cemo smjestiti I, A, A^2, ... A^n    
        pomocne_matrice[0] = StvoriMatricu<TipElemenata>(A.br_redova, A.br_redova);       
        brojac++;
        for (int i=0; i<A.br_redova; i++)                                                 // Inicijalizujemo samo jedinicnu matricu a ostale dobijamo rekurzivno, A^i = A^(i-1) x A
            pomocne_matrice[0].elementi[i][i] = 1;                                   
    
        for (int i=1; i<vel; i++) {                                                       
            pomocne_matrice[i] = ProduktMatrica(pomocne_matrice[i-1], A);
            brojac++;
        }
    } catch (...) {
        for (int i=0; i<brojac; i++)
            UnistiMatricu(pomocne_matrice[i]);
        UnistiMatricu(rezultat_matrica);
        throw;
    }
    for (int i=0; i<A.br_redova; i++)                                       // Suma pk*A^k
        for (int j=0; j<A.br_redova; j++)
            for (int k=0; k<vel; k++)
            rezultat_matrica.elementi[i][j] +=  koeficijenti[k] * pomocne_matrice[k].elementi[i][j];
    for (int i=0; i<vel; i++)                                               // Pospremanje - brisanje pomocnih matrica
        UnistiMatricu(pomocne_matrice[i]);
    return rezultat_matrica;
}


int main ()
{
    
    Matrica<double> a;
    int n;
    std::cout<<"Unesite dimenziju kvadratne matrice: ";
    std::cin>>n;
    
    try {
        a = StvoriMatricu<double>(n,n);
        std::cout<<"Unesite elemente matrice A: \n";
        UnesiMatricu('A',a);
        int vel;
        std::cout<<"Unesite red polinoma: ";
        std::cin>>vel;
        if (vel < -1) vel = -1;
        std::cout<<"Unesite koeficijente polinoma: ";
        std::vector<double> koeficijenti(vel+1);
        for (double &x : koeficijenti)
            std::cin>>x;
        IspisiMatricu(MatricniPolinom(a,koeficijenti),10 , 6, true);
    } catch (std::bad_alloc) {
        std::cout<<"Nema dovoljno memorije!\n";
    }
    UnistiMatricu(a);
}
