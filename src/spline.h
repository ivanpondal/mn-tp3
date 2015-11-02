#ifndef SPLINE_H_
#define SPLINE_H_

#include <vector>
#include <cmath>

using namespace std;

class Spline{
	public:
		Spline();
		Spline(int n);
		Spline(const vector<int> &y);
		void recalcular(const vector<int> &y);
		double evaluar(double x);
	private:
		vector<int> a;
		vector<double> b;
		vector<double> c;
		vector<double> d;
		vector<vector<double> > factorizacionLU;
		void generarSistema(int n);
		void generarSpline(const vector<int> &y);
};

#endif // SPLINE_H_INCLUDED
