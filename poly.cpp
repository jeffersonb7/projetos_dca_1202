#include "poly.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

Poly::Poly(): grau(-1), a(nullptr) {}

Poly::Poly(const Poly& P): grau(P.grau) {
    delete[] a;
    
    if (P.grau >= 0) {  
        a = new double(grau + 1);
        for (int i = 0; i <= P.grau; i++) a[i] = P.a[i];
    } else {
        a = nullptr;
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
    delete[] a;
    if (grau > 0) {
        a = new double(grau+1);
        for (int i = 0; i < grau; i++) a[i] = 0.0;
        a[grau] = 1.0;
    } else if (grau == 0) {
        a = new double(grau+1);
        a[0] = 0.0;
    } else {
        a = nullptr;
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
	for (int i = 0; i <= grau; i++) {
		valor += getCoef(i)*pow(x,i);
	}
	return valor;
}

double Poly::operator()(int x) const {
	return getValor(x);
}

void Poly::setCoef(int i, double v) {
	if (!(i <= grau && i >= 0)) {
		cerr << "Indíce inválido";
	} else if (i == grau && v == 0.0) { 
		cerr << "Valor deve ser > 0";
	} else {
		a[i] = v;
	}
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
		double valor;
		while (i >= 0) {
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
	if (!arq.good() || grau_leitura < 0) {
		Poly prov;
		arq.close();
		*this = prov;
		return true;
	}

	Poly prov(grau_leitura);
	for (int i = 0; i <= prov.grau; i++) {
		double valor;
		arq >> valor;

		prov.a[i] = valor;
	}

	recriar(prov.grau);
	for (int i = 0; i <= prov.grau; i++) a[i] = prov.a[i];

	arq.close();
	return true;
}