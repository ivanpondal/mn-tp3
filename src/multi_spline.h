#ifndef MULTI_SPLINE_
#define MULTI_SPLINE_

#include <spline.h>

using namespace std;

class MultiSpline: public Interpolador{
	public:
		MultiSpline();
		MultiSpline(int n, int tramo);
		MultiSpline(const vector<int> &y, int tramo);
		void recalcular(const vector<int> &y);
		double evaluar(double x);
	private:
		int longitud_tramo;
		int numero_tramos;
		vector<Spline> tramos;
		void generarMultiSpline(const vector<int> &y);
};

#endif // MULTI_SPLINE_H_INCLUDED
