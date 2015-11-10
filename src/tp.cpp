#include "interpolador.h"
#include "vecinos.h"
#include "lineal.h"
#include "spline.h"
#include "multi_spline.h"
#include "video.h"
#include "test_utils.h"

#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include <ctime>
#include <cstdio>
#include <chrono>
#include <random>
#include <iomanip>
#include <utility>
#include <vector>

using namespace std;
using namespace utils;

void resolver(const char* inputfile, const char* outputfile, int metodo, int cuadros);

// ****************************** CHRONO ***************************************
static chrono::time_point<chrono::high_resolution_clock> start_time;

void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
}

// ************************* TESTS DEL GRUPO ***********************************
void test_video_a_texto() {
    video_a_texto("data/baby.avi","data/baby.txt");
}

void test_texto_a_video() {
    texto_a_video("data/baby.txt","data/baby_re.avi");
}

// f(x) = 42
void test_vecinos_constante(){
	vector<double> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	InterpolacionVecinos vecinos(y, 1);
	assert_interpolacion(&vecinos, esperados, 0.5);
}

// f(x) = x
void test_vecinos_lineal(){
	vector<double> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	InterpolacionVecinos vecinos(y, 1);
	assert_interpolacion(&vecinos, esperados, 0.5, 1);
}

// f(x) = x^2
void test_vecinos_cuadratico(){
	vector<double> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	InterpolacionVecinos vecinos(y, 1);
	assert_interpolacion(&vecinos, esperados, 0.5, 3);
}

// f(x) = 42
void test_lineal_constante(){
	vector<double> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	InterpolacionLineal lineal(y, 1);
	assert_interpolacion(&lineal, esperados, 0.5);
}

// f(x) = x
void test_lineal_lineal(){
	vector<double> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	InterpolacionLineal lineal(y, 1);
	assert_interpolacion(&lineal, esperados, 0.5);
}

// f(x) = x^2
void test_lineal_cuadratico(){
	vector<double> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	InterpolacionLineal lineal(y, 1);
	assert_interpolacion(&lineal, esperados, 0.5, 0.5);
}

// f(x) = 42
void test_spline_constante(){
	vector<double> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	Spline spline(y);
	assert_interpolacion(&spline, esperados, 0.5);
}

// f(x) = x
void test_spline_lineal(){
	vector<double> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	Spline spline(y);
	assert_interpolacion(&spline, esperados, 0.5);
}

// f(x) = x^2
void test_spline_cuadratico(){
	vector<double> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	Spline spline(y);
	assert_interpolacion(&spline, esperados, 0.5, 0.5);
}

// f(x) = 42
void test_multi_spline_un_tramo_constante(){
	vector<double> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	MultiSpline multi_spline(y, 1);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

void test_multi_spline_dos_tramos_constante(){
	vector<double> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	MultiSpline multi_spline(y, 2);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

void test_multi_spline_tres_tramos_constante(){
	vector<double> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	MultiSpline multi_spline(y, 3);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

// f(x) = x
void test_multi_spline_un_tramo_lineal(){
	vector<double> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	MultiSpline multi_spline(y, 1);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

void test_multi_spline_dos_tramos_lineal(){
	vector<double> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	MultiSpline multi_spline(y, 2);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

void test_multi_spline_tres_tramos_lineal(){
	vector<double> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	MultiSpline multi_spline(y, 3);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

// f(x) = x^2
void test_multi_spline_un_tramo_cuadratico(){
	vector<double> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	MultiSpline multi_spline(y, 1);
	assert_interpolacion(&multi_spline, esperados, 0.5, 0.5);
}

void test_multi_spline_dos_tramos_cuadratico(){
	vector<double> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	MultiSpline multi_spline(y, 2);
	assert_interpolacion(&multi_spline, esperados, 0.5, 0.5);
}

void test_multi_spline_tres_tramos_cuadratico(){
	vector<double> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	MultiSpline multi_spline(y, 3);
	assert_interpolacion(&multi_spline, esperados, 0.5, 0.5);
}

void test_vecinos_varios() {
    test_interpolacion_funcion(VECINOS, F_CONSTANTE, 50, 0.5);
    test_interpolacion_funcion(VECINOS, F_LINEAL, 50, 0.5, 10);
    test_interpolacion_funcion(VECINOS, F_CUADRATICA, 50, 0.5, 1000);
    test_interpolacion_funcion(VECINOS, F_CUBICA, 50, 0.5, 100000);
}

void test_lineal_varios() {
    test_interpolacion_funcion(LINEAL, F_CONSTANTE, 50, 0.5);
    test_interpolacion_funcion(LINEAL, F_LINEAL, 50, 0.5);
    test_interpolacion_funcion(LINEAL, F_CUADRATICA, 50, 0.5, 10);
    test_interpolacion_funcion(LINEAL, F_CUBICA, 50, 0.5, 1000);
}

void test_spline_varios() {
    test_interpolacion_funcion(SPLINES, F_CONSTANTE, 50, 0.5);
    test_interpolacion_funcion(SPLINES, F_LINEAL, 50, 0.5);
    test_interpolacion_funcion(SPLINES, F_CUADRATICA, 50, 0.5, 1);
    test_interpolacion_funcion(SPLINES, F_CUBICA, 50, 0.5, 200);
}

void test_multi_spline_varios() {
    // tamaño de bloques 2
    cout << "Bloque 2" << endl;
    test_interpolacion_funcion(MULTI_SPLINES, F_CONSTANTE, 50, 0.5, DELTA, 2);
    test_interpolacion_funcion(MULTI_SPLINES, F_LINEAL, 50, 0.5, DELTA, 2);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUADRATICA, 50, 0.5, 10, 2);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUBICA, 50, 0.5, 1000, 2);

    // tamaño de bloques 4
    cout << "Bloque 4" << endl;
    test_interpolacion_funcion(MULTI_SPLINES, F_CONSTANTE, 50, 0.5, DELTA, 4);
    test_interpolacion_funcion(MULTI_SPLINES, F_LINEAL, 50, 0.5, DELTA, 4);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUADRATICA, 50, 0.5, 5, 4);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUBICA, 50, 0.5, 500, 4);

    // tamaño de bloques 8
    cout << "Bloque 8" << endl;
    test_interpolacion_funcion(MULTI_SPLINES, F_CONSTANTE, 50, 0.5, DELTA, 8);
    test_interpolacion_funcion(MULTI_SPLINES, F_LINEAL, 50, 0.5, DELTA, 8);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUADRATICA, 50, 0.5, 1, 8);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUBICA, 50, 0.5, 200, 8);

    // tamaño de bloques 16
    cout << "Bloque 16" << endl;
    test_interpolacion_funcion(MULTI_SPLINES, F_CONSTANTE, 50, 0.5, DELTA, 16);
    test_interpolacion_funcion(MULTI_SPLINES, F_LINEAL, 50, 0.5, DELTA, 16);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUADRATICA, 50, 0.5, 1, 16);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUBICA, 50, 0.5, 200, 16);

    // tamaño de bloques 32
    cout << "Bloque 32" << endl;
    test_interpolacion_funcion(MULTI_SPLINES, F_CONSTANTE, 50, 0.5, DELTA, 32);
    test_interpolacion_funcion(MULTI_SPLINES, F_LINEAL, 50, 0.5, DELTA, 32);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUADRATICA, 50, 0.5, 1, 32);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUBICA, 50, 0.5, 200, 32);

    // tamaño de bloques 64
    cout << "Bloque 64" << endl;
    test_interpolacion_funcion(MULTI_SPLINES, F_CONSTANTE, 50, 0.5, DELTA, 64);
    test_interpolacion_funcion(MULTI_SPLINES, F_LINEAL, 50, 0.5, DELTA, 64);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUADRATICA, 50, 0.5, 1, 64);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUBICA, 50, 0.5, 200, 64);
}

// ********************** EXPERIMENTACION DEL GRUPO ****************************

void exp_error(MetodoInterpolacion metodo, int cuadros_a_agregar, const char * input_text, const char * out) {
    cout << "Calculando error al interpolar usando " << getTextForMetodo(metodo) << ", tomando del video original 1 frame de cada " << cuadros_a_agregar + 1 << ": " << endl;

    // armo los strings de inputfiles
    string input_video = input_text;
    ostringstream os_real;
    os_real << "data/video_exp_" << getTextForMetodo(metodo) << cuadros_a_agregar << "_real.txt";
    string real_text = os_real.str();
    ostringstream os_aux;
    os_aux << "data/video_exp_" << getTextForMetodo(metodo) << cuadros_a_agregar << "_aux.txt";
    string aux_text = os_aux.str();

    // convierto a texto el video original
    video_a_texto(input_video.c_str(), real_text.c_str(), 1);
    Video video_real(real_text.c_str(), 0);
    vector<vector<vector<double> > > frames_real = video_real.obtenerFramesOriginales();
    int ancho = frames_real.size();
    int alto = frames_real[0].size();

    // convierto a texto el video original, tomando un cuadro de cada 1 + cuadros_a_agregar frames
    video_a_texto(input_video.c_str(), aux_text.c_str(), 1 + cuadros_a_agregar);

    // agrego cuadros_a_agregar frames entre cada frame del output_text
    Video video(aux_text.c_str(), cuadros_a_agregar);
	  video.aplicarCamaraLenta(metodo);
    vector<vector<vector<double> > > frames_out = video.obtenerFramesCalculados();

    // calculo el error cuadratico por frame y el psnr
    int frames = frames_out[0][0].size();
    vector<double> err_per_frame_ecm(frames, 0);
    vector<double> err_per_frame_psnr(frames, 0);
    error_cuadratico_medio_per_frame(frames_out, frames_real, err_per_frame_ecm);
    peak_to_signal_noise_per_frame(frames_out, frames_real, err_per_frame_psnr);

    // escribo info en el el archiv out
    FILE *file = fopen(out, "a+");
    // fprintf(file, "# Metodo: %s\n", getTextForMetodo(metodo));
    // fprintf(file, "%s %s %s\n", "frame", "err_per_frame_ecm", "err_per_frame_psnr");

    int count = 0;
    for(int i = 0; i < frames; i++) {
        if (err_per_frame_ecm[i] != 0) {
            fprintf(file, "%d %.4f %.4f \n", count, err_per_frame_ecm[i], err_per_frame_psnr[i]);
            count++;
        }
    }

    //double err_frame_ecm = video_prom_error_cuadratico_medio(frames_out, frames_real);
    //double err_frame_psnr = video_prom_peak_to_signal_noise_ratio(frames_out, frames_real);
    fclose(file);
	// cout << "ECM promedio por frame: " << setprecision(15) << prom_err_frame_ecm << endl;
    // cout << "PSNR promedio por frame: " << setprecision(15) << prom_err_frame_psnr << endl;
}

double exp_tiempo(MetodoInterpolacion metodo, int cuadros_a_agregar, const char * input_text, const char * out) {
    string input_video = input_text;
    ostringstream os_aux;
    os_aux << "data/video_exp_" << getTextForMetodo(metodo) <<  cuadros_a_agregar <<"_aux.txt";
    string aux_text = os_aux.str();
    // convierto a texto el video original
    video_a_texto(input_video.c_str(), aux_text.c_str());
    // agrego cuadros_a_agregar frames entre cada frame del output_text
    Video video(aux_text.c_str(), cuadros_a_agregar);
    start_timer();
    video.aplicarCamaraLenta(metodo);
    double time = stop_timer();

    return time;

}

void exp_tiempo_lineal(const char* video){
  const char* input_video = video;
  const char* input_tiempo = "data/timelineal";
  FILE *file = fopen(input_tiempo, "w+");

  int cuadros_a_agregar = 6;
  int iteraciones = 4;
  vector<double> resultado(cuadros_a_agregar, 0);
  double aux = 0;
  for (int i = 0; i < cuadros_a_agregar; i++) {
    for (int j = 0; j < iteraciones; j++) {
        aux = exp_tiempo(LINEAL, i+1, input_video, input_tiempo);
        resultado[i] += aux;
    }
  }
  double time = 0;
  double dividido_k = 0;
  for (unsigned int i = 0; i < resultado.size(); i++) {
    time = resultado[i] / (double) iteraciones;
    dividido_k = time / (double) (i+1);
    fprintf(file, "%d %.4f %.4f \n", i+1, time, dividido_k);

  }

  fclose(file);

}

void exp_tiempo_vecinos(const char* video){
  const char* input_video = video;
  const char* input_tiempo = "data/timevecinos";
  FILE *file = fopen(input_tiempo, "w+");

  int cuadros_a_agregar = 6;
  int iteraciones = 4;
  vector<double> resultado(cuadros_a_agregar, 0);
  double aux = 0;
  for (int i = 0; i < cuadros_a_agregar; i++) {
    for (int j = 0; j < iteraciones; j++) {
        aux = exp_tiempo(VECINOS, i+1, input_video, input_tiempo);
        resultado[i] += aux;
    }
  }
  double time = 0;
  double dividido_k = 0;
  for (unsigned int i = 0; i < resultado.size(); i++) {
    time = resultado[i] / (double) iteraciones;
    dividido_k = time / (double) (i+1);
    fprintf(file, "%d %.4f %.4f \n", i+1, time, dividido_k);

  }

  fclose(file);

}

void exp_tiempo_splines(const char* video){
  const char* input_video = video;
  const char* input_tiempo = "data/timesplines";
  FILE *file = fopen(input_tiempo, "w+");

  int cuadros_a_agregar = 6;
  int iteraciones = 4;
  vector<double> resultado(cuadros_a_agregar, 0);
  double aux = 0;
  for (int i = 0; i < cuadros_a_agregar; i++) {
    for (int j = 0; j < iteraciones; j++) {
        aux = exp_tiempo(SPLINES, i+1, input_video, input_tiempo);
        resultado[i] += aux;
    }
  }
  double time = 0;
  double dividido_k = 0;
  for (unsigned int i = 0; i < resultado.size(); i++) {
    time = resultado[i] / (double) iteraciones;
    dividido_k = time / (double) (i+1);
    fprintf(file, "%d %.4f %.4f \n", i+1, time, dividido_k);

  }

  fclose(file);

}

void exp_artifacts(int metodo){
  const char* input[3] = {"data/messi.txt","data/skate.txt","data/sunrise.txt"};
  const char* output[3] = {"data/out/messi_out.txt","data/out/skate_out.txt","data/out/sunrise_out.txt"};
  for (int i = 0; i < 3; ++i) {
      resolver(input[i], output[i], metodo, 10);
  }
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

		// test_video_a_texto();
        // test_texto_a_video();

		// RUN_TEST(test_vecinos_constante);
		// RUN_TEST(test_vecinos_lineal);
		// RUN_TEST(test_vecinos_cuadratico);
        // RUN_TEST(test_lineal_constante);
		// RUN_TEST(test_lineal_lineal);
		// RUN_TEST(test_lineal_cuadratico);
		// RUN_TEST(test_spline_constante);
		// RUN_TEST(test_spline_lineal);
		// RUN_TEST(test_spline_cuadratico);
		// RUN_TEST(test_multi_spline_un_tramo_constante);
		// RUN_TEST(test_multi_spline_dos_tramos_constante);
		// RUN_TEST(test_multi_spline_tres_tramos_constante);
		// RUN_TEST(test_multi_spline_un_tramo_lineal);
		// RUN_TEST(test_multi_spline_dos_tramos_lineal);
		// RUN_TEST(test_multi_spline_tres_tramos_lineal);
		// RUN_TEST(test_multi_spline_un_tramo_cuadratico);
		// RUN_TEST(test_multi_spline_dos_tramos_cuadratico);
		// RUN_TEST(test_multi_spline_tres_tramos_cuadratico);

        // RUN_TEST(test_vecinos_varios);
        // RUN_TEST(test_lineal_varios);
        // RUN_TEST(test_spline_varios);
        // RUN_TEST(test_multi_spline_varios);

        // exp grupo

        // exp_tiempo_vecinos("data/funnybaby.avi");
        // exp_tiempo_lineal("data/funnybaby.avi");
        // exp_tiempo_splines("data/funnybaby.avi");

        // exp_error(VECINOS, 1, "data/messi.avi", "exp/error-messi-vecinos1");
        // exp_error(LINEAL, 1, "data/messi.avi", "exp/error-messi-lineal1");
        // exp_error(SPLINES, 1, "data/messi.avi", "exp/error-messi-spline1");
        // exp_error(VECINOS, 1, "data/sunrise.avi", "exp/error-sunrise-vecinos1");
        // exp_error(LINEAL, 1, "data/sunrise.avi", "exp/error-sunrise-lineal1");
        // exp_error(SPLINES, 1, "data/sunrise.avi", "exp/error-sunrise-spline1");
        // exp_error(VECINOS, 1, "data/skate.avi", "exp/error-skate-vecinos1");
        // exp_error(LINEAL, 1, "data/skate.avi", "exp/error-skate-lineal1");
        // exp_error(SPLINES, 1, "data/skate.avi", "exp/error-skate-spline1");

        // exp_error(VECINOS, 5, "data/messi.avi", "exp/error-messi-vecinos5");
        // exp_error(LINEAL, 5, "data/messi.avi", "exp/error-messi-lineal5");
        // exp_error(SPLINES, 5, "data/messi.avi", "exp/error-messi-spline5");
        // exp_error(VECINOS, 5, "data/sunrise.avi", "exp/error-sunrise-vecinos5");
        // exp_error(LINEAL, 5, "data/sunrise.avi", "exp/error-sunrise-lineal5");
        // exp_error(SPLINES, 5, "data/sunrise.avi", "exp/error-sunrise-spline5");
        // exp_error(VECINOS, 5, "data/skate.avi", "exp/error-skate-vecinos5");
        // exp_error(LINEAL, 5, "data/skate.avi", "exp/error-skate-lineal5");
        // exp_error(SPLINES, 5, "data/skate.avi", "exp/error-skate-spline5");

        // exp_artifacts(0);// {VECINOS = 0, LINEAL = 1, SPLINES = 2, MULTI_SPLINES = 3};
        exp_artifacts(1);
        // exp_artifacts(2);
        // exp_artifacts(3);

	} else {
        cout << "Usage: ./tp <archivo_entrada> <archivo_salida> <metodo> <cantidad_cuadros_a_agregar>" << endl;
        return 1;
    }
	return 0;
}
