#ifndef SECUENCIA_H
#define SECUENCIA_H

#include <string>
#include <vector>

struct Secuencia {
    std::string nombre;
    std::string bases;
    int ancho;

    Secuencia();
    Secuencia(const std::string& desc, const std::string& data, int width);

    int CantidadBases() const;
    bool VerificarSecuencia() const;
    int ContadorBasesReales() const;
    static const std::string& CaracteresOrden();
    std::vector<long> NumeroBasesOrdenado() const;
    long ContarSubsecuencia(const std::string& sub) const;
    long EnmascararSubsecuencia(const std::string& sub);
    void EstablecerTamanoSecuencia(int nuevoAncho);
};

#endif
