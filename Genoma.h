#ifndef GENOMA_H
#define GENOMA_H

#include "Secuencia.h"
#include <string>
#include <vector>

struct Genoma {
    std::vector<Secuencia> secuencias;

    void VaciarGenoma();
    int SecuenciasCargadas() const;
    Secuencia* EncontrarSecuencia(const std::string& nombre);
    long ContarBases() const;
    long BuscarSubsecuencia(const std::string& sub) const;
    long EnmascararBases(const std::string& sub);
};

#endif
