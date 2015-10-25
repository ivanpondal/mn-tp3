#include "mini_test.h"
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

static chrono::time_point<chrono::high_resolution_clock> start_time;

void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
}

void video_a_texto(const char* videofile, const char* textfile, int salto = 1) {
    char command[1024];
    sprintf(command, "octave --path tools/ --eval \"source('videoToTextfile.m'); videoToTextfile(%s, %s, %d);\" >> /dev/null",
        videofile, textfile, salto);
    if(system(command)) { cout << "videoToTextfile failed" << endl; };
}

void texto_a_video(const char* videofile, const char* textfile) {
    char command[1024];
    sprintf(command, "octave --path tools/ --eval \"source('textfileToVideo.m'); textfileToVideo(%s, %s);\" >> /dev/null",
        videofile, textfile);
    if(system(command)) { cout << "textfileToVideo failed" << endl; };
}

void resolver(const char* inputfile, const char* outputfile, int metodo, int cuadros) {
    cout << "resolver" << endl;
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
        cout << "tests grupo" << endl;
	} else {
        cout << "Usage: ./tp <archivo_entrada> <archivo_salida> <metodo> <cantidad_cuadros_a_agregar>" << endl;
        return 1;
    }
	return 0;
}
