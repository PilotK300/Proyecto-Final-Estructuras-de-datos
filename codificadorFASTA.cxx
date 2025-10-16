//
// Created by HP on 13/10/2025.
//
#include <string>
#include <vector>
#include "Secuencia.h"
#include "Huffmann.h"
#include <fstream>
#include "CodificadorFASTA.h"
#include <fstream>
#include <cstring>

std::vector<unsigned char> CodificadorFASTA::BinarioABytes(const std::string& binario) {
    std::vector<unsigned char> bytes;
    std::string temp = binario;

    // Completar con ceros si no es múltiplo de 8
    while (temp.length() % 8 != 0) {
        temp += "0";
    }

    for (size_t i = 0; i < temp.length(); i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8; j++) {
            if (temp[i + j] == '1') {
                byte |= (1 << (7 - j));
            }
        }
        bytes.push_back(byte);
    }

    return bytes;
}

std::string CodificadorFASTA::BytesABinario(const std::vector<unsigned char>& bytes, long numBits) {
    std::string binario;

    for (unsigned char byte : bytes) {
        for (int i = 7; i >= 0; i--) {
            binario += ((byte >> i) & 1) ? '1' : '0';
        }
    }

    // Truncar al número exacto de bits
    if (numBits > 0 && (long)binario.length() > numBits) {
        binario = binario.substr(0, numBits);
    }

    return binario;
}

template<typename T>
void CodificadorFASTA::EscribirValor(std::ofstream& archivo, T valor) {
    archivo.write(reinterpret_cast<const char*>(&valor), sizeof(T));
}

template<typename T>
T CodificadorFASTA::LeerValor(std::ifstream& archivo) {
    T valor;
    archivo.read(reinterpret_cast<char*>(&valor), sizeof(T));
    return valor;
}

bool CodificadorFASTA::Codificar(const std::string& nombreArchivo,
                                 const std::vector<Secuencia>& secuencias) {
    if (secuencias.empty()) {
        return false;
    }

    std::ofstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    // Construir árbol de Huffman
    std::vector<std::string> secuenciasTexto;
    for (const auto& seq : secuencias) {
        secuenciasTexto.push_back(seq.bases);
    }

    ArbolHuffman arbol;
    arbol.Construir(secuenciasTexto);

    auto frecuencias = arbol.ObtenerFrecuencias();

    // 1. Escribir número de bases diferentes (n) - 2 bytes
    unsigned short n = static_cast<unsigned short>(frecuencias.size());
    EscribirValor(archivo, n);

    // 2. Escribir cada base y su frecuencia (ci: 1 byte, fi: 8 bytes)
    for (const auto& par : frecuencias) {
        unsigned char ci = static_cast<unsigned char>(par.first);
        long long fi = par.second;
        EscribirValor(archivo, ci);
        EscribirValor(archivo, fi);
    }

    // 3. Escribir número de secuencias (ns) - 4 bytes
    unsigned int ns = static_cast<unsigned int>(secuencias.size());
    EscribirValor(archivo, ns);

    // 4. Para cada secuencia
    for (const auto& seq : secuencias) {
        // li: Tamaño del nombre - 2 bytes
        unsigned short li = static_cast<unsigned short>(seq.nombre.length());
        EscribirValor(archivo, li);

        // sij: Caracteres del nombre
        for (char c : seq.nombre) {
            EscribirValor(archivo, static_cast<unsigned char>(c));
        }

        // Codificar la secuencia
        std::string binario = arbol.Codificar(seq.bases);

        // wi: Longitud de la secuencia binaria - 8 bytes
        long long wi = binario.length();
        EscribirValor(archivo, wi);

        // xi: Justificación (bits de relleno) - 2 bytes
        unsigned short xi = (8 - (binario.length() % 8)) % 8;
        EscribirValor(archivo, xi);

        // binary_codei: Secuencia binaria convertida a bytes
        std::vector<unsigned char> bytes = BinarioABytes(binario);
        for (unsigned char byte : bytes) {
            EscribirValor(archivo, byte);
        }
    }

    archivo.close();
    return true;
}

std::vector<Secuencia> CodificadorFASTA::Decodificar(const std::string& nombreArchivo) {
    std::vector<Secuencia> secuencias;

    std::ifstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo.is_open()) {
        return secuencias;
    }

    try {
        // 1. Leer número de bases diferentes (n)
        unsigned short n = LeerValor<unsigned short>(archivo);

        // 2. Leer frecuencias
        std::map<char, long> frecuencias;
        for (int i = 0; i < n; i++) {
            unsigned char ci = LeerValor<unsigned char>(archivo);
            long long fi = LeerValor<long long>(archivo);
            frecuencias[static_cast<char>(ci)] = fi;
        }

        // Construir árbol desde frecuencias
        ArbolHuffman arbol;
        arbol.ConstruirDesdeTabla(frecuencias);

        // 3. Leer número de secuencias
        unsigned int ns = LeerValor<unsigned int>(archivo);

        // 4. Leer cada secuencia
        for (unsigned int i = 0; i < ns; i++) {
            // li: Tamaño del nombre
            unsigned short li = LeerValor<unsigned short>(archivo);

            // sij: Nombre
            std::string nombre;
            for (int j = 0; j < li; j++) {
                unsigned char c = LeerValor<unsigned char>(archivo);
                nombre += static_cast<char>(c);
            }

            // wi: Longitud de la secuencia binaria
            long long wi = LeerValor<long long>(archivo);

            // xi: Justificación
            unsigned short xi = LeerValor<unsigned short>(archivo);

            // binary_codei: Leer bytes
            long long numBytes = (wi + 7) / 8; // Redondeo hacia arriba
            std::vector<unsigned char> bytes;
            for (long long j = 0; j < numBytes; j++) {
                bytes.push_back(LeerValor<unsigned char>(archivo));
            }

            // Convertir a binario y decodificar
            std::string binario = BytesABinario(bytes, wi);
            std::string secuenciaDecodificada = arbol.Decodificar(binario);

            // Crear y agregar la secuencia
            Secuencia seq;
            seq.nombre = nombre;
            seq.bases = secuenciaDecodificada;
            secuencias.push_back(seq);
        }
    } catch (...) {
        secuencias.clear();
    }

    archivo.close();
    return secuencias;
}
