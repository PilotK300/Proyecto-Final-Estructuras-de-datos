//
// Created by HP on 13/10/2025.
//

#ifndef HUFFMANN_H
#define HUFFMANN_H
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <memory>

// Nodo del árbol de Huffman
class NodoHuffman {
public:
    char simbolo;
    long frecuencia;
    std::shared_ptr<NodoHuffman> izquierdo;
    std::shared_ptr<NodoHuffman> derecho;

    NodoHuffman(char s, long f);
    NodoHuffman(std::shared_ptr<NodoHuffman> izq, std::shared_ptr<NodoHuffman> der);
    bool EsHoja() const;
};

// Comparador para la cola de prioridad
struct ComparadorNodo {
    bool operator()(const std::shared_ptr<NodoHuffman>& a,
                   const std::shared_ptr<NodoHuffman>& b) const {
        return a->frecuencia > b->frecuencia;
    }
};

// Clase principal para el árbol de Huffman
class ArbolHuffman {
private:
    std::shared_ptr<NodoHuffman> raiz;
    std::map<char, std::string> tablaCodigos;
    std::map<char, long> tablaFrecuencias;

    void GenerarCodigos(std::shared_ptr<NodoHuffman> nodo, const std::string& codigo);
    void CalcularFrecuencias(const std::vector<std::string>& secuencias);

public:
    ArbolHuffman();

    // Construir el árbol desde las secuencias
    void Construir(const std::vector<std::string>& secuencias);

    // Construir el árbol desde frecuencias guardadas
    void ConstruirDesdeTabla(const std::map<char, long>& frecuencias);

    // Codificar una secuencia
    std::string Codificar(const std::string& secuencia) const;

    // Decodificar una secuencia binaria
    std::string Decodificar(const std::string& binario) const;

    // Obtener la tabla de frecuencias
    std::map<char, long> ObtenerFrecuencias() const;

    // Obtener la tabla de códigos
    std::map<char, std::string> ObtenerCodigos() const;
};
#endif //HUFFMANN_H
