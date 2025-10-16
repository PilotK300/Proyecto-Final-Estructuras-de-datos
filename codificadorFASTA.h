//
// Created by HP on 13/10/2025.
//
#ifndef CODIFICADOR_FASTA_H
#define CODIFICADOR_FASTA_H

#include <string>
#include <vector>
#include "Secuencia.h"
#include "Huffmann.h"

class CodificadorFASTA {
private:
    // Convierte una cadena binaria a bytes
    static std::vector<unsigned char> BinarioABytes(const std::string& binario);

    // Convierte bytes a una cadena binaria
    static std::string BytesABinario(const std::vector<unsigned char>& bytes, long numBits);

    // Escribe un valor en el archivo
    template<typename T>
    static void EscribirValor(std::ofstream& archivo, T valor);

    // Lee un valor del archivo
    template<typename T>
    static T LeerValor(std::ifstream& archivo);

public:
    // Codifica las secuencias y las guarda en un archivo binario
    static bool Codificar(const std::string& nombreArchivo,
                         const std::vector<Secuencia>& secuencias);

    // Decodifica un archivo binario y retorna las secuencias
    static std::vector<Secuencia> Decodificar(const std::string& nombreArchivo);
};
#endif //CODIFICADOR_FASTA_H
