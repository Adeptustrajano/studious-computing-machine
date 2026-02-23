extern void resolverReinas (int n, int reinas[]); 

int main () { 
	const int NREINAS = 11;  // Numero de reinas 
	int solucion[NREINAS];  // Vector de posiciones de reinas en cada fila 
	
	// Inicializar vector (ninguna reina colocada) 
	for (int i=0; i<NREINAS; i++) { 
		solucion[i] = -1; 
	} 
	if (NREINAS>=4) { 
		resolverReinas (NREINAS, solucion); 
	} 
	while(1); // bucle infinito finalizacion 
} 