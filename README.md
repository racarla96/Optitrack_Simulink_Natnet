# OptiTrack S-Function para Simulink

Este proyecto proporciona una S-Function en C++ para recibir en tiempo real la posición y orientación de un Rigid Body de OptiTrack Motive directamente en Simulink, usando el SDK oficial de NatNet.

## Requisitos

- MATLAB y Simulink (probado en R2024b o superior)
- Microsoft Visual C++ (para compilar)
- NatNet SDK 4.x (probado con 4.3)
- Motive ejecutándose y transmitiendo datos por red
- Archivo `optitrack_sfunction.cpp` (incluido en este repositorio)
- Archivos de cabecera y librería de NatNet (`NatNetClient.h`, `NatNetTypes.h`, `NatNetLib.lib`, `NatNetLib.dll`)

## Compilación

1. **Configura el compilador de C++ en MATLAB**  
   Ejecuta en la ventana de comandos:
   ```matlab
   mex -setup C++
   ```

2. **Compila la S-Function**  
   Ajusta las rutas según la ubicación de tu SDK y ejecuta:
   ```matlab
   mex optitrack_sfunction.cpp ...
       -I"RUTA\A\NatNetSDK\include" ...
       -L"RUTA\A\NatNetSDK\lib\x64" ...
       -lNatNetLib
   ```
   Ejemplo:
   ```matlab
   mex optitrack_sfunction.cpp ...
       -I"C:\NatNetSDK\include" ...
       -L"C:\NatNetSDK\lib\x64" ...
       -lNatNetLib
   ```

3. **Copia `NatNetLib.dll`**  
   Asegúrate de que `NatNetLib.dll` esté en la misma carpeta que tu modelo de Simulink o en una ruta accesible por el sistema de Matlab.

## Uso en Simulink

1. **Abre Simulink** y crea un nuevo modelo.

2. **Agrega un bloque S-Function**  
   Busca "S-Function" y arrástralo al modelo.

3. **Configura el bloque**  
   - **S-function name:**  
     ```
     optitrack_sfunction
     ```
   - **S-function parameters:**  
     ```
     'IP_SERVIDOR', 'IP_CLIENTE', ID_RIGID_BODY
     ```
     Ejemplo:
     ```
     '192.168.0.2', '192.168.0.3', 1
     ```
     - Primer parámetro: IP del PC donde corre Motive
     - Segundo parámetro: IP de tu PC (cliente)
     - Tercer parámetro: ID del Rigid Body a trackear

4. **Conecta las salidas**  
   El bloque tiene 3 salidas:
   - **Salida 1:** Posición `[x; y; z]`
   - **Salida 2:** Cuaternión `[qw; qx; qy; qz]`
   - **Salida 3:** Estado de conexión  
     - `1.0` = tracking válido  
     - `0.5` = conectado pero sin tracking  
     - `0.0` = desconectado

5. **Ejecuta la simulación**  
   Pulsa **Run**. Si todo está bien, verás los datos en tiempo real.

## Notas

- El bloque solo funciona en modo **Normal** de simulación.
- No es compatible con generación de código ni modo acelerado.
- Si tienes problemas de conexión, revisa firewalls y que las IPs sean correctas.
- Puedes ver mensajes de estado en la ventana de comandos de MATLAB.

## Créditos

Desarrollado por Rafael Carbonell Lázaro.  
Basado en el SDK oficial de [OptiTrack NatNet](https://optitrack.com/).