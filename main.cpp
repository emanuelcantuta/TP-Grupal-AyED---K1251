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
	int cantCorredores = 0;
	int cantClasificados = 0;
	float tiempoTotal = 0.0; // es la suma de todos los tiempos de los corredores de una ciudad, el promedio lo hago en la funcion de mostrar por pantalla
};

struct Localidad{
	char nombre[40];
	int cantCiudades = 0;
	Ciudad ciudades[400]; // Tiene tamaño arbitrario
};

//prototipos
void cargarCorredoresVector(FILE *, RegCorredores [], int&);
int horarioASegundos(char []);
void segundosAHorario(int, char *);
void ordenarPorTiempo (RegCorredores [], int);
void separarEnVectoresPorCarrera(RegCorredores [], int, RegCorredores [], int&, RegCorredores [], int&);
void cargarArchivoConVector(RegCorredores [], int, FILE *);
int compararTiempos(float , float );
void inicializarReporte(RegCorredores [], int, ReporteCorredores []);
void calcularPosiciones(ReporteCorredores [] , int);
void imprimirReporte(ReporteCorredores [], int);
void noTermino(RegCorredores [], int);
void crearVectorCategorias (RegCorredores [], int, char [][50], int&);
void ordenarCategorias(char [][50], int);
void crearVectorPodio(char [][50], int, RegCorredores [], int, RegCorredores [], int&);
//genera 2 archivos binario del reporte de los corredores por Clasica y NonStop
void generarReportePorCarreras(const char *, const char *, ReporteCorredores [], int, ReporteCorredores [], int);
void calcularTiempos(ReporteCorredores[], int);
void leerVectorPodios(RegCorredores [], int);
//punto 3
void cargarCiudadVector(FILE *, CorredoresCiudad [], int&);
void imprimirReporteLocalidades(Localidad [], int);
void ordenarAlfabeticamenteLocalidad(Localidad [], int);
void ordenarAlfabeticamenteCiudad(Ciudad [], int);
int posLocalidad(Localidad [], int, char*);
int posCiudad(Localidad , char*);
void registrarLocalidad(Localidad [], int&, CorredoresCiudad, char []);
int cantTotalCorredores(Localidad);

int main() {

	const char rutaEntrada [] = "Archivo corredores 4Refugios.bin";
	const char rutaCiudades [] = "Ciudades.bin";
	const char rutaCarrera1 [] = "Carrera1.bin";
	const char rutaCarrera2 [] = "Carrera2.bin";
	const char rutaReporteCarrera1 [] = "reporteCarreraClasica.bin";
	const char rutaReporteCarrera2 [] = "reporteCarreraNonStop.bin";
	const char rutaPodios [] = "podios.bin";

    int longitud = 1000; //longitud base general
	int longitud1 = 0; //longitud para vector Clasica
	int longitud2 = 0; //longitud para vector NonStop

	RegCorredores regCorredoresV[longitud];
	RegCorredores carrera1V[longitud];
	RegCorredores carrera2V[longitud];
	RegCorredores podiosV[longitud];

	ReporteCorredores reporte1V[longitud];
	ReporteCorredores reporte2V[longitud];

	FILE* entrada = fopen(rutaEntrada, "rb");
    FILE* carrera1 = fopen(rutaCarrera1, "wb");
    FILE* carrera2 = fopen(rutaCarrera2, "wb");


    if (!entrada || !carrera1 || !carrera2) {
        cout << "Error al abrir uno de los archivos.\n";
        return 1;
    }

	cargarCorredoresVector(entrada, regCorredoresV, longitud);
	noTermino(regCorredoresV, longitud);
	ordenarPorTiempo(regCorredoresV,longitud);
	separarEnVectoresPorCarrera(regCorredoresV, longitud, carrera1V, longitud1, carrera2V,longitud2);

	inicializarReporte(carrera1V,longitud1,reporte1V); //trabajamos sigue con el vector
	inicializarReporte(carrera2V,longitud2,reporte2V); //trabajamos sigue con el vector

	calcularPosiciones(reporte1V , longitud1);
	calcularPosiciones(reporte2V , longitud2);

	//falta funcion de tiempos
	calcularTiempos(reporte1V, longitud1);
	calcularTiempos(reporte2V, longitud2);

	//imprime por consola CONSIGNA 1 MUESTRA
	cout << "Reporte Corresdores Clasica" << endl;
	imprimirReporte(reporte1V, longitud1);
	cout << endl;
	cout << "Reporte Corresdores NonStop" << endl;
	imprimirReporte(reporte2V, longitud2);
	cout << endl;


	//genera archivo reporte (una vez ya habiendo validado por imprimirReporte()1 y 2
	generarReportePorCarreras(rutaReporteCarrera1, rutaReporteCarrera2, reporte1V, longitud1, reporte2V, longitud2);
	fclose(carrera1);
    fclose(carrera2);
// Punto 2
	char vectorCategorias[longitud][50];
	int aux = 0;
	int longPodio = aux*3;

	crearVectorCategorias(regCorredoresV,longitud,vectorCategorias,aux); //separa las categorias en una matriz
	ordenarCategorias(vectorCategorias,aux);
	crearVectorPodio (vectorCategorias,aux,regCorredoresV,longitud,podiosV, longPodio);
	leerVectorPodios(podiosV,longPodio);

	FILE* podios = fopen(rutaPodios, "wb");

	if (!podios) {
		cout << "Error al crear archivo 'podios.bin'";
		return 1;
	}

	cargarArchivoConVector(podiosV,longPodio,podios); //podiamos pasar la ruta y que ahi dentro valide si se abrio y abrir el archivo (quiza mejorar o no)
	fclose(podios);
//punto 3
    int longitud3 = 300;
    CorredoresCiudad ciudadV[longitud3];
	int cantLocalidades = 0;
	Localidad localidades[100];

	entrada = fopen(rutaCiudades, "rb");
    if (!entrada) {
        cout << "Error al abrir uno de los archivos.\n";
        return 1;
    }

    cargarCiudadVector(entrada, ciudadV, longitud3);

    //Recorro el vector de ciudades para ir registrando cada localidad en el vector localidades
    for (int i = 0; i < longitud3; i++) {
    	//Recorro el vector RegCorredores para encontrar el tiempo de cada corredor
    	for (int j = 0; j < longitud; j++){
    		if (ciudadV[i].numero == regCorredoresV[j].numero){
		        registrarLocalidad(localidades, cantLocalidades, ciudadV[i], regCorredoresV[j].llegada);
		        break;
			}
		}
    }
	ordenarAlfabeticamenteLocalidad(localidades, cantLocalidades);
	imprimirReporteLocalidades(localidades, cantLocalidades);
    fclose(entrada);
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

int horarioASegundos(char horario[]){
	int totalSegundos = 0;

	totalSegundos = ((horario[0]-'0')*10 + (horario[1]-'0')) * 36000
    + ((horario[3]-'0')*10 + (horario[4]-'0')) * 600
    + ((horario[6]-'0')*10 + (horario[7]-'0')) * 10
    + (horario[9]-'0');

    return totalSegundos;
}

void segundosAHorario(int totalSegundos, char* resultado) {
	int horas = totalSegundos / 36000;
    int restoHoras = totalSegundos % 36000;

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
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("Numero | Nombre y Apellido                     | Categoria                                        | Genero | Localidad          | Llegada    \n");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < longitud; i++) {
        printf("%6d | %-37s | %-48s |   %c    | %-18s | %-10s\n",
               regCorredores[i].numero,
               regCorredores[i].nombreApellido,
               regCorredores[i].categoria,
               regCorredores[i].genero,
               regCorredores[i].localidad,
               regCorredores[i].llegada);
    }

    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
}

void leerVectorPodios(RegCorredores regCorredores[], int longitud) {
    cout << "========== PODIOS POR CATEGORIA ==========" << endl;

    for (int i = 0; i < longitud; ) {
        cout << "\nCATEGORIA: " << regCorredores[i].categoria << endl;

        int posicion = 1;
        // Mostrar hasta 3 corredores de esa categoría
        for (int j = 0; j < 3 && i + j < longitud; j++) {
            // Si la categoría cambia, salimos
            if (strcmp(regCorredores[i].categoria, regCorredores[i + j].categoria) != 0) {
                break;
            }

            cout << "----------------------------------------" << endl;
            cout << "Posicion " << posicion << ":" << endl;
            cout << "Numero: " << regCorredores[i + j].numero << endl;
            cout << "Nombre y Apellido: " << regCorredores[i + j].nombreApellido << endl;
            cout << "Horario de llegada: " << regCorredores[i + j].llegada << endl;
            posicion++;
        }
        // Saltar a la siguiente categoría
        i += posicion - 1;
    }
    cout << "\n========== FIN DE PODIOS ==========" << endl;
    cout << endl;
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

void calcularTiempos(ReporteCorredores reporte[], int longitud){
	int aux;

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

void crearVectorCategorias(RegCorredores vectorARecorrer[], int longitud, char categoriasEncontradas[][50], int &cantCategorias){
	cantCategorias = 0; // Inicializo en 0, aún no se separaron categorias

	for (int i = 0;i < longitud; i++){
		bool yaLeido = false; // Se reinicia el valor a false porque aún no hizo comprobaciones
		// Chequea el vector categoriasEncontradas en busca de coincidencias
		for (int j = 0;j < cantCategorias; j++){
			//Si encuentra una coincidencia, deja de buscar y pasa al siguiente valor.
			if (strcmp(vectorARecorrer[i].categoria,categoriasEncontradas[j])  == 0){
				yaLeido = true;
				break;
			}
		}
		if (!yaLeido){
			strcpy(categoriasEncontradas[cantCategorias], vectorARecorrer[i].categoria);
			cantCategorias++;
		}
	}
}

void ordenarCategorias(char vectorAOrdenar[][50],int longitud)
{
	char aux[50];
	int posCaracteres1;
	int posCaracteres2;

	// Comparo el valor del caracter decenas de las categorías y los ordena de menor a mayor.
	for (int i=0;i<longitud-1;i++)
	{

		for(int j = 0; j < longitud-1-i; j++){
			posCaracteres1 = strlen(vectorAOrdenar[j]) - 9;
			posCaracteres2 = strlen(vectorAOrdenar[j+1]) - 9;

			if (vectorAOrdenar[j][posCaracteres1] > vectorAOrdenar[j+1][posCaracteres2])
			{
				strcpy(aux,vectorAOrdenar [j]);
				strcpy(vectorAOrdenar [j],vectorAOrdenar [j+1]);
				strcpy(vectorAOrdenar [j+1],aux);
			}
		}
	}

	// Compara el valor de los primeros 22 caracteres, para ordenar asi entre Non-Stop/Clásica y entre Dama/Caballero.
	for(int i = 0; i < longitud-1; i++){
		for(int j = 0; j < longitud-1-i; j++){
			if(strncmp(vectorAOrdenar[j],vectorAOrdenar[j+1],22) > 0){
				strcpy(aux,vectorAOrdenar [j]);
				strcpy(vectorAOrdenar [j],vectorAOrdenar [j+1]);
				strcpy(vectorAOrdenar [j+1],aux);
			}
		}
	}
}

void crearVectorPodio(char vectorCategorias[][50],int cantCategorias, RegCorredores vectorARecorrer[], int longitud, RegCorredores vectorSalida[], int &corredoresCopiados){
	corredoresCopiados = 0;

	for (int i = 0; i < cantCategorias; i++){
		int podio = 0;

		for (int j = 0; j < longitud; j++){
			// Recorre el vector de corredores hasta encontrar el primer resultado que coincida con la categoria
			if (strcmp (vectorCategorias[i],vectorARecorrer[j].categoria) == 0){
				// Chequea si el corredor que encontró terminó la carrera y lo agrega
				if (strncmp(vectorARecorrer[j].llegada,"No termino", 8) != 0 ){
					vectorSalida[corredoresCopiados] = vectorARecorrer[j];
					podio++;
					corredoresCopiados++;
				}
			}
			// Si ya copió 3, sale del for j.
			if(podio == 3) break;
		}
	}
}

//punto3
void cargarCiudadVector(FILE *entrada, CorredoresCiudad ciudadV[], int &longitud){
	longitud = 0;
    CorredoresCiudad temp;

    while(fread(&temp, sizeof(CorredoresCiudad), 1, entrada) == 1){
		ciudadV[longitud] = temp;
		longitud++;
    }
}

void ordenarAlfabeticamenteLocalidad(Localidad entrada[], int cantLocalidades){
	Localidad aux;

	for(int i = 0; i < cantLocalidades - 1; i++){
		for(int j = 0; j < cantLocalidades - i - 1;j++){
			if(strcmp(entrada[j].nombre, entrada[j+1].nombre) > 0){
				aux = entrada[j];
				entrada[j] = entrada[j+1];
				entrada[j+1] = aux;
			}
		}
	}

	for(int i = 0; i < cantLocalidades; i++){
		ordenarAlfabeticamenteCiudad(entrada[i].ciudades, entrada [i].cantCiudades);
	}
}

void ordenarAlfabeticamenteCiudad(Ciudad entrada[], int cantCiudades){
	Ciudad aux;

	for(int i = 0; i < cantCiudades - 1; i++){
		for(int j = 0; j < cantCiudades - i - 1; j++){
			if(strcmp(entrada[j].nombre, entrada[j+1].nombre) > 0){
				aux = entrada[j];
				entrada[j] = entrada [j+1];
				entrada [j+1] = aux;
			}
		}
	}
}

int posLocalidad (Localidad localidades[], int cantLocalidades, char *nombre){
	for(int i = 0; i < cantLocalidades; i++){
		if(strncmp(localidades[i].nombre, nombre, 40) == 0){
			return i;
		}
	}
	return -1;
}

int posCiudad (Localidad localidad, char *nombre){
	for(int i = 0; i < localidad.cantCiudades; i++){
		if(strncmp(localidad.ciudades[i].nombre, nombre, 11) == 0){
			return i;
		}
	}
	return -1;
}

//Funcion que carga el vector de localidades, viendo si la localidad/ciudad ya esta o no
void registrarLocalidad(Localidad salida[], int &cantLocalidades, CorredoresCiudad entrada, char llegada[]){
	//Veo primero si la localidad ya existe o no en el vector de salida
	int posicionLocalidad = posLocalidad (salida, cantLocalidades, entrada.localidad);

	if(posicionLocalidad == -1){ // agrega si es una nueva localidad
		posicionLocalidad = cantLocalidades++;
		strncpy(salida[posicionLocalidad].nombre, entrada.localidad, 40);
	}

	int posicionCiudad = posCiudad(salida[posicionLocalidad], entrada.ciudad);

	if(posicionCiudad == -1){
		posicionCiudad = salida[posicionLocalidad].cantCiudades++;
		strncpy(salida[posicionLocalidad].ciudades[posicionCiudad].nombre, entrada.ciudad, 11);
	}
	salida[posicionLocalidad].ciudades[posicionCiudad].cantCorredores++;
	//Voy sumando los tiempos de los que terminaron para obtener el tiempoTotal (El promedio lo calculo al informar)
	if(strncmp(llegada, "No termino", 11) != 0){
    	int aux = horarioASegundos(llegada);
    	salida[posicionLocalidad].ciudades[posicionCiudad].tiempoTotal += aux;
    	salida[posicionLocalidad].ciudades[posicionCiudad].cantClasificados++;
	}
}

void imprimirReporteLocalidades(Localidad localidad[], int cantLocalidades){
	float tiempoTotalSegundos = 0.0;
	int clasificadosTotal = 0;

	cout << "============================= Reporte Ciudades =============================" << endl;

	cout << "----------------------------------------------------------------------------" << endl;
	cout << " Localidad       | Ciudad        | Cant. de Corredores   | Tiempo promedio   " << endl;
	cout << "----------------------------------------------------------------------------" << endl;

	for(int i = 0; i < cantLocalidades; i++) {
		for(int j = 0; j < localidad[i].cantCiudades; j++) {
		    if(localidad[i].ciudades[j].cantClasificados > 0){   // Solo mostra las ciudades que si haya terminado al menos un corredor
				float promedioSegundos = localidad[i].ciudades[j].tiempoTotal / localidad[i].ciudades[j].cantClasificados;
		        char promedio[11];

		        segundosAHorario(promedioSegundos, promedio);

		        if(j == 0){
		        	printf("%-17s %-17s %-23d %-3s\n", localidad[i].nombre, localidad[i].ciudades[j].nombre, localidad[i].ciudades[j].cantCorredores, promedio);
		        }else{
		            printf("%-17s %-17s %-23d %-3s\n", "", localidad[i].ciudades[j].nombre, localidad[i].ciudades[j].cantCorredores, promedio);
		        }
		    }
			tiempoTotalSegundos += localidad[i].ciudades[j].tiempoTotal; // Suma los tiempos de las ciudades de una misma localidad
			clasificadosTotal += localidad[i].ciudades[j].cantClasificados; // Cant de corredores clasificados de una misma localidad
		}

		// Evita mostrar basura si no hay corredores clasficidos en una localidad
		if(clasificadosTotal != 0){
			float promedioTotalSegundos = tiempoTotalSegundos / clasificadosTotal;
			char promedioTotal[11];
		    segundosAHorario(promedioTotalSegundos, promedioTotal);

			printf("%s %-29s %-23d %-3s \n", "Total", localidad[i].nombre, cantTotalCorredores(localidad[i]), promedioTotal);
			cout << "----------------------------------------------------------------------------" << endl;
		}else{
			printf("%s %-29s %-23d %-3s \n", "Total", localidad[i].nombre, cantTotalCorredores(localidad[i]), "Sin clasificados");
			cout << "----------------------------------------------------------------------------" << endl;
		}
		tiempoTotalSegundos = 0; // Reincio la suma de tiempo, para hacerlo con otra localidad
		clasificadosTotal = 0; // Reinicio de clasificados
	}
}

int cantTotalCorredores(Localidad entrada){
	int cantTotal = 0;

	for (int i = 0; i < entrada.cantCiudades; i++){
		cantTotal += entrada.ciudades[i].cantCorredores;
	}

	return cantTotal;
}
