# 🏁 Procesador de Resultados de Carrera "4 Refugios"

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)

Este proyecto es un trabajo práctico integrador para la materia **Algoritmos y Estructuras de Datos**. Su objetivo principal es leer archivos binarios con datos de corredores de una carrera de montaña, procesar la información y generar múltiples reportes de resultados.

El programa gestiona los resultados de las dos modalidades de la carrera: "4 Refugios Clásica" y "4 Refugios NonStop".

## 🚀 Funcionalidades Principales

El programa `main.cpp` realiza las siguientes tareas:

1.  **Generación de Reportes por Carrera:**
    * Lee el archivo `Archivo corredores 4Refugios.bin` y carga a los corredores en memoria.
    * Separa a los corredores según su carrera: "Clásica" o "NonStop".
    * Ordena a los corredores por su tiempo de llegada.
    * Calcula las posiciones: General, por Género y por Categoría.
    * Calcula las diferencias de tiempo respecto al primer puesto y al corredor anterior.
    * Imprime en la consola un reporte detallado para cada una de las dos carreras.
    * Genera dos archivos binarios (`reporteCarreraClasica.bin` y `reporteCarreraNonStop.bin`) con los datos de estos reportes.

2.  **Creación de Archivo de Podios:**
    * Identifica todas las categorías existentes en ambas carreras.
    * Genera un único archivo binario (`podios.bin`) que contiene a los 3 primeros corredores (podio) de cada categoría.

3.  **Reporte por Ciudades y Localidades:**
    * Lee el archivo `Ciudades.bin` para obtener la ciudad de origen de cada corredor.
    * Cruza esta información con los tiempos de llegada del archivo principal.
    * Genera un reporte impreso en consola, agrupado por Localidad y Ciudad, que muestra la cantidad total de corredores y el tiempo promedio de llegada.

### Manejo de Tiempos y Descalificaciones

* El programa convierte los tiempos de formato `HH:MM:SS.D` a segundos para facilitar los cálculos y ordenamiento, y luego los reconverte para los reportes.
* Los corredores con códigos de llegada `DNF`, `DSQ (FE)`, etc., son marcados como "No termino" en los reportes y no se consideran para los cálculos de tiempo promedio en el reporte de ciudades.

## 📂 Estructura del Proyecto

```

/TP-AyED-Corredores/
├── main.cpp                              \# Código fuente principal en C++
├── Archivo corredores 4Refugios.bin      \# (ENTRADA) Datos binarios de todos los corredores
├── Ciudades.bin                          \# (ENTRADA) Datos binarios de las ciudades por corredor
├── AyED 2025-Enunciado TP Integrador.pdf   \# Enunciado original del trabajo práctico
├── reporteCarreraClasica.bin             \# (SALIDA) Archivo binario con el reporte de la carrera Clásica
├── reporteCarreraNonStop.bin             \# (SALIDA) Archivo binario con el reporte de la carrera NonStop
└── podios.bin                            \# (SALIDA) Archivo binario con los podios de todas las categorías

```

## 🛠️ Cómo Compilar y Ejecutar

1.  **Requisitos:**
    * Un compilador de C++ (como `g++`).
    * Los archivos de entrada `Archivo corredores 4Refugios.bin` y `Ciudades.bin` deben estar en el mismo directorio que el ejecutable.

2.  **Compilación:**
    Abra una terminal y ejecute el siguiente comando para compilar:

    ```bash
    g++ main.cpp -o procesador_carrera
    ```

3.  **Ejecución:**
    Una vez compilado, ejecute el programa:

    ```bash
    ./procesador_carrera
    ```

4.  **Salida:**
    * El programa imprimirá los reportes (Punto 1 y Punto 3) directamente en la consola.
    * Se generarán los archivos binarios `reporteCarreraClasica.bin`, `reporteCarreraNonStop.bin` y `podios.bin` en el mismo directorio.

## 📄 Contexto Académico

Este repositorio es la solución al Trabajo Práctico Integrador de 2025 para la materia **Algoritmos y Estructuras de Datos** de la carrera Ingeniería en Sistemas de Información en la **Universidad Tecnológica Nacional (UTN)**.
