#ifndef VIDEO_H_
#define VIDEO_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <spline.h>

using namespace std;

enum MetodoInterpolacion : int {VECINOS = 0, LINEAL = 1, SPLINES = 2};

class Video{
	public:
		Video();
		Video(const char* entrada, int cuadrosNuevos);
		vector<vector<vector<int> > > obtenerFramesOriginales();
		vector<vector<vector<int> > > obtenerFramesCalculados();
		void guardar(const char* salida);
		void aplicarCamaraLenta(MetodoInterpolacion metodo);
	private:
		int numero_frames;
		int numero_frames_out;
		int ancho;
		int alto;
		int fps;
		int cuadros_nuevos;
		vector<vector<vector<int> > > frames;
		vector<vector<vector<int> > > frames_out;
		void interpolarSplines();
		void interpolarLineal();
		void interpolarVecinos();
};

#endif // VIDEO_H_INCLUDED
