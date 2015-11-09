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
	vector<int> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	InterpolacionVecinos vecinos(y, 1);
	assert_interpolacion(&vecinos, esperados, 0.5);
}

// f(x) = x
void test_vecinos_lineal(){
	vector<int> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	InterpolacionVecinos vecinos(y, 1);
	assert_interpolacion(&vecinos, esperados, 0.5, 1);
}

// f(x) = x^2
void test_vecinos_cuadratico(){
	vector<int> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	InterpolacionVecinos vecinos(y, 1);
	assert_interpolacion(&vecinos, esperados, 0.5, 3);
}

// f(x) = 42
void test_lineal_constante(){
	vector<int> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	InterpolacionLineal lineal(y, 1);
	assert_interpolacion(&lineal, esperados, 0.5);
}

// f(x) = x
void test_lineal_lineal(){
	vector<int> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	InterpolacionLineal lineal(y, 1);
	assert_interpolacion(&lineal, esperados, 0.5);
}

// f(x) = x^2
void test_lineal_cuadratico(){
	vector<int> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	InterpolacionLineal lineal(y, 1);
	assert_interpolacion(&lineal, esperados, 0.5, 0.5);
}

// f(x) = 42
void test_spline_constante(){
	vector<int> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	Spline spline(y);
	assert_interpolacion(&spline, esperados, 0.5);
}

// f(x) = x
void test_spline_lineal(){
	vector<int> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	Spline spline(y);
	assert_interpolacion(&spline, esperados, 0.5);
}

// f(x) = x^2
void test_spline_cuadratico(){
	vector<int> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	Spline spline(y);
	assert_interpolacion(&spline, esperados, 0.5, 0.5);
}

// f(x) = 42
void test_multi_spline_un_tramo_constante(){
	vector<int> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	MultiSpline multi_spline(y, 1);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

void test_multi_spline_dos_tramos_constante(){
	vector<int> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	MultiSpline multi_spline(y, 2);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

void test_multi_spline_tres_tramos_constante(){
	vector<int> y = {42, 42, 42, 42};
	vector<double> esperados = {42, 42, 42, 42, 42, 42, 42};
	MultiSpline multi_spline(y, 3);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

// f(x) = x
void test_multi_spline_un_tramo_lineal(){
	vector<int> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	MultiSpline multi_spline(y, 1);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

void test_multi_spline_dos_tramos_lineal(){
	vector<int> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	MultiSpline multi_spline(y, 2);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

void test_multi_spline_tres_tramos_lineal(){
	vector<int> y = {0, 1, 2, 3};
	vector<double> esperados = {0, 0.5, 1, 1.5, 2, 2.5, 3};
	MultiSpline multi_spline(y, 3);
	assert_interpolacion(&multi_spline, esperados, 0.5);
}

// f(x) = x^2
void test_multi_spline_un_tramo_cuadratico(){
	vector<int> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	MultiSpline multi_spline(y, 1);
	assert_interpolacion(&multi_spline, esperados, 0.5, 0.5);
}

void test_multi_spline_dos_tramos_cuadratico(){
	vector<int> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	MultiSpline multi_spline(y, 2);
	assert_interpolacion(&multi_spline, esperados, 0.5, 0.5);
}

void test_multi_spline_tres_tramos_cuadratico(){
	vector<int> y = {0, 1, 4, 9};
	vector<double> esperados = {0, 0.25, 1, 2.25, 4, 6.25, 9};
	MultiSpline multi_spline(y, 3);
	assert_interpolacion(&multi_spline, esperados, 0.5, 0.5);
}

void test_vecinos_varios() {
    test_interpolacion_funcion(VECINOS, F_CONSTANTE, 50, 0.5);
    test_interpolacion_funcion(VECINOS, F_LINEAL, 50, 0.5, 10);
    test_interpolacion_funcion(VECINOS, F_CUADRATICA, 50, 0.5, 1000);
    test_interpolacion_funcion(VECINOS, F_CUBICA, 50, 0.5, 10000);
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
    test_interpolacion_funcion(SPLINES, F_CUBICA, 50, 0.5, 10);
}

void test_multi_spline_varios() {
    // tamaño de bloques 2
    test_interpolacion_funcion(MULTI_SPLINES, F_CONSTANTE, 50, 0.5, DELTA, 2);
    test_interpolacion_funcion(MULTI_SPLINES, F_LINEAL, 50, 0.5, DELTA, 2);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUADRATICA, 50, 0.5, 10, 2);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUBICA, 50, 0.5, 1000, 2);

    // tamaño de bloques 4
    test_interpolacion_funcion(MULTI_SPLINES, F_CONSTANTE, 50, 0.5, DELTA, 4);
    test_interpolacion_funcion(MULTI_SPLINES, F_LINEAL, 50, 0.5, DELTA, 4);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUADRATICA, 50, 0.5, 10, 4);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUBICA, 50, 0.5, 1000, 4);

    // tamaño de bloques 8
    test_interpolacion_funcion(MULTI_SPLINES, F_CONSTANTE, 50, 0.5, DELTA, 8);
    test_interpolacion_funcion(MULTI_SPLINES, F_LINEAL, 50, 0.5, DELTA, 8);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUADRATICA, 50, 0.5, 10, 8);
    test_interpolacion_funcion(MULTI_SPLINES, F_CUBICA, 50, 0.5, 1000, 8);
}

// ********************** EXPERIMENTACION DEL GRUPO ****************************

void exp_error(MetodoInterpolacion metodo, int cuadros_a_agregar, const char * input_text, const char * out) {
    //cout << "Calculando error al interpolar usando " << getTextForMetodo(metodo) << ", tomando del video original 1 frame de cada " << cuadros_a_agregar + 1 << ": " << endl;
    FILE *file = fopen(out, "a+");
    string input_video = input_text;
    ostringstream os_real;
    os_real << "data/video_exp_" << getTextForMetodo(metodo) << "_real.txt";
    string real_text = os_real.str();

    ostringstream os_aux;
    os_aux << "data/video_exp_" << getTextForMetodo(metodo) << "_aux.txt";
    string aux_text = os_aux.str();

    // convierto a texto el video original
    video_a_texto(input_video.c_str(), real_text.c_str(), 1);
    Video video_real(real_text.c_str(), 0);
    vector<vector<vector<int> > > frames_real = video_real.obtenerFramesOriginales();
    int ancho = frames_real.size();
    int alto = frames_real[0].size();

    // convierto a texto el video original, tomando un cuadro de cada 1 + cuadros_a_agregar frames
    video_a_texto(input_video.c_str(), aux_text.c_str(), 1 + cuadros_a_agregar);
    // agrego cuadros_a_agregar frames entre cada frame del output_text
    Video video(aux_text.c_str(), cuadros_a_agregar);
	  video.aplicarCamaraLenta(metodo);
    vector<vector<vector<int> > > frames_out = video.obtenerFramesCalculados();

    int frames = frames_out[0][0].size();

    vector<double> err_per_frame_ecm(frames, 0);
    vector<double> err_per_frame_psnr(frames, 0);

    error_cuadratico_medio_per_frame(frames_out, frames_real, err_per_frame_ecm);
    peak_to_signal_noise_per_frame(frames_out, frames_real, err_per_frame_psnr);

    fprintf(file, "%s %s %s\n", "frame", "err_per_frame_ecm", "err_per_frame_psnr");
    fprintf(file, "%s %s\n","Metodo: ", getTextForMetodo(metodo));

    vector<pair<int, double> > resultado_ecm;
    vector<pair<int, double> > resultado_psnr;
    pair<int, double> aux_ecm;
    pair<int, double> aux_psnr;
    for(int i = 0; i < frames; i++) {
        if(err_per_frame_ecm[i] != 0) {
            aux_ecm = make_pair(i, err_per_frame_ecm[i]);
            aux_psnr = make_pair(i, err_per_frame_psnr[i]);
            resultado_ecm.push_back(aux_ecm);
            resultado_psnr.push_back(aux_psnr);
        }
    }

    for(unsigned int i = 0; i < resultado_ecm.size(); i++) {
        fprintf(file, "%d %.4f %.4f \n", resultado_ecm[i].first , resultado_ecm[i].second, resultado_psnr[i].second);
    }


    //double err_frame_ecm = video_prom_error_cuadratico_medio(frames_out, frames_real);
    //double err_frame_psnr = video_prom_peak_to_signal_noise_ratio(frames_out, frames_real);
    fclose(file);
	  // cout << "ECM promedio por frame: " << setprecision(15) << prom_err_frame_ecm << endl;
    // cout << "PSNR promedio por frame: " << setprecision(15) << prom_err_frame_psnr << endl;
}

void exp_tiempo(MetodoInterpolacion metodo, int cuadros_a_agregar, const char * input_text, const char * out) {
    //cout << "Calculando tiempo de computo al interpolar usando " << getTextForMetodo(metodo) << ", agregando " << cuadros_a_agregar << " frames: " << endl;
    FILE *file = fopen(out, "a+");
    string input_video = input_text;
    ostringstream os_aux;
    os_aux << "data/video_exp_" << getTextForMetodo(metodo) <<  cuadros_a_agregar <<"_aux.txt";
    string aux_text = os_aux.str();

    // convierto a texto el video original

    video_a_texto(input_video.c_str(), aux_text.c_str());

    // agrego cuadros_a_agregar frames entre cada frame del output_text
    start_timer();

    Video video(aux_text.c_str(), cuadros_a_agregar);

    video.aplicarCamaraLenta(metodo);

    double time = stop_timer();

    fprintf(file, "%s %s %.6f \n","Metodo: ", getTextForMetodo(metodo), time);
    //cout << "Tiempo de computo para todo el video: " << setprecision(15) << time << " ns." << endl;
    //vector<vector<vector<int> > > frames_out = video.obtenerFramesCalculados();
    //cout << "Tiempo de computo promedio por frame: " << setprecision(15) << time/double(frames_out[0][0].size()) << " ns." << endl;
    //cout << "Tiempo de computo promedio por pixel: " << setprecision(15) << time/double(frames_out.size() * frames_out[0].size() *frames_out[0][0].size()) << " ns." << endl;

    fclose(file);


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

/*		RUN_TEST(test_vecinos_constante);
		RUN_TEST(test_vecinos_lineal);
		RUN_TEST(test_vecinos_cuadratico);
        RUN_TEST(test_lineal_constante);
		RUN_TEST(test_lineal_lineal);
		RUN_TEST(test_lineal_cuadratico);
		RUN_TEST(test_spline_constante);
		RUN_TEST(test_spline_lineal);
		RUN_TEST(test_spline_cuadratico);
		*/
    /*
		RUN_TEST(test_multi_spline_un_tramo_constante);
		RUN_TEST(test_multi_spline_dos_tramos_constante);
		RUN_TEST(test_multi_spline_tres_tramos_constante);
		RUN_TEST(test_multi_spline_un_tramo_lineal);
		RUN_TEST(test_multi_spline_dos_tramos_lineal);
		RUN_TEST(test_multi_spline_tres_tramos_lineal);
		RUN_TEST(test_multi_spline_un_tramo_cuadratico);
		RUN_TEST(test_multi_spline_dos_tramos_cuadratico);
		RUN_TEST(test_multi_spline_tres_tramos_cuadratico);
    */

        RUN_TEST(test_vecinos_varios);
        RUN_TEST(test_lineal_varios);
        RUN_TEST(test_spline_varios);
        RUN_TEST(test_multi_spline_varios)


        // exp grupo
        /*
        const char* input_error = "data/error_funnybaby";
        FILE *file = fopen(input_error, "w+");
        fclose(file);
        exp_error(SPLINES, 1,"data/funnybaby.avi",input_error);
        exp_error(SPLINES, 2,"data/funnybaby.avi",input_error);
        exp_error(SPLINES, 3,"data/funnybaby.avi",input_error);

        exp_error(LINEAL, 1,"data/funnybaby.avi",input_error);
        exp_error(SPLINES, 1,"data/funnybaby.avi",input_error);
        */

        const char* input_tiempo = "data/time_funnybaby";
        FILE *file = fopen(input_tiempo, "w+");
        fclose(file);
        // PROBLEMA DOUBLE FREE CUANDO EJECUTA CON CUADROS NUEVOS 1 Y DESPUES 2 en exp_tiempo CON VECINOS Y LINEAL
        exp_tiempo(LINEAL, 1,"data/funnybaby.avi", input_tiempo);
        exp_tiempo(LINEAL, 2,"data/funnybaby.avi",input_tiempo);
        exp_tiempo(SPLINES, 3,"data/funnybaby.avi",input_tiempo);

        /*
        exp_tiempo(LINEAL, i,"data/funnybaby.avi","data/time_funnybaby", input_tiempo);
        exp_tiempo(SPLINES, i,"data/funnybaby.avi","data/time_funnybaby", input_tiempo);
        */
	} else {
        cout << "Usage: ./tp <archivo_entrada> <archivo_salida> <metodo> <cantidad_cuadros_a_agregar>" << endl;
        return 1;
    }
	return 0;
}
