#include "GSenku.hpp"
#include <iostream> // iostream
#include <cstdlib>  // cerr
#include <fstream>  // fstream
#include <string>   // string
#include <chrono>   // chrono
#include <thread>   // thread para pausa

using namespace std;

// inicializa el tablero a partir del fichero txt primero se lee el número de filas y columnas luego el estado
bool inicializarTablero(const string nombreFichero, tpTablero &tablero) {
    ifstream archivo(nombreFichero);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el fichero del tablero." << endl;
        return false;
    }

    archivo >> tablero.nfils;
    archivo >> tablero.ncols;

    // inicializamos la matriz 
    for (int i = 0; i < MAXDIM; ++i) {
        for (int j = 0; j < MAXDIM; ++j) {
            tablero.matriz[i][j] = NO_USADA; // Inicializamos todo como NO_USADA
        }
    }
    // leemos el estado del tablero desde el fichero
    for (int i = 0; i < tablero.nfils; ++i) {
        for (int j = 0; j < tablero.ncols; ++j) {
            char estado;
            archivo >> estado;
            switch (estado) {
                case '-':
                    tablero.matriz[i][j] = NO_USADA;
                    break;
                case 'o':
                    tablero.matriz[i][j] = OCUPADA;
                    break;
                case 'x':
                    tablero.matriz[i][j] = VACIA;
                    break;
                default:
                    cerr << "Error: Estado inválido en el fichero del tablero." << endl;
                    return false;
            }
        }
    }

    archivo.close();

    // depuración para mostrar el tablero inicializado
    cout << "Tablero inicializado:" << endl;
    mostrarTablero(tablero);
    
    return true;
}

// lee el fichero de movimientos válidos y lo inicializa 
// preparamos el vector de movimientos válidos para que la funcion EsMovimientoValido lo use en la búsqueda de la solución
bool inicializarMovimientosValidos(const string nombreFichero, tpMovimientosValidos &movimientos) {
    ifstream archivo(nombreFichero);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el fichero de movimientos válidos." << endl;
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            char c;
            archivo >> c;
    
            // Evitar la celda central
            if (i != 1 || j != 1) { // si no es la celda central:
                movimientos.validos[i * 3 + j] = (c == '+');
                //ejemplo seria: movimientos.validos = {true, true, true, true, true, true, true, true};
            }
        }
    }

    archivo.close();

    // depuración: Mostrar los movimientos válidos en la consola modo vector
    cout << "Movimientos válidos inicializados:" << endl;
    for (int i = 0; i < 8; ++i) {
        cout << (movimientos.validos[i] ? "+" : "-") << " ";
    }
    cout << endl;

    return true;
}

// implementa mostrarTablero
void mostrarTablero(const tpTablero &tablero) {
    for (int i = 0; i < tablero.nfils; ++i) {
        for (int j = 0; j < tablero.ncols; ++j) {
            switch (tablero.matriz[i][j]) {
                case NO_USADA:
                    cout << "-";
                    break;
                case VACIA:
                    cout << "x";
                    break;
                case OCUPADA:
                    cout << "o";
                    break;
            }
        }
        cout << endl;
    }
}

// verificar si un movimiento es válido
bool esMovimientoValido(const tpTablero &tablero, const tpMovimientoPieza &mov, const tpMovimientosValidos &movValidos) {
    int dx = mov.destino.x - mov.origen.x;
    int dy = mov.destino.y - mov.origen.y;

    // Depuración: Mostrar el movimiento que se está evaluando
    cout << "Evaluando movimiento: (" << mov.origen.x << ", " << mov.origen.y << ") -> ("
         << mov.destino.x << ", " << mov.destino.y << ")" << endl;

    // Asegurarse de que el movimiento está dentro de los límites establecidos
    if (mov.destino.x < 0 || mov.destino.x >= tablero.nfils || mov.destino.y < 0 || mov.destino.y >= tablero.ncols) {
        return false;
    }

    // Verificar si las celdas origen, destino y la intermedia cumplen las condiciones de movimiento
    int interX = mov.origen.x + dx / 2;
    int interY = mov.origen.y + dy / 2;

    if (tablero.matriz[mov.origen.x][mov.origen.y] == OCUPADA &&
        tablero.matriz[mov.destino.x][mov.destino.y] == VACIA &&
        tablero.matriz[interX][interY] == OCUPADA) {
        // Validar si el movimiento está permitido según movValidos
        int direccion = (dx + 2) / 2 * 3 + (dy + 2) / 2; 
        // Mapeo de dirección de dx y dy a (i) de movValidos
        // 0: (-2, -2) -> superior izquierda
        // 1: (-2,  0) -> superior
        // 2: (-2,  2) -> superior derecha
        // 3: ( 0, -2) -> izquierda
        // 4: ( 0,  2) -> derecha
        // 5: ( 2, -2) -> inferior izquierda
        // 6: ( 2,  0) -> inferior
        // 7: ( 2,  2) -> inferior derecha
        // Asegurarse de que la dirección es válida
        if (movValidos.validos[direccion]) {
            return true;
        }
    }

    // Si no se cumple alguna de las condiciones, el movimiento no es válido
    cout << "Movimiento inválido." << endl;
    return false;
}

// movimiento de la pieza en el tablero
// pre: el movimiento es válido
// post: se ha realizado el movimiento en el tablero
void realizarMovimiento(tpTablero &tablero, const tpMovimientoPieza &mov) {
    int dx = mov.destino.x - mov.origen.x;
    int dy = mov.destino.y - mov.origen.y;

    int interX = mov.origen.x + dx / 2;
    int interY = mov.origen.y + dy / 2;

    tablero.matriz[mov.origen.x][mov.origen.y] = VACIA;
    tablero.matriz[mov.destino.x][mov.destino.y] = OCUPADA;
    tablero.matriz[interX][interY] = VACIA;
}

// deshacer un movimiento
// pre: el movimiento es válido
// post: se ha deshecho el movimiento en el tablero
void deshacerMovimiento(tpTablero &tablero, const tpMovimientoPieza &mov) {
    int dx = mov.destino.x - mov.origen.x;
    int dy = mov.destino.y - mov.origen.y;

    int interX = mov.origen.x + dx / 2;
    int interY = mov.origen.y + dy / 2;

    tablero.matriz[mov.origen.x][mov.origen.y] = OCUPADA;
    tablero.matriz[mov.destino.x][mov.destino.y] = VACIA;
    tablero.matriz[interX][interY] = OCUPADA;
}

// Algoritmo de búsqueda recursiva con backtracking
// Pre: tablero contiene el estado inicial del que se parte para la búsqueda, 
//      movimientosValidos contiene los movimientos que están permitidos, 
//      solucionParcial contiene la solución actual como lista de movimientos, En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
// Post: solucionParcial contendrá la lista de movimientos completa (si no se llega a una solución, estará vacía, numMovs == 0)
//       Devuelve 1 si encuentra solución, -1 si no la encuentra.
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo) {
    bool hayMovimientos = false;

    for (int i = 0; i < tablero.nfils; ++i) {
        for (int j = 0; j < tablero.ncols; ++j) {
            if (tablero.matriz[i][j] == OCUPADA) {
                for (int dx = -2; dx <= 2; dx += 2) {
                    for (int dy = -2; dy <= 2; dy += 2) {
                        tpMovimientoPieza mov = {{i, j}, {i + dx, j + dy}};
                        // Corrección: esMovimientoValido ahora solo requiere tablero y mov
                        if (esMovimientoValido(tablero, mov, movValidos)) {
                            hayMovimientos = true;
                            realizarMovimiento(tablero, mov);
                            solucionParcial.movs[solucionParcial.numMovs++] = mov;

                            if (retardo > 0) {
                                mostrarTablero(tablero);
                                this_thread::sleep_for(chrono::milliseconds(retardo));
                            }

                            if (buscaSolucion(tablero, movValidos, solucionParcial, retardo) == 1) {
                                return 1; // Solución encontrada
                            }
                            
                            //backtracking deshacer movimiento para probar otro 
                            deshacerMovimiento(tablero, mov); 
                            //importante: deshacer el movimiento
                            solucionParcial.numMovs--;
                        }
                    }
                }
            }
        }
    }

    if (!hayMovimientos) {
        int piezasRestantes = 0;
        for (int i = 0; i < tablero.nfils; ++i) {
            for (int j = 0; j < tablero.ncols; ++j) {
                if (tablero.matriz[i][j] == OCUPADA) {
                    piezasRestantes++;
                }
            }
        }
        //depuración: mostrar el número de piezas restantes
        cout << "Piezas restantes: " << piezasRestantes << endl;
        return (piezasRestantes == 1) ? 1 : -1; // condicional si es 1 true, sino -1. forma corta del if-else
    }

    return -1;
}

// Implementa escribeListaMovimientos
void escribeListaMovimientos(const string nombreFichero, const tpListaMovimientos &solucion) {
    ofstream archivo(nombreFichero);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el fichero de salida." << endl;
        return;
    }

    if (solucion.numMovs == 0) {
        archivo << -1 << endl; // Escribir -1 si no hay solución
    } else {
        for (int i = 0; i < solucion.numMovs; ++i) {  // bucle for movimientos de la solución
            archivo << solucion.movs[i].origen.x << " " << solucion.movs[i].origen.y << " "
                    << solucion.movs[i].destino.x << " " << solucion.movs[i].destino.y << endl;
        }
    }

    archivo.close();
}
