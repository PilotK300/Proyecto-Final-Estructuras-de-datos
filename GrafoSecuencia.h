//
// Created by HP on 24/11/2025.
//

#ifndef GRAFOSECUENCIA_H
#define GRAFOSECUENCIA_H
#include "Secuencia.h"
#include <vector>
#include <string>
#include <cmath>

struct ResultadoRuta {
    bool existe = false;
    std::string camino;
    char baseOrigen = '\0';
    char baseDestino = '\0';
    double costo = 0.0;
    int fila = -1, col = -1;
};

class GrafoSecuencia {
private:
    const Secuencia* secuencia;
    int filas, columnas;

    inline int Posicion(int i, int j) const { return i * columnas + j; }
    inline double Peso(char a, char b) const { return 1.0 /( 1.0 + std::abs((int)a - (int)b)); }

public:
    GrafoSecuencia(const Secuencia* s);

    // Calcula la ruta más corta (Dijkstra) entre (i,j) y (x,y).
    ResultadoRuta RutaMasCorta(int i, int j, int x, int y);

    // Busca la base más lejana (misma letra) desde (i,j) y devuelve su ruta y costo.
    ResultadoRuta BaseRemota(int i, int j);
};
#endif //GRAFOSECUENCIA_H
