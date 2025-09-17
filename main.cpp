#include <iostream>
using namespace std;

float stringToSeconds(char [], int);
void secondsToString(float, char *);

int main() {
    char horas[11] = {'1','2',':','3','5',':','1','6','.','2','\0'};
    
	cout << "Horario original: " << horas << endl;

    float totalSegundos = stringToSeconds(horas, 11);
    cout << "Total en segundos: " << totalSegundos << endl;

    char resultado[11];
    secondsToString(totalSegundos, resultado);
    cout << "Horario reconstruido: " << resultado << endl;
    
	return 0;
}

float stringToSeconds(char horario[], int longitud){
	float totalSegundos = 0.0;
    
	totalSegundos = ((horario[0]-'0')*10 + (horario[1]-'0')) * 3600
    + ((horario[3]-'0')*10 + (horario[4]-'0')) * 60
    + ((horario[6]-'0')*10 + (horario[7]-'0'))
    + (static_cast<float>(horario[9]-'0'))/10;
    
    return totalSegundos;
}

void secondsToString(float totalSegundos, char* resultado) {
	totalSegundos *= 10;	
	int aux = totalSegundos;
	
	int horasAux = aux / 36000;
    int minAux = aux % 36000;
    int segAux = minAux % 600;
    minAux = minAux / 600;
    int decAux = segAux % 10;
    segAux = segAux / 10;
    
    resultado[0] = (horasAux / 10) + 48;
    resultado[1] = (horasAux % 10) + 48;
    resultado[2] = ':';
    resultado[3] = (minAux / 10) + 48;
    resultado[4] = (minAux % 10) + 48;
    resultado[5] = ':';
    resultado[6] = (segAux / 10) + 48;
    resultado[7] = (segAux % 10) + 48;
    resultado[8] = '.';
    resultado[9] = decAux + 48;
    resultado[10] = '\0';
}	
	
	
