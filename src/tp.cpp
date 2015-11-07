#define DELTA 0.0001
#define DEBUG false

#include "mini_test.h"
#include "spline.h"
#include "video.h"
//#include "utils.h"

#include <string>
#include <sstream>
#include <math.h>
#include <ctime>
#include <cstdio>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
//using namespace utils;

// ****************************** CHRONO ***************************************
static chrono::time_point<chrono::high_resolution_clock> start_time;

void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
}

// ****************************** AUXILIARES ***********************************

void assert_precision(double valor, double esperado, double precision){
	ASSERT(abs(valor - esperado) < precision);
}

void assert_precision(double valor, double esperado){
	assert_precision(valor, esperado, DELTA);
}

void assert_interpolacion_spline(Spline spline, vector<double> esperados, double intervalo, double precision){
	double xj = 0;
	for(unsigned int i = 0; i < esperados.size(); i++){
		if(DEBUG){
			cout << "S" << i << "(" << xj << ") = " << spline.evaluar(xj)<< endl;
		}
		// Si el intervalo equivale a un punto interpolado, el spline TIENE que
		// darme el "mismo" valor que la función original
		if(abs(xj - i) < DELTA){
			assert_precision(spline.evaluar(xj), esperados[i]);
		}
		else{
			assert_precision(spline.evaluar(xj), esperados[i], precision);
		}
		xj += intervalo;
	}
}

void assert_interpolacion_spline(Spline spline, vector<double> esperados, double intervalo){
	assert_interpolacion_spline(spline, esperados, intervalo, DELTA);
}

double frame_error_cuadratico_medio(const vector<vector<int > > &output, const vector<vector<int > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0)
    ASSERT(real.size() != 0 && real[0].size() != 0)
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size())
    int ancho = output.size();
    int alto = output[0].size();
    int sum = 0;
    for(int x = 0; x < ancho; x++){
		for(int y = 0; y < alto; y++){
            sum += pow(real[x][y] - output[x][y], 2);
        }
    }
    return double(sum)/double(ancho)*double(alto);
}

double frame_peak_to_signal_noise_ratio(const vector<vector<int > > &output, const vector<vector<int > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0)
    ASSERT(real.size() != 0 && real[0].size() != 0)
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size())

    double aux = double(pow(255,2));
    return 10*log10(aux/frame_error_cuadratico_medio(output, real));
}

int video_max_error_cuadratico_medio(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() == real[0][0].size());

    int ancho = output.size();
    int alto = output[0].size();
    int frames = output[0][0].size();
    double max_error = 0;

    vector<vector<int > > frame_output(ancho, vector<int>(alto, 0));
    vector<vector<int > > frame_real(ancho, vector<int>(alto, 0));
    for (int k = 0; k < frames; k++) {
        for(int x = 0; x < ancho; x++){
    		for(int y = 0; y < alto; y++){
                frame_output[x][y] = output[x][y][k];
                frame_real[x][y] = real[x][y][k];
            }
        }
        double error = frame_error_cuadratico_medio(frame_output, frame_real);
        if (error > max_error)  {
            max_error = error;
        }
    }

    return max_error;
}

int video_max_peak_to_signal_noise_ratio(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() == real[0][0].size());

    int ancho = output.size();
    int alto = output[0].size();
    int frames = output[0][0].size();
    double max_error = 0;

    vector<vector<int > > frame_output(ancho, vector<int>(alto, 0));
    vector<vector<int > > frame_real(ancho, vector<int>(alto, 0));
    for (int k = 0; k < frames; k++) {
        for(int x = 0; x < ancho; x++){
    		for(int y = 0; y < alto; y++){
                frame_output[x][y] = output[x][y][k];
                frame_real[x][y] = real[x][y][k];
            }
        }
        double error = frame_peak_to_signal_noise_ratio(frame_output, frame_real);
        if (error > max_error)  {
            max_error = error;
        }
    }

    return max_error;
}

int video_prom_error_cuadratico_medio(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() == real[0][0].size());

    int ancho = output.size();
    int alto = output[0].size();
    int frames = output[0][0].size();
    double sum_error = 0;

    vector<vector<int > > frame_output(ancho, vector<int>(alto, 0));
    vector<vector<int > > frame_real(ancho, vector<int>(alto, 0));
    for (int k = 0; k < frames; k++) {
        for(int x = 0; x < ancho; x++){
    		for(int y = 0; y < alto; y++){
                frame_output[x][y] = output[x][y][k];
                frame_real[x][y] = real[x][y][k];
            }
        }
        sum_error += frame_error_cuadratico_medio(frame_output, frame_real);
    }

    return sum_error/(double(ancho)*double(alto));
}

int video_prom_peak_to_signal_noise_ratio(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() == real[0][0].size());

    int ancho = output.size();
    int alto = output[0].size();
    int frames = output[0][0].size();
    double sum_error = 0;

    vector<vector<int > > frame_output(ancho, vector<int>(alto, 0));
    vector<vector<int > > frame_real(ancho, vector<int>(alto, 0));
    for (int k = 0; k < frames; k++) {
        for(int x = 0; x < ancho; x++){
    		for(int y = 0; y < alto; y++){
                frame_output[x][y] = output[x][y][k];
                frame_real[x][y] = real[x][y][k];
            }
        }
        sum_error += frame_peak_to_signal_noise_ratio(frame_output, frame_real);
    }

    return sum_error/(double(ancho)*double(alto));
}

void video_a_texto(const char* videofile, const char* textfile, int salto = 1) {
    cout << "Convirtiendo video " << videofile << " a texto " <<  textfile << " con salto " << salto << endl;
    char command[1024];
    sprintf(command, "python tools/videoToTextfile.py %s %s %d >> /dev/null",
        videofile, textfile, salto);
    if(system(command)) { cout << "videoToTextfile failed" << endl; };
}

void texto_a_video(const char* textfile, const char* videofile) {
    cout << "Convirtiendo texto " << textfile << " a video " <<  videofile << endl;
    char command[1024];
    sprintf(command, "python tools/textfileToVideo.py %s %s >> /dev/null",
        textfile, videofile);
    if(system(command)) { cout << "textfileToVideo failed" << endl; };
}

// ************************* TESTS DEL GRUPO ***********************************
void test_video_a_texto() {
    video_a_texto("data/baby.avi","data/baby.txt");
}

void test_texto_a_video() {
    texto_a_video("data/baby.txt","data/baby_re.avi");
}

// f(x) = 42
void test_spline_constante(){
	vector<int> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	Spline spline(y);
	assert_interpolacion_spline(spline, esperados, 0.5);
}

// f(x) = x
void test_spline_lineal(){
	vector<int> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	Spline spline(y);
	assert_interpolacion_spline(spline, esperados, 0.5);
}

// f(x) = x^2
void test_spline_cuadratico(){
	vector<int> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	Spline spline(y);
	assert_interpolacion_spline(spline, esperados, 0.5, 0.5);
}

void test_baby_spline_error(int cuadros_a_agregar) {
    cout << "Calculando error al interpolar usando Spline, tomando del video original 1 frame de cada " << cuadros_a_agregar + 1 << ": " << endl;

    string input_video = "data/baby.avi";
    string real_text = "data/baby_test_spline_real.txt";
    string aux_text = "data/baby_test_spline_aux.txt";

    // convierto a texto el video original
    video_a_texto(input_video.c_str(), real_text.c_str(), 1);
    Video video_real(real_text.c_str(), 0);
    vector<vector<vector<int> > > frames_real = video_real.obtenerFramesOriginales();
    int ancho = frames_real.size();
    int alto = frames_real[0].size();

    // convierto a texto el video original, tomando un cuadro de cada 1 + cuadros_a_agregar frames
    video_a_texto(input_video.c_str(), aux_text.c_str(), 1 + cuadros_a_agregar);
    // agrego un frame entre cada frame del output_text
    Video video(aux_text.c_str(), cuadros_a_agregar);
	video.aplicarCamaraLenta(SPLINES);
    vector<vector<vector<int> > > frames_out = video.obtenerFramesCalculados();

    double prom_err_frame_ecm = video_prom_error_cuadratico_medio(frames_out, frames_real);
    double prom_err_frame_psnr = video_prom_peak_to_signal_noise_ratio(frames_out, frames_real);
	cout << "ECM promedio por frame: " << setprecision(15) << prom_err_frame_ecm << endl;
    cout << "PSNR promedio por frame: " << setprecision(15) << prom_err_frame_psnr << endl;
    cout << "ECM promedio por pixel: " << setprecision(15) << prom_err_frame_ecm/(double(ancho)*double(alto)) << endl;
}

// ****************** FUNCION PARA FORMATO DE LA CATEDRA ***********************
void resolver(const char* inputfile, const char* outputfile, int metodo, int cuadros) {
    cout << "Input file: " << inputfile << endl;
    cout << "Output file: " << outputfile << endl;
	Video video(inputfile, cuadros);
	video.aplicarCamaraLenta((MetodoInterpolacion) metodo);
	video.guardar(outputfile);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	// si no hay argumentos corro tests del grupo, sino resuelvo lo que pasen por parametro
	if(argc == 5){
    	char* inputfile = argv[1];
        char* outputfile = argv[2];
        int metodo = 0;
        sscanf(argv[3], "%u", &metodo);
        int cuadros = 0;
        sscanf(argv[4], "%u", &cuadros);

		resolver(inputfile, outputfile, metodo, cuadros);
	} else if(argc == 1){
        // tests grupo
		RUN_TEST(test_spline_constante);
		RUN_TEST(test_spline_lineal);
		RUN_TEST(test_spline_cuadratico);
        // test_video_a_texto();
        // test_texto_a_video();
        test_baby_spline_error(1);
        // test_baby_spline_error(2);
	} else {
        cout << "Usage: ./tp <archivo_entrada> <archivo_salida> <metodo> <cantidad_cuadros_a_agregar>" << endl;
        return 1;
    }
	return 0;
}
