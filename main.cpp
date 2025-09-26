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

struct CorredoresCiudad {
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
void cargarCorredoresVector(FILE *, RegCorredores [], int&);
float horarioASegundos(char []);
void segundosAHorario(float, char *);
void ordenarPorTiempo (RegCorredores [], int);
void separarEnVectoresPorCarrera(RegCorredores [], int, RegCorredores [], int&, RegCorredores [], int&);
void cargarArchivoConVector(RegCorredores [], int, FILE *);
void separarPorCarrera(RegCorredores [], int, FILE *, FILE *);
int compararTiempos (float , float );
void cambioStruct (RegCorredores [], int, ReporteCorredores []);
void calcularPosiciones(ReporteCorredores [] , int);
void imprimirReporte(ReporteCorredores [], int);
void noTermino(RegCorredores [], int longitud);
//funcion para leer
void leerArchivoConsola(const char *);

//pasar a vector para trabajar en él y leer (prueba)
void leerVectorCorredores(RegCorredores [], int);
void calcularTiempos(ReporteCorredores[], int);
int main() {
	const char rutaEntrada [] = "Archivo corredores 4Refugios.bin";
	const char rutaCiudades [] = "Ciudades.bin";
	const char rutaCategoria1 [] = "categoria1.bin";
	const char rutaCategoria2 [] = "categoria2.bin";
	
    int longitud = 1000; //longitud base general
	int longitud1 = 0; //longitud para vector Clasica
	int longitud2 = 0; //longitud para vector NonStop
	
	RegCorredores regCorredoresV[longitud];
	RegCorredores categoria1V[longitud]; 
	RegCorredores categoria2V[longitud]; 

	ReporteCorredores reporte1V[longitud];
	ReporteCorredores reporte2V[longitud];

	FILE* entrada = fopen(rutaEntrada, "rb");
    FILE* categoria1 = fopen(rutaCategoria1, "wb");
    FILE* categoria2 = fopen(rutaCategoria2, "wb");

    if (!entrada || !categoria1 || !categoria2) {
        cout << "Error al abrir uno de los archivos.\n";
        return 1;
    }

	cargarCorredoresVector(entrada, regCorredoresV, longitud);
	noTermino(regCorredoresV, longitud);
	ordenarPorTiempo(regCorredoresV,longitud);
	
	separarEnVectoresPorCarrera(regCorredoresV, longitud, categoria1V, longitud1, categoria2V,longitud2);
	cout << "Vector Categoria Clasica" << endl;
	leerVectorCorredores(categoria1V,longitud1); //categoria: 4 Refugios Clasica
	cout << "----------------------------------------------------------------------------------------------" << endl;
	cout << "Archivo Categoria NonStop" << endl;
	leerVectorCorredores(categoria2V,longitud2); //categoria: 4 Refugios NonStop


	//Cargo los archivos mediante los vectores de las 2 categorias
	cargarArchivoConVector(categoria1V, longitud1, categoria1);
	cargarArchivoConVector(categoria2V, longitud2, categoria2);	
	
	//cierre de archivos, cerrando los archivos porque ya se pasaron a un vector
    fclose(entrada);
    fclose(categoria1);
    fclose(categoria2);
	
	//cambiandolo a modo lectura para que se pueda leer 
	categoria1 = fopen(rutaCategoria1, "rb");
	categoria2 = fopen(rutaCategoria2, "rb");
	
	//ver si se creo y separo correctamente el archivo
	cout << "Archivo Categoria Clasica" << endl;
	leerArchivoConsola(rutaCategoria1);
	cout << "----------------------------------------------------------------------------------------------" << endl;
	cout << "Archivo Categoria NonStop" << endl;
	leerArchivoConsola(rutaCategoria2);

	//TRANSFORMACION DE VECTOR CON FORMATO RegCorredores a ReporteCorredores (no hace nada aún, Base ESTRUCTURA PARA EL REPORTE)
	cambioStruct(categoria1V,longitud1,reporte1V);
	cambioStruct(categoria2V,longitud2,reporte2V);
 	
	calcularPosiciones(reporte1V , longitud1);
	calcularPosiciones(reporte2V , longitud2);

	//falta funcion de tiempos
	calcularTiempos(reporte1V, longitud1);
	calcularTiempos(reporte2V, longitud2);
	//
	imprimirReporte(reporte1V, longitud1);
	imprimirReporte(reporte2V, longitud2);

	fclose(categoria1);
    fclose(categoria2);	
	return 0;
}

void noTermino(RegCorredores vectorEntrada[], int longitud){
	for(int i = 0; i < longitud; i++){	
		if (strncmp(vectorEntrada[i].llegada, "DNF", 3) == 0 ||	strncmp(vectorEntrada[i].llegada, "DSQ", 3) == 0 ){
			strcpy(vectorEntrada[i].llegada, "No termino");
		}
	}	
}

void cargarCorredoresVector(FILE *archivo, RegCorredores regCorredoresV[], int &longitud){
	longitud = 0;
    RegCorredores temp;

    while (fread(&temp, sizeof(RegCorredores), 1, archivo) == 1) {
        regCorredoresV[longitud] = temp;
        longitud++;
    }
}

float horarioASegundos(char horario[]){
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

void ordenarPorTiempo (RegCorredores vector[],int longitud){
	RegCorredores aux;
	float tiempo1 = 0.0, tiempo2 = 0.0;

	for (int i = 0; i < longitud - 1; i++){

		for (int j = 0;j<longitud - i - 1;j++){
			tiempo1 = horarioASegundos(vector[j].llegada);
			tiempo2 = horarioASegundos(vector[j+1].llegada);

			if (tiempo1>tiempo2){
				aux = vector[j];
				vector[j] = vector [j+1];
				vector [j+1] = aux;
			}
		}
	}
}

void separarEnVectoresPorCarrera(RegCorredores vectorCorredores[], int longitud, RegCorredores categoria1[], int &long1, RegCorredores categoria2[], int &long2){
	for (int i = 0; i < longitud; i++){
		if (strncmp(vectorCorredores[i].categoria, "4 Refugios Clasica", 18) == 0) {
			categoria1[long1] = vectorCorredores[i];
			long1++;
		}else {
			categoria2[long2] = vectorCorredores[i];
			long2++;
		}
	}
}

void leerVectorCorredores(RegCorredores regCorredores[], int longitud){
	//BORRAR
	cout << "EJECUTANDO: leerVectorCorredores" << endl;
	////////
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
	//BORRAR
	cout << "FINALIZANDO: leerVectorCorredores" << endl;
	////////
}

void cargarArchivoConVector(RegCorredores vectorEntrada[], int longitud, FILE *archivoSalida){
	fwrite(vectorEntrada, sizeof(RegCorredores), longitud, archivoSalida);
}

int compararTiempos (float tiempoReferencia, float tiempoActual){
	int diferencia = 0;
	diferencia = tiempoReferencia - tiempoActual;
	return diferencia;
}

void cambioStruct (RegCorredores vector1[],int longitud1, ReporteCorredores vector2[]){
	for (int i = 0; i < longitud1; i++){
		vector2[i].posGral = i;
		vector2[i].posCategoria = -1; // Reemplazar por funcion?
		vector2[i].posGenero = -1;  // Reemplazar por funcion?

		vector2[i].numero = vector1[i].numero;
		strcpy(vector2[i].nombreApellido, vector1[i].nombreApellido);
		strcpy(vector2[i].categoria, vector1[i].categoria);
		vector2[i].genero = vector1[i].genero;
		strcpy(vector2[i].localidad, vector1[i].localidad);
		strcpy(vector2[i].llegada, vector1[i].llegada);

		strcpy(vector2[i].difPrimero, "--:--:--.-");
		strcpy(vector2[i].difAnterior, "--:--:--.-");
	}
}

void calcularPosiciones(ReporteCorredores reporte[] , int longitud) {
    for (int i=0; i<longitud; i++) {
        // Posición general
        reporte[i].posGral = i+1;

        // Posición por género
        int rankingGenero = 1;
        for (int j=0; j<i; j++) {
            if (reporte[j].genero == reporte[i].genero) {
                rankingGenero++;
            }
        }
        reporte[i].posGenero = rankingGenero;

        // Posición por categoría
        int rankingCat = 1;
        for (int j=0; j<i; j++) {
            if (strcmp(reporte[j].categoria, reporte[i].categoria) == 0) {
                rankingCat++;
            }
        }
        reporte[i].posCategoria = rankingCat;
    }
}

void imprimirReporte(ReporteCorredores reporte[], int longitud){
	printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("PosGral | PosGen | PosCat | Numero | Nombre y Apellido                     | Categoria                                        | Genero | Localidad          | Llegada    | DifPrimero | DifAnterior\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < longitud; i++) {
        printf("%7d | %6d | %6d | %6d | %-37s | %-48s |   %c    | %-18s | %-10s | %-10s | %-10s\n",
               reporte[i].posGral,
               reporte[i].posGenero,
               reporte[i].posCategoria,
               reporte[i].numero,
               reporte[i].nombreApellido,
               reporte[i].categoria,
               reporte[i].genero,
               reporte[i].localidad,
               reporte[i].llegada,
               reporte[i].difPrimero,
               reporte[i].difAnterior);
    }
}

void leerArchivoConsola(const char* ruta){
    FILE* f = fopen(ruta, "rb");
    if(!f){
        cout << "No se pudo abrir el archivo: " << ruta << endl;
        return;
    }
    RegCorredores reg;
    while(fread(&reg, sizeof(RegCorredores), 1, f) == 1){
        cout << "Numero: " << reg.numero << " | ";
        printf("%-48s  | ", reg.categoria);
        cout << "Genero: " << reg.genero << endl;
    }
    fclose(f);
}

void calcularTiempos(ReporteCorredores reporte[], int longitud){
	float aux;
	for(int i = 1; i<longitud; i++){	
		if(strcmp(reporte[i].llegada, "No termino") == 0){
			strcpy(reporte[i].difPrimero, "--:--:--.-");
			strcpy(reporte[i].difAnterior, "--:--:--.-");
			
			return;
		}
		aux = horarioASegundos(reporte[i].llegada) - horarioASegundos(reporte[i-1].llegada);
		char buffer[20];
		segundosAHorario(aux, buffer);
		strcpy(reporte[i].difAnterior, buffer);
		
		aux = horarioASegundos(reporte[i].llegada) - horarioASegundos(reporte[0].llegada);
		segundosAHorario(aux, buffer);
		strcpy(reporte[i].difPrimero, buffer);
	}
}


