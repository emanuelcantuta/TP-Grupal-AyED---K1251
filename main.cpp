#include <iostream>
using namespace std;

void SepararPorCategoria(char* , char*, char* );
float stringToSeconds(char [], int);
void secondsToString(float, char *);
int compararTiempos (float valor, float referencia);

struct RegCorredores; // Estructura con los tiempos
struct CorredoresCiudad; // Localidades con detalles
struct ReporteCorredores; // Struct del Archivo final
int main() {

	
	
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

void SepararPorCategoria(char* archivoentrada, char* archivocategoria1, char* archivocategoria2){
	
	RegCorredores Actual;
    FILE* entrada = fopen(archivoentrada,"rb");
    
    if(!entrada){
    	cout<<" El archivo no pudo leerse correctamente";
    	return;
	}
	
	FILE* categoria1 = fopen(archivocategoria1, "wb");
	
	if(!categoria1){
    	cout<<" El archivo no pudo crearse correctamente";
    	return;
	}
	
	FILE* categoria2 = fopen(archivocategoria2, "wb");
	
		if(!categoria2){
    	cout<<" El archivo no pudo crearse correctamente";
    	return;
	}
    
    while ( fread( &Actual ,sizeof(RegCorredores), 1, entrada) == 1){
    	if(strncmp( Actual.categoria, "4 Refugios Clasica", 18 ) == 0){	
    		fwrite(&Actual, sizeof(RegCorredores), 1, categoria1);
		}
		else{
			fwrite(&Actual, sizeof(RegCorredores), 1, categoria2);
		}
	}
	fclose(entrada); 
	fclose(categoria1);
	fclose(categoria2);
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

int compararTiempos (float referencia, float valor)
{
	int diferencia = referencia - valor;
	return diferencia;
}

struct RegCorresdores {
	int numero;
	char nombreApellido[50];
	char categoria[50];
	char genero; // F/M
	char localidad[40];
	char llegada[11];
};

struct CorresdoresCiudad {
	int numero;
	char nombreApellido[50];
	char localidad[40];
	char ciudad[11];
};

struct ReporteCorredores{
	int posGral;
	int posGenero;
	int posCategoria;
	int numero;
	char nombreApellido[50];
	char categoria[50];
	char genero; // F/M
	char localidad[40];
	char llegada[11];
	char difPrimero[11];
	char difAnterior[11];
};
