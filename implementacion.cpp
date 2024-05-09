#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstdlib> 
#include <cstring> 
#include <cstdlib>

using namespace std;

struct PistaSector {
    public:
        int bytes;
        int free = 0;
        char* data;
        PistaSector() {};
        PistaSector(int _bytes) {
            this->bytes = _bytes;
        }
        int getFreeSize() {
            return this->bytes - this->free;
        }
};

struct Superficie {
    PistaSector** pistaSector;
    PistaSector** bloques;
    public:
        int bloquesTam =0;
        Superficie(int _pistas, int _bloquestam, int _sectores, int _bytes) {
            this->bloquesTam = _bloquestam;
            pistaSector = new PistaSector*[_pistas];
            bloques = new PistaSector*[_pistas]; // Cambiado a un doble puntero
            for(int i=0; i < _pistas; i++) {
                this->pistaSector[i] = new PistaSector[_sectores];
                int countBloque = 0;
                for(int j=0; j < _sectores; j++) {
                    this->pistaSector[i][j] = PistaSector(_bytes);
                    if(j%_bloquestam==0){
                        this->pistaSector[i][countBloque] = this->pistaSector[i][j];
                        countBloque++;
                    }
                }
            }
        }
};

struct Plato {
    Superficie* superficies;
    public:
        int total = 0;
        Plato() {};
        Plato(int _pistas, int _bloquestam, int _sectores, int _bytes) {
            superficies = new Superficie[2]{ Superficie(_pistas, _bloquestam, _sectores, _bytes), Superficie(_pistas, _bloquestam, _sectores, _bytes) };
        };

};

struct Disco {
    Plato* platos;
    public:
        int platosTotal = 0;
        Disco() {};
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
        void crearDisco(int numPlatos, int pistasPorSuperficie, int sectoresPorPista,int bloquestam ,int bytesPorSector) {
            this->platosTotal = numPlatos;
            this->platos = new Plato[numPlatos];
            for(int i=0; i < numPlatos; i++) {
                this->platos[i] = Plato(pistasPorSuperficie, bloquestam, sectoresPorPista, bytesPorSector);
            }
            //linea 1: datos del disco
            
            const char* filename = "directorio2.txt";
            const char* contenidoInicial = "";
             char contenido[100]; 
            std::sprintf(contenido, "%s%d,%d,%d,%d,%d,%d", contenidoInicial, numPlatos,2, pistasPorSuperficie, sectoresPorPista,bloquestam, bytesPorSector);
            crearArchivo(filename, contenido);
            const char* nuevaLinea = "0";
            agregarLinea(filename, nuevaLinea);
            cout<<"-> Disco Creado"<<endl;
        }
        char* obtenerLinea(const char* nombreArchivo, int numeroLinea) {
            std::ifstream archivo(nombreArchivo);
            if (!archivo.is_open()) {
                std::cerr << "Error: No se pudo leer el disco." << std::endl;
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
};

int main() {
    Disco disco;
    char opcion;
    int platos=0;
    int pistas=0;
    int sectores=0;
    int bloquestam =0;
    int espacio=0;
    if(disco.obtenerLinea("directorio2.txt",1)==nullptr){
        std::cout<<"-----------> Formateando Disco <--------"<<std::endl;
        std::cout << "Ingrese número de platos: ";
        std::cin >> platos;
        std::cout << "Ingrese número de pistas por superficie: ";
        std::cin >> pistas;
        std::cout << "Ingrese número de sectores por pista: ";
        std::cin >> sectores;
        std::cout << "Ingrese número de sectores por bloque ";
        std::cin >> bloquestam;
        std::cout << "Ingrese espacio en bytes por sector: ";
        std::cin >> espacio;
        disco.crearDisco(platos,pistas,sectores,bloquestam,espacio);
    }
        do {
            // Mostrar el menú
            std::cout << std::endl;
            std::cout << "Gestor de Base de Datos:" << std::endl;
            std::cout << "1) Formatear Disco" << std::endl;
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
                    std::cout<<"-----------> Formateando Disco <--------"<<std::endl;
                    std::cout << "Ingrese número de platos: ";
                    std::cin >> platos;
                    std::cout << "Ingrese número de pistas por superficie: ";
                    std::cin >> pistas;
                    std::cout << "Ingrese número de sectores por pista: ";
                    std::cin >> sectores;
                    std::cout << "Ingrese número de sectores por bloque ";
                    std::cin >> bloquestam;
                    std::cout << "Ingrese espacio en bytes por sector: ";
                    std::cin >> espacio;
                    disco.crearDisco(platos,pistas,sectores,bloquestam,espacio);
                    break;
                case '2':
                    std::cout << "CREAR BASE DE DATOS" << std::endl;
                    std::cout << "SQL: ";
                    std::cin.ignore();
                    break;
                case '3':
                    std::cout << "CREAR RELACION:" << std::endl;
                    std::cout << "SQL: ";
                    std::cin.ignore();
                    break;
                case '4':
                    std::cout << "CREAR REGISTRO:" << std::endl;
                    std::cout << "SQL: ";
                    std::cin.ignore();
                    break;
                case '5':
                    std::cout << "VER BASES DE DATOS:" << std::endl;
                    break;
                case '6':
                    std::cout << "VER TABLAS:" << std::endl;
                    break;
                case '7':
                    std::cout << "VER Registros:" << std::endl;
                    break;
                default:
                    std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
                    break;
            }
        } while (opcion != '9');

    return 0;
}
