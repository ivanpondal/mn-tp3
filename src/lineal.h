#ifndef LINEAL_H_
#define LINEAL_H_

#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <chrono>
#include <random>
#include <iomanip>

#include <interpolador.h>

using namespace std;

class InterpolacionLineal: public Interpolador{
	public:
		InterpolacionLineal();
		InterpolacionLineal(const vector<int> &y, int valores_a_agregar);
		void recalcular(const vector<int> &y, int valores_a_agregar);
		double evaluar(double x);
	private:
		vector<double> datos_generados;
		int valores_a_agregar;
		int cant_datos_originales;
};

#endif // LINEAL_H_INCLUDED
