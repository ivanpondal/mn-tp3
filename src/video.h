#ifndef VIDEO_H_
#define VIDEO_H_

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

enum MetodoInterpolacion : int {VECINOS = 0, LINEAL = 1, SPLINES = 2};

class Video{
	public:
		Video();
		Video(const char* entrada);
		void guardar(const char* salida);
		void aplicarCamaraLenta(MetodoInterpolacion metodo, int cuadrosNuevos);
	private:
		int numero_frames;
		int ancho;
		int alto;
		int fps;
		vector<vector<vector<int> > > frames;
		void interpolarSplines(int cuadrosNuevos);
};

#endif // VIDEO_H_INCLUDED
