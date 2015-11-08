#include <vecinos.h>

InterpolacionVecinos::InterpolacionVecinos(){
}

InterpolacionVecinos::InterpolacionVecinos(const vector<int> &y, int valores_a_agregar){
	recalcular(y, valores_a_agregar);
}

void InterpolacionVecinos::recalcular(const vector<int> &y, int valores_a_agregar){
	this->cant_datos_originales = y.size();
	this->valores_a_agregar = valores_a_agregar;

	datos_generados.clear();
	for(unsigned int i = 0; i < y.size() - 1; i++){
		datos_generados.push_back(y[i]);
		for(int k = 0; k < valores_a_agregar/2; k++){
			datos_generados.push_back(y[i]);
		}
		for(int k = valores_a_agregar/2; k < valores_a_agregar; k++){
			datos_generados.push_back(y[i + 1]);
		}
	}
	datos_generados.push_back(y[y.size()-1]);
}

double InterpolacionVecinos::evaluar(double x){
	double aux = x * double(valores_a_agregar+1);
	return datos_generados[(int)floor(aux)];
}
