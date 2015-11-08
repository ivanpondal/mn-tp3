#ifndef VECINOS_H_
#define VECINOS_H_

#include <vector>
#include <cmath>

#include <interpolador.h>

using namespace std;

class InterpolacionVecinos: public Interpolador{
	public:
		InterpolacionVecinos();
		InterpolacionVecinos(const vector<int> &y, int valores_a_agregar);
		void recalcular(const vector<int> &y, int valores_a_agregar);
		double evaluar(double x);
	private:
		vector<int> datos_generados;
};

#endif // VECINOS_H_INCLUDED
