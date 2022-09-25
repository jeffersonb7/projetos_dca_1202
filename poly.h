#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

class Poly {
    private:
        int grau;
        double *a;

    public:
        Poly();
        Poly(const Poly& P);
        Poly(Poly&& P);
        Poly(int g);
        ~Poly();

        Poly& operator=(const Poly& P);
        Poly& operator=(Poly&& P);
        
        void recriar(int grau);
        bool empty() const;
        bool isZero() const;
        int getGrau() const;
        double getCoef(int i) const;
        double getValor(int x) const;
		void setCoef(int i, double v);

        double operator[](int n) const;
        double operator()(int x) const;	

        friend std::ostream& operator<<(std::ostream& X, const Poly& poly);
        friend std::istream& operator>>(std::istream& X, Poly& poly);
        
        bool salvar(std::string nome_arquivo);
		bool ler(std::string nome_arquivo);
};