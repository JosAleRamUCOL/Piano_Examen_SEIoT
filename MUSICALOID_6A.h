#ifndef MUSICALOID_6A_H
#define MUSICALOID_6A_H

#include <Arduino.h>
struct MySong {
    uint8_t ID;
    bool estado;
    char nombre[16];
    uint16_t notas[10];
    uint8_t numNotas;
    char horaCreacion[9];  // HH:MM:SS
    char horaEdicion[9];
    uint8_t numReproducciones;
};

class MUSICALOID_6A {
  public:
    MySong arraySongs[10];
    uint8_t numSongs = 0;
    uint8_t numPapelera = 0; 
    void mostrarCancion(uint8_t index);
    
  public:
    MUSICALOID_6A();
    bool agregar(const char* nombre, const uint16_t* notas, uint8_t numNotas, const char* tiempo);
    void reproducir(uint8_t buzzerPin);
    void mostrarMenuReproduccion(bool list);
    void eliminar(uint8_t);
    void restaurar(uint8_t);
};

#endif