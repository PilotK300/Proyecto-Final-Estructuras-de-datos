# DOCUMENTO DE DISEÑO
## Sistema de Codificación y Decodificación de Archivos FASTA con Huffman

**Fecha:** Octubre 16 2025
**Asignatura:** Estructuras de Datos
**Nombres** Juan David Ramos, Silvana Molano, Laura Catalina Gómez

---

## 1. DESCRIPCIÓN GENERAL

Este documento describe el diseño del Componente 2 del sistema de análisis de secuencias genéticas. El componente implementa un algoritmo de compresión Huffman para codificar y decodificar archivos FASTA en formato binario (.fabin), permitiendo optimizar el almacenamiento de datos genéticos.

---

## 2. OPERACIONES PRINCIPALES (COMANDOS)

### 2.1 COMANDO: codificar

**Descripción:** Comprime las secuencias genéticas cargadas en memoria utilizando el algoritmo de Huffman y las almacena en un archivo binario.

**Entrada:**
- Nombre del archivo de destino (string)
- Vector de secuencias en memoria (Genoma.secuencias)

**Salida:**
- Archivo binario .fabin con estructura especificada
- Mensaje de éxito o error al usuario

**Precondiciones:**
- Debe haber al menos una secuencia cargada en memoria
- El nombre del archivo debe ser válido y accesible
- El directorio de destino debe existir

**Postcondiciones:**
- Se crea el archivo .fabin con las secuencias codificadas
- Los datos originales en memoria permanecen sin cambios
- Se muestra mensaje confirmando la operación

**Mensajes de Salida:**
- Éxito: "Secuencias codificadas y almacenadas en [nombre_archivo]"
- Error sin secuencias: "No hay secuencias cargadas en memoria"
- Error de escritura: "No se pueden guardar las secuencias cargadas en [nombre_archivo]"

**Algoritmo:**
```
1. Verificar si hay secuencias cargadas
2. Si no, mostrar error y terminar
3. Crear árbol de Huffman desde frecuencias de todas las secuencias
4. Generar tabla de códigos binarios
5. Abrir archivo en modo escritura binaria
6. Escribir estructura del archivo:
   a) Número de bases diferentes (2 bytes)
   b) Para cada base: código (1 byte) y frecuencia (8 bytes)
   c) Número de secuencias (4 bytes)
   d) Para cada secuencia:
      - Longitud del nombre (2 bytes)
      - Caracteres del nombre (variable)
      - Longitud de secuencia binaria (8 bytes)
      - Bits de relleno (2 bytes)
      - Secuencia codificada en bytes (variable)
7. Cerrar archivo
8. Mostrar mensaje de éxito
```

---

### 2.2 COMANDO: decodificar

**Descripción:** Lee un archivo binario .fabin y recupera las secuencias genéticas originales, cargándolas en memoria.

**Entrada:**
- Nombre del archivo .fabin a leer (string)

**Salida:**
- Vector de secuencias recuperadas
- Actualización de Genoma.secuencias
- Mensaje de éxito o error al usuario

**Precondiciones:**
- El archivo .fabin debe existir y ser legible
- El archivo debe tener formato válido según especificación

**Postcondiciones:**
- Las secuencias anteriores en memoria se descartan (sobrescritura)
- Genoma.secuencias contiene las secuencias decodificadas
- Se muestra mensaje confirmando la operación

**Mensajes de Salida:**
- Éxito: "Secuencias decodificadas desde [nombre_archivo] y cargadas en memoria"
- Error: "No se pueden cargar las secuencias desde [nombre_archivo]"

**Algoritmo:**
```
1. Abrir archivo en modo lectura binaria
2. Si no abre, mostrar error y terminar
3. Leer número de bases diferentes (n)
4. Para cada base:
   a) Leer código de base (1 byte)
   b) Leer frecuencia (8 bytes)
   c) Almacenar en tabla de frecuencias
5. Reconstruir árbol de Huffman desde tabla de frecuencias
6. Leer número de secuencias (ns)
7. Para cada secuencia:
   a) Leer longitud del nombre (2 bytes)
   b) Leer nombre (variable)
   c) Leer longitud de secuencia binaria (8 bytes)
   d) Leer bits de relleno (2 bytes)
   e) Leer secuencia codificada en bytes
   f) Convertir bytes a binario
   g) Decodificar binario usando árbol de Huffman
   h) Crear estructura Secuencia
   i) Agregar al vector de resultado
8. Cerrar archivo
9. Retornar vector de secuencias
10. Actualizar Genoma.secuencias
11. Mostrar mensaje de éxito
```

---

## 3. TIPOS ABSTRACTOS DE DATOS (TADs)

### 3.1 TAD: NodoHuffman

**Descripción:** Representa un nodo en el árbol de Huffman, que puede ser una hoja (símbolo) o un nodo interno (combinación de dos nodos).

**Elementos:**
- `simbolo` (char): El símbolo almacenado (solo en hojas)
- `frecuencia` (long): Frecuencia del símbolo o suma de frecuencias de hijos
- `izquierdo` (shared_ptr<NodoHuffman>): Puntero al hijo izquierdo
- `derecho` (shared_ptr<NodoHuffman>): Puntero al hijo derecho

**Operaciones:**

| Operación | Entrada | Salida | Precondición | Postcondición |
|-----------|---------|--------|--------------|---------------|
| Crear(simbolo, frecuencia) | char, long | NodoHuffman | Ninguna | Crea nodo hoja con símbolo y frecuencia |
| Crear(izq, der) | NodoHuffman*, NodoHuffman* | NodoHuffman | Ambos no nulos | Crea nodo interno con suma de frecuencias |
| EsHoja() | Ninguna | bool | Ninguna | true si no tiene hijos, false si los tiene |

**Representación:**
```
NodoHuffman
├─ simbolo: char
├─ frecuencia: long
├─ izquierdo: NodoHuffman*
└─ derecho: NodoHuffman*
```

---

### 3.2 TAD: ArbolHuffman

**Descripción:** Árbol binario que implementa el algoritmo de Huffman para codificación óptima de símbolos según su frecuencia.

**Elementos:**
- `raiz` (shared_ptr<NodoHuffman>): Nodo raíz del árbol
- `tablaCodigos` (map<char, string>): Mapeo de símbolos a códigos binarios
- `tablaFrecuencias` (map<char, long>): Mapeo de símbolos a frecuencias

**Operaciones:**

| Operación | Entrada | Salida | Precondición | Postcondición |
|-----------|---------|--------|--------------|---------------|
| Construir(secuencias) | vector<string> | void | Secuencias no vacías | Árbol construido y tabla de códigos generada |
| ConstruirDesdeTabla(frecuencias) | map<char, long> | void | Tabla no vacía | Árbol reconstruido desde frecuencias |
| Codificar(secuencia) | string | string | Árbol existe | Retorna secuencia en código binario |
| Decodificar(binario) | string | string | Árbol existe, binario válido | Retorna secuencia original |
| ObtenerFrecuencias() | Ninguna | map<char, long> | Ninguna | Retorna tabla de frecuencias |
| ObtenerCodigos() | Ninguna | map<char, string> | Ninguna | Retorna tabla de códigos |

**Representación Interna:**
```
ArbolHuffman
├─ raiz: NodoHuffman*
├─ tablaCodigos: map<char, string>
│  └─ Ejemplo: {'A': "01", 'C': "11", 'G': "001", 'T': "10"}
└─ tablaFrecuencias: map<char, long>
   └─ Ejemplo: {'A': 450, 'C': 300, 'G': 200, 'T': 150}
```

---

### 3.3 TAD: CodificadorFASTA

**Descripción:** Clase utilitaria que gestiona la serialización y deserialización de secuencias genéticas usando Huffman, implementando entrada/salida de archivos binarios.

**Operaciones:**

| Operación | Entrada | Salida | Precondición | Postcondición |
|-----------|---------|--------|--------------|---------------|
| Codificar(nombreArchivo, secuencias) | string, vector<Secuencia> | bool | Secuencias no vacías | Archivo .fabin creado con estructura especificada |
| Decodificar(nombreArchivo) | string | vector<Secuencia> | Archivo existe y es válido | Retorna vector de secuencias o vacío si error |
| BinarioABytes(binario) | string | vector<unsigned char> | Ninguna | Convierte string binario a bytes, rellenando con 0s |
| BytesABinario(bytes, numBits) | vector<unsigned char>, long | string | numBits >= 0 | Convierte bytes a string binario truncado |

---

## 4. DISEÑO DE LA ESTRUCTURA DE ARCHIVO .fabin

```
┌─────────────────────────────────────────────────────────────┐
│ ESTRUCTURA DEL ARCHIVO BINARIO .fabin                       │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│ ┌─ TABLA DE FRECUENCIAS ─────────────────────────────────┐ │
│ │ n (2 bytes): número de bases diferentes               │ │
│ │ For i = 1 to n:                                        │ │
│ │   ci (1 byte): código de la base i                    │ │
│ │   fi (8 bytes): frecuencia de la base i               │ │
│ └────────────────────────────────────────────────────────┘ │
│                                                               │
│ ┌─ NÚMERO DE SECUENCIAS ─────────────────────────────────┐ │
│ │ ns (4 bytes): número de secuencias                    │ │
│ └────────────────────────────────────────────────────────┘ │
│                                                               │
│ ┌─ PARA CADA SECUENCIA ──────────────────────────────────┐ │
│ │ FOR j = 1 to ns:                                       │ │
│ │   li (2 bytes): longitud del nombre de secuencia j    │ │
│ │   sij (li bytes): caracteres del nombre               │ │
│ │   wi (8 bytes): longitud de secuencia binaria         │ │
│ │   xi (2 bytes): bits de relleno (padding)             │ │
│ │   binary_codej (variable): secuencia en bytes         │ │
│ └────────────────────────────────────────────────────────┘ │
│                                                               │
└─────────────────────────────────────────────────────────────┘
```

---

## 5. DIAGRAMAS DE FLUJO

### 5.1 Diagrama de Flujo: Comando CODIFICAR

```
                         ┌─────────────────────┐
                         │   INICIO codificar  │
                         │  (nombreArchivo)    │
                         └────────┬────────────┘
                                  │
                                  ▼
                    ┌─────────────────────────────┐
                    │ ¿Hay secuencias cargadas?   │
                    └────┬────────────────┬───────┘
                         │ NO             │ SÍ
                         │                ▼
                         │    ┌──────────────────────────┐
                         │    │ Construir Árbol Huffman  │
                         │    │ desde secuencias         │
                         │    └───────┬──────────────────┘
                         │            │
                         │            ▼
                         │    ┌──────────────────────────┐
                         │    │ Generar tabla de códigos │
                         │    └───────┬──────────────────┘
                         │            │
                         │            ▼
                         │    ┌──────────────────────────┐
                         │    │ Abrir archivo para       │
                         │    │ escribir (binario)       │
                         │    └───┬────────────┬─────────┘
                         │        │ ERROR      │ OK
                         │        │            ▼
                         │        │    ┌──────────────────────────┐
                         │        │    │ Escribir tabla de        │
                         │        │    │ frecuencias (n, ci, fi)  │
                         │        │    └───────┬──────────────────┘
                         │        │            │
                         │        │            ▼
                         │        │    ┌──────────────────────────┐
                         │        │    │ Escribir número de       │
                         │        │    │ secuencias (ns)          │
                         │        │    └───────┬──────────────────┘
                         │        │            │
                         │        │            ▼
                         │        │    ┌──────────────────────────┐
                         │        │    │ FOR cada secuencia:      │
                         │        │    │ - Escribir nombre        │
                         │        │    │ - Codificar con Huffman  │
                         │        │    │ - Escribir binario       │
                         │        │    │   codificado             │
                         │        │    └───────┬──────────────────┘
                         │        │            │
                         │        │            ▼
                         │        │    ┌──────────────────────────┐
                         │        │    │ Cerrar archivo           │
                         │        │    └───────┬──────────────────┘
                         │        │            │
         ┌───────────────┴─┬──────┴────────────┤
         │                │                    │
         ▼                ▼                    ▼
    ┌──────────────┐ ┌──────────────┐ ┌──────────────────┐
    │ Mostrar      │ │ Mostrar      │ │ Mostrar mensaje  │
    │ "No hay      │ │ "No se       │ │ de éxito         │
    │ secuencias"  │ │ pueden       │ │ "Secuencias      │
    │              │ │ guardar..."  │ │ codificadas..."  │
    └────┬─────────┘ └────┬─────────┘ └────┬─────────────┘
         │                │                 │
         └────────────────┴─────────────────┘
                          │
                          ▼
                    ┌──────────────┐
                    │   FIN        │
                    └──────────────┘
```

### 5.2 Diagrama de Flujo: Comando DECODIFICAR

```
                         ┌──────────────────────┐
                         │  INICIO decodificar  │
                         │  (nombreArchivo)     │
                         └────────┬─────────────┘
                                  │
                                  ▼
                    ┌─────────────────────────────┐
                    │ Abrir archivo para leer     │
                    │ (binario)                   │
                    └────┬────────────────┬───────┘
                         │ ERROR          │ OK
                         │                ▼
                         │    ┌──────────────────────────┐
                         │    │ Leer número de bases (n) │
                         │    └───────┬──────────────────┘
                         │            │
                         │            ▼
                         │    ┌──────────────────────────┐
                         │    │ FOR i = 1 to n:          │
                         │    │ Leer ci (base)           │
                         │    │ Leer fi (frecuencia)     │
                         │    │ Guardar en tabla         │
                         │    └───────┬──────────────────┘
                         │            │
                         │            ▼
                         │    ┌──────────────────────────┐
                         │    │ Reconstruir Árbol        │
                         │    │ Huffman desde tabla      │
                         │    │ de frecuencias           │
                         │    └───────┬──────────────────┘
                         │            │
                         │            ▼
                         │    ┌──────────────────────────┐
                         │    │ Leer número de           │
                         │    │ secuencias (ns)          │
                         │    └───────┬──────────────────┘
                         │            │
                         │            ▼
                         │    ┌──────────────────────────┐
                         │    │ FOR j = 1 to ns:         │
                         │    │ - Leer nombre            │
                         │    │ - Leer binario codificado│
                         │    │ - Decodificar con Huffman│
                         │    │ - Crear Secuencia        │
                         │    │ - Agregar al vector      │
                         │    └───────┬──────────────────┘
                         │            │
                         │            ▼
                         │    ┌──────────────────────────┐
                         │    │ Cerrar archivo           │
                         │    └───────┬──────────────────┘
                         │            │
         ┌───────────────┴────────────┤
         │                            │
         ▼                            ▼
    ┌──────────────────┐    ┌──────────────────────┐
    │ Retornar vector  │    │ Retornar vector      │
    │ vacío            │    │ de secuencias        │
    │ Mostrar error    │    │ Actualizar Genoma    │
    └──────┬───────────┘    │ Mostrar mensaje      │
           │                │ de éxito             │
           │                └──────┬───────────────┘
           │                       │
           └───────────────────────┘
                      │
                      ▼
                ┌──────────────┐
                │   FIN        │
                └──────────────┘
```

---

## 6. DIAGRAMA DEL ÁRBOL DE HUFFMAN

### 6.1 Ejemplo: Construcción de un árbol de Huffman

```
ENTRADA: Secuencias con frecuencias
A: 450, C: 300, G: 200, T: 150

PASO 1: Cola de prioridad con hojas
┌─────────────────────────────────────────────┐
│ Frecuencia │    Node                         │
├─────────────────────────────────────────────┤
│    150     │    T                            │
│    200     │    G                            │
│    300     │    C                            │
│    450     │    A                            │
└─────────────────────────────────────────────┘

PASO 2: Combinar los dos menores (T:150, G:200 = TG:350)
┌─────────────────────────────────────────────┐
│    300     │    C                            │
│    350     │   TG                            │
│    450     │    A                            │
└─────────────────────────────────────────────┘

PASO 3: Combinar C:300, TG:350 = CTG:650
┌─────────────────────────────────────────────┐
│    450     │    A                            │
│    650     │   CTG                           │
└─────────────────────────────────────────────┘

PASO 4: Combinar A:450, CTG:650 = ACTG:1100 (RAÍZ)
┌─────────────────────────────────────────────┐
│   1100     │  ACTG                           │
└─────────────────────────────────────────────┘

ÁRBOL FINAL:
                    ACTG(1100)
                   /          \
                 A(450)      CTG(650)
                            /        \
                          C(300)    TG(350)
                                   /        \
                                 T(150)    G(200)

TABLA DE CÓDIGOS GENERADA:
A → 0         (1 bit)
C → 10        (2 bits)
T → 110       (3 bits)
G → 111       (3 bits)

CODIFICACIÓN:
Secuencia: "ACGT"
Binaria:   "01011110"
Bytes:     0x5E (94 en decimal)
```

---

## 7. DIAGRAMA DE INTERACCIÓN DE CLASES

```
┌─────────────────────────────────────────────────────────────────┐
│                         GENOMA                                   │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │ - secuencias: vector<Secuencia>                         │   │
│  │ + SecuenciasCargadas(): int                             │   │
│  │ + VaciarGenoma(): void                                  │   │
│  └─────────────────────────────────────────────────────────┘   │
└────┬──────────────────────────────────────────────────────┬─────┘
     │ usa                                              usa │
     │                                                      │
     ▼                                                      ▼
┌─────────────────────────┐                  ┌──────────────────────┐
│   CODIFICADORFASTA      │                  │   ARBOLHUFFMAN       │
├─────────────────────────┤                  ├──────────────────────┤
│ + Codificar()           │                  │ - raiz: NodoHuffman* │
│ + Decodificar()         │                  │ - tablaCodigos       │
│ - BinarioABytes()       │◄─────────────────│ - tablaFrecuencias   │
│ - BytesABinario()       │      usa         ├──────────────────────┤
│ - EscribirValor()       │                  │ + Construir()        │
│ - LeerValor()           │                  │ + Codificar()        │
└──────────┬──────────────┘                  │ + Decodificar()      │
           │                                 │ + ObtenerFrecuencias│
           │                                 └──────────┬───────────┘
           │ crea                                       │
           │                                           │ usa
           ▼                                           ▼
      ┌──────────────────┐                    ┌──────────────────┐
      │   Archivo.fabin  │                    │  NODOHUFFMAN     │
      │ (binario)        │                    ├──────────────────┤
      │                  │                    │ - simbolo: char  │
      │ Estructura:      │                    │ - frecuencia     │
      │ ├─ n (2 bytes)   │                    │ - izquierdo*     │
      │ ├─ ci, fi        │                    │ - derecho*       │
      │ ├─ ns (4 bytes)  │                    ├──────────────────┤
      │ └─ seq binarias  │                    │ + EsHoja()       │
      └──────────────────┘                    └──────────────────┘
```

---

## 8. PSEUDOCÓDIGO DE FUNCIONES CLAVE

### 8.1 Construcción del Árbol de Huffman

```
FUNCIÓN Construir(secuencias: vector<string>)
  INICIO
    1. CalcularFrecuencias(secuencias)
    2. SI tablaFrecuencias está vacía ENTONCES
         raiz ← null
         RETORNA
       FIN SI
    
    3. SI tablaFrecuencias tiene 1 elemento ENTONCES
         raiz ← CrearNodo(elemento, frecuencia)
         tablaCodigos[elemento] ← "0"
         RETORNA
       FIN SI
    
    4. cola ← ColaDePropridad()
    
    5. PARA CADA (símbolo, frecuencia) EN tablaFrecuencias
         nodo ← CrearNodo(símbolo, frecuencia)
         cola.insertar(nodo)
       FIN PARA
    
    6. MIENTRAS cola.tamaño() > 1
         izquierdo ← cola.extraerMinimo()
         derecho ← cola.extraerMinimo()
         padre ← CrearNodo(izquierdo, derecho)
         cola.insertar(padre)
       FIN MIENTRAS
    
    7. raiz ← cola.extraerMinimo()
    
    8. GenerarCodigos(raiz, "")
  FIN
```

### 8.2 Codificación de Huffman

```
FUNCIÓN Codificar(secuencia: string) → string
  INICIO
    1. resultado ← ""
    2. PARA CADA carácter c EN secuencia
         SI c EXISTE EN tablaCodigos ENTONCES
           resultado ← resultado + tablaCodigos[c]
         FIN SI
       FIN PARA
    3. RETORNA resultado
  FIN
```

### 8.3 Decodificación de Huffman

```
FUNCIÓN Decodificar(binario: string) → string
  INICIO
    1. SI raiz es null ENTONCES
         RETORNA ""
       FIN SI
    
    2. resultado ← ""
    3. nodoActual ← raiz
    
    4. SI raiz es hoja ENTONCES
         PARA i ← 0 HASTA binario.longitud()-1
           resultado ← resultado + raiz.simbolo
         FIN PARA
         RETORNA resultado
       FIN SI
    
    5. PARA CADA bit EN binario
         SI bit = '0' ENTONCES
           nodoActual ← nodoActual.izquierdo
         SI NO
           nodoActual ← nodoActual.derecho
         FIN SI
         
         SI nodoActual es hoja ENTONCES
           resultado ← resultado + nodoActual.simbolo
           nodoActual ← raiz
         FIN SI
       FIN PARA
    
    6. RETORNA resultado
  FIN
```
