//
// Created by HP on 13/10/2025.
//
#include "Huffmann.h"
#include <queue>
#include <memory>
#include <map>
#include <string>
#include <vector>

// Implementación NodoHuffman
NodoHuffman::NodoHuffman(char s, long f)
    : simbolo(s), frecuencia(f), izquierdo(nullptr), derecho(nullptr) {}

NodoHuffman::NodoHuffman(std::shared_ptr<NodoHuffman> izq, std::shared_ptr<NodoHuffman> der)
    : simbolo('\0'), frecuencia(izq->frecuencia + der->frecuencia),
      izquierdo(izq), derecho(der) {}

bool NodoHuffman::EsHoja() const {
    return izquierdo == nullptr && derecho == nullptr;
}

// Implementación ArbolHuffman
ArbolHuffman::ArbolHuffman() : raiz(nullptr) {}

void ArbolHuffman::CalcularFrecuencias(const std::vector<std::string>& secuencias) {
    tablaFrecuencias.clear();
    for (const auto& seq : secuencias) {
        for (char c : seq) {
            tablaFrecuencias[c]++;
        }
    }
}

void ArbolHuffman::Construir(const std::vector<std::string>& secuencias) {
    CalcularFrecuencias(secuencias);

    if (tablaFrecuencias.empty()) {
        raiz = nullptr;
        return;
    }

    // Caso especial: solo un símbolo único
    if (tablaFrecuencias.size() == 1) {
        auto it = tablaFrecuencias.begin();
        raiz = std::make_shared<NodoHuffman>(it->first, it->second);
        tablaCodigos[it->first] = "0";
        return;
    }

    // Cola de prioridad para construir el árbol
    std::priority_queue<std::shared_ptr<NodoHuffman>,
                       std::vector<std::shared_ptr<NodoHuffman>>,
                       ComparadorNodo> cola;

    for (const auto& par : tablaFrecuencias) {
        cola.push(std::make_shared<NodoHuffman>(par.first, par.second));
    }

    while (cola.size() > 1) {
        auto izq = cola.top();
        cola.pop();
        auto der = cola.top();
        cola.pop();

        auto padre = std::make_shared<NodoHuffman>(izq, der);
        cola.push(padre);
    }

    raiz = cola.top();
    tablaCodigos.clear();
    GenerarCodigos(raiz, "");
}

void ArbolHuffman::ConstruirDesdeTabla(const std::map<char, long>& frecuencias) {
    tablaFrecuencias = frecuencias;

    if (tablaFrecuencias.empty()) {
        raiz = nullptr;
        return;
    }

    if (tablaFrecuencias.size() == 1) {
        auto it = tablaFrecuencias.begin();
        raiz = std::make_shared<NodoHuffman>(it->first, it->second);
        tablaCodigos[it->first] = "0";
        return;
    }

    std::priority_queue<std::shared_ptr<NodoHuffman>,
                       std::vector<std::shared_ptr<NodoHuffman>>,
                       ComparadorNodo> cola;

    for (const auto& par : tablaFrecuencias) {
        cola.push(std::make_shared<NodoHuffman>(par.first, par.second));
    }

    while (cola.size() > 1) {
        auto izq = cola.top();
        cola.pop();
        auto der = cola.top();
        cola.pop();

        auto padre = std::make_shared<NodoHuffman>(izq, der);
        cola.push(padre);
    }

    raiz = cola.top();
    tablaCodigos.clear();
    GenerarCodigos(raiz, "");
}

void ArbolHuffman::GenerarCodigos(std::shared_ptr<NodoHuffman> nodo, const std::string& codigo) {
    if (!nodo) return;

    if (nodo->EsHoja()) {
        tablaCodigos[nodo->simbolo] = codigo;
        return;
    }

    GenerarCodigos(nodo->izquierdo, codigo + "0");
    GenerarCodigos(nodo->derecho, codigo + "1");
}

std::string ArbolHuffman::Codificar(const std::string& secuencia) const {
    std::string resultado;
    for (char c : secuencia) {
        auto it = tablaCodigos.find(c);
        if (it != tablaCodigos.end()) {
            resultado += it->second;
        }
    }
    return resultado;
}

std::string ArbolHuffman::Decodificar(const std::string& binario) const {
    if (!raiz) return "";

    std::string resultado;
    auto nodoActual = raiz;

    // Caso especial: solo un símbolo
    if (raiz->EsHoja()) {
        long cantidad = binario.length();
        for (long i = 0; i < cantidad; i++) {
            resultado += raiz->simbolo;
        }
        return resultado;
    }

    for (char bit : binario) {
        if (bit == '0') {
            nodoActual = nodoActual->izquierdo;
        } else {
            nodoActual = nodoActual->derecho;
        }

        if (nodoActual->EsHoja()) {
            resultado += nodoActual->simbolo;
            nodoActual = raiz;
        }
    }

    return resultado;
}

std::map<char, long> ArbolHuffman::ObtenerFrecuencias() const {
    return tablaFrecuencias;
}

std::map<char, std::string> ArbolHuffman::ObtenerCodigos() const {
    return tablaCodigos;
}
