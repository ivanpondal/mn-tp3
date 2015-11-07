#ifndef LINEAL_H_
#define LINEAL_H_

#include <vector>
#include <cmath>

using namespace std;

class InterpolacionLineal {
	public:
		InterpolacionLineal();
		InterpolacionLineal(const vector<int> &y, int valores_a_agregar);
		void recalcular(const vector<int> &y, int valores_a_agregar);
		double evaluar(double x);
	private:
		vector<int> datos_generados;
};

#endif // LINEAL_H_INCLUDED
