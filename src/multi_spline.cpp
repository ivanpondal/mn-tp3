#include <multi_spline.h>

MultiSpline::MultiSpline(){
}

MultiSpline::MultiSpline(int n, int tramo){
	generarSistema(n, tramo);
}

MultiSpline::MultiSpline(const vector<int>&y, int tramo){
	generarSistema(y.size(), tramo);
	generarMultiSpline(y);
}

void MultiSpline::recalcular(const vector<int> &y){
	generarMultiSpline(y);
}

void MultiSpline::generarSistema(int n, int tramo){
	this->numero_tramos = (n - 1)/tramo;
	this->longitud_tramo = tramo;
	this->longitud_tramo_menor = (n - 1)%tramo;
	if(this->longitud_tramo_menor != 0){
		this->numero_tramos++;
		this->tramos = vector<Spline> (this->numero_tramos, Spline(this->longitud_tramo + 1));
		this->tramos[this->numero_tramos - 1] = Spline(this->longitud_tramo_menor + 1);
	}
	else{
		this->tramos = vector<Spline> (this->numero_tramos, Spline(this->longitud_tramo + 1));
	}
}

void MultiSpline::generarMultiSpline(const vector<int> &y){
	vector<int>::const_iterator it = y.begin();
	for(int i = 0; i < this->numero_tramos - 1; i++){
		this->tramos[i].recalcular(vector<int>(it, it + this->longitud_tramo + 1));
		it += this->longitud_tramo;
	}

	if(this->longitud_tramo_menor != 0){
		this->tramos[this->numero_tramos - 1].recalcular(vector<int>(it, it + this->longitud_tramo_menor + 1));
	}
	else{
		this->tramos[this->numero_tramos - 1].recalcular(vector<int>(it, it + this->longitud_tramo + 1));
	}
}

double MultiSpline::evaluar(double x){
	int xj = (int)floor(x);
	double result = 0;

	if(this->longitud_tramo_menor != 0 && xj >= (this->numero_tramos - 1)*this->longitud_tramo){
		xj = (this->numero_tramos - 1)*this->longitud_tramo;
		result = this->tramos[this->numero_tramos - 1].evaluar(x - xj);
	}
	else{
		if(xj == this->numero_tramos*this->longitud_tramo){
			xj = this->longitud_tramo;
			result = this->tramos[this->numero_tramos - 1].evaluar(xj);
		}
		else{
			xj /= this->longitud_tramo;
			result = this->tramos[xj].evaluar(x - xj*this->longitud_tramo);
		}
	}
	return result;
}
