#ifndef SPLINE_H_
#define SPLINE_H_

#include <vector>
#include <cmath>

#include <interpolador.h>

using namespace std;

class Spline: public Interpolador{
	public:
		Spline();
		Spline(int n);
		Spline(const vector<double> &y);
		void recalcular(const vector<double> &y);
		double evaluar(double x);
	private:
		vector<double> a;
		vector<double> b;
		vector<double> c;
		vector<double> d;
		vector<vector<double> > factorizacionLU;
		void generarSistema(int n);
		void generarSpline(const vector<double> &y);
};

#endif // SPLINE_H_INCLUDED
