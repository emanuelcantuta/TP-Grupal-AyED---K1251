#include <iostream>
#include <cstring>

using namespace std;

struct RegCorredores {
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
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

//prototipos
void separarPorCarrera(FILE* , FILE*, FILE* );
float horarioASegundos(char [], int);
void segundosAHorario(float, char *);
int compararTiempos (float , float );
//funcion para leer
void leerArchivoConsola(char *);
//pasar a vector para trabajar en él y leer (prueba)
void cargarCorredoresVector(FILE *, RegCorredores [], int&);
void leerVectorCorredores(RegCorredores [], int);

int main() {
	char rutaEntrada[] = "Archivo corredores 4Refugios.bin";
    char rutaCategoria1[] = "categoria1.bin";
    char rutaCategoria2[] = "categoria2.bin";
    int longitud = 1000;
	RegCorredores regCorredoresV[longitud];
	
	FILE* entrada = fopen(rutaEntrada, "rb");
    FILE* categoria1 = fopen(rutaCategoria1, "wb");
    FILE* categoria2 = fopen(rutaCategoria2, "wb");

    if (!entrada || !categoria1 || !categoria2) {
        cout << "Error al abrir uno de los archivos.\n";
        return 1;
    }
    
	//funcional
	/*cargarCorredoresVector(entrada, regCorredoresV, longitud);
	leerVectorCorredores(regCorredoresV, longitud);
	*/	
	//funcional
	
    separarPorCarrera(entrada, categoria1, categoria2);
	//ver si se creo y separo correctamente el archivo
	cout << "Archivo Categoria Clasica" << endl;
	leerArchivoConsola(rutaCategoria1);
	cout << "----------------------------------------------------------------------------------------------" << endl;
	cout << "Archivo Categoria NonStop" << endl;
	leerArchivoConsola(rutaCategoria2);

	//cierre de archivos
    fclose(entrada);
    fclose(categoria1);
    fclose(categoria2);
	return 0;
}

void separarPorCarrera(FILE* entrada, FILE* categoria1, FILE* categoria2){
	RegCorredores Actual;

    while (fread(&Actual, sizeof(RegCorredores), 1, entrada) == 1) {
        if (strncmp(Actual.categoria, "4 Refugios Clasica", 18) == 0) {
            fwrite(&Actual, sizeof(RegCorredores), 1, categoria1);
        } else {
            fwrite(&Actual, sizeof(RegCorredores), 1, categoria2);
        }
    }
}

float horarioASegundos(char horario[], int longitud){
	float totalSegundos = 0.0;
    
	totalSegundos = ((horario[0]-'0')*10 + (horario[1]-'0')) * 3600
    + ((horario[3]-'0')*10 + (horario[4]-'0')) * 60
    + ((horario[6]-'0')*10 + (horario[7]-'0'))
    + (static_cast<float>(horario[9]-'0'))/10;
    
    return totalSegundos;
}

void segundosAHorario(float totalSegundos, char* resultado) {
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

int compararTiempos (float referencia, float valor){
	int diferencia = referencia - valor; 
	return diferencia;
}

void leerArchivoConsola(char *archivo){
	FILE* f = fopen(archivo, "rb");
	
	if(!f){
		cout << "No se pudo abrir el archivo: " << archivo;
		return;
	}
	
	RegCorredores reg;
	
	while(fread(&reg, sizeof(RegCorredores), 1, f) == 1){
		cout << "Numero: " << reg.numero << " | ";
		printf("%-48s  | ", reg.categoria); 
		cout << "Genero: " << reg.genero;
		cout << endl;
	}
	
	fclose(f);
}

void cargarCorredoresVector(FILE *archivo, RegCorredores regCorredoresV[], int &longitud){
	longitud = 0;
    RegCorredores temp;

    while (fread(&temp, sizeof(RegCorredores), 1, archivo) == 1) {
        regCorredoresV[longitud] = temp;
        longitud++;
    }
}

void leerVectorCorredores(RegCorredores regCorredores[], int longitud){
	for (int i = 0; i < longitud; i++) {
        cout << "-----------------------------" << endl;
        cout << "Corredor #" << i + 1 << endl; //contador
        cout << "Numero: " << regCorredores[i].numero << endl;
        cout << "Nombre y Apellido: " << regCorredores[i].nombreApellido << endl;
        cout << "Categoria: " << regCorredores[i].categoria << endl;
        cout << "Genero: " << regCorredores[i].genero << endl;
        cout << "Localidad: " << regCorredores[i].localidad << endl;
        cout << "Horario de llegada: " << regCorredores[i].llegada << endl;
    }
    cout << "-----------------------------" << endl;
}
