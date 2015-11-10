#ifndef MULTI_SPLINE_
#define MULTI_SPLINE_

#include <spline.h>

using namespace std;

class MultiSpline: public Interpolador{
	public:
		MultiSpline();
		MultiSpline(int n, int tramo);
		MultiSpline(const vector<double> &y, int tramo);
		void recalcular(const vector<double> &y);
		double evaluar(double x);
	private:
		int longitud_tramo;
		int longitud_tramo_menor;
		int numero_tramos;
		vector<Spline> tramos;
		void generarSistema(int n, int tramo);
		void generarMultiSpline(const vector<double> &y);
};
 
#endif // MULTI_SPLINE_H_INCLUDED
