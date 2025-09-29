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

struct CorredoresCiudad {
	int numero;
	char nombreApellido[50];
	char localidad[40];
	char ciudad[11];
};

struct Ciudad{
	char nombre[11];
	int cantCorredores;
	float tiempoTotal = 0; // es la suma de todos los tiempos de los corredores de una ciudad, el promedio lo hago en la funcion de mostrar por pantalla
};

struct Localidad{
	char nombre[40];
	int cantCiudades; // Cuantas ciudades existen de verdad en el vector ciudades
	Ciudad ciudades[400]; // Tiene tamaño arbitrario
};

//prototipos
void cargarCorredoresVector(FILE *, RegCorredores [], int&);
float horarioASegundos(char []);
void segundosAHorario(float, char *);
void ordenarPorTiempo (RegCorredores [], int);
void separarEnVectoresPorCarrera(RegCorredores [], int, RegCorredores [], int&, RegCorredores [], int&);
void cargarArchivoConVector(RegCorredores [], int, FILE *);
void separarPorCarrera(RegCorredores [], int, FILE *, FILE *);
int compararTiempos(float , float );
void inicializarReporte(RegCorredores [], int, ReporteCorredores []);

void calcularPosiciones(ReporteCorredores [] , int);
void imprimirReporte(ReporteCorredores [], int);
void noTermino(RegCorredores [], int);

//genera 2 archivos binario del reporte de los corredores por Clasica y NonStop
void generarReportePorCarreras(const char *, const char *, ReporteCorredores [], int, ReporteCorredores [], int);

//funcion para leer
void leerArchivoConsola(const char *);

//pasar a vector para trabajar en él y leer (prueba)
void leerVectorCorredores(RegCorredores [], int);
void calcularTiempos(ReporteCorredores[], int);

//funciones para el Punto 3
void cargarCiudadVector(FILE *, CorredoresCiudad[], int&);

void imprimirReporteLocalidades(Localidad localidad[], int cantLocalidades);

void ordenarAlfabeticamenteLocalidad(Localidad entrada[], int cantLocalidades);

void ordenarAlfabeticamenteCiudad(Ciudad entrada[], int cantCiudades);

int posLocalidad (Localidad localidades[], int cantLocalidades, char *nombre);

int posCiudad (Localidad localidad, char *nombre);

int registrarLocalidad (Localidad salida[], int &cantLocalidades, CorredoresCiudad entrada, const char llegada[]);

int main() {
	const char rutaEntrada [] = "Archivo corredores 4Refugios.bin";
	const char rutaCiudades [] = "Ciudades.bin";
	const char rutaCategoria1 [] = "categoria1.bin";
	const char rutaCategoria2 [] = "categoria2.bin";
	const char rutaReporteCarrera1 [] = "reporteCarreraClasica.bin";
	const char rutaReporteCarrera2 [] = "reporteCarreraNonStop.bin";
	
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
	
	//cierre de archivos, porque ya se pasaron a un vector
    fclose(entrada);
    fclose(categoria1);
    fclose(categoria2);
	
	//cambiandolo a modo lectura
	categoria1 = fopen(rutaCategoria1, "rb");
	categoria2 = fopen(rutaCategoria2, "rb");
	
	//ver si se creo e imprime el archivo por consola
	cout << "Archivo Categoria Clasica" << endl;
	leerArchivoConsola(rutaCategoria1);
	cout << "----------------------------------------------------------------------------------------------" << endl;
	cout << "Archivo Categoria NonStop" << endl;
	leerArchivoConsola(rutaCategoria2);

	//TRANSFORMACION DE VECTOR CON FORMATO RegCorredores a ReporteCorredores
	inicializarReporte(categoria1V,longitud1,reporte1V);
	inicializarReporte(categoria2V,longitud2,reporte2V);
 	
	calcularPosiciones(reporte1V , longitud1);
	calcularPosiciones(reporte2V , longitud2);

	//falta funcion de tiempos
	calcularTiempos(reporte1V, longitud1);
	calcularTiempos(reporte2V, longitud2);
	
	//imprime por consola
	imprimirReporte(reporte1V, longitud1);
	imprimirReporte(reporte2V, longitud2);
	
	//genera archivo reporte
	generarReportePorCarreras(rutaReporteCarrera1, rutaReporteCarrera2, reporte1V, longitud1, reporte2V, longitud2);

	fclose(categoria1);
    fclose(categoria2);	
    
    //Punto 3 - “Ciudades.bin”
    
    int longitud3 = 10; //longitud arbitraria para vector Ciudad (Solo va a contener las que pide el punto 3 (BsAs, Alemania y Brasil))
    CorredoresCiudad ciudadV[longitud3];
    
    Localidad localidades[10]; // Vector que va a contener a las ciudades (tamaño de 10 arbitrario, ya que hay solo 3 localidades)
    int cantLocalidades = 0; // Variable que contiene el tamaño real del vector localidades, despues cambia mediante las funciones
	
	entrada = fopen(rutaCiudades, "rb");
    if (!entrada) {
        cout << "Error al abrir uno de los archivos.\n";
        return 1;
    }
    
    //Cargo el vector Ciudad solo con las 3 localidades que pide el tp
    cargarCiudadVector(entrada, ciudadV, longitud3);
    
    //Recorro el vector de ciudades para ir registrando cada localidad en el vector localidades 
    for (int i = 0; i < longitud3; i++) {
    	//Recorro el vector RegCorredores para encontrar el tiempo de cada corredor
    	for (int j = 0; j < longitud; j++){
    		if (ciudadV[i].numero == regCorredoresV[j].numero)
    		{
    			        registrarLocalidad(localidades, cantLocalidades, ciudadV[i], regCorredoresV[j].llegada);
    			        break;
			}
		}
    }
	
	//Ordeno alfabeticamente el vector de localidades/ciudades
	ordenarAlfabeticamenteLocalidad(localidades, cantLocalidades);
	
	imprimirReporteLocalidades(localidades, cantLocalidades);
    
    fclose(entrada);
    
	return 0;
}

//funciones para Punto 3

//funcion que carga el vector de ciudades (Solo agrega las localidades de BsAs, Alemania y Brasil como pide el informe)
void cargarCiudadVector(FILE *entrada, CorredoresCiudad ciudadV[], int &longitud){
	longitud = 0;
    CorredoresCiudad temp;

    while (fread(&temp, sizeof(CorredoresCiudad), 1, entrada) == 1) {
    	if (strncmp(temp.localidad, "Buenos Aires", 12) == 0 ||
			strncmp(temp.localidad, "Alemania", 8) == 0 ||
			strncmp(temp.localidad, "Brasil", 6) == 0 )
        	{
        		ciudadV[longitud] = temp;
      	 		longitud++;
			}
    }
}

void ordenarAlfabeticamenteLocalidad(Localidad entrada[], int cantLocalidades){
	
	Localidad aux;

	for (int i = 0; i < cantLocalidades - 1; i++){

		for (int j = 0; j < cantLocalidades - i - 1;j++){

			if (strcmp(entrada[j].nombre, entrada[j+1].nombre) > 0){
				aux = entrada[j];
				entrada[j] = entrada [j+1];
				entrada [j+1] = aux;
			}
		}
	}
	
		for (int i = 0; i < cantLocalidades; i++){
		ordenarAlfabeticamenteCiudad(entrada[i].ciudades, entrada [i].cantCiudades);
	}
}

void ordenarAlfabeticamenteCiudad(Ciudad entrada[], int cantCiudades){
	
	Ciudad aux;

	for (int i = 0; i < cantCiudades - 1; i++){

		for (int j = 0; j < cantCiudades - i - 1;j++){

			if (strcmp(entrada[j].nombre, entrada[j+1].nombre) > 0){
				aux = entrada[j];
				entrada[j] = entrada [j+1];
				entrada [j+1] = aux;
			}
		}
	}
}

int posLocalidad (Localidad localidades[], int cantLocalidades, char *nombre){
	for (int i = 0; i < cantLocalidades; i++)
	{
		if (strncmp(localidades[i].nombre, nombre, 40) == 0)
		{
			return i; // Devuelve la posicion donde esta la localidad a buscar (Osea que ya existe)
		}
	}
	return -1; //Devuelve -1 si no esta
}

int posCiudad (Localidad localidad, char *nombre){
	for (int i = 0; i < localidad.cantCiudades; i++)
	{
		if (strncmp(localidad.ciudades[i].nombre, nombre, 11) == 0)
		{
			return i; // Devuelve la posicion donde esta la ciudad a buscar (Osea que ya existe)
		}
	}
	return -1; //Devuelve -1 si no esta
}
	
//Funcion que carga el vector de localidades, viendo si la localidad/ciudad ya esta o no
int registrarLocalidad (Localidad salida[], int &cantLocalidades, CorredoresCiudad entrada, const char llegada[]){
	
	//Veo primero si la localidad(Alguna de las 3) ya existe o no en el vector de salida	
	int posicionLocalidad = posLocalidad (salida, cantLocalidades, entrada.localidad);
	if (posicionLocalidad == -1) // agrega si es una nueva localidad
	{
		posicionLocalidad = cantLocalidades++;
		strncpy(salida[posicionLocalidad].nombre, entrada.localidad, 40);
		salida[posicionLocalidad].cantCiudades = 0;
	}
	
	int posicionCiudad = posCiudad(salida[posicionLocalidad], entrada.ciudad);
	if(posicionCiudad == -1)
	{
		posicionCiudad = salida[posicionLocalidad].cantCiudades++;
		strncpy(salida[posicionLocalidad].ciudades[posicionCiudad].nombre, entrada.ciudad, 11);
		salida[posicionLocalidad].ciudades[posicionCiudad].cantCorredores = 0;
	}
	
	salida[posicionLocalidad].ciudades[posicionCiudad].cantCorredores++;
	
	//Voy sumando los tiempos para obtener el tiempoTotal (El promedio lo calculo al informar)
	float aux = horarioASegundos((char*)llegada);
	salida[posicionLocalidad].ciudades[posicionCiudad].tiempoTotal += aux;
}

//Falta poner el total de corredores/promedio de cada ciudad al final
void imprimirReporteLocalidades(Localidad localidad[], int cantLocalidades){
	

	
	printf("-------------------------------------------------------------------------\n");
	printf("Localidad    | Ciudad        | Cant. de Corredores   | Tiempo promedio\n ");
    printf("-------------------------------------------------------------------------\n");

	for (int i = 0; i < cantLocalidades; i++) 
	{
        
		for (int j = 0; j < localidad[i].cantCiudades; j++) 
		{
            
            float promedioSegundos = localidad[i].ciudades[j].tiempoTotal / localidad[i].ciudades[j].cantCorredores;
            char promedio[12];
            segundosAHorario(promedioSegundos, promedio);
			
			if (j == 0)
			{
            	printf("%-15s %-15s %-23d %s\n",
                localidad[i].nombre,
                localidad[i].ciudades[j].nombre,
                localidad[i].ciudades[j].cantCorredores,
				promedio);	
			}
			else 
			{
				printf("%-15s %-15s %-23d %s \n",
				"",
				localidad[i].ciudades[j].nombre,
                localidad[i].ciudades[j].cantCorredores,
				promedio);	
			}
        }
        		printf("%s %s\n \n", "Total ",
                localidad[i].nombre);
    }
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
	int decimales = (int)(totalSegundos * 10); // 

    int horas = decimales / 36000;
    int restoHoras = decimales % 36000;

    int minutos = restoHoras / 600;
    int restoMin = restoHoras % 600;

    int segundos = restoMin / 10;
    int decimal = restoMin % 10;

    resultado[0] = (horas / 10) + '0';
    resultado[1] = (horas % 10) + '0';
    resultado[2] = ':';
    resultado[3] = (minutos / 10) + '0';
    resultado[4] = (minutos % 10) + '0';
    resultado[5] = ':';
    resultado[6] = (segundos / 10) + '0';
    resultado[7] = (segundos % 10) + '0';
    resultado[8] = '.';
    resultado[9] = decimal + '0';
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

void inicializarReporte(RegCorredores vector1[],int longitud1, ReporteCorredores vector2[]){ //base de la estructura de REPORTECORREDORES 
	for (int i = 0; i < longitud1; i++){
		vector2[i].posGral = i;
		vector2[i].posCategoria = -1; // Reemplazar por funcion?
		vector2[i].posGenero = -1;  // Reemplazar por funcion?

		vector2[i].numero = vector1[i].numero;
		strcpy(vector2[i].nombreApellido, vector1[i].nombreApellido);
		strcpy(vector2[i].categoria, vector1[i].categoria);
		vector2[i].genero = vector1[i].genero;
		strcpy(vector2[i].localidad, vector1[i].localidad);
		strcpy(vector2[i].llegada, vector1[i].llegada); //mover

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

        // Posición por categoría, para contar debe estar previamente ordenado por categoria
        int rankingCat = 1;
        for (int j = 0; j < i; j++) {
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

    for(int i = 0; i < longitud; i++) {
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

void generarReportePorCarreras(const char* ruta1, const char* ruta2, ReporteCorredores reporte1[], int longitud1, ReporteCorredores reporte2[], int longitud2){
	FILE* r1 = fopen(ruta1, "wb");
	FILE* r2 = fopen(ruta2, "wb");
	
	if(!r1 || !r2){
		cout << "No se pudo crear el archivo final del reporteCorredores" << endl;
        return;
	}
	
	fwrite(reporte1, sizeof(ReporteCorredores), longitud1, r1);
	fwrite(reporte2, sizeof(ReporteCorredores), longitud2, r2);
	
	fclose(r1);
	fclose(r2);
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
