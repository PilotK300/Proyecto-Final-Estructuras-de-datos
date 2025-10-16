#include "Punto.h"

Punto::Punto() : x(0.0), y(0.0), z(0.0) {}
Punto::Punto(double X, double Y, double Z) : x(X), y(Y), z(Z) {}

static double raizCuadradaBasica(double n) {
    if (n <= 0.0) return 0.0;
    double x = n; int i = 0;
    while (i < 20) { x = 0.5 * (x + n / x); i++; }
    return x;
}

double DistanciaEuclidiana(const Punto& a, const Punto& b) {
    double dx = a.x - b.x; double dy = a.y - b.y; double dz = a.z - b.z;
    double suma = dx*dx + dy*dy + dz*dz; return raizCuadradaBasica(suma);
}

double DistanciaManhattan(const Punto& a, const Punto& b) {
    double dx = (a.x > b.x) ? (a.x - b.x) : (b.x - a.x);
    double dy = (a.y > b.y) ? (a.y - b.y) : (b.y - a.y);
    double dz = (a.z > b.z) ? (a.z - b.z) : (b.z - a.z);
    return dx + dy + dz;
}
