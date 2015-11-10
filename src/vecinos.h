#ifndef VECINOS_H_
#define VECINOS_H_

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

class InterpolacionVecinos: public Interpolador{
	public:
		InterpolacionVecinos();
		InterpolacionVecinos(const vector<double> &y, int valores_a_agregar);
		void recalcular(const vector<double> &y, int valores_a_agregar);
		double evaluar(double x);
	private:
		vector<double> datos_generados;
		int valores_a_agregar;
		int cant_datos_originales;
};

#endif // VECINOS_H_INCLUDED
