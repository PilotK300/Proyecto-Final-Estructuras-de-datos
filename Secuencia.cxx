#include "Secuencia.h"
#include <cctype>

Secuencia::Secuencia() : nombre(""), bases(""), ancho(60) {}

Secuencia::Secuencia(const std::string& desc, const std::string& data, int width)
    : nombre(desc), bases(data), ancho(width > 0 ? width : 60) {}

int Secuencia::CantidadBases() const {
    return (int)bases.size();
}

int Secuencia::ContadorBasesReales() const {
    int c = 0; int i = 0;
    while (i < (int)bases.size()) {
        if (bases[(std::size_t)i] != '-') c++;
        i++;
    }
    return c;
}

bool Secuencia::VerificarSecuencia() const {
    int i = 0;
    while (i < (int)bases.size()) {
        if (bases[(std::size_t)i] == '-') return false;
        i++;
    }
    return true;
}

const std::string& Secuencia::CaracteresOrden() {
    static const std::string orden = "ACGTURYKMSWBDHVNX-";
    return orden;
}

std::vector<long> Secuencia::NumeroBasesOrdenado() const {
    const std::string& ord = CaracteresOrden();
    std::vector<long> f;
    int j = 0; while (j < (int)ord.size()) { f.push_back(0); j++; }

    int i = 0;
    while (i < (int)bases.size()) {
        char ch = bases[(std::size_t)i];
        if (ch >= 'a' && ch <= 'z') ch = (char)std::toupper((unsigned char)ch);
        int k = 0;
        while (k < (int)ord.size()) {
            if (ch == ord[(std::size_t)k]) { f[(std::size_t)k] = f[(std::size_t)k] + 1; break; }
            k++;
        }
        i++;
    }
    return f;
};

long Secuencia::ContarSubsecuencia(const std::string& sub) const {
    if (sub.empty()) return 0;
    long total = 0; int p = 0;

    while (p + (int)sub.size() <= (int)bases.size()) {
        int k = 0; bool ok = true;
        while (k < (int)sub.size()) {
            if (bases[(std::size_t)(p + k)] != sub[(std::size_t)k]) { ok = false; break; }
            k++;
        }
        if (ok) { total = total + 1; p = p + (int)sub.size(); }
        else { p = p + 1; }
    }
    return total;
};

long Secuencia::EnmascararSubsecuencia(const std::string& sub) {
    if (sub.empty()) return 0;
    long total = 0; int p = 0;

    while (p + (int)sub.size() <= (int)bases.size()) {
        int k = 0; bool ok = true;
        while (k < (int)sub.size()) {
            if (bases[(std::size_t)(p + k)] != sub[(std::size_t)k]) { ok = false; break; }
            k++;
        }
        if (ok) {
            int t = 0; while (t < (int)sub.size()) { bases[(std::size_t)(p + t)] = 'X'; t++; }
            total = total + 1; p = p + (int)sub.size();
        } else { p = p + 1; }
    }
    return total;
};

void Secuencia::EstablecerTamanoSecuencia(int nuevoAncho) {
    if (nuevoAncho > 0) ancho = nuevoAncho;
};
