# Proyecto GPS y Firebase con ESP32

![Diagrama del proyecto](https://github.com/Enbebido-ubicate/IoT-ubicate/raw/main/image.png)

Este proyecto utiliza un **ESP32** con un módulo GPS para leer las coordenadas de ubicación y enviarlas a una base de datos en **Firebase Realtime Database**. Además, utiliza un **LED** para indicar el estado de la señal GPS. El sistema está diseñado para registrar la ubicación de un autobús y actualizarla en tiempo real.

## Componentes

- **ESP32**: Microcontrolador que gestiona la conexión Wi-Fi y la comunicación con Firebase.
- **Módulo GPS**: Se utiliza para obtener las coordenadas de ubicación.
- **Firebase**: Servicio en la nube para almacenar y gestionar datos en tiempo real.
- **LED**: Indicador del estado de la señal GPS.

## Requisitos

### Hardware
- ESP32
- Módulo GPS (conexión serial)
- LED (opcional, se usa para indicar el estado de la señal GPS)
- Cables de conexión

### Software
- Arduino IDE (con soporte para ESP32)
- Librerías necesarias:
  - `WiFi.h`
  - `Firebase_ESP_Client.h`
  - `TinyGPS++.h`
- Cuenta en **Firebase** y configuración de la base de datos en tiempo real.

## Configuración

### 1. Configuración del Wi-Fi
Modifica las siguientes líneas con tus credenciales de Wi-Fi:

```cpp
#define WIFI_SSID "tu_SSID"
#define WIFI_PASSWORD "tu_contraseña"
