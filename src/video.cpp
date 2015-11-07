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

void Video::interpolarLineal(){
	// Matriz 2x2 que uso para calcular los coeficientes de cada grado del polinimio interpolador
	vector<vector<double>> polinomio_lineal(2, vector<double>(2, 0));

	int pixel = 0;
	for(int x = 0; x < this->ancho; x++){
		for(int y = 0; y < this->alto; y++){
			for(int i = 1; i < this->numero_frames_out - this->cuadros_nuevos; i += this->cuadros_nuevos + 1){
				// Calculo polinimio usando dif divididas, posible problema con el ultimo
				polinomio_lineal[0][0] = this->frames_out[x][y][i-1];
				int segundo_x = i + this->cuadros_nuevos + 1;
				double diverencia_divida = (this->frames_out[x][y][segundo_x-1] - this->frames_out[x][y][i-1]) / (double)(segundo_x - i);
				polinomio_lineal[1][0] = diverencia_divida*(-1*i);
				polinomio_lineal[1][1] = diverencia_divida;
				// Los coeficientes quedan en la segunda fila de la matriz
				polinomio_lineal[1][0] += polinomio_lineal[0][0];
				for(int n = 0; n < this->cuadros_nuevos; n++){
					pixel = (int)(polinomio_lineal[1][0] + polinomio_lineal[1][1]*(i + n));

					if(pixel < 0){
						pixel = 0;
					}
					else if(pixel > 255){
						pixel = 255;
					}
					this->frames_out[x][y][i + n] = pixel;
				}
			}
		}
	}
}

void Video::interpolarVecinos() {
	for(int x = 0; x < this->ancho; x++){
		for(int y = 0; y < this->alto; y++){
			InterpolacionVecinos vecinos(this->frames[x][y], this->cuadros_nuevos);
			for (int k = 0; k < this->numero_frames_out; k++) {
				this->frames_out[x][y][k] = vecinos.evaluar(k);
			}
		}
	}
}
