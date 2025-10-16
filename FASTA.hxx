#ifndef FASTA_HXX
#define FASTA_HXX

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include "Secuencia.h"

namespace FASTA {

static inline bool EsEspacio(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

inline std::vector<Secuencia> LecturaArchivo(const std::string& ruta) {
    std::ifstream in(ruta.c_str());
    if (!in) { throw std::runtime_error("IO"); }

    std::vector<Secuencia> v;
    std::string linea; std::string desc = ""; std::string buf = "";
    int anchoActual = 0; bool dentro = false; bool algunaCabecera = false;

    while (std::getline(in, linea)) {
        if (!linea.empty() && linea[(std::size_t)0] == '>') {
            if (dentro) {
                if (anchoActual <= 0) anchoActual = 60;
                v.push_back(Secuencia(desc, buf, anchoActual));
                buf = ""; anchoActual = 0;
            }
            std::string d = ""; std::size_t i = 1;
            while (i < linea.size()) { d.push_back(linea[i]); i++; }
            desc = d; dentro = true; algunaCabecera = true;
        } else {
            int noesp = 0; std::size_t i = 0;
            while (i < linea.size()) {
                char c = linea[i];
                if (!EsEspacio(c)) { buf.push_back(c); noesp++; }
                i++;
            }
            if (noesp > 0 && anchoActual == 0) anchoActual = noesp;
        }
    }

    if (dentro) {
        if (anchoActual <= 0) anchoActual = 60;
        v.push_back(Secuencia(desc, buf, anchoActual));
    }

    if (!algunaCabecera) return std::vector<Secuencia>();
    return v;
}

inline bool EscrituraArchivo(const std::string& ruta, const std::vector<Secuencia>& v) {
    std::ofstream out(ruta.c_str());
    if (!out) return false;

    std::size_t i = 0;
    while (i < v.size()) {
        out << ">" << v[i].nombre << "\n";
        const std::string& b = v[i].bases;
        int ancho = v[i].ancho; if (ancho <= 0) ancho = 60;

        std::size_t p = 0;
        while (p < b.size()) {
            int cuenta = 0;
            while (p < b.size() && cuenta < ancho) {
                out << b[p]; p++; cuenta++;
            }
            out << "\n";
        }
        i++;
    }
    return true;
}

}

#endif
