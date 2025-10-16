#ifndef PUNTO_H
#define PUNTO_H

struct Punto {
    double x; double y; double z;
    Punto(); Punto(double X, double Y, double Z);
};

double DistanciaEuclidiana(const Punto& a, const Punto& b);
double DistanciaManhattan(const Punto& a, const Punto& b);

#endif
