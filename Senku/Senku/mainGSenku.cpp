#include "GSenku.hpp"
#include <iostream>
#include <string>

using namespace std;

// main del programa que necesita [4] argumentos para la ejecución:
// 1. Fichero de tablero
// 2. Fichero de movimientos válidos    
// 3. Retardo en milisegundos
// 4. Nombre fichero de salida opcional o resultado.txt automático 

// Ejecutable en ubuntu: ./mainGSenku tablero.txt movimientos.txt 100 salida.txt o directamente con el makefile
// argv[0] = "./mainGSenku"
// argv[1] = "tablero.txt"
// argv[2] = "movimientos.txt"
// argv[3] = "100" //retardo
// argv[4] = "salida.txt" opcional el último argumento

int main(int argc, char *argv[]) {
    // Verificación de argumentos de entrada para usuario
    // argc = 5 si se proporciona el fichero de salida
    // argc = 4 si no se proporciona el fichero de salida
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <ficheroTablero> <ficheroMovimientos> <retardo> [ficheroSalida]" << endl;
        cerr << "Ejemplo: " << argv[0] << " tablero.txt movimientos.txt 100 resultado.txt" << endl;
        return 1;
    }

    // Parámetros de entrada
    string ficheroTablero = argv[1];
    string ficheroMovimientos = argv[2];
    int retardo = stoi(argv[3]);
    // Si no se proporciona un fichero de salida en el argumento 5, se usa "resultado.txt" por defecto
    string ficheroSalida = (argc == 5) ? argv[4] : "resultado.txt";

    // Struct principales
    tpTablero tablero;
    tpMovimientosValidos movimientosValidos;
    tpListaMovimientos solucion;

    // Inicialización del tablero según el fichero
    if (!inicializarTablero(ficheroTablero, tablero)) {
        cerr << "Error al inicializar el tablero desde el fichero: " << ficheroTablero << endl;
        return 1;
    }

    // Inicialización de movimientos válidos
    if (!inicializarMovimientosValidos(ficheroMovimientos, movimientosValidos)) {
        cerr << "Error al inicializar los movimientos válidos desde el fichero: " << ficheroMovimientos << endl;
        return 1;
    }

    // Mostrar el estado inicial del tablero con MostrarTablero
    cout << "Estado inicial del tablero:" << endl;
    mostrarTablero(tablero);

    // Buscar solución
    cout << "Buscando solución..." << endl;
    int resultado = buscaSolucion(tablero, movimientosValidos, solucion, retardo);

    // Mostrar resultados solo si se encontró solución
    if (resultado == 1) {
        cout << "¡Solución encontrada!" << endl;
        escribeListaMovimientos(ficheroSalida, solucion);
        cout << "La solución ha sido escrita en el fichero: " << ficheroSalida << endl;
    } else {
        cout << "No se encontró solución." << endl;
        // Generar archivo con -1
        tpListaMovimientos solucionVacia;
        solucionVacia.numMovs = 0;
        escribeListaMovimientos(ficheroSalida, solucionVacia);
    }

    return 0;
}
