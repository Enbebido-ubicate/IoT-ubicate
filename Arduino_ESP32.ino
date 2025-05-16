#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Configuración de red WiFi
#define WIFI_SSID "jcv"
#define WIFI_PASSWORD "jcv12345678"

// Configuración de Firebase
#define API_KEY "AIzaSyCr05S1VhQA8VzW8_63t_mwSH9-wxqdGyE"
#define DATABASE_URL "https://iot-ubicate-default-rtdb.firebaseio.com"

// Configuración del GPS
HardwareSerial neogps(1);
TinyGPSPlus gps;
#define RXD2 16  // Pin RX del ESP32
#define TXD2 17  // Pin TX del ESP32

// Definir el objeto de datos de Firebase
FirebaseData firebaseData;

// Definir la autenticación de Firebase
FirebaseAuth auth;
FirebaseConfig config;

// Configuración del LED
#define LED_PIN 2

// Credenciales de usuario
#define USER_EMAIL "jorgecastillow4@gmail.com"   // Reemplaza con tu correo de Firebase
#define USER_PASS "12345679s"         // Reemplaza con tu contraseña de Firebase

void setup() {
  Serial.begin(115200);  // Iniciar la comunicación serial con el monitor
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);  // Configuración del puerto serial para el GPS

  // Conectar a Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  // Configuración de Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  // Autenticación de usuario en Firebase
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASS;

  // Registrarse en Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("Conectado a Firebase y autenticado");
}

void loop() {
  boolean newData = false;

  // Leer datos del GPS durante 1 segundo
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (neogps.available()) {
      if (gps.encode(neogps.read())) {
        newData = true;
      }
    }
  }

  if (newData) {
    if (gps.location.isValid()) {
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();

      // Mostrar la ubicación en el monitor serial
      Serial.print("Lat: ");
      Serial.print(latitude, 6);
      Serial.print(", Lon: ");
      Serial.println(longitude, 6);

      // Definir el busId, en este caso, el ID del bus sería "001"
      String busId = "001";  // Puedes cambiar esto a cualquier ID del bus

      // Ruta para verificar si el bus ya existe
      String busPath = "/buses/" + busId;

      // Comprobar si el bus ya existe
      if (Firebase.RTDB.getJSON(&firebaseData, busPath)) {
        if (firebaseData.dataType() == "undefined") {
          // El bus no existe, crear uno nuevo
          Serial.println("Bus no encontrado. Registrando nuevo bus...");
          FirebaseJson newBus;
          newBus.set("busId", busId);
          newBus.set("isActive", true);
          newBus.set("name", "Bus 1");
          newBus.set("route", "Ruta A");

          // Solo almacenamos las coordenadas dentro del subnodo "location"
          FirebaseJson location;
          location.set("lat", latitude);
          location.set("lng", longitude);

          // Ahora pasamos el puntero a FirebaseJson
          newBus.set("location", location);

          if (Firebase.RTDB.pushJSON(&firebaseData, busPath, &newBus)) {
            Serial.println("Nuevo bus registrado en Firebase.");
          } else {
            Serial.println("Error al registrar el nuevo bus: " + firebaseData.errorReason());
          }
        } else {
          // El bus ya existe, actualizar la ubicación
          Serial.println("Bus encontrado. Actualizando ubicación...");
          FirebaseJson locationUpdate;
          locationUpdate.set("lat", latitude);
          locationUpdate.set("lng", longitude);

          // Ahora pasamos el puntero a FirebaseJson
          if (Firebase.RTDB.updateNode(&firebaseData, busPath + "/location", &locationUpdate)) {
            Serial.println("Ubicación del bus actualizada en Firebase.");
          } else {
            Serial.println("Error al actualizar la ubicación: " + firebaseData.errorReason());
          }
        }
      } else {
        Serial.println("Error al verificar el bus: " + firebaseData.errorReason());
      }

      delay(6000);  // Esperar un segundo antes de leer la siguiente ubicación
    }
  } else {
    Serial.println("Esperando señal GPS...");
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // Parpadeo de LED mientras espera GPS
    delay(500);
  }
}