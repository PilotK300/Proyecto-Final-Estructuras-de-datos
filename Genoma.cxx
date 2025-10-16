#include "Genoma.h"

void Genoma::VaciarGenoma() {
    secuencias.clear();
}

Secuencia* Genoma::EncontrarSecuencia(const std::string& nombre) {
    int i = 0;
    while (i < (int)secuencias.size()) {
        if (secuencias[(std::size_t)i].nombre == nombre) return &secuencias[(std::size_t)i];
        i++;
    }
    return 0;
}

int Genoma::SecuenciasCargadas() const {
    return (int)secuencias.size();
}

long Genoma::ContarBases() const {
    long total = 0; int i = 0;
    while (i < (int)secuencias.size()) {
        total += (long)secuencias[(std::size_t)i].bases.size();
        i++;
    }
    return total;
}

long Genoma::BuscarSubsecuencia(const std::string& sub) const {
    if (sub.empty()) return 0;
    long total = 0; int i = 0;
    while (i < (int)secuencias.size()) { total += secuencias[(std::size_t)i].ContarSubsecuencia(sub); i++; }
    return total;
}

long Genoma::EnmascararBases(const std::string& sub) {
    if (sub.empty()) return 0;
    long total = 0; int i = 0;
    while (i < (int)secuencias.size()) { total += secuencias[(std::size_t)i].EnmascararSubsecuencia(sub); i++; }
    return total;
}
