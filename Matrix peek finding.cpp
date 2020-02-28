#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>
#include <iomanip>

using std::cin;
using std::cout;

typedef std::vector<std::vector<int>> MatricaInt;
typedef std::vector<int> VektorInt;
int CijeliDio (double broj, double epsilon = 1e-1) {
	return int(broj+epsilon*broj);
}

MatricaInt UnesiMatricuInt (int m,int n) {
	MatricaInt mat(m,VektorInt (n));
	for (int i=0; i<m; i++)
		for(int j=0; j<n; j++)
			cin>>mat.at(i).at(j);
	return mat;
}
std::complex<double> VrhMatrice (const MatricaInt &mat, int i, int j) {
	int m = mat.size();
	if (m==0) throw std::domain_error("Nekorektna matrica");
	int n = (mat.at(0)).size();
	if (n==0) throw std::domain_error("Nekorektna matrica");
	
	for (int k=1; k<m; k++)
		if (mat.at(k).size() != n) throw std::domain_error("Nekorektna matrica");
	if (i<0 || i>=m || j<0 || j>=n) throw std::range_error("Nekorektna pocetna pozicija");
	
	int max,max_novi;
	while(true) {
		max=mat.at(i).at(j);
		max_novi=max;
		VektorInt poz_i {i-1,i-1,i,i+1,i+1,i+1,i,i-1};
		VektorInt poz_j {j,j+1,j+1,j+1,j,j-1,j-1,j-1};
		
		for (int k=0; k<8; k++)
			if (!(poz_i.at(k)<0 || poz_i.at(k)>=m || poz_j.at(k)<0 || poz_j.at(k)>=n) // Uslov za indekse u okviru matrice
			   && mat.at(poz_i.at(k)).at(poz_j.at(k)) > max_novi) {					  // Uslov za maximalni element
				max_novi=mat.at(poz_i.at(k)).at(poz_j.at(k));
				i=poz_i.at(k);
				j=poz_j.at(k);
			}
		if (max==max_novi) break;
	}
	return {double(i),double(j)};
}

int main () {
	
	cout<<"Unesite broj redova i kolona matrice:"<<std::endl;
	int m,n;
	cin>>m>>n;
	cout<<"Unesite elemente matrice:"<<std::endl;
	MatricaInt mat = UnesiMatricuInt(m,n);
	cout<<"Unesite poziciju odakle zapocinje pretraga:"<<std::endl;
	int i,j;
	cin>>i>>j;
	try {
		auto z = VrhMatrice(mat,i,j);
		cout<<"Nadjen je vrh matrice na poziciji "<<CijeliDio(z.real())<<" "<<CijeliDio(z.imag())<<std::endl
			<<"Njegova vrijednost je "<<mat.at(CijeliDio(z.real())).at(CijeliDio(z.imag()));
	}
	catch (std::domain_error izuzetak) {
		cout<<izuzetak.what();
	}
	catch (std::range_error izuzetak) {
		cout<<"Greska: "<<izuzetak.what();;
	}
	return 0;
}