#include "MUSICALOID_6A.h"
#include <string.h>


MUSICALOID_6A::MUSICALOID_6A() {}

bool MUSICALOID_6A::agregar(const char* nombre, const uint16_t* notas, uint8_t numNotas, const char* tiempo) {
    if (numSongs >= 10) {
        Serial.println(F("Error: Máximo 10 canciones"));
        return false;
    }
    if (strlen(nombre) > 15) { 
        Serial.println(F("Error: Nombre muy largo (max 15 chars)"));
        return false;
    }
    if (numNotas < 1 || numNotas > 10) {
        Serial.println(F("Error: 1-10 notas por canción"));
        return false;
    }

    // crear nueva canción
    MySong nueva;
    nueva.ID = numSongs + 1;
    nueva.estado = true;
    strlcpy(nueva.nombre, nombre, sizeof(nueva.nombre));
    memcpy(nueva.notas, notas, numNotas * sizeof(uint16_t));
    nueva.numNotas = numNotas;
    strlcpy(nueva.horaCreacion, tiempo, sizeof(nueva.horaCreacion));
    strcpy(nueva.horaEdicion, "00:00:00");
    nueva.numReproducciones = 0;
    arraySongs[numSongs++] = nueva;
    
    Serial.print(F("Canción agregada ID: "));
    Serial.println(nueva.ID); 
    Serial.print(F("Hora de creación: "));
    Serial.println(nueva.horaCreacion);
    Serial.print(F("Nombre: "));
    Serial.println(nueva.nombre); 
    return true;
}

void MUSICALOID_6A::mostrarCancion(uint8_t index) {
    if (index >= numSongs) return;
    
    Serial.print(F("ID: ")); Serial.print(arraySongs[index].ID);
    Serial.print(F(" | Nombre: ")); Serial.print(arraySongs[index].nombre);
    Serial.print(F(" | Notas: ")); Serial.println(arraySongs[index].numNotas);
}

void MUSICALOID_6A::mostrarMenuReproduccion(bool list) {
    
    for (uint8_t i = 0; i < numSongs; i++) {
      if(arraySongs[i].estado == true && list == true){
        Serial.print(F("[")); Serial.print(arraySongs[i].ID); Serial.print(F("] "));
        Serial.print(arraySongs[i].nombre);
        Serial.print(F(" ("));
        Serial.print(arraySongs[i].numNotas);
        Serial.println(F(" notas)"));
      } else if(arraySongs[i].estado == false && list == false){
        Serial.print(F("[")); Serial.print(arraySongs[i].ID); Serial.print(F("] "));
        Serial.print(arraySongs[i].nombre);
        Serial.print(F(" ("));
        Serial.print(arraySongs[i].numNotas);
        Serial.println(F(" notas)"));
      }
    }
    
}

void MUSICALOID_6A::reproducir(uint8_t buzzerPin) {
    if (numSongs == 0) {
        Serial.println(F("No hay canciones disponibles"));
        delay(2000);
        return;
    }
    
    Serial.println(F("\n=== SELECCIONAR CANCION ==="));
    mostrarMenuReproduccion(true);
    Serial.println(F("[0] Cancelar"));
    Serial.print(F("Seleccione opcion: "));
    
    while (!Serial.available());
    uint8_t opcion = Serial.parseInt();
    Serial.println(opcion);
    
    if (opcion == 0 || opcion > numSongs) {
        Serial.println(F("Reproduccion cancelada"));
        return;
    }
    
    uint8_t index = opcion - 1;
    
    
    Serial.print(F("Reproduciendo: "));
    Serial.println(arraySongs[index].nombre);
    
    for (uint8_t i = 0; i < arraySongs[index].numNotas; i++) {
        Serial.print(F("Nota ")); Serial.print(i + 1); 
        Serial.print(F(": ")); Serial.println(arraySongs[index].notas[i]);
        
        tone(buzzerPin, arraySongs[index].notas[i], 500);
        delay(600);
      
        if (digitalRead(3) == LOW) {
            noTone(buzzerPin);
            Serial.println(F("Reproduccion pausada"));
            while (digitalRead(3) == LOW); // Esperar a que suelten el botón
            while (digitalRead(3) == HIGH); // Esperar a que lo presionen de nuevo
            delay(500);
        }
      arraySongs[index].numReproducciones++;
    }
    noTone(buzzerPin);
    
    Serial.println(F("Reproduccion finalizada"));
    delay(2000);
}
void MUSICALOID_6A::eliminar(uint8_t id){
  for(uint8_t i = 0; i<numSongs; i++){
    if(arraySongs[i].ID == id && arraySongs[i].estado == true){
      arraySongs[i].estado = false;
      numPapelera++;
      numSongs--;
      break;
    }
  }
  Serial.print(F("Se eliminó la melodía con ID: "));
  Serial.println(id);
}

void MUSICALOID_6A::restaurar(uint8_t id){
  for(uint8_t i = 0; i<numSongs; i++){
    if(arraySongs[i].ID == id && arraySongs[i].estado == false){
      arraySongs[i].estado = true;
      numSongs++;
      numPapelera--;
      break;
    }
  }
  Serial.print(F("Se restauró la melodía con ID: "));
  Serial.println(id);
}