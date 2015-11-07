#include <lineal.h>

InterpolacionLineal::InterpolacionLineal(){
}

InterpolacionLineal::InterpolacionLineal(const vector<int> &y, int valores_a_agregar){
	recalcular(y, valores_a_agregar);
}

void InterpolacionLineal::recalcular(const vector<int> &y, int valores_a_agregar){
	datos_generados.clear();
	for(unsigned int i = 0; i < y.size() - 1; i++){
		for(int k = 0; k < valores_a_agregar/2; k++){
			datos_generados.push_back(y[i]);
		}
		for(int k = valores_a_agregar/2; k < valores_a_agregar; k++){
			datos_generados.push_back(y[i + 1]);
		}
	}
}

double InterpolacionLineal::evaluar(double x){
	int xj = (int)floor(x);
	return datos_generados[xj];
}
