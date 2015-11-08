#define DELTA 0.0001
#define DEBUG false

#include "mini_test.h"
#include "interpolador.h"
#include "vecinos.h"
#include "lineal.h"
#include "spline.h"
#include "multi_spline.h"
#include "video.h"
//#include "utils.h"

#include <string>
#include <sstream>
#include <iostream>
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

void assert_interpolacion(Interpolador* interpolador, vector<double> esperados, double intervalo, double precision){
	double xj = 0;
	for(unsigned int i = 0; i < esperados.size(); i++){
		if(DEBUG){
			cout << "P(" << xj << ") = " << interpolador->evaluar(xj) << ", Expected(" << xj <<") = " << esperados[i] << endl;
		}
		// Si el intervalo equivale a un punto interpolado, el interpolador TIENE que
		// darme el "mismo" valor que la función original
		if(abs(xj - i) < DELTA){
			assert_precision(interpolador->evaluar(xj), esperados[i]);
		} else{
			assert_precision(interpolador->evaluar(xj), esperados[i], precision);
		}
		xj += intervalo;
	}
}

void assert_interpolacion(Interpolador* interpolador, vector<double> esperados, double intervalo){
	assert_interpolacion(interpolador, esperados, intervalo, DELTA);
}

double frame_error_cuadratico_medio(const vector<vector<int > > &output, const vector<vector<int > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size());

    int ancho = output.size();
    int alto = output[0].size();
    unsigned long sum = 0;
    for(int x = 0; x < ancho; x++){
		for(int y = 0; y < alto; y++){
            sum += pow(real[x][y] - output[x][y], 2);
        }
    }
    return double(sum)/(double(ancho)*double(alto));
}

double frame_peak_to_signal_noise_ratio(const vector<vector<int > > &output, const vector<vector<int > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size());

    double aux = double(pow(255,2));
    double ecm = frame_error_cuadratico_medio(output, real);
    if (ecm < DELTA) {
        // el frame output es igual al frame real
        return 0;
    } else {
        return 10*log10(aux/ecm);
    }
}

double video_max_error_cuadratico_medio(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

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

double video_max_peak_to_signal_noise_ratio(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

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

double video_prom_error_cuadratico_medio(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

    int ancho = output.size();
    int alto = output[0].size();
    int frames = output[0][0].size();
    int frames_iguales = 0;
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
        double error = frame_error_cuadratico_medio(frame_output, frame_real);
        if (error < DELTA) {
            // el frame output es igual al frame real
            frames_iguales++;
        } else {
            sum_error += error;
        }

    }

    cout << "Hay " << frames_iguales << " frames iguales de " << frames << ", o: " << setprecision(4) << double(frames_iguales)/double(frames) << "%" << endl;

    return sum_error/double(frames - frames_iguales);
}

double video_prom_peak_to_signal_noise_ratio(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real) {
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

    int ancho = output.size();
    int alto = output[0].size();
    int frames = output[0][0].size();
    int frames_iguales = 0;
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
        double error = frame_peak_to_signal_noise_ratio(frame_output, frame_real);
        if (error < DELTA) {
            // el frame output es igual al frame real
            frames_iguales++;
        } else {
            sum_error += error;
        }

    }

    cout << "Hay " << frames_iguales << " frames iguales de " << frames << ", o: " << setprecision(4) << double(frames_iguales)/double(frames) << "%" << endl;

    return sum_error/double(frames - frames_iguales);
}

void error_cuadratico_medio_per_frame(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real, vector<double> &resultados) {
    /*
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());
    */
    int ancho = output.size();
    int alto = output[0].size();
    int frames = output[0][0].size();
    //int frames_iguales = 0;
    //double sum_error = 0;

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
        if (error < DELTA) {
            // el frame output es igual al frame real
            //frames_iguales++;
        } else {
            //sum_error += error;
            resultados[k] = error;
        }
    }

    //cout << "Hay " << frames_iguales << " frames iguales de " << frames << ", o: " << setprecision(4) << double(frames_iguales)/double(frames) << "%" << endl;

    //return sum_error/double(frames - frames_iguales);
}

void peak_to_signal_noise_per_frame(const vector<vector<vector<int> > > &output, const vector<vector<vector<int> > > &real, vector<double> &resultados) {
    ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
    ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
    ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

    int ancho = output.size();
    int alto = output[0].size();
    int frames = output[0][0].size();
    //int frames_iguales = 0;
    //double sum_error = 0;

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
        if (error < DELTA) {
            // el frame output es igual al frame real
            //frames_iguales++;
        } else {
            //sum_error += error;
            resultados[k] = error;
        }

    }

    //cout << "Hay " << frames_iguales << " frames iguales de " << frames << ", o: " << setprecision(4) << double(frames_iguales)/double(frames) << "%" << endl;

    //return sum_error/double(frames - frames_iguales);
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
// ********************** EXPERIMENTACION DEL GRUPO ****************************

void exp_error(MetodoInterpolacion metodo, int cuadros_a_agregar, const char * input_text, const char * out) {
    //cout << "Calculando error al interpolar usando " << getTextForMetodo(metodo) << ", tomando del video original 1 frame de cada " << cuadros_a_agregar + 1 << ": " << endl;
    FILE *file = fopen(out, "w+");
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


    //fprintf(file, "%s\n", metodo);
    //fprintf(file, "%s %s %s\n", frame, err_per_frame_ecm, err_per_frame_psnr);
    for(int i = 0; i < frames; i++) {
        if(err_per_frame_ecm[i] != 0) {
              fprintf(file, "%d %.4f %.4f \n", i, err_per_frame_ecm[i], err_per_frame_psnr[i]);
        }
    }

    //double err_frame_ecm = video_prom_error_cuadratico_medio(frames_out, frames_real);
    //double err_frame_psnr = video_prom_peak_to_signal_noise_ratio(frames_out, frames_real);
    fclose(file);
	  // cout << "ECM promedio por frame: " << setprecision(15) << prom_err_frame_ecm << endl;
    // cout << "PSNR promedio por frame: " << setprecision(15) << prom_err_frame_psnr << endl;
}

void exp_tiempo(MetodoInterpolacion metodo, int cuadros_a_agregar, const char * input_text, const char * out) {
    //cout << "Calculando tiempo de computo al interpolar usando " << getTextForMetodo(metodo) << ", agregando " << cuadros_a_agregar << " frames: " << endl;
    FILE *file = fopen(out, "w+");
    string input_video = input_text;
    ostringstream os_aux;
    os_aux << "data/video_exp_" << getTextForMetodo(metodo) << "_aux.txt";
    string aux_text = os_aux.str();

    // convierto a texto el video original
    video_a_texto(input_video.c_str(), aux_text.c_str());
    // agrego cuadros_a_agregar frames entre cada frame del output_text
    start_timer();
    Video video(aux_text.c_str(), cuadros_a_agregar);
	  video.aplicarCamaraLenta(metodo);
    double time = stop_timer();
    fprintf(file, "%d %.6f \n", metodo, time);
    //cout << "Tiempo de computo para todo el video: " << setprecision(15) << time << " ns." << endl;
    vector<vector<vector<int> > > frames_out = video.obtenerFramesCalculados();
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
        // exp grupo
        // exp_baby_error(SPLINES, 1);
        // exp_baby_error(LINEAL, 1);
        // exp_baby_error(VECINOS, 1);
        // exp_baby_error(SPLINES, 2);
        // exp_baby_error(LINEAL, 2);
        // exp_baby_error(VECINOS, 2);
        // exp_baby_error(SPLINES, 5);
        // exp_baby_error(LINEAL, 5);
        // exp_baby_error(VECINOS, 5);
        // exp_baby_tiempo(SPLINES, 1);
        // exp_baby_tiempo(LINEAL, 1);
        // exp_baby_tiempo(VECINOS, 1);
        // exp_baby_tiempo(SPLINES, 2);
        // exp_baby_tiempo(LINEAL, 2);
        // exp_baby_tiempo(VECINOS, 2);
        // exp_baby_tiempo(SPLINES, 5);
        // exp_baby_tiempo(LINEAL, 5);
        // exp_baby_tiempo(VECINOS, 5);

	} else {
        cout << "Usage: ./tp <archivo_entrada> <archivo_salida> <metodo> <cantidad_cuadros_a_agregar>" << endl;
        return 1;
    }
	return 0;
}
