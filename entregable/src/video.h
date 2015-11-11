#ifndef VIDEO_H_
#define VIDEO_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <vecinos.h>
#include <lineal.h>
#include <spline.h>
#include <multi_spline.h>

using namespace std;

enum MetodoInterpolacion : int {VECINOS = 0, LINEAL = 1, SPLINES = 2, MULTI_SPLINES = 3};
static const char * MetodoInterpolacionStrings[] = { "Vecinos", "Lineal", "Splines", "Multi-Splines"};
static const char * getTextForMetodo(MetodoInterpolacion metodo) {
  return MetodoInterpolacionStrings[metodo];
}

class Video{
	public:
		Video();
		Video(const char* entrada, int cuadrosNuevos);
		vector<vector<vector<double> > > obtenerFramesOriginales();
		vector<vector<vector<double> > > obtenerFramesCalculados();
		void guardar(const char* salida);
		void aplicarCamaraLenta(MetodoInterpolacion metodo);
        void cambiarTamanioBloques(int tamanio_bloques);
	private:
		int numero_frames;
		int numero_frames_out;
		int ancho;
		int alto;
		int fps;
		int cuadros_nuevos;
        int tamanio_bloques = 5;
		vector<vector<vector<double> > > frames;
		vector<vector<vector<double> > > frames_out;
		void interpolarSplines();
		void interpolarMultiSplines(int longitud_tramo);
		void interpolarLineal();
		void interpolarVecinos();
};

#endif // VIDEO_H_INCLUDED
