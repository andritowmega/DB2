#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstdlib> 
#include <cstring> 
#include <cstdlib>

using namespace std;

struct Sector {
    int bytes;
};

struct Pista {
    int sectores;
    Sector* sectorArray;
};

struct Plato {
    int pistas;
    Pista* pistaArray;
};

struct DiscoMegatron {
    int platos;
    Plato* platoArray;
};

void crearArchivo(const char* filename, const char* contenido) {
    // Abrir el archivo de texto en modo escritura
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se pudo crear el archivo de texto: " << filename << endl;
        return; // Salir de la función si no se puede crear el archivo
    }

    // Escribir el contenido en el archivo de texto
    file << contenido;

    // Cerrar el archivo
    file.close();
}
char* obtenerLinea(const char* nombreArchivo, int numeroLinea) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de texto." << std::endl;
        return nullptr;
    }


    char linea[200];
    int contador = 0;
    while (contador < numeroLinea && archivo.getline(linea, sizeof(linea))) {
        contador++;
    }

    if (contador == numeroLinea) {
        // La línea se leyó correctamente, ahora puedes copiarla en un char*
        int longitud = std::strlen(linea); // Calcular la longitud de la línea
        char* valor = new char[longitud + 1]; // Crear un char* para almacenar el valor de la línea
        std::strcpy(valor, linea); // Copiar la línea en el char*
        archivo.close();
        return valor;
    } else {
        std::cerr << "Error: No se pudo leer la línea " << numeroLinea << " del archivo de texto." << std::endl;
        archivo.close();
        return nullptr; //si hay error se devuelve null
    }
}
void agregarLinea(const char* nombreArchivo, const char* nuevaLinea) {
    std::ofstream archivo(nombreArchivo, std::ios::app);
    
    // Verificar si el archivo se abrió correctamente
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo." << std::endl;
        return;
    }
    
    // Escribir la nueva línea en el archivo
    archivo << std::endl <<nuevaLinea;
    
    // Cerrar el archivo
    archivo.close();
}

DiscoMegatron* crearDisco(int numPlatos, int pistasPorSuperficie, int sectoresPorPista, int bytesPorSector) {
    DiscoMegatron* disco = new DiscoMegatron;
    disco->platos = numPlatos;
    disco->platoArray = new Plato[numPlatos];

    // Inicializar las propiedades de cada plato y pista
    for (int i = 0; i < numPlatos; ++i) {
        disco->platoArray[i].pistas = pistasPorSuperficie * 2; // Dos superficies por plato
        disco->platoArray[i].pistaArray = new Pista[disco->platoArray[i].pistas];
        for (int j = 0; j < disco->platoArray[i].pistas; ++j) {
            disco->platoArray[i].pistaArray[j].sectores = sectoresPorPista;
            disco->platoArray[i].pistaArray[j].sectorArray = new Sector[sectoresPorPista];
            for (int k = 0; k < sectoresPorPista; ++k) {
                disco->platoArray[i].pistaArray[j].sectorArray[k].bytes = bytesPorSector;
            }
        }
    }
    int tam = numPlatos*2*pistasPorSuperficie*sectoresPorPista*bytesPorSector;

    //linea 1: datos del disco
    const char* filename = "directorio.txt";
    const char* contenidoInicial = "";
    char contenido[100]; 
    std::sprintf(contenido, "%s%d,%d,%d,%d,%d,%d", contenidoInicial, numPlatos,2, pistasPorSuperficie, sectoresPorPista, bytesPorSector,tam);
    crearArchivo(filename, contenido);
    const char* nuevaLinea = "0";
    agregarLinea(filename, nuevaLinea);
    cout<<"Disco Creado"<<endl;
    return disco;
}


int contarLineas(const char* nombreArchivo) {
    // Abrir el archivo
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo." << std::endl;
        return -1; // Devolver -1 en caso de error
    }

    // Contar el número de líneas
    int contador = 0;
    std::string linea;
    while (std::getline(archivo, linea)) {
        contador++;
    }

    // Cerrar el archivo
    archivo.close();

    return contador;
}
void reemplazarLinea(char* nombreArchivo, int numLinea, char* nuevaLinea, bool endlineadd=true) {
    FILE* archivoEntrada = fopen(nombreArchivo, "r");
    if (archivoEntrada == nullptr) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    FILE* archivoTemporal = tmpfile();
    if (archivoTemporal == nullptr) {
        std::cerr << "Error: No se pudo crear el archivo temporal." << std::endl;
        fclose(archivoEntrada);
        return;
    }

    char buffer[256];
    int lineaActual = 1;

    while (fgets(buffer, sizeof(buffer), archivoEntrada) != nullptr) {
        if (lineaActual == numLinea) {
            fputs(nuevaLinea, archivoTemporal);
            if(endlineadd){
                fputc('\n', archivoTemporal);
            }
        } else {
            fputs(buffer, archivoTemporal);
        }
        lineaActual++;
    }

    fclose(archivoEntrada);
    rewind(archivoTemporal);

    archivoEntrada = fopen(nombreArchivo, "w");
    if (archivoEntrada == nullptr) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << " para escritura." << std::endl;
        fclose(archivoTemporal);
        return;
    }

    while (fgets(buffer, sizeof(buffer), archivoTemporal) != nullptr) {
        fputs(buffer, archivoEntrada);
    }

    fclose(archivoEntrada);
    fclose(archivoTemporal);
}
char* intToCharPtr(int num) {
    char buffer[20]; //numero de dìgitos
    snprintf(buffer, sizeof(buffer), "%d", num);

    char* charPtr = new char[strlen(buffer) + 1];
    strcpy(charPtr, buffer);

    return charPtr;
}
int contarCaracteres(const char* cadena) {
    int contador = 0;
    while (*cadena != '\0' && *cadena != '#') {
        contador++;
        cadena++;
    }
    return contador;
}
char* insertarEnBloque(char * valorInsertar){
    char* retornarDatos = new char[200];
    char* bloques = obtenerLinea("directorio.txt",2);
    int numeroBloques = std::atoi(bloques);
    if(numeroBloques==0){
        crearArchivo("b1.txt","270");
        numeroBloques++;
        reemplazarLinea("directorio.txt",2,"1",false);
    }
    bloques = intToCharPtr(numeroBloques);
    char * nombreBloques = new char[200];
    std::strcpy(nombreBloques,"b");
    std::strcat(nombreBloques,bloques);
    std::strcat(nombreBloques,".txt");
    char * valor = obtenerLinea(nombreBloques,1);
    int espacioEnBloque = std::atoi(valor);
    //---------------------------------->>calcular tamaño<<---------------------------------------------------------------------------
    int tamano = contarCaracteres(valorInsertar);
     cout<<"actual tam "<<espacioEnBloque<<endl;
     cout<<"nuevmo tam "<<tamano<<endl;
    if(espacioEnBloque>=tamano){
        cout<<"se insertara "<<valorInsertar<<endl;
        agregarLinea(nombreBloques,valorInsertar);
        int linea = contarLineas(nombreBloques);
        std::strcpy(retornarDatos,bloques);
        std::strcat(retornarDatos,",");
        std::strcat(retornarDatos,intToCharPtr(linea));
        char * nuevoTamanoBloque = new char[200];
        nuevoTamanoBloque = intToCharPtr(espacioEnBloque-tamano);
        cout<<"nuevmo tam "<<nuevoTamanoBloque<<endl;
        reemplazarLinea(nombreBloques,1,nuevoTamanoBloque);
        return retornarDatos;
    }
    //creamos nuevo bloque
    numeroBloques++;
    bloques = intToCharPtr(numeroBloques);
    std::strcpy(nombreBloques,"b");
    std::strcat(nombreBloques,bloques);
    std::strcat(nombreBloques,".txt");
    //actualizamos directorio
    reemplazarLinea("directorio.txt",numeroBloques,"1");
    crearArchivo(nombreBloques,"270");
    agregarLinea(nombreBloques,valorInsertar);
    int linea = contarLineas(nombreBloques);
    std::strcpy(retornarDatos,bloques);
    std::strcat(retornarDatos,",");
    std::strcat(retornarDatos,intToCharPtr(linea));
    char * nuevoTamanoBloque = new char[3];
        nuevoTamanoBloque = intToCharPtr(espacioEnBloque-tamano);
        reemplazarLinea(nombreBloques,1,nuevoTamanoBloque);
    return retornarDatos;
}

void insertData(const char * line){
      const char* nombreArchivo = "directorio.txt";
    // Obtener la cantidad de líneas del archivo
    const int TAMANIO_MAXIMO = 200;
    int cantidadLineas = contarLineas(nombreArchivo);
    char* nuevaCadena = new char[TAMANIO_MAXIMO];
   

        std::strncpy(nuevaCadena,line,152);
        nuevaCadena[15] = '\0';
        if(std::strcmp(nuevaCadena, "create database") == 0) {
            char* nuevoValor = new char[TAMANIO_MAXIMO];
            char* paraInsertar = new char[TAMANIO_MAXIMO];
            char* paraInsertarBloque = new char[TAMANIO_MAXIMO];
            const char* espacio = std::strchr(line + 16, ';');
            int longitud;
            if (espacio != nullptr) {
                longitud = espacio - (line + 16); // Longitud de la subcadena hasta el espacio
            } else {
                longitud = std::strlen(line + 16); // Si no se encuentra un espacio, la longitud es hasta el final de la cadena
            }
            longitud++;
            std::strncpy(nuevoValor, line + 16, longitud);
            nuevoValor[longitud-1] = '\0';
            std::strcpy(paraInsertarBloque,nuevoValor);
            std::strcpy(paraInsertar,"DATABASE,");
            std::cout << "Se insertará base de datos: "<< nuevoValor << std::endl;
            char* bloque = insertarEnBloque(nuevoValor);
            std:strcat(paraInsertar,bloque);
            agregarLinea(nombreArchivo, paraInsertar);
        } else {
            std::cout << "Se insertará registro." << std::endl;
        }
}

void imprimirContenidoArchivo(const string& filename) {
    // Abrir el archivo de texto
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de texto: " << filename << endl;
        return; // Salir de la función si no se puede abrir el archivo
    }

    // Leer y mostrar el contenido del archivo línea por línea
    
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    // Cerrar el archivo
    file.close();
}
int contarCaracteresHastaEspacio(char* cadena) {
    int contador = 0;
    while (*cadena != '\0' && *cadena != ' ') {
        contador++;
        cadena++;
    }
    return contador;
}
void insertarRelacion(const char * line){
      const char* nombreArchivo = "directorio.txt";
    // Obtener la cantidad de líneas del archivo
    const int TAMANIO_MAXIMO = 200;
    int cantidadLineas = contarLineas(nombreArchivo);
    char* nuevaCadena = new char[TAMANIO_MAXIMO];
        std::strncpy(nuevaCadena,line,13);
        nuevaCadena[13] = '\0';
        if(std::strcmp(nuevaCadena, "create table ") == 0) {
            std::cout<<"crear relacion"<<endl;
            
            char* paraInsertar = new char[TAMANIO_MAXIMO];
            char* paraInsertarBloque = new char[TAMANIO_MAXIMO];
            const char* espacio = std::strchr(line + 13, ' ');
           
            int longitud;
            
            if (espacio != nullptr) {
                longitud = espacio - (line + 13); // Longitud de la subcadena hasta el espacio
            } else {
                longitud = std::strlen(line + 13); // Si no se encuentra un espacio, la longitud es hasta el final de la cadena
            }
            longitud++;
            
            char* nuevoValor = new char[longitud];
        
            std::strncpy(nuevoValor, line + 13, longitud);
            nuevoValor[longitud-1] = '\0';
            std::strcpy(paraInsertarBloque,nuevoValor);
            std::strcpy(paraInsertar,"TABLE,");
            std::cout << "Se insertará base de datos: "<< nuevoValor << std::endl;
            char* bloque = insertarEnBloque(nuevoValor);
            std::strcat(paraInsertar,bloque);
            agregarLinea(nombreArchivo, paraInsertar);

            const char* espacio2 = std::strchr(line + 13+longitud,';');
            int longitud2;
            if (espacio2 != nullptr) {
                longitud2 = espacio2 - (line + 13+longitud+1); // Longitud de la subcadena hasta el espacio
            } else {
                longitud2 = std::strlen(line + 13+longitud+1); // Si no se encuentra un espacio, la longitud es hasta el final de la cadena
            }

            char* nuevoValorrelacion = new char[longitud2];
            std::strncpy(nuevoValorrelacion, line + 13+longitud+1, longitud2);
            nuevoValorrelacion[longitud2-1] = '\0';
            std::strcpy(paraInsertarBloque,nuevoValorrelacion);
            std::strcpy(paraInsertar,"RELATION,");
            //std::cout << "Se insertará base de datos: "<< nuevoValor << std::endl;
            bloque = insertarEnBloque(nuevoValorrelacion);
            std::strcat(paraInsertar,nuevoValor);
            std::strcat(paraInsertar,",");
            std::strcat(paraInsertar,bloque);
            agregarLinea(nombreArchivo, paraInsertar);
        }else{
            std::cout<<"error de sintaxys"<<endl;
        }
}

void insertarRegistro(const char * line){
      const char* nombreArchivo = "directorio.txt";
    // Obtener la cantidad de líneas del archivo
    const int TAMANIO_MAXIMO = 200;
    int cantidadLineas = contarLineas(nombreArchivo);
    char* nuevaCadena = new char[TAMANIO_MAXIMO];
        std::strncpy(nuevaCadena,line,12);
        nuevaCadena[12] = '\0';
        if(std::strcmp(nuevaCadena, "insert into ") == 0) {
            std::cout<<"crear registro"<<endl;
            
            char* paraInsertar = new char[TAMANIO_MAXIMO];
            char* paraInsertarBloque = new char[TAMANIO_MAXIMO];
            const char* espacio = std::strchr(line + 12, ' ');
           
            int longitud;
            
            if (espacio != nullptr) {
                longitud = espacio - (line + 12); // Longitud de la subcadena hasta el espacio
            } else {
                longitud = std::strlen(line + 12); // Si no se encuentra un espacio, la longitud es hasta el final de la cadena
            }
            longitud++;
            
            char* nuevoValor = new char[longitud];
        
            std::strncpy(nuevoValor, line + 12, longitud);
            nuevoValor[longitud-1] = '\0';
            std::strcpy(paraInsertarBloque,nuevoValor);
            std::strcpy(paraInsertar,"ROW,");
            std::cout << "Se insertará registro: "<< nuevoValor << std::endl;
            char* bloque = insertarEnBloque(nuevoValor);
            std::strcat(paraInsertar,bloque);
            //agregarLinea(nombreArchivo, paraInsertar);

            const char* espacio2 = std::strchr(line + 12+longitud,';');
            int longitud2;
            if (espacio2 != nullptr) {
                longitud2 = espacio2 - (line + 12+longitud+1); // Longitud de la subcadena hasta el espacio
            } else {
                longitud2 = std::strlen(line + 12+longitud+1); // Si no se encuentra un espacio, la longitud es hasta el final de la cadena
            }

            char* nuevoValorrelacion = new char[longitud2];
            std::strncpy(nuevoValorrelacion, line + 12+longitud+1, longitud2);
            nuevoValorrelacion[longitud2-1] = '\0';
            std::strcpy(paraInsertarBloque,nuevoValorrelacion);
            std::strcpy(paraInsertar,"ROW,");
            //std::cout << "Se insertará base de datos: "<< nuevoValor << std::endl;
            bloque = insertarEnBloque(nuevoValorrelacion);
            std::strcat(paraInsertar,nuevoValor);
            std::strcat(paraInsertar,",");
            std::strcat(paraInsertar,bloque);
            agregarLinea(nombreArchivo, paraInsertar);
        }else{
            std::cout<<"error de sintaxys"<<endl;
        }
}

void liberarDisco(DiscoMegatron* disco) {
    if (disco == nullptr) {
        return; // Si el disco es nullptr, no hay nada que liberar
    }

    for (int i = 0; i < disco->platos; ++i) {
        for (int j = 0; j < disco->platoArray[i].pistas; ++j) {
            delete[] disco->platoArray[i].pistaArray[j].sectorArray;
        }
        delete[] disco->platoArray[i].pistaArray;
    }
    delete[] disco->platoArray;
    delete disco;
}
const int MAX_TOKENS = 10; // Número máximo de tokens por línea
bool contieneDATABASE(const char* tokens[], int numTokens,const char* palabraBuscada) {
    
    for (int i = 0; i < numTokens; ++i) {
        if (std::strcmp(tokens[i], palabraBuscada) == 0) {
            int line = std::atoi(tokens[i+2]);
            char * nuevalinea = obtenerLinea("b1.txt",line);
            std::cout<<"->:"<<nuevalinea<<endl;
            return true; // Se encontró la palabra "DATABASE"
        }
    }

    return false; // No se encontró la palabra "DATABASE"
}
bool contieneROW(const char* tokens[], int numTokens,const char* palabraBuscada) {
    
    for (int i = 0; i < numTokens; ++i) {
        if (std::strcmp(tokens[i], palabraBuscada) == 0) {
            int line = std::atoi(tokens[i+3]);
            char * nuevalinea = obtenerLinea("b1.txt",line);
            std::cout<<"->:"<<tokens[i+1]<<"::"<<nuevalinea<<endl;
            return true; // Se encontró la palabra "DATABASE"
        }
    }

    return false; // No se encontró la palabra "DATABASE"
}

// Función para leer un archivo línea por línea y procesar las líneas
void leerArchivo(const char* nombreArchivo,const char* palabraBuscada) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    char linea[200];
    int numLinea = 0;

    while (archivo.getline(linea, 200)) {
        numLinea++;

        // Tokenizar la línea por comas
        const char* delimitador = ",";
        char* token = std::strtok(linea, delimitador);
        const int MAX_TOKENS = 100; // Máximo número de tokens por línea
        const char* tokens[MAX_TOKENS];
        int numTokens = 0;

        while (token != nullptr && numTokens < MAX_TOKENS) {
            tokens[numTokens++] = token;
            token = std::strtok(nullptr, delimitador);
        }
        // Verificar si alguno de los tokens es igual a "DATABASE"
        if (contieneDATABASE(tokens, numTokens,palabraBuscada)) {
            char * valor = obtenerLinea("directorio.txt",numLinea);
        }
    }

    archivo.close();
}
void leerRow(const char* nombreArchivo,const char* palabraBuscada) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    char linea[200];
    int numLinea = 0;

    while (archivo.getline(linea, 200)) {
        numLinea++;

        // Tokenizar la línea por comas
        const char* delimitador = ",";
        char* token = std::strtok(linea, delimitador);
        const int MAX_TOKENS = 100; // Máximo número de tokens por línea
        const char* tokens[MAX_TOKENS];
        int numTokens = 0;

        while (token != nullptr && numTokens < MAX_TOKENS) {
            tokens[numTokens++] = token;
            token = std::strtok(nullptr, delimitador);
        }
        // Verificar si alguno de los tokens es igual a "DATABASE"
        if (contieneROW(tokens, numTokens,palabraBuscada)) {
            char * valor = obtenerLinea("directorio.txt",numLinea);
        }
    }

    archivo.close();
}
void obtenerBases(){
    const char* nombreArchivo = "directorio.txt";
    const char* palabraBuscada = "DATABASE";
    leerArchivo(nombreArchivo,palabraBuscada);
}
void obtenerTablas(){
    const char* nombreArchivo = "directorio.txt";
    const char* palabraBuscada = "TABLE";
    leerArchivo(nombreArchivo,palabraBuscada);
}
void obtenerRow(){
    const char* nombreArchivo = "directorio.txt";
    const char* palabraBuscada = "ROW";
    leerRow(nombreArchivo,palabraBuscada);
}
int main() {
    string filename = "test.txt";
    const int TAMANIO_MAXIMO = 200;
    char* texto = new char[TAMANIO_MAXIMO];
    DiscoMegatron* disco;
     char opcion;
     do {
        // Mostrar el menú
        std::cout << std::endl;
        std::cout << "Gestor de Base de Datos:" << std::endl;
        std::cout << "1) Crear Disco" << std::endl;
        std::cout << "2) Crear Base de datos" << std::endl;
        std::cout << "3) Crear Relación" << std::endl;
        std::cout << "4) Insertar Registro" << std::endl;
        std::cout << "5) Obtener bases de datos" << std::endl;
        std::cout << "6) Obtener tablas" << std::endl;
        std::cout << "7) Obtener registros" << std::endl;
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;
        system("clear");
        
        // Procesar la opción seleccionada
        switch (opcion) {
            case '1':
                std::cout << "Creando disco" << std::endl;
                //platos,pista,sectores,bytes
                disco = crearDisco(8, 10, 128, 90);
                break;
            case '2':
                std::cout << "CREAR BASE DE DATOS" << std::endl;
                std::cout << "SQL: ";
                std::cin.ignore();
                std::cin.getline(texto, TAMANIO_MAXIMO);
                std::cout << "Usted ingresó: " << texto+16 << std::endl;
                insertData(texto);
                break;
            case '3':
                std::cout << "CREAR RELACION:" << std::endl;
                std::cout << "SQL: ";
                std::cin.ignore();
                std::cin.getline(texto, TAMANIO_MAXIMO);
                std::cout << "Usted ingresó: " << texto << std::endl;
                insertarRelacion(texto);
                break;
            case '4':
                std::cout << "CREAR REGISTRO:" << std::endl;
                std::cout << "SQL: ";
                std::cin.ignore();
                std::cin.getline(texto, TAMANIO_MAXIMO);
                std::cout << "Usted ingresó: " << texto << std::endl;
                insertarRegistro(texto);

                break;
            case '5':
                std::cout << "VER BASES DE DATOS:" << std::endl;
                obtenerBases();
                break;
            case '6':
                std::cout << "VER TABLAS:" << std::endl;
                obtenerTablas();
                break;
            case '7':
                std::cout << "VER Registros:" << std::endl;
                obtenerRow();

                break;
            default:
                std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
                break;
        }
    } while (opcion != '9');
    // Crear disco con 8 platos, 16384 pistas por superficie, 128 sectores por pista y 4096 bytes por sector
    

    // Leer el archivo de texto
    //imprimirContenidoArchivo(filename);
    

    return 0;
}

