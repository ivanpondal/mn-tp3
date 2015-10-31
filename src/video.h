#ifndef VIDEO_H_
#define VIDEO_H_

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Video{
	public:
		Video();
		Video(const char* entrada);
		void guardar(const char* salida);
	private:
		int numero_frames;
		int ancho;
		int alto;
		int fps;
		vector<vector<vector<int> > > frames;
};

#endif // VIDEO_H_INCLUDED
