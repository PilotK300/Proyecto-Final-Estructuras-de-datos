#include <iostream>
#include <string>
#include <vector>
#include "Genoma.h"
#include "FASTA.hxx"
#include "CodificadorFASTA.h"
static const std::string ORDEN_HISTO = "ACGTURYKMSWBDHVNX-";

static void partir(const std::string& s, std::string& a, std::string& b) {
    a.clear(); b.clear();
    std::size_t i = 0;
    while (i < s.size() && (s[i] == ' ' || s[i] == '\t')) i++;
    while (i < s.size() && s[i] != ' ' && s[i] != '\t') { a.push_back(s[i]); i++; }
    while (i < s.size() && (s[i] == ' ' || s[i] == '\t')) i++;
    while (i < s.size()) { b.push_back(s[i]); i++; }
}

static void listar(const Genoma& g) {
    int n = g.SecuenciasCargadas();
    if (n == 0) { std::cout << "No hay secuencias cargadas en memoria" << std::endl; return; }
    std::cout << "hay " << n << " secuencias cargadas en memoria" << std::endl;
    int i = 0;
    while (i < n) {
        const Secuencia& s = g.secuencias[(std::size_t)i];
        if (s.VerificarSecuencia())
            std::cout << "Secuencia " << s.nombre << " contiene " << s.CantidadBases() << " bases" << std::endl;
        else
            std::cout << "Secuencia " << s.nombre << " contiene al menos " << s.ContadorBasesReales() << " bases" << std::endl;
        i++;
    }
}

static void histo(Genoma& g, const std::string& d) {
    if (d.size() == 0) { std::cout << "Secuencia invalida" << std::endl; return; }
    Secuencia* p = g.EncontrarSecuencia(d);
    if (!p) { std::cout << "Secuencia invalida" << std::endl; return; }
    std::vector<long> f = p->NumeroBasesOrdenado();
    int i = 0; while (i < (int)ORDEN_HISTO.size()) { std::cout << ORDEN_HISTO[(std::size_t)i] << ":" << f[(std::size_t)i] << std::endl; i++; }
}

static void ayuda() {
    std::cout << "Comandos disponibles: cargar, listar_secuencias, histograma, es_subsecuencia, enmascarar, guardar,codificar, decodificar y salir" << std::endl;
}

static void ayuda1(const std::string& c) {
    if (c == "cargar") std::cout << "Carga en memoria los datos contenidos en el archivo identificado por nombre_archivo ,es decir, utiliza adecuadamente las estructuras lineales para cargar la información de los genomas en memoria. Si dentro de la misma sesión de trabajo ya se han cargado otros archivos de secuencias (usando el comando cargar), la información debe sobreescribirse en memoria, es decir, no se deben combinar informaciones de secuencias de diferentes archivos." << std::endl;
    else if (c == "listar_secuencias") std::cout << "Imprime la cantidad de secuencias actualmente en memoria, y luego, en n líneas (una para secuencia) la información básica (nombre y cantidad de bases diferentes) de cada secuencia. Si la secuencia es completa (no tiene el código ’-’) indica la cantidad de bases exactas (contiene b bases); si la secuencia es incompleta (incluye uno o varios códigos ’-’) indica la cantidad mínima de bases, sin contar el código ’-’ (contiene al menos b bases)." << std::endl;
    else if (c == "histograma") std::cout << "Imprime el histograma de una secuencia, en caso de que exista. El histograma se define como el conteo (frecuencia) de cada código en la secuencia. Por cada línea (’\n’ es el caracter de salto 2 de línea) se escribe el código y la cantidad de veces que aparece en la secuencia. El ordenamiento del histograma está dado por la Tabla 1. El código ’-’ debe incluirse también como parte del histograma." << std::endl;
    else if (c == "es_subsecuencia") std::cout << "Determina si una subsecuencia (secuencia corta de bases) dada por el usuario, existe dentro de las secuencias cargadas en memoria. Si es así, determina la cantidad de veces en las que esta subsecuencia dada se repite. No es necesario indicar el nombre de la secuencia donde se encuentra la repetición (el conteo es general sobre todas las secuencias cargadas en memoria)." << std::endl;
    else if (c == "enmascarar") std::cout << "Enmascara una subsecuencia (secuencia corta de bases) dada por el usuario, si existe. Los elementos que pertenecen a la subsecuencia se enmascaran cambiando cada base individual por el código ’X’. No es necesario indicar el nombre de la secuencia donde se realiza el enmascarado (el proceso es general sobre todas las secuencias cargadas en memoria)." << std::endl;
    else if (c == "guardar") std::cout << "Guarda en el archivo nombre_archivo las secuencias cargadas en memoria. Se debe tener en cuenta la justificación (de líneas) de cada secuencia inicial, así como las posibles modificaciones que hayan sufrir las secuencias en memoria (después de enmascarar)." << std::endl;
	else if (c == "codificar") std::cout << "Uso: codificar nombre_archivo.fabin" << std::endl;
    else if (c == "decodificar") std::cout << "Uso: decodificar nombre_archivo.fabin" << std::endl;
    else if (c == "salir") std::cout << "Termina la ejecución de la aplicación." << std::endl;
    else std::cout << "Comando desconocido" << std::endl;
}

int main() {
    Genoma g; std::string line;

    while (true) {
        std::cout << "$ ";
        if (!std::getline(std::cin, line)) break;
        if (line.size() == 0) continue;

        std::string cmd, arg; partir(line, cmd, arg);

        if (cmd == "salir") { break; }
        else if (cmd == "ayuda") { if (arg.size() == 0) ayuda(); else ayuda1(arg); }
        else if (cmd == "cargar") {
            if (arg.size() == 0) { std::cout << "nombre_archivo  no existe o no se puede leer" << std::endl; continue; }
            try {
                std::vector<Secuencia> v = FASTA::LecturaArchivo(arg);
                if (v.empty()) { std::cout << arg << " no contiene ninguna secuencia" << std::endl; }
                else {
                    g.VaciarGenoma();
                    int i = 0; while (i < (int)v.size()) { g.secuencias.push_back(v[(std::size_t)i]); i++; }
                    if ((int)v.size() == 1) std::cout << "se han cargado 1 secuencia correctamente" << std::endl;
                    else std::cout << "se han cargado " << v.size() << " secuencias correctamente" << std::endl;
                }
            } catch (...) { std::cout << arg << "  no existe o no se puede leer" << std::endl; }
        }
        else if (cmd == "listar_secuencias") { listar(g); }
        else if (cmd == "histograma") { histo(g, arg); }
        else if (cmd == "es_subsecuencia") {
            if (g.SecuenciasCargadas() == 0) std::cout << "no hay secuencias cargadas en memoria" << std::endl;
            else if (arg.size() == 0) std::cout << "la subsecuencia dada no existe dentro de las secuencias cargadas en memoria" << std::endl;
            else {
                long s = g.BuscarSubsecuencia(arg);
                if (s <= 0) std::cout << "la subsecuencia dada no existe dentro de las secuencias cargadas en memoria" << std::endl;
                else std::cout << "la subsecuencia dada se repite " << s << " veces dentro de las secuencias cargadas en memoria" << std::endl;
            }
        }
        else if (cmd == "enmascarar") {
            if (g.SecuenciasCargadas() == 0) std::cout << "no hay secuencias cargadas en memoria" << std::endl;
            else if (arg.size() == 0) std::cout << "la subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada." << std::endl;
            else {
                long s = g.EnmascararBases(arg);
                if (s <= 0) std::cout << "la subsecuencia dada no existe dentro de las secuencias cargadas en memoria, por tanto no se enmascara nada." << std::endl;
                else std::cout << s << " subsecuencias han sido enmascaradas dentro de las secuencias cargadas en memoria" << std::endl;
            }
        }
        else if (cmd == "guardar") {
            if (g.SecuenciasCargadas() == 0) std::cout << "no hay secuencias cargadas en memoria" << std::endl;
            else if (arg.size() == 0) std::cout << "Error guardando en nombre_archivo" << std::endl;
            else {
                bool ok = FASTA::EscrituraArchivo(arg, g.secuencias);
                if (ok) std::cout << "las secuencias han sido guardas en " << arg << std::endl;
                else std::cout << "Error guardando en " << arg << std::endl;
            }
        }
  else if (cmd == "codificar") {
            if (g.SecuenciasCargadas() == 0) {
                std::cout << "No hay secuencias cargadas en memoria" << std::endl;
            }
            else if (arg.size() == 0) {
                std::cout << "No se pueden guardar las secuencias cargadas en nombre_archivo.fabin" << std::endl;
            }
            else {
                bool ok = CodificadorFASTA::Codificar(arg, g.secuencias);
                if (ok) {
                    std::cout << "Secuencias codificadas y almacenadas en " << arg << std::endl;
                } else {
                    std::cout << "No se pueden guardar las secuencias cargadas en " << arg << std::endl;
                }
            }
        }
        else if (cmd == "decodificar") {
            if (arg.size() == 0) {
                std::cout << "No se pueden cargar las secuencias desde nombre_archivo.fabin" << std::endl;
            }
            else {
                std::vector<Secuencia> v = CodificadorFASTA::Decodificar(arg);
                if (v.empty()) {
                    std::cout << "No se pueden cargar las secuencias desde " << arg << std::endl;
                } else {
                    g.VaciarGenoma();
                    int i = 0;
                    while (i < (int)v.size()) {
                        g.secuencias.push_back(v[(std::size_t)i]);
                        i++;
                    }
                    std::cout << "Secuencias decodificadas desde " << arg << " y cargadas en memoria" << std::endl;
                }
            }
        }
        else { std::cout << "Comando desconocido" << std::endl; }
    }

    return 0;
}
