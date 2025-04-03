#include "MUSICALOID_6A.h"
#include <LiquidCrystal_I2C.h>

MUSICALOID_6A disco;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char tiempo[] = __TIME__;

#define PIN_DO 13
#define PIN_RE 12
#define PIN_MI 11
#define PIN_FA 10
#define PIN_SOL 9
#define PIN_LA 8
#define PIN_SI 7
#define PIN_DO_PLUS 6
#define PIN_BUZZER 4
#define BSTOP 2
#define BPLAY 3
#define BREC 5

const uint16_t DO = 523, RE = 587, MI = 659, FA = 698, SOL = 784, LA = 880, SI = 988, DO_PLUS = 1047;

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MUSICALOID");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  pinMode(PIN_DO, INPUT_PULLUP);
  pinMode(PIN_RE, INPUT_PULLUP);
  pinMode(PIN_MI, INPUT_PULLUP);
  pinMode(PIN_FA, INPUT_PULLUP);
  pinMode(PIN_SOL, INPUT_PULLUP);
  pinMode(PIN_LA, INPUT_PULLUP);
  pinMode(PIN_SI, INPUT_PULLUP);
  pinMode(PIN_DO_PLUS, INPUT_PULLUP);
  pinMode(BSTOP, INPUT_PULLUP);
  pinMode(BPLAY, INPUT_PULLUP);
  pinMode(BREC, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
    
  delay(1000);
  lcd.clear();
  mostrarMenu();
}

void loop() {
  if (Serial.available() > 0) {
    String entrada = "";
    while(Serial.available() > 0) {
      entrada = Serial.readStringUntil('\n');
    }
    Serial.println(entrada.charAt(0));
    if(entrada == "m" || entrada == "M"){
      mostrarMenu();
    }else {
      procesarOpcion(entrada.charAt(0));
    }
    delay(50);
  }
    
  reproducirNotas();
    
  if (digitalRead(BPLAY) == LOW) {
    disco.reproducir(PIN_BUZZER);
    lcd.clear();
    lcd.print("Reproduciendo...");
    delay(300);
  }
}

void mostrarMenu() {
    Serial.println(F("\n===== Menú Interactivo ====="));
    if (disco.numSongs == 0) {
        Serial.println(F("1. Agregar melodía"));
    }
    else if (disco.numSongs < 5) {
        Serial.println(F("1. Reporte"));
        Serial.println(F("2. Agregar melodía"));
        Serial.println(F("3. Editar melodía"));
        Serial.println(F("4. Eliminar melodía"));
        Serial.println(F("5. Reproducir melodía"));
        Serial.println(F("6. Mostrar lista de melodías"));
        Serial.println(F("7. Papelera"));
        Serial.println(F("8. Ordenar lista por nombre"));
    }
    else if (disco.numSongs >= 5) {
        Serial.println(F("1. Eliminar melodía"));
        Serial.println(F("2. Reproducir melodía"));
        Serial.println(F("Límite de melodías alcanzado (5)"));
    }
    Serial.println(F("M. Mostrar menú"));
    Serial.println(F("============================"));
    Serial.println(F("Ingrese opción:"));
}

void procesarOpcion(char opcion) {
  if (disco.numSongs == 0) {
    if (opcion == '1') {
      lcd.setCursor(0,1);
      lcd.print("Presiona REC");
      agregarMelodia();
      delay(50);
    } else {
      Serial.println(F("Opción inválida"));
    }
  }else if (disco.numSongs < 5 && disco.numSongs > 0) {
    switch (opcion) {
      case '1': 
        reporte();
        delay(50);
        break;
      case '2':
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Presiona REC");
        agregarMelodia();
        mostrarMenu();
        delay(50);
        break;
      case '3':
        Serial.println(F("Editar melodía"));
        break;
      case '4': 
        eliminarMelodia();
        delay(50);
        break;
      case '5': 
        disco.reproducir(PIN_BUZZER);
        mostrarMenu();
        delay(50);
        break;
      case '6':
        Serial.println("\n=== LISTA DE CANCIONES ===");
        disco.mostrarMenuReproduccion(true);
        delay(50);
        break;
      case '7': 
        papelera();
        mostrarMenu();
        delay(50);
        break;
      case '8':
        ordenar();
        mostrarMenu();
        break;
      default:
        Serial.println(F("Opción inválida"));
    }
  }else { // si disco.numSongs >= 5 
    if (opcion == '1') {
      Serial.println(F("Eliminar melodía"));
      eliminarMelodia();
      delay(50);
    }else if (opcion == '2') {
      Serial.println(F("Reproducir melodía"));
      disco.reproducir(PIN_BUZZER);
      delay(50);

    }else {
      Serial.println(F("Opción inválida"));
    }
  }
}
void ordenar(){
        Serial.println(F("Ordenar lista por nombre"));
        Serial.println(F("Elige la opción por la quieres ordenar"));
        Serial.println(F("1. Num de notas"));
        Serial.println(F("2. Nombre"));
        Serial.println(F("3. ID de melodia"));
        Serial.println(F("4. Bubble sort"));

        String input;
        while(Serial.available() <= 0){
          delay(50);
        }
        input = Serial.readStringUntil("\n");
        input.trim(); 
        Serial.print("Se ingreso: ");
        Serial.println(input);

        if(input == "1"){
          Serial.println("Entraste a la opción 1");

          for(uint8_t i = 0; i < disco.numSongs-1; i++){
            for(uint8_t j = i + 1; j < disco.numSongs; j++){
              if(disco.arraySongs[i].numNotas > disco.arraySongs[j].numNotas){
                Serial.println(disco.arraySongs[j].numNotas);
              }
            }
          }
        }
        if(input == "3"){
          disco.mostrarMenuReproduccion(true);
        }
}

void reproducirNotas() {
    if (digitalRead(PIN_DO) == LOW) {
        tone(PIN_BUZZER, DO, 200);
        lcd.clear();
        lcd.print("Tocando: DO");
    }
    if (digitalRead(PIN_RE) == LOW) {
        tone(PIN_BUZZER, RE, 200);
        lcd.clear();
        lcd.print("Tocando: RE");
    }
    if (digitalRead(PIN_MI) == LOW) {
        tone(PIN_BUZZER, MI, 200);
        lcd.clear();
        lcd.print("Tocando: MI");
    }
    if (digitalRead(PIN_FA) == LOW) {
        tone(PIN_BUZZER, FA, 200);
        lcd.clear();
        lcd.print("Tocando: FA");
    }
    if (digitalRead(PIN_SOL) == LOW) {
        tone(PIN_BUZZER, SOL, 200);
        lcd.clear();
        lcd.print("Tocando: SOL");
    }
    if (digitalRead(PIN_LA) == LOW) {
        tone(PIN_BUZZER, LA, 200);
        lcd.clear();
        lcd.print("Tocando: LA");
    }
    if (digitalRead(PIN_SI) == LOW) {
        tone(PIN_BUZZER, SI, 200);
        lcd.clear();
        lcd.print("Tocando: SI");
    }
    if (digitalRead(PIN_DO_PLUS) == LOW) {
        tone(PIN_BUZZER, DO_PLUS, 200);
        lcd.clear();
        lcd.print("Tocando: DO_PLUS");
    }
    else {
        static unsigned long lastUpdate = 0;
        if (millis() - lastUpdate > 5000) {
            lcd.clear();
            lcd.print("MUSICALOID 6A");
            lcd.setCursor(0, 1);
            lcd.print("Listo...");
            lastUpdate = millis();
        }
    }
}
void reporte(){
  Serial.println(F("\n=== CANCIONES ALMACENADAS ==="));
          uint8_t totalSongs = disco.numSongs + disco.numPapelera;
          
          for(uint8_t i = 0; i < totalSongs; i++) {
            if(disco.arraySongs[i].estado) {
              Serial.print(F("[")); Serial.print(i+1); Serial.print(F("] "));
              Serial.print(disco.arraySongs[i].nombre);
              Serial.print(F(" (")); Serial.print(disco.arraySongs[i].numNotas);
              Serial.println(F(" notas)"));
              Serial.print(F(" (")); Serial.print(disco.arraySongs[i].numReproducciones);
              Serial.println(F(" reproducciones)"));

            }
          }
          
          Serial.println(F("\n=== CANCIONES ELIMINADAS ==="));
          for(uint8_t i = 0; i < totalSongs; i++) {
            if(!disco.arraySongs[i].estado) {
              Serial.print(F("[")); Serial.print(i+1); Serial.print(F("] "));
              Serial.print(disco.arraySongs[i].nombre);
              Serial.print(F(" (")); Serial.print(disco.arraySongs[i].numNotas);
              Serial.println(F(" notas)"));
              Serial.print(F(" (")); Serial.print(disco.arraySongs[i].numReproducciones);
              Serial.println(F(" notas)"));
            }
          }
}
void agregarMelodia() {
  Serial.println(F("Pulsa el botón REC para comenzar a grabar... (Timeout 10s)"));
  unsigned long inicio = millis();
  bool inicioGrabacion = false;
  
  while (millis() - inicio < 10000) {
    if (digitalRead(BREC) == LOW) {
      Serial.println(F("Entrando a grabar..."));
      inicioGrabacion = true;
      break;
    }
    delay(50);
  }
  
  if (!inicioGrabacion) {
    Serial.println(F("Tiempo de espera agotado"));
    return;
  }
  
  lcd.clear();
  lcd.print("Grabando...");
  Serial.println(F("\nIniciando grabación"));
  
  uint16_t notasGrabadas[10] = {0};
  uint8_t numNotas = 0;
  bool grabando = true;
  
  while (grabando && numNotas < 10) {
    if (digitalRead(BSTOP) == LOW) {
      grabando = false;
      delay(200);
      continue;
    }
    lcd.setCursor(0, 1);
    if (digitalRead(PIN_DO) == LOW) {
      notasGrabadas[numNotas++] = DO;
      tone(PIN_BUZZER, DO, 200);
      mostrarNotaLCD("DO", numNotas);
    }
    else if (digitalRead(PIN_RE) == LOW) {
      notasGrabadas[numNotas++] = RE;
      tone(PIN_BUZZER, RE, 200);
      mostrarNotaLCD("RE", numNotas);
    }
    else if (digitalRead(PIN_MI) == LOW) {
      notasGrabadas[numNotas++] = MI;
      tone(PIN_BUZZER, MI, 200);
      mostrarNotaLCD("MI", numNotas);
    }
    else if (digitalRead(PIN_FA) == LOW) {
      notasGrabadas[numNotas++] = FA;
      tone(PIN_BUZZER, FA, 200);
      mostrarNotaLCD("FA", numNotas);
    }
    else if (digitalRead(PIN_SOL) == LOW) {
      notasGrabadas[numNotas++] = SOL;
      tone(PIN_BUZZER, SOL, 200);
      mostrarNotaLCD("SOL", numNotas);
    }
    else if (digitalRead(PIN_LA) == LOW) {
      notasGrabadas[numNotas++] = LA;
      tone(PIN_BUZZER, LA, 200);
      mostrarNotaLCD("LA", numNotas);
    }
    else if (digitalRead(PIN_SI) == LOW) {
      notasGrabadas[numNotas++] = SI;
      tone(PIN_BUZZER, SI, 200);
      mostrarNotaLCD("SI", numNotas);
    }
    else if (digitalRead(PIN_DO_PLUS) == LOW) {
      notasGrabadas[numNotas++] = DO_PLUS;
      tone(PIN_BUZZER, DO_PLUS, 200);
      mostrarNotaLCD("DO_PLUS", numNotas);
    }
    
    delay(200);
  }
  
  noTone(PIN_BUZZER);
  
  if (numNotas > 0) {
    Serial.println(F("\nGrabación finalizada"));
    Serial.print(F("Notas grabadas: "));
    Serial.println(numNotas);
    
    Serial.println(F("Ingrese nombre para la melodía:"));
    String nombre;
    while (Serial.available() <= 0){
      delay(50);
    }
    nombre = Serial.readStringUntil('\n');
    nombre.trim();
    if (disco.agregar(nombre.c_str(), notasGrabadas, numNotas, tiempo)) {
      lcd.clear();
      lcd.print(F("Guardada!"));
      lcd.setCursor(0, 1);
      lcd.print(nombre);
    } else {
      lcd.clear();
      lcd.print(F("Error al guardar"));
    }
  } else {
    lcd.clear();
    lcd.print(F("Grab. cancelada"));
  }
  
  delay(1000);
  lcd.clear();
}
void mostrarNotaLCD(const char* nota, uint8_t num) {
    lcd.clear();
    lcd.print("Nota ");
    lcd.print(num);
    lcd.print(": ");
    lcd.print(nota);
}
void eliminarMelodia(){
  Serial.println(F("\n===== Opciones ====="));
  Serial.println(F("1. Buscar"));
  Serial.println(F("2. Lista de melodías"));
  Serial.println(F("============================"));
  while(Serial.available() <= 0){
    delay(50);
  }
  String input = Serial.readStringUntil('\n');
  uint8_t idCancion;
  Serial.println(input);


  if(input == "1"){
    input = "";
    Serial.println(F("Escribe el nombre de la canción"));
    while(Serial.available() <= 0){
      delay(50);
    }
    input = Serial.readStringUntil('\n');
    input.trim();
    Serial.println(input);
    for(uint8_t i = 0; i < disco.numSongs; i++){
      if(input.equals(disco.arraySongs[i].nombre)){
        Serial.print(F("[")); Serial.print(i+1); Serial.print(F("] "));
        Serial.print(disco.arraySongs[i].nombre);
        Serial.print(F(" (")); Serial.print(disco.arraySongs[i].numNotas);
        Serial.println(F(" notas)"));
        idCancion = disco.arraySongs[i].ID;
        Serial.println(idCancion);
      }
      delay(50);
    }
    if(!idCancion){
      delay(50);
      return;
    }
    input = "";
    Serial.println(F("\n===== Opciones ====="));
    Serial.println(F("1. Eliminar"));
    Serial.println(F("0. Cancelar"));
    Serial.println(F("============================"));
    while(Serial.available() <= 0){
      delay(50);
    }
    input = Serial.readStringUntil('\n');
    Serial.println(input);
    if(input == "1"){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("ELIMINANDO...");
      disco.eliminar(idCancion);
      Serial.print(F("Cancion "));
      Serial.print(idCancion);
      Serial.println(F(" eliminada"));
    }else if(input == "0"){
      Serial.println(F("Eliminación cancelada"));
    }else{
      Serial.println(F("Opción inválida de eliminar/buscar"));
    }
  }else if(input == "2"){
    Serial.println(F("Elige la canción que deseas eliminar"));
    disco.mostrarMenuReproduccion(true); 

    input = "";
    while(Serial.available() <= 0){
        delay(50);
    }
    input = Serial.readStringUntil('\n');
    input.trim();
    Serial.println(input);

    idCancion = input.toInt();

    if(idCancion > 0 && idCancion <= disco.numSongs){
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("ELIMINANDO..."); 
        disco.eliminar(idCancion);
        Serial.print(F("Cancion "));
        Serial.print(idCancion);
        Serial.println(F(" eliminada"));
    } else {
        Serial.println(F("Opción inválida o ID no válido"));
    }
  }
}

void papelera(){
  disco.mostrarMenuReproduccion(false);
  while(Serial.available() <= 0){
    delay(50);
  }
  String input = Serial.readStringUntil('\n');

  Serial.println(disco.numSongs);
  uint8_t id = input.toInt();
  for(uint8_t i = 0; i < disco.numSongs; i++){
      if(id == disco.arraySongs[i].ID){
        disco.restaurar(id);
        delay(50);
        return;
      }
      delay(50);
    }
  Serial.print(F("No se encontró la canción "));
  Serial.println(input.toInt());
}