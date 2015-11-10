#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#define DELTA 0.0001
#define DEBUG false

#include "mini_test.h"

#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include <ctime>
#include <stdlib.h>
#include <cstdio>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;

namespace utils {

    static void assert_precision(double valor, double esperado, double precision){
    	ASSERT(abs(valor - esperado) < precision);
    }

    static void assert_precision(double valor, double esperado){
    	assert_precision(valor, esperado, DELTA);
    }

    static void assert_interpolacion(Interpolador* interpolador, vector<double> esperados, double intervalo, double precision){
    	double xj = 0;
    	for(unsigned int i = 0; i < esperados.size(); i++){
    		if(DEBUG){
    			cout << "P(" << xj << ") = " << setprecision(15) << interpolador->evaluar(xj) << ", Expected(" << xj <<") = " << esperados[i] << endl;
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

    static void assert_interpolacion(Interpolador* interpolador, vector<double> esperados, double intervalo){
    	assert_interpolacion(interpolador, esperados, intervalo, DELTA);
    }

    static double frame_error_cuadratico_medio(const vector<vector<double> > &output, const vector<vector<double> > &real) {
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

    static double frame_peak_to_signal_noise_ratio(const vector<vector<double> > &output, const vector<vector<double> > &real) {
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

    static double video_max_error_cuadratico_medio(const vector<vector<vector<double> > > &output, const vector<vector<vector<double> > > &real) {
        ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
        ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
        ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

        int ancho = output.size();
        int alto = output[0].size();
        int frames = output[0][0].size();
        double max_error = 0;

        vector<vector<double> > frame_output(ancho, vector<double>(alto, 0));
        vector<vector<double> > frame_real(ancho, vector<double>(alto, 0));
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

    static double video_max_peak_to_signal_noise_ratio(const vector<vector<vector<double> > > &output, const vector<vector<vector<double> > > &real) {
        ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
        ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
        ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

        int ancho = output.size();
        int alto = output[0].size();
        int frames = output[0][0].size();
        double max_error = 0;

        vector<vector<double> > frame_output(ancho, vector<double>(alto, 0));
        vector<vector<double> > frame_real(ancho, vector<double>(alto, 0));
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

    static double video_prom_error_cuadratico_medio(const vector<vector<vector<double> > > &output, const vector<vector<vector<double> > > &real) {
        ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
        ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
        ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

        int ancho = output.size();
        int alto = output[0].size();
        int frames = output[0][0].size();
        int frames_iguales = 0;
        double sum_error = 0;

        vector<vector<double> > frame_output(ancho, vector<double>(alto, 0));
        vector<vector<double> > frame_real(ancho, vector<double>(alto, 0));
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

    static double video_prom_peak_to_signal_noise_ratio(const vector<vector<vector<double> > > &output, const vector<vector<vector<double> > > &real) {
        ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
        ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
        ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

        int ancho = output.size();
        int alto = output[0].size();
        int frames = output[0][0].size();
        int frames_iguales = 0;
        double sum_error = 0;

        vector<vector<double> > frame_output(ancho, vector<double>(alto, 0));
        vector<vector<double> > frame_real(ancho, vector<double>(alto, 0));
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

    static void error_cuadratico_medio_per_frame(const vector<vector<vector<double> > > &output, const vector<vector<vector<double> > > &real, vector<double> &resultados) {
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

        vector<vector<double> > frame_output(ancho, vector<double>(alto, 0));
        vector<vector<double> > frame_real(ancho, vector<double>(alto, 0));
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

    static void peak_to_signal_noise_per_frame(const vector<vector<vector<double> > > &output, const vector<vector<vector<double> > > &real, vector<double> &resultados) {
        ASSERT(output.size() != 0 && output[0].size() != 0 && output[0][0].size() != 0);
        ASSERT(real.size() != 0 && real[0].size() != 0 && real[0][0].size() != 0);
        ASSERT(output.size() == real.size() && output[0].size() == real[0].size() && output[0][0].size() <= real[0][0].size());

        int ancho = output.size();
        int alto = output[0].size();
        int frames = output[0][0].size();
        //int frames_iguales = 0;
        //double sum_error = 0;

        vector<vector<double> > frame_output(ancho, vector<double>(alto, 0));
        vector<vector<double> > frame_real(ancho, vector<double>(alto, 0));
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

    static void video_a_texto(const char* videofile, const char* textfile, int salto = 1) {
        cout << "Convirtiendo video " << videofile << " a texto " <<  textfile << " con salto " << salto << endl;
        char command[1024];
        sprintf(command, "python tools/videoToTextfile.py %s %s %d >> /dev/null",
            videofile, textfile, salto);
        if(system(command)) { cout << "videoToTextfile failed" << endl; };
    }

    static void texto_a_video(const char* textfile, const char* videofile) {
        cout << "Convirtiendo texto " << textfile << " a video " <<  videofile << endl;
        char command[1024];
        sprintf(command, "python tools/textfileToVideo.py %s %s >> /dev/null",
            textfile, videofile);
        if(system(command)) { cout << "textfileToVideo failed" << endl; };
    }

    enum Funcion : int {F_CONSTANTE = 0, F_LINEAL = 1, F_CUADRATICA = 2, F_CUBICA = 3};

    static int random_in_range(int min, int max) {
        srand (time(NULL));
        return min + (rand() % (max - min + 1));
    }

    static vector<double> generarEsperados(Funcion funcion, double rango, double incremento) {
        vector<double> ret;
        if (funcion == F_CONSTANTE) {
            int a = random_in_range(1,10);
            for (double i = 0; i < rango; i+= incremento) {
                ret.push_back(a);
            }
        } else if (funcion == F_LINEAL) {
            int a = random_in_range(1,10);
            int b = random_in_range(1,10);
            for (double i = 0; i < rango; i+= incremento) {
                ret.push_back(a + b*i);
            }

        } else if (funcion == F_CUADRATICA) {
            int a = random_in_range(1,10);
            int b = random_in_range(1,10);
            int c = random_in_range(1,10);
            for (double i = 0; i < rango; i+= incremento) {
                ret.push_back(a + b*i + c*i*i);
            }
        } else if (funcion == F_CUBICA) {
            int a = random_in_range(1,10);
            int b = random_in_range(1,10);
            int c = random_in_range(1,10);
            int d = random_in_range(1,10);
            for (double i = 0; i < rango; i+= incremento) {
                ret.push_back(a + b*i + c*i*i + d*i*i*i);
            }
        }
        return ret;
    }

    static void test_interpolacion_funcion(MetodoInterpolacion metodo, Funcion funcion, double rango, double incremento, double precision = DELTA, int bloques = 2) {
        vector<double> esperados(generarEsperados(funcion, rango, incremento));
        vector<double> y;
        int salto = (int)(double(1)/incremento);
        for (unsigned int i = 0; i < esperados.size(); i+= salto) {
            y.push_back(esperados[i]);
        }

        vector<double>::const_iterator first = esperados.begin();
        vector<double>::const_iterator last = esperados.begin() + rango*salto - salto + 1;

		if (metodo == VECINOS) {
            InterpolacionVecinos vecinos(y, salto-1);
        	assert_interpolacion(&vecinos, vector<double>(first, last), incremento, precision);
        } else if (metodo == LINEAL) {
            InterpolacionLineal lineal(y, salto-1);
        	assert_interpolacion(&lineal, vector<double>(first, last), incremento, precision);
        } else if (metodo == SPLINES) {
            Spline spline(y);
        	assert_interpolacion(&spline, vector<double>(first, last), incremento, precision);
        } else if (metodo == MULTI_SPLINES) {
            MultiSpline multi_spline(y, bloques-1);
        	assert_interpolacion(&multi_spline, vector<double>(first, last), incremento, precision);
        }
    }
}

#endif // TEST_UTILS_H_INCLUDED
