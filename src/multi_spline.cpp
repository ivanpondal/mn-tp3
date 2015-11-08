#include <multi_spline.h>

MultiSpline::MultiSpline(){
}

MultiSpline::MultiSpline(int n, int tramo){
	this->numero_tramos = n/tramo - 1;
	this->tramos = vector<Spline> (this->numero_tramos, Spline(tramo + 1));
	this->longitud_tramo = tramo;
}

MultiSpline::MultiSpline(const vector<int>&y, int tramo){
	this->numero_tramos = y.size()/tramo - 1;
	this->tramos = vector<Spline> (this->numero_tramos, Spline(tramo + 1));
	this->longitud_tramo = tramo;
	generarMultiSpline(y);
}

void MultiSpline::recalcular(const vector<int> &y){
	generarMultiSpline(y);
}

void MultiSpline::generarMultiSpline(const vector<int> &y){
	vector<int>::const_iterator it = y.begin();
	for(int i = 0; i < this->numero_tramos; i++){
		this->tramos[i].recalcular(vector<int>(it, it + this->longitud_tramo + 1));
		it += this->longitud_tramo;
	}
}

double MultiSpline::evaluar(double x){
	int xj = (int)floor(x)/this->longitud_tramo;
	return (xj < this->numero_tramos) ?
		this->tramos[xj].evaluar(x - xj) : this->tramos[xj - 1].evaluar(this->longitud_tramo);
}
