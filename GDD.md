# Tempus Fugit - Game Design Document

**Version**: 0.1  
**Fecha**: Marzo 2026  
**Autor**: Javier (LevoOne)  
**Concurso**: C:\DOS\CONTEST II - MS-DOS Club 2026 (tema: viajes en el tiempo)  
**Deadline**: 30 de septiembre de 2026  

---
 
## 1. Concepto
 
**Tempus Fugit** es un juego de plataformas 2D para MS-DOS en el que el protagonista
viaja entre tres epocas historicas distintas para recuperar los fragmentos de un
artefacto temporal. La mecanica central combina plataformas clasicas de 8 bits con
puzzles basados en el paso del tiempo al estilo Day of the Tentacle: las acciones
realizadas en una epoca tienen consecuencias en las epocas futuras.
 
---
 
## 2. Historia
 
Eric es un joven universitario cuya familia lleva tres generaciones pasando por el
mismo colegio de secundaria. Su abuelo le ha pedido que recupere un antiguo artilugio
que escondio en la caja fuerte del colegio hace decadas, sin darle ninguna explicacion.
 
Eric aprovecha una reunion de antiguos alumnos para escabullirse durante el discurso
del director, recorrer los pasillos de su antigua escuela y abrir la caja fuerte.
Al activar el artilugio sin saber lo que hace, el dispositivo se fragmenta: sus tres
piezas salen disparadas a traves del tiempo y caen en tres epocas distintas.
 
Eric queda atrapado en un bucle temporal. Puede viajar entre las tres epocas pero
no puede volver a su presente hasta que recupere todos los fragmentos y reconstruya
el artilugio.
 
El juego termina con Eric de vuelta en el presente, artilugio en mano, saliendo del
colegio hacia donde le espera su abuelo. La razon por la que el abuelo necesitaba
el artilugio no se desvela, dejando abierta una segunda parte.
 
---
 
## 3. Protagonista
 
**Nombre**: Eric  
**Edad**: ~25 anos  
**Descripcion**: Estudiante universitario, heredero espiritual del Eric de Skool Daze.
Joven curioso y resolutivo, acostumbrado a resolver problemas por su cuenta. Tercera
generacion de su familia en pasar por el mismo colegio.  
**Aspecto**: El sprite de Eric se adapta visualmente a cada epoca con la ropa tipica
del periodo, aunque su silueta y personalidad son reconocibles en las tres.
 
---
 
## 4. Epocas
 
El juego transcurre en el mismo lugar geografico en tres momentos distintos de la
historia. El jugador puede reconocer que esta en el mismo sitio aunque todo haya
cambiado.
 
### 4.1 Prehistoria
 
El terreno en su estado mas primitivo. Naturaleza salvaje sin domar, vegetacion
exuberante, un rio caudaloso que divide el mapa. La estructura de referencia es
una roca monolitica en la zona baja.
 
### 4.2 Edad Media
 
El mismo terreno siglos despues. El rio tiene un puente de piedra, hay construcciones
humanas en el nivel medio, y en la colina se alza una torre de vigilancia. La
estructura de referencia es la torre.
 
### 4.3 Futuro
 
El mismo terreno en un futuro lejano. El rio esta seco, las construcciones son
ruinas cubiertas de vegetacion o arena. La estructura de referencia son los restos
de la torre medieval.
 
---
 
## 5. Geografia y Mapa
 
### 5.1 Descripcion del terreno
 
El mapa se divide en tres zonas geograficas:
 
- **Colina**: zona elevada con abrigo rocoso en Prehistoria, torre en Edad Media,
  colina erosionada en Futuro.
- **Nivel medio**: zona principal de transito con construcciones humanas en Edad
  Media, ruinas en Futuro.
- **Zona baja**: curso de agua (rio caudaloso en Prehistoria, puente en Edad Media,
  rio seco en Futuro) y estructura vertical de referencia.
 
### 5.2 Estructura del mapa
 
Cada epoca tiene exactamente 9 pantallas estaticas con la misma estructura de
conexiones. La navegacion entre pantallas se realiza mediante transiciones de
fade negro. No hay scroll.
 
```
[1]-[2]-[3]
          |
     [4]-[5]
     |
[6]-[7]-[8]-[9]
```
 
| Pantalla | Zona |
|---|---|
| 1 | Cima de la colina |
| 2 | Ladera de la colina |
| 3 | Pie de la colina, conexion con nivel medio |
| 4 | Nivel medio, zona izquierda |
| 5 | Nivel medio, zona central (punto de guardado) |
| 6 | Zona baja, extremo izquierdo |
| 7 | Zona baja, centro izquierda |
| 8 | Zona baja, centro derecha, junto al rio |
| 9 | Zona baja, extremo derecho, estructura vertical |
 
### 5.3 Accesibilidad por epoca
 
La accesibilidad del mapa cambia entre epocas segun el paso del tiempo:
 
- **Prehistoria**: el rio en la zona baja es infranqueable. Pantallas 6-9
  inicialmente inaccesibles.
- **Edad Media**: el puente permite acceso a la zona baja. Algunos accesos
  bloqueados por construcciones o guardias.
- **Futuro**: el rio seco permite acceso total a la zona baja desde el inicio.
 
---
 
## 6. Mecanicas
 
### 6.1 Movimiento
 
- Caminar izquierda y derecha
- Saltar
- Agacharse
- Recoger y depositar objetos (un objeto a la vez)
 
Eric no puede atacar ni disparar. Los enemigos se esquivan, no se eliminan.
 
### 6.2 Regla temporal
 
**Las acciones solo tienen consecuencias hacia el futuro, nunca hacia el pasado.**
 
- Lo que Eric hace en Prehistoria puede afectar a Edad Media y Futuro.
- Lo que hace en Edad Media puede afectar al Futuro.
- Lo que hace en el Futuro no afecta a nada anterior.
 
El Futuro es siempre el resultado de las acciones anteriores, nunca su causa.
 
### 6.3 Objetos
 
Eric puede llevar un unico objeto a la vez. Los objetos se pueden depositar en
cualquier punto del mapa y recuperar mas tarde. Persisten entre viajes temporales
dentro de la misma epoca.
 
### 6.4 Rebobinado temporal
 
Eric dispone de una reserva limitada de tiempo rebobinable por zona. Al activarlo,
el estado completo del mundo vuelve atras: posicion de Eric, posicion de enemigos,
estado de objetos y modificaciones del mapa en todas las epocas simultaneamente.
 
El rebobinado es una mecanica exclusiva de puzzles, no de combate. Sirve para
deshacer acciones incorrectas como colocar un objeto en el lugar equivocado o
activar algo que no debia activarse.
 
La reserva se recarga al conseguir cada fragmento.
 
### 6.5 Enemigos
 
Cada epoca tiene un tipo de enemigo con patrones de movimiento fijos:
 
- **Prehistoria**: animales prehistoricos que patrullan zonas concretas.
- **Edad Media**: guardias con rutas fijas.
- **Futuro**: robots o drones con trayectorias predefinidas.
 
El contacto con un enemigo hace retroceder a Eric al inicio de la pantalla,
conservando los objetos que lleva.
 
---
 
## 7. Puzzles
 
### 7.1 Estructura
 
Cada epoca tiene 3 puzzles:
 
- **1 puzzle principal**: da acceso al fragmento del artefacto. Implica cruce
  temporal entre epocas.
- **2 puzzles intermedios**: desbloquean zonas del mapa u objetos necesarios.
  Pueden ser internos a la epoca o con cruce temporal menor.
 
Total: 9 puzzles. Ritmo aproximado de 1 puzzle cada 3 pantallas.
 
### 7.2 Ubicacion de los fragmentos
 
| Fragmento | Epoca | Pantalla |
|---|---|---|
| 1 | Prehistoria | 1 (cima de la colina) |
| 2 | Edad Media | 9 (estructura vertical) |
| 3 | Futuro | 6 (zona baja extremo izquierdo) |
 
El fragmento del Futuro siempre se recupera en ultimo lugar.
 
### 7.3 Progresion
 
La progresion es semi-libre: el jugador puede explorar Prehistoria y Edad Media
con cierta libertad, pero hay al menos un puzzle que requiere haber resuelto algo
en Prehistoria antes de poder completar la Edad Media. El fragmento del Futuro
siempre es el ultimo.
 
### 7.4 Puzzles de Prehistoria
 
#### Puzzle Simple: El oso y la miel
 
**Objetivo**: neutralizar al oso que bloquea el paso en pantalla 3.
 
```
1. Eric recoge el palo en zona baja (pantallas 6-9)
2. Viaje a Edad Media: recoge la taza (accesible directamente)
3. Viaje a Prehistoria: agita la colmena con el palo (pantalla 2)
   la colmena gotea, Eric deja el palo
4. Eric coge la taza, la coloca bajo las gotas, taza llena de miel
5. Eric lleva la taza con miel a pantalla 3
6. El oso se distrae comiendo, Eric cruza hacia pantalla 4
```
 
Objetos: palo (Prehistoria), taza (Edad Media), miel (Prehistoria)
Viajes temporales: 2 (Prehistoria, Edad Media, Prehistoria)
Dificultad: baja, introductoria
 
#### Puzzle Medio: El tronco y el rio
 
**Objetivo**: conseguir el tronco de la zona baja para mover la roca que bloquea pantalla 1.
 
```
1. Eric cruza el rio saltando sobre rocas estaticas
   esquivando peces prehistoricos con patrones fijos (secuencia arcade)
2. Eric recoge el tronco en zona baja
3. Eric transporta el tronco hasta la roca (pantalla 2)
4. Eric usa el tronco como palanca, mueve la roca
5. Acceso a pantalla 1 desbloqueado
```
 
Objetos: tronco (Prehistoria)
Viajes temporales: ninguno, puzzle interno a la epoca
Dificultad: media, introduce elemento arcade
 
#### Puzzle Principal: La ofrenda al chaman
 
**Objetivo**: conseguir la planta sagrada para entregarla al chaman que custodia el fragmento 1.
 
```
1. Eric llega a pantalla 1, encuentra el circulo de piedras
   El chaman custodia el fragmento y pide una planta sagrada como ofrenda
2. Viaje a Edad Media: Eric encuentra la planta (todavia existe en esta epoca)
   Eric la entierra en un recipiente de barro sellado en un lugar marcado
3. Viaje a Futuro: Eric desentierra el recipiente
   La planta sigue viva dentro, perfectamente conservada
4. Eric coge el recipiente con la planta
5. Viaje a Prehistoria: Eric entrega la planta al chaman
   El chaman se aparta, Eric recoge el fragmento 1
```
 
Objetos: recipiente de barro (Edad Media), planta sagrada
Viajes temporales: 3 (Prehistoria, Edad Media, Futuro, Prehistoria)
Dificultad: alta, introduce la cadena temporal completa
 
### 7.5 Puzzles de Edad Media
 
*Pendiente de definir.*
 
### 7.6 Puzzles de Futuro
 
*Pendiente de definir.*
 
---
 
## 8. Sistema de guardado
 
| Evento | Tipo |
|---|---|
| Al iniciar el juego | Carga automatica si existe SAVEGAME.DAT |
| Al llegar a pantalla 5 de cada epoca | Guardado manual en objeto especial |
| Al conseguir cada fragmento | Guardado automatico |
| Al completar el juego | Guardado automatico |
 
El objeto de guardado manual en pantalla 5 es distinto en cada epoca:
 
- **Prehistoria**: hoguera ceremonial.
- **Edad Media**: capilla o posada.
- **Futuro**: terminal o capsula de hibernacion.
 
---
 
## 9. Arte grafico
 
*Pendiente de definir con la colaboradora artistica.*
 
Restricciones tecnicas: 320x200 pixeles, 256 colores indexados, paleta VGA.
 
Cada epoca debe tener una identidad visual inmediata y reconocible. El jugador
debe sentir el cambio de epoca antes de leer ningun texto.
 
Elementos a definir por epoca:
- Paleta de colores predominante
- Estilo de los fondos (BMP de 320x200 por pantalla y epoca)
- Tileset de plataformas y elementos del entorno
- Sprites de Eric (spritesheet por epoca)
- Sprites de enemigos
- Objeto de guardado manual
 
---
 
## 10. Musica y sonido
 
*Pendiente de definir con la colaboradora musical.*
 
### Musica
 
Una pista por epoca en formato XM, en bucle. Debe poder escucharse durante
sesiones largas de exploracion sin resultar agotadora.
 
- **Prehistoria**: organica, primitiva, mas atmosfera que melodia.
- **Edad Media**: tension contenida, sin caer en lo epico o solemne.
- **Futuro**: fria o extrananamente hermosa, con sensacion de perdida.
 
### Efectos de sonido
 
Efectos en formato WAV para:
 
- Salto
- Recoger objeto
- Depositar objeto
- Activar mecanismo
- Contacto con enemigo
- Viaje temporal (fade entre epocas)
- Guardado
- Conseguir fragmento
 
---
 
## 11. Tecnologia
 
- **Plataforma**: MS-DOS 6.22, compatible con 486DX66 / 16MB RAM
- **Compilador**: OpenWatcom 2.0, C89
- **Video**: VGA modo 13h (320x200, 256 colores), double buffering
- **Sonido**: Judas Sound System (Sound Blaster, XM + WAV)
- **Emulacion**: DOSBox-X, PCem, 86Box
- **Repositorio**: https://github.com/LevoOne/Tempus-Fugit (privado)
 
---
 
## 12. Pendiente de definir
 
- Diseno detallado de los 9 puzzles con solucion completa
- Descripcion pantalla a pantalla de cada epoca
- Diseno visual de cada epoca (colaboradora artistica)
- Composicion musical de cada epoca (colaboradora musical)
- Nombre definitivo del artilugio
- Diseno del HUD (vidas, reserva de rebobinado, objeto actual)
- Pantalla de titulo y pantalla de fin
 
---
 
*Documento vivo. Se actualiza a medida que avanza el desarrollo.*
