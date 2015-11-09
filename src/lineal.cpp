#include <lineal.h>

InterpolacionLineal::InterpolacionLineal(){
}

InterpolacionLineal::InterpolacionLineal(const vector<int> &y, int valores_a_agregar){
	recalcular(y, valores_a_agregar);
}

void InterpolacionLineal::recalcular(const vector<int> &y, int valores_a_agregar){
	this->cant_datos_originales = y.size();
	this->valores_a_agregar = valores_a_agregar;

	datos_generados.clear();

	for(unsigned int i = 0; i < y.size() - 1; i++){
		datos_generados.push_back(double(y[i]));

		double dif_dividida_cero = y[i];
		double dif_dividida_uno = double(y[i+1] - y[i]) / double(valores_a_agregar+1);

		for(int k = 0; k < valores_a_agregar; k++){
			double pixel = dif_dividida_cero + dif_dividida_uno*double(k + 1);
			datos_generados.push_back(pixel);
		}
	}
	datos_generados.push_back(double(y[y.size()-1]));
}

double InterpolacionLineal::evaluar(double x){
	double aux = x * double(valores_a_agregar+1);
	int xj = (int)floor(aux);
	//cout << x << " -> " << xj << endl;
	return datos_generados[xj];
}
