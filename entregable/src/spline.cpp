#include <spline.h>

Spline::Spline(){
}

Spline::Spline(int n){
	generarSistema(n);
}

Spline::Spline(const vector<double> &y){
	generarSistema(y.size());
	generarSpline(y);
}

void Spline::generarSistema(int n){
	vector<vector<double> > sistema(n, vector<double>(2, 0));

	// Primera y última fila
	sistema[0] = {1, 0};
	sistema[n-1] = {0, 1};

	for(int i = 1; i < n-1; i++){
		sistema[i] = {1, 4};
	}

	// Factorización LU
	for(int i = 1; i < n - 1; i++){
		double coef = sistema[i + 1][0]/sistema[i][1];
		sistema[i + 1][0] = coef; // Guardo coef de mi L
		sistema[i + 1][1] -= coef;
	}

	this->factorizacionLU = sistema;
}

void Spline::generarSpline(const vector<double> &y){
	int n = y.size();
	this->a = y;
	// Primero necesito calcular mis coeficientes "c"

	// Resuelvo triangular inferior (Lx = b)
	// b = 3*(a[i + 1] - 2*a[i] + a[i - 1])
	vector<double> x(n, 0);
	for(int i = 1 ; i < n - 1; i++){
		x[i] = 3*(this->a[i + 1] - 2*this->a[i] + this->a[i - 1]);
		x[i] -= x[i - 1]*this->factorizacionLU[i][0];
	}

	// Resuelvo triangular superior (Uc = x)
	this->c = vector<double>(n, 0);
	for(int i = n - 2; i > 0; i--){
		this->c[i] = x[i];
		this->c[i] -= this->c[i + 1];
		this->c[i] /= factorizacionLU[i][1];
	}

	// Calculo mis coeficientes "b" y "d"
	this->b = vector<double>(n, 0);
	this->d = vector<double>(n, 0);
	for(int i = 0; i < n - 1; i++){
		this->b[i] = this->a[i + 1] - this->a[i] - (2*this->c[i] + this->c[i + 1])/3;
		this->d[i] = (c[i + 1] - c[i])/3;
	}
}

void Spline::recalcular(const vector<double> &y){
	generarSpline(y);
}

double Spline::evaluar(double x){
	int xj = (int)floor(x);
	return this->a[xj] + this->b[xj]*(x - xj) + this->c[xj]*(x - xj)*(x - xj) + this->d[xj]*(x - xj)*(x - xj)*(x - xj);
}
