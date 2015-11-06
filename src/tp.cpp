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

void video_a_texto(const char* videofile, const char* textfile, int salto = 1) {
    char command[1024];
    sprintf(command, "python tools/videoToTextfile.py %s %s %d >> /dev/null",
        videofile, textfile, salto);
    if(system(command)) { cout << "videoToTextfile failed" << endl; };
}

void texto_a_video(const char* textfile, const char* videofile) {
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
	} else {
        cout << "Usage: ./tp <archivo_entrada> <archivo_salida> <metodo> <cantidad_cuadros_a_agregar>" << endl;
        return 1;
    }
	return 0;
}
