#include "poly.h"
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>

using namespace std;

Poly::Poly(): grau(-1), a(nullptr) {}

Poly::Poly(const Poly& P): grau(P.grau) {
    delete[] a;
    a = nullptr;

    if (P.grau >= 0) {  
        a = new double(grau + 1);
        for (int i = 0; i <= P.grau; i++) a[i] = P.a[i];
    }
}

Poly::Poly(Poly&& P): grau(P.grau), a(P.a) {
    P.a = nullptr;
    P.grau = 0;
}

Poly::Poly(int g): grau(g), a(nullptr) {
    if (grau > 0) {
        a = new double(grau + 1);
        for (int i = 0; i < grau; i++) a[i] = 0.0;
        a[grau] = 1.0;
    } else if (grau == 0) {
        a = new double(grau + 1);
        a[0] = 0.0;
    }
}

Poly::~Poly() {
    if (grau >= 0) delete[] a;
    grau = 0;
	a = nullptr;
}

Poly& Poly::operator=(const Poly& P) {
    if (this != &P) {
        delete[] a;

        grau = P.grau;
        if (grau >= 0) {
            a = new double(grau + 1);
            for (int i = 0; i <= grau; i++) a[i] = P.a[i];
        } else {
            a = nullptr;
        }
    }
    return *this;
}

Poly& Poly::operator=(Poly&& P) {
    delete[] a;

    grau = P.grau;
    a = P.a;

    P.grau = 0;
    P.a = nullptr;

    return *this;
}

void Poly::recriar(int g) {
    grau = g;
    if (grau >= 0) delete[] a;
	a = nullptr;

    if (grau > 0) {
        a = new double(grau+1);
        for (int i = 0; i < grau; i++) a[i] = 0.0;
        a[grau] = 1.0;
    } else if (grau == 0) {
        a = new double(1);
        a[0] = 0.0;
    }
}

bool Poly::empty() const {
    return grau < 0;
}

bool Poly::isZero() const {
    if (!empty()) return (grau == 0 && a[0] == 0);
    return false;
}

int Poly::getGrau() const {
	return grau;
}

double Poly::getCoef(int i) const {
	if (!empty()) return a[i];
	return 0.0;
}

double Poly::operator[](int i) const {
	return getCoef(i);
}

double Poly::getValor(int x) const {
	if (empty() || isZero()) return 0.0;	

	double valor = 0;
	for (int i = 0; i <= getGrau(); i++) {
		valor += getCoef(i)*pow(x,i);
	}
	return valor;
}

double Poly::operator()(int x) const {
	return getValor(x);
}

void Poly::setCoef(int i, double v) {
	if (i > grau && i < 0) {
		cerr << "Indíce inválido";
		return;
	}
	if (getGrau() == i && v == 0.0) { 
		cerr << "Valor deve ser > 0";
		return;	
	}
	a[i] = v;
}

ostream& operator<<(ostream& X, const Poly& poly) {
	if (!poly.empty()) {
		
		for (int i = poly.getGrau(); i >= 0; i--) {		
			if (poly.getCoef(i) == 0.0) {
				if (i == 0 && poly.getGrau() == 0) {
					cout << poly.getCoef(i);
				}
			} else {
				if (poly.getCoef(i) < 0.0) {
					cout << "-";
				} else {
					if (i != poly.getGrau()) {
						cout << "+";
					}
				}
				if (abs(poly.getCoef(i)) != 1.0 || i ==0) {
					cout << abs(poly.getCoef(i));
				}
				if (i != 0) {
					if (abs(poly.getCoef(i) != 1.0)) {
						cout << "*";
					}
					cout << "x";
					if (i > 1) {
						cout << "^";
						cout << i;
					}
				}
			}
		}
	}
	return X;
}

istream& operator>>(istream& X, Poly& poly) {
	if (poly.empty()) {
		cerr << "Polinômino vazio!";
	} else {
		int i = poly.getGrau();
		while (i >= 0) {
			double valor;			
			cout << "x^" << i << ": ";
			cin >> valor;
			poly.setCoef(i, valor);
			i--;
			if (i == poly.getGrau() && valor > 0.0) {
				i++;	
			}
		}
	}
	return X;
}

bool Poly::salvar(string nome_arquivo) {
	ofstream arq(nome_arquivo);
	if (!arq.is_open()) return false;
	
	arq << "POLY " << grau << endl;
	if (!empty()) {
		for (int i = 0; i <= grau; i++) {
			arq << a[i] << " ";
		}
	}

	arq << "\n";

	arq.close();
	return true;
}

bool Poly::ler(string nome_arquivo) {
	ifstream arq(nome_arquivo);
	if (!arq.is_open()) return false;

	string cabecalho;
	arq >> cabecalho;	
	if (!arq.good() || cabecalho != "POLY") {
		arq.close();
		return false;
	}	

	int grau_leitura;
	arq >> grau_leitura;
	if (arq.good() && grau_leitura < 0) {
		Poly prov;
		arq.close();
		*this = prov;
		return true;
	}

	Poly prov(grau_leitura);
	for (int i = 0; i <= prov.getGrau(); i++) {
		arq >> prov.a[i];

		if (arq.eof() || !arq.good()) {
			arq.close();

			return false;
		}
	}

	*this = prov;

	arq.close();
	return true;
}

Poly Poly::operator+(const Poly& poly) const {	
	if (poly.empty() || poly.isZero()) return Poly(*this);
	if (empty() || isZero()) return Poly(poly);

	Poly aux(max(getGrau(), poly.getGrau()));
	for (int i = 0; i <= aux.getGrau(); i++) {
		double coef_soma = 0;
		
		if (i <= getGrau()) {
			coef_soma += a[i];
		} 

		if (i <= poly.getGrau()) {
			coef_soma += poly.a[i];
		}
		
		aux.a[i] = coef_soma;
	}

	if (getGrau() != aux.getGrau()) {
		int grau_corrigido = aux.getGrau();
		while (aux.getCoef(grau_corrigido) == 0.0 && grau_corrigido > 0) grau_corrigido--;	

		Poly prov(grau_corrigido);
		for (int i = 0; i <= grau_corrigido; i++) prov.a[i] = aux.a[i];
		
		aux.grau = 0;
		delete[] aux.a;
		aux.a = nullptr;

		return prov;
	}

	return aux;
}

Poly Poly::operator-(const Poly& poly) const {
	if (empty() || isZero()) {
		Poly prov(poly.getGrau());
		for (int i = 0; i <= poly.getGrau(); i++) {
			prov.a[i] = - poly.a[i];
		}
		return prov;
	}

	Poly aux(max(getGrau(), poly.getGrau()));
	for (int i = 0; i <= grau; i++) {
		double coef_soma = 0;
		
		if (i <= getGrau()) {
			coef_soma += a[i];
		} 

		if (i <= poly.getGrau()) {
			coef_soma -= poly.a[i];
		}
		
		aux.a[i] = coef_soma;
	}

	if (getGrau() != aux.getGrau()) {
		int grau_corrigido = aux.getGrau();
		while (aux.getCoef(grau_corrigido) == 0.0 && grau_corrigido > 0) grau_corrigido--;	

		Poly prov(grau_corrigido);
		for (int i = 0; i <= grau_corrigido; i++) prov.a[i] = aux.a[i];
		
		aux.grau = 0;
		delete[] aux.a;
		aux.a = nullptr;

		return prov;
	}

	return aux;
}

Poly Poly::operator-() const {
	if (empty()) return Poly();
	if (isZero()) return Poly(0);

	Poly prov(getGrau());
	for (int i = 0; i <= prov.getGrau(); i++) {
		prov.setCoef(i, - (getCoef(i)));
	}

	return prov;
}

Poly Poly::operator*(const Poly& poly) const {
	if (empty() || poly.empty()) return Poly();
	if (isZero() || poly.isZero()) return Poly(0);

	Poly prov(max(getGrau(), poly.getGrau()));
	for (int i = 0; i <= getGrau(); i++) {
		for (int j = 0; j <= getGrau(); j++) {
			prov.a[i+j] = prov.a[i + j] + getCoef(i) * getCoef(j);
		}	
	}

	return prov;

}