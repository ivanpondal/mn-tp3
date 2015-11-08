#include <video.h>

Video::Video(){
	this->numero_frames = 0;
	this->numero_frames_out = 0;
	this->ancho = 0;
	this->alto = 0;
	this->fps = 0;
	this->cuadros_nuevos = 0;
}

Video::Video(const char* entrada, int cuadrosNuevos){
	ifstream archivo_entrada;
	archivo_entrada.open(entrada);

	if(archivo_entrada.good()){
		char sep;
		archivo_entrada >> this->numero_frames;
		archivo_entrada >> this->alto;
		archivo_entrada >> sep;
		archivo_entrada >> this->ancho;
		archivo_entrada >> this->fps;
		this->cuadros_nuevos = cuadrosNuevos;
		this->numero_frames_out = this->numero_frames + (this->numero_frames - 1)*this->cuadros_nuevos;
		this->frames = vector<vector<vector<int> > >(this->ancho, vector<vector<int> >(this->alto, vector<int>(this->numero_frames, 0)));
		this->frames_out = vector<vector<vector<int> > >(this->ancho, vector<vector<int> >(this->alto, vector<int>(this->numero_frames_out, 0)));

		for(int i = 0; i < this->numero_frames_out ; i += this->cuadros_nuevos + 1){
			for(int y = 0; y < this->alto; y++){
				for(int x = 0; x < this->ancho - 1; x++){
					archivo_entrada >> this->frames_out[x][y][i];
					this->frames[x][y][i/(this->cuadros_nuevos + 1)] = this->frames_out[x][y][i];
					archivo_entrada >> sep;
				}
				archivo_entrada >> this->frames_out[this->ancho - 1][y][i];
				this->frames[this->ancho - 1][y][i/(this->cuadros_nuevos + 1)] = this->frames_out[this->ancho - 1][y][i];
			}
		}
		archivo_entrada.close();
	}
	else{
		cout << "Error al leer el archivo " << entrada << endl;
	}
}

vector<vector<vector<int> > > Video::obtenerFramesOriginales() {
	return frames;
}
vector<vector<vector<int> > > Video::obtenerFramesCalculados() {
	return frames_out;
}

void Video::guardar(const char* salida){
	ofstream archivo_salida;
	archivo_salida.open(salida);

	archivo_salida << this->numero_frames_out << endl;
	archivo_salida << this->alto << "," << this->ancho << endl;
	archivo_salida << this->fps << endl;

	for(int i = 0; i < this->numero_frames_out; i++){
		for(int y = 0; y < this->alto; y++){
			for(int x = 0; x < this->ancho - 1; x++){
				archivo_salida << this->frames_out[x][y][i] << ",";
			}
			archivo_salida << this->frames_out[this->ancho - 1][y][i] << endl;
		}
	}
	archivo_salida.close();
}

void Video::aplicarCamaraLenta(MetodoInterpolacion metodo){
	switch(metodo){
		case VECINOS:
		cout << "Aplicando interpolación por vecinos" << endl;
			interpolarVecinos();
			break;
		case LINEAL:
			cout << "Aplicando interpolación lineal" << endl;
			interpolarLineal();
			break;
		case SPLINES:
			cout << "Aplicando interpolación cúbica" << endl;
			interpolarSplines();
			break;
		case MULTI_SPLINES:
			cout << "Aplicando interpolación cúbica de a tramos" << endl;
			interpolarMultiSplines();
			break;

	}
}

void Video::interpolarSplines(){
	Spline spline_xy (this->numero_frames);

	double spline_x;
	int spline_y;
	for(int x = 0; x < this->ancho; x++){
		for(int y = 0; y < this->alto; y++){
			spline_xy.recalcular(this->frames[x][y]);
			for(int i = 1; i < this->numero_frames_out; i += this->cuadros_nuevos + 1){
				for(int n = 0; n < this->cuadros_nuevos; n++){
					spline_x = (i - 1)/(this->cuadros_nuevos + 1) + float(n + 1)/(this->cuadros_nuevos + 1);
					spline_y = round(spline_xy.evaluar(spline_x));
					if(spline_y < 0){
						spline_y = 0;
					}
					else if(spline_y > 255){
						spline_y = 255;
					}
					this->frames_out[x][y][i + n] = spline_y;
				}
			}
		}
	}
}

void Video::interpolarMultiSplines(){
}

void Video::interpolarLineal(){
	for(int x = 0; x < this->ancho; x++){
		for(int y = 0; y < this->alto; y++){
			InterpolacionLineal lineal(this->frames[x][y], this->cuadros_nuevos);
			int count = 0;
			for (int i = 0; i < this->numero_frames; i++) {
				double aux = double(1)/double(this->cuadros_nuevos + 1);
				for (int k = 0; k < this->cuadros_nuevos + 1; k++) {
					this->frames_out[x][y][count] = lineal.evaluar(double(i) + double(k)*aux);
					count++;
				}
			}
		}
	}
}

void Video::interpolarVecinos() {
	for(int x = 0; x < this->ancho; x++){
		for(int y = 0; y < this->alto; y++){
			InterpolacionVecinos vecinos(this->frames[x][y], this->cuadros_nuevos);
			int count = 0;
			for (int i = 0; i < this->numero_frames; i++) {
				double aux = double(1)/double(this->cuadros_nuevos + 1);
				for (int k = 0; k < this->cuadros_nuevos + 1; k++) {
					this->frames_out[x][y][count] = vecinos.evaluar(double(i) + double(k)*aux);
					count++;
				}
			}
		}
	}
}
