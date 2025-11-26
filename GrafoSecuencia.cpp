//
// Created by HP on 24/11/2025.
//
#include "GrafoSecuencia.h"
#include <queue>
#include <limits>
#include <sstream>
#include <algorithm>

GrafoSecuencia::GrafoSecuencia(const Secuencia* s) : secuencia(s) {
    // filas: número de filas necesarias para representar la secuencia con ancho = s->ancho
    columnas = s->ancho > 0 ? s->ancho : 60;
    filas = (int)(s->bases.size() / columnas);
    if (s->bases.size() % columnas != 0) filas++;
}

ResultadoRuta GrafoSecuencia::RutaMasCorta(int i, int j, int x, int y) {
    ResultadoRuta r; r.existe = false;

    if (!secuencia) return r;
    // Validar índices (i,j) y (x,y)
    if (i < 0 || j < 0 || x < 0 || y < 0) return r;
    if (i >= filas || j >= columnas || x >= filas || y >= columnas) return r;

    int nBases = (int)secuencia->bases.size();
    int origen = Posicion(i, j);
    int destino = Posicion(x, y);
    if (origen < 0 || origen >= nBases) return r;
    if (destino < 0 || destino >= nBases) return r;

    int n = filas * columnas;
    std::vector<double> dist(n, std::numeric_limits<double>::max());
    std::vector<int> prev(n, -1);
    std::vector<char> used(n, 0);

    auto cmp = [&](int a, int b){ return dist[a] > dist[b]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp);

    dist[origen] = 0;
    pq.push(origen);

    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        if (used[u]) continue;
        used[u] = 1;
        if (u == destino) break;

        int fi = u / columnas, co = u % columnas;
        char baseU = secuencia->bases[u];

        int di[4] = {-1,1,0,0};
        int dj[4] = {0,0,-1,1};
        for (int k=0;k<4;k++){
            int ni = fi + di[k], nj = co + dj[k];
            if (ni < 0 || nj < 0 || ni >= filas || nj >= columnas) continue;
            int v = Posicion(ni, nj);
            if (v < 0 || v >= nBases) continue;
            char baseV = secuencia->bases[v];
            double peso = Peso(baseU, baseV);
            if (dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                prev[v] = u;
                pq.push(v);
            }
        }
    }

    if (dist[destino] == std::numeric_limits<double>::max()) return r;

    std::vector<int> camino;
    for (int at = destino; at != -1; at = prev[at]) camino.push_back(at);
    std::reverse(camino.begin(), camino.end());

    std::ostringstream oss;
    for (int idx : camino) {
        int fi = idx / columnas, co = idx % columnas;
        oss << "[" << fi << "," << co << "] ";
    }

    r.existe = true;
    r.camino = oss.str();
    r.baseOrigen = secuencia->bases[origen];
    r.baseDestino = secuencia->bases[destino];
    r.costo = dist[destino];
    r.fila = x;
    r.col = y;
    return r;
}

ResultadoRuta GrafoSecuencia::BaseRemota(int i, int j) {
    ResultadoRuta r; r.existe = false;
    if (!secuencia) return r;
    if (i < 0 || j < 0) return r;
    if (i >= filas || j >= columnas) return r;

    int nBases = (int)secuencia->bases.size();
    int origen = Posicion(i, j);
    if (origen < 0 || origen >= nBases) return r;

    char baseBuscada = secuencia->bases[origen];

    int n = filas * columnas;
    std::vector<double> dist(n, std::numeric_limits<double>::max());
    std::vector<int> prev(n, -1);
    std::vector<char> used(n, 0);

    auto cmp = [&](int a,int b){ return dist[a] > dist[b]; };
    std::priority_queue<int,std::vector<int>,decltype(cmp)> pq(cmp);

    dist[origen] = 0; pq.push(origen);

    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        if (used[u]) continue;
        used[u] = 1;

        int fi = u / columnas, co = u % columnas;
        char baseU = secuencia->bases[u];

        int di[4] = {-1,1,0,0}; int dj[4] = {0,0,-1,1};
        for (int k=0;k<4;k++){
            int ni = fi + di[k], nj = co + dj[k];
            if (ni<0 || nj<0 || ni>=filas || nj>=columnas) continue;
            int v = Posicion(ni,nj);
            if (v < 0 || v >= nBases) continue;
            char baseV = secuencia->bases[v];
           double peso = Peso(baseU, baseV);
            if (dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                prev[v] = u;
                pq.push(v);
            }
        }
    }

    int mejor = -1; double maxDist = -1.0;
    for (int idx = 0; idx < nBases; idx++) {
        if (secuencia->bases[idx] == baseBuscada && dist[idx] != std::numeric_limits<double>::max()) {
            if (dist[idx] > maxDist) { maxDist = dist[idx]; mejor = idx; }
        }
    }
    if (mejor == -1) return r;

    std::vector<int> camino;
    for (int at = mejor; at != -1; at = prev[at]) camino.push_back(at);
    std::reverse(camino.begin(), camino.end());

    std::ostringstream oss;
    for (int idx : camino) {
        int fi = idx / columnas, co = idx % columnas;
        oss << "[" << fi << "," << co << "] ";
    }

    r.existe = true;
    r.camino = oss.str();
    r.baseOrigen = baseBuscada;
    r.baseDestino = baseBuscada;
    r.costo = maxDist;
    r.fila = mejor / columnas;
    r.col = mejor % columnas;
    r.col = mejor % columnas;
    return r;
}