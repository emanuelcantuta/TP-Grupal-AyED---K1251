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
void BinToTxt(const char *archivoEntrada,const char *archivoSalida);
void separarPorCarrera(RegCorredores vectorCorredores[], int longitud, FILE* categoria1, FILE* categoria2);
void separarEnVectoresPorCarrera(RegCorredores vectorCorredores[], int longitud, RegCorredores categoria1[], int &long1, RegCorredores categoria2[], int &long2);
float horarioASegundos(char [], int);
void segundosAHorario(float, char *);
int compararTiempos (float , float );
void CambioStruct (RegCorredores vector1[],int longitud1, ReporteCorredores vector2[]);
//funcion para leer
void leerArchivoConsola(char *);
//pasar a vector para trabajar en él y leer (prueba)
void cargarCorredoresVector(FILE *, RegCorredores [], int&);
void leerVectorCorredores(RegCorredores [], int);
void OrdenarPorTiempo (RegCorredores vector[],int longitud);
void calcularPosiciones(ReporteCorredores[] , int );
int main() {
	char rutaEntrada[] = "Archivo corredores 4Refugios.bin";
    char rutaCategoria1[] = "categoria1.bin";
    char rutaCategoria2[] = "categoria2.bin";
    int longitud = 1000;


	RegCorredores regCorredoresV[longitud];
	RegCorredores categoria1V[longitud]; int longitud1 = 0;
	RegCorredores categoria2V[longitud]; int longitud2 = 0;

	ReporteCorredores reporte1V[longitud];
	ReporteCorredores reporte2V[longitud];


	FILE* entrada = fopen(rutaEntrada, "rb");
    FILE* categoria1 = fopen(rutaCategoria1, "wb");
    FILE* categoria2 = fopen(rutaCategoria2, "wb");

    if (!entrada || !categoria1 || !categoria2) {
        cout << "Error al abrir uno de los archivos.\n";
        return 1;
    }

	//funcional
	cargarCorredoresVector(entrada, regCorredoresV, longitud);
	//leerVectorCorredores(regCorredoresV, longitud);


	//funcional
	OrdenarPorTiempo (regCorredoresV,longitud);
	separarEnVectoresPorCarrera(regCorredoresV, longitud, categoria1V, longitud1, categoria2V,longitud2);
	leerVectorCorredores(categoria1V,longitud1);
	leerVectorCorredores(categoria2V,longitud2);

	//TRANSFORMACION DE VECTOR CON FORMATO RegCorredores a ReporteCorredores
	CambioStruct(categoria1V,longitud1,reporte1V);
	CambioStruct(categoria2V,longitud2,reporte2V);

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
	BinToTxt("categoria1.bin","categoria1.txt");
	BinToTxt("categoria2.bin","categoria2.txt");
	return 0;
}



/* SEPARA VECTOR ORIGINAL EN ARCHIVOS
 *void separarPorCarrera(RegCorredores vectorCorredores[], int longitud, FILE* categoria1, FILE* categoria2){

	for (int i=0;i<longitud;i++)
	{
		if (strncmp(vectorCorredores[i].categoria, "4 Refugios Clasica", 18) == 0) {

			fwrite(&vectorCorredores[i], sizeof(RegCorredores), 1, categoria1);
		} else {
			fwrite(&vectorCorredores[i], sizeof(RegCorredores), 1, categoria2);
		}

	}
}
*/

void separarEnVectoresPorCarrera(RegCorredores vectorCorredores[], int longitud, RegCorredores categoria1[], int &long1, RegCorredores categoria2[], int &long2){
	long1 = 0;
	long2 = 0;

	for (int i=0;i<longitud;i++)
	{
		if (strncmp(vectorCorredores[i].categoria, "4 Refugios Clasica", 18) == 0) {
			categoria1[long1] = vectorCorredores[i];
			long1++;

		} else {
			categoria2[long2] = vectorCorredores[i];
			long2++;
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

int compararTiempos (float tiempoReferencia, float tiempoActual){
	int diferencia = tiempoReferencia - tiempoActual;
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
	//BORRAR
	cout << "EJECUTANDO: cargarCorredoresVector" << endl;
	////////
	longitud = 0;
    RegCorredores temp;

    while (fread(&temp, sizeof(RegCorredores), 1, archivo) == 1) {
        regCorredoresV[longitud] = temp;
        longitud++;
    }
	//BORRAR
	cout << "FINALIZANDO: cargarCorredoresVector" << endl;
	////////
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

void BinToTxt(const char *archivoEntrada,const char *archivoSalida){

	FILE* archivoBin = fopen(archivoEntrada, "rb");
	FILE* archivoTxt = fopen(archivoSalida, "w");

	if (!archivoBin || !archivoTxt) {
		printf("Error al abrir los archivos.\n");
		return;
	}

	struct RegCorredores reg;

	while (fread(&reg, sizeof(struct RegCorredores), 1, archivoBin) == 1) {
		fprintf(archivoTxt,
			"Numero: %d | Nombre y Apellido: %-40s | Categoria: %-50s | Genero: %c | Localidad: %-25s | Llegada: %-10s\n",
			reg.numero, reg.nombreApellido, reg.categoria, reg.genero,
			reg.localidad, reg.llegada);

		/* fprintf(archivoTxt,
			"PosGral: %3d | PosGenero: %3d | PosCategoria: %3d | Número: %4d | NombreApellido: %-50s | Categoría: %-20s | Género: %c | Localidad: %-30s | Llegada: %-10s | DifPrimero: %-10s | DifAnterior: %-10s\n",
			reg.posGral, reg.posGenero, reg.posCategoria, reg.numero,
			reg.nombreApellido, reg.categoria, reg.genero,
			reg.localidad, reg.llegada, reg.difPrimero, reg.difAnterior); */
	}
	fclose(archivoBin);
	fclose(archivoTxt);
	printf("Archivo de texto generado correctamente.\n");
}

void OrdenarPorTiempo (RegCorredores vector[],int longitud)
{
	RegCorredores aux;
	float tiempo1,tiempo2;

	for (int i=0;i<longitud - 1;i++)
	{

		for (int j = 0;j<longitud - i - 1;j++)
		{
			tiempo1 = horarioASegundos(vector[j].llegada,11);
			tiempo2 = horarioASegundos(vector[j+1].llegada,11);

			if (tiempo1>tiempo2)
			{
				aux = vector[j];
				vector[j] = vector [j+1];
				vector [j+1] = aux;
			}
		}
	}
}

void CambioStruct (RegCorredores vector1[],int longitud1, ReporteCorredores vector2[])
{
	for (int i=0;i<longitud1;i++)
	{
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
