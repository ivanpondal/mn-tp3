#include <video.h>

Video::Video(){
	this->numero_frames = 0;
	this->ancho = 0;
	this->alto = 0;
	this->fps = 0;
}

Video::Video(const char* entrada){
	ifstream archivo_entrada;
	archivo_entrada.open(entrada);

	if(archivo_entrada.good()){
		char sep;
		archivo_entrada >> this->numero_frames;
		archivo_entrada >> this->ancho;
		archivo_entrada >> sep;
		archivo_entrada >> this->alto;
		archivo_entrada >> this->fps;
		this->frames = vector<vector<vector<int> > >(this->numero_frames, vector<vector<int> >(this->alto, vector<int>(this->ancho, 0)));

		for(int i = 0; i < this->numero_frames; i++){
			for(int y = 0; y < this->alto; y++){
				for(int x = 0; x < this->ancho - 1; x++){
					archivo_entrada >> this->frames[i][y][x];
					archivo_entrada >> sep;
				}
				archivo_entrada >> this->frames[i][y][this->ancho - 1];
			}
		}
		archivo_entrada.close();
	}
	else{
		cout << "Error al leer el archivo " << entrada << endl;
	}
}

void Video::guardar(const char* salida){
	ofstream archivo_salida;
	archivo_salida.open(salida);

	archivo_salida << this->numero_frames << endl;
	archivo_salida << this->ancho << "," << this->alto << endl;
	archivo_salida << this->fps << endl;

	for(int i = 0; i < this->numero_frames; i++){
		for(int y = 0; y < this->alto; y++){
			for(int x = 0; x < this->ancho - 1; x++){
				archivo_salida << this->frames[i][y][x] << ",";
			}
			archivo_salida << this->frames[i][y][this->ancho - 1] << endl;
		}
	}
	archivo_salida.close();
}
