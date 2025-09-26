## README (English)

````markdown
# OptiTrack Simulink NatNet S-Function

This project provides a C++ S-Function to stream real-time position and orientation data of an OptiTrack rigid body (from Motive) directly into Simulink using the official NatNet SDK.

---

## Features

- Real-time acquisition of pose (position + quaternion) from OptiTrack via network  
- Simple S-Function interface for Simulink models  
- Connection state output (connected, tracking, no tracking)  
- Tested with MATLAB/Simulink R2024b (and newer)  
- Based on NatNet SDK 4.x  

---

## Requirements

- MATLAB & Simulink (tested with R2024b or newer)  
- Microsoft Visual C++ (for compilation)  
- NatNet SDK 4.x (tested with version 4.3)  
- Running Motive that streams data via network  
- Source file `optitrack_sfunction.cpp` (included)  
- NatNet SDK headers & library: `NatNetClient.h`, `NatNetTypes.h`, `NatNetLib.lib`, `NatNetLib.dll`  

---

## Build / Compilation

1. Configure MATLAB’s C++ compiler:  
   ```matlab
   mex -setup C++
````

2. Compile the S-Function, pointing to your NatNet SDK paths:

   ```bash
   mex optitrack_sfunction.cpp \
     -I"path/to/NatNetSDK/include" \
     -L"path/to/NatNetSDK/lib/x64" \
     -lNatNetLib
   ```

   Example (Windows):

   ```bash
   mex optitrack_sfunction.cpp \
     -I"C:\NatNetSDK\include" \
     -L"C:\NatNetSDK\lib\x64" \
     -lNatNetLib
   ```

3. Copy `NatNetLib.dll` to your Simulink model folder or another location accessible by MATLAB at runtime.

---

## Usage (in Simulink)

1. Open Simulink and create a new model.

2. Add an S-Function block (search “S-Function”).

3. Configure the block:

   * **S-function name:** `optitrack_sfunction`
   * **Parameters (in order):**

     1. IP address of the machine running Motive
     2. IP address of this (client) machine
     3. Rigid Body ID to track

   Example parameter string:

   ```
   '192.168.0.2', '192.168.0.3', 1
   ```

   * 192.168.0.2 = Motive server IP
   * 192.168.0.3 = client (this machine) IP
   * 1 = ID of the tracked rigid body

4. Connect the outputs of the S-Function block:

   * Channel 1: Position — a 3×1 vector `[x; y; z]`
   * Channel 2: Quaternion — a 4×1 vector `[qw; qx; qy; qz]`
   * Channel 3: Connection state

     * `1.0` = valid tracking
     * `0.5` = connected but no tracking
     * `0.0` = disconnected

5. Run the simulation. If all is configured correctly, you should see real-time pose data coming into Simulink.

---

## Notes & Limitations

* The block works only in **Normal** simulation mode — code generation or accelerated modes are **not supported**.
* If you face connectivity issues, check firewall settings and ensure the IPs are set correctly.
* Diagnostic or status messages are printed in MATLAB’s command window.

---

## Credits & License

* **Author**: Rafael Carbonell Lázaro
* Based on the official OptiTrack NatNet SDK
* (You may add a license file or specify an open-source license here.)

---

## Possible Enhancements

* Support for multiple rigid bodies in a single S-Function
* Support for streamed marker data or other object types
* Compatibility with accelerated or code-generation modes
* Error handling improvements and reconnection strategies
* Cross-platform support (e.g. Linux, macOS)

````

---

## README (Español)

```markdown
# Función S de OptiTrack para Simulink con NatNet

Este proyecto provee una S-Function en C++ para recibir en tiempo real la posición y orientación de un cuerpo rígido (Rigid Body) de OptiTrack (desde Motive) directamente en Simulink, usando el SDK oficial de NatNet.

---

## Características

- Adquisición en tiempo real de pose (posición + cuaternión) desde OptiTrack por red  
- Interfaz sencilla tipo S-Function para modelos Simulink  
- Salida del estado de conexión (conectado, tracking válido, sin tracking)  
- Probado con MATLAB/Simulink R2024b (y versiones superiores)  
- Basado en NatNet SDK 4.x  

---

## Requisitos

- MATLAB & Simulink (probado con R2024b o superior)  
- Microsoft Visual C++ (para compilar)  
- NatNet SDK 4.x (probado con la versión 4.3)  
- Motive ejecutándose y enviando datos por red  
- Archivo fuente `optitrack_sfunction.cpp` (incluido)  
- Archivos del SDK NatNet: `NatNetClient.h`, `NatNetTypes.h`, `NatNetLib.lib`, `NatNetLib.dll`  

---

## Compilación

1. Configura el compilador de C++ en MATLAB:  
   ```matlab
   mex -setup C++
````

2. Compila la S-Function, indicando las rutas de tu SDK NatNet:

   ```bash
   mex optitrack_sfunction.cpp \
     -I"ruta/a/NatNetSDK/include" \
     -L"ruta/a/NatNetSDK/lib/x64" \
     -lNatNetLib
   ```

   Ejemplo (Windows):

   ```bash
   mex optitrack_sfunction.cpp \
     -I"C:\NatNetSDK\include" \
     -L"C:\NatNetSDK\lib\x64" \
     -lNatNetLib
   ```

3. Copia `NatNetLib.dll` a la carpeta de tu modelo Simulink o a otra ubicación accesible por MATLAB en tiempo de ejecución.

---

## Uso (en Simulink)

1. Abre Simulink y crea un nuevo modelo.

2. Añade un bloque S-Function (busca “S-Function”).

3. Configura el bloque:

   * **Nombre de la S-function:** `optitrack_sfunction`
   * **Parámetros (orden):**

     1. Dirección IP del equipo que ejecuta Motive
     2. Dirección IP de este equipo (cliente)
     3. ID del cuerpo rígido que deseas trackear

   Ejemplo de parámetros:

   ```
   '192.168.0.2', '192.168.0.3', 1
   ```

   * 192.168.0.2 = IP del servidor Motive
   * 192.168.0.3 = IP del cliente (este equipo)
   * 1 = ID del cuerpo rígido a trackear

4. Conecta las salidas del bloque:

   * Salida 1: Posición — vector 3×1 `[x; y; z]`
   * Salida 2: Cuaternión — vector 4×1 `[qw; qx; qy; qz]`
   * Salida 3: Estado de conexión

     * `1.0` = seguimiento válido
     * `0.5` = conectado pero sin tracking
     * `0.0` = desconectado

5. Ejecuta la simulación. Si todo está bien configurado, deberías ver datos de pose en tiempo real dentro de Simulink.

---

## Notas y Limitaciones

* El bloque solo funciona en modo de simulación **Normal** — no es compatible con generación de código ni modo acelerado.
* Si tienes problemas de conexión, verifica las reglas del firewall y que las IP estén correctamente asignadas.
* Los mensajes de diagnóstico o estado aparecen en la ventana de comandos de MATLAB.

---

## Créditos & Licencia

* **Autor**: Rafael Carbonell Lázaro
* Basado en el SDK oficial de OptiTrack NatNet
* (Puedes agregar un archivo de licencia o especificar una licencia de código abierto aquí.)

---

## Mejoras Sugeridas

* Soporte para múltiples cuerpos rígidos en la misma S-Function
* Lectura de datos de marcadores u otros tipos de objetos
* Compatibilidad con modos acelerado o generación de código
* Mejoras en manejo de errores y estrategias de reconexión
* Soporte multiplataforma (Linux, macOS)
::contentReference[oaicite:0]{index=0}
```
