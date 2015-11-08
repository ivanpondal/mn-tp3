#include <lineal.h>

InterpolacionLineal::InterpolacionLineal(){
}

InterpolacionLineal::InterpolacionLineal(const vector<int> &y, int valores_a_agregar){
	recalcular(y, valores_a_agregar);
}

void InterpolacionLineal::recalcular(const vector<int> &y, int valores_a_agregar){
	// Matriz 2x2 que uso para calcular los coeficientes de cada grado del polinimio interpolador
	vector<vector<double>> polinomio_lineal(2, vector<double>(2, 0));
	datos_generados.clear();

	for(unsigned int i = 0; i < y.size() - 1; i++){
		datos_generados.push_back(y[i]);

		// Calculo polinimio usando dif divididas, posible problema con el ultimo
		polinomio_lineal[0][0] = y[i];
		double diverencia_divida = (y[i+1] - y[i]) / (double)(valores_a_agregar + 1);
		polinomio_lineal[1][0] = diverencia_divida*(-1*i);
		polinomio_lineal[1][1] = diverencia_divida;
		// Los coeficientes quedan en la segunda fila de la matriz
		polinomio_lineal[1][0] += polinomio_lineal[0][0];

		for(int k = 0; k < valores_a_agregar; k++){
			int pixel = (int)(polinomio_lineal[1][0] + polinomio_lineal[1][1]*(i + k));
			if(pixel < 0){
				pixel = 0;
			}
			else if(pixel > 255){
				pixel = 255;
			}
			datos_generados.push_back(pixel);
		}
	}
	datos_generados.push_back(y[y.size()-1]);
	cout << endl;
	for (unsigned int i = 0; i < datos_generados.size()-1; i++) {
		cout << datos_generados[i] << ", ";
	}
	cout << datos_generados[datos_generados.size()-1] << endl;
}

double InterpolacionLineal::evaluar(double x){
	int xj = (int)floor(x);
	return datos_generados[xj];
}
