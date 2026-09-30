# Time Daze - Game Design Document

**Version**: 1.0  
**Fecha**: Abril 2026  
**Autor**: Javier  
**Concurso**: C:\DOS\CONTEST 2 (tema: viajes en el tiempo)  
**Deadline**: 30 de septiembre de 2026  

---

## 1. Concepto

**Time Daze** es un juego de plataformas 2D para MS-DOS en el que el protagonista
viaja entre tres épocas historicas distintas para recuperar los fragmentos de un
artefacto temporal. La mecanica central combina plataformas clasicas de 8 bits con
puzzles basados en el paso del tiempo al estilo Day of the Tentacle: las acciones
realizadas en una época tienen consecuencias en las épocas futuras.

El nombre es un homenaje a "Skool Daze" (ZX Spectrum, 1984), juego del que toma
prestado al protagonista Eric, un estudiante universitario atrapado esta vez no en
el colegio sino entre el tiempo mismo.

---

## 2. Historia

Eric es un joven universitario cuya familia lleva tres generaciones pasando por el
mismo colegio de secundaria. Su abuelo le ha pedido que recupere un antiguo artilugio
que escondio en la caja fuerte del colegio hace decadas, sin darle ninguna explicacion.

Eric aprovecha una reunion de antiguos alumnos para escabullirse durante el discurso
del director, recorrer los pasillos de su antigua escuela y abrir la caja fuerte.
Al activar el artilugio sin saber lo que hace, el dispositivo se fragmenta: sus tres
piezas salen disparadas a traves del tiempo y caen en tres épocas distintas.

Eric queda atrapado en un bucle temporal. Puede viajar entre las tres épocas pero
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
**Aspecto**: El sprite de Eric se adapta visualmente a cada época con la ropa tipica
del periodo, aunque su silueta y personalidad son reconocibles en las tres.

---

## 4. Épocas

El juego transcurre en el mismo lugar geografico en tres momentos distintos de la
historia. El jugador puede reconocer que esta en el mismo sitio aunque todo haya
cambiado.

### 4.1 Prehistoria

El terreno en su estado mas primitivo. Naturaleza salvaje sin domar, vegetación
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

Cada época tiene exactamente 9 pantallas estaticas con la misma estructura de
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

### 5.3 Accesibilidad por época

La accesibilidad del mapa cambia entre épocas segun el paso del tiempo:

- **Prehistoria**: el rio en la zona baja es infranqueable. Pantallas 6-9
  inicialmente inaccesibles.
- **Edad Media**: el puente permite acceso a la zona baja. Algunos accesos
  bloqueados por construcciones o guardias.
- **Futuro**: el rio seco permite acceso total a la zona baja desde el inicio.

✅ **Limite real de la demo shareware** (implementado, no solo diseño):
en Edad Media y Futuro, la escalera de P3 a P5 esta bloqueada con un
`DIALOG_HINT` ("Inaccesible en la demo") en vez de completar el
`logic_screen_change(DIR_DOWN)` real — Prehistoria es la unica época
jugable mas alla de P3 en esta version. Deteccion por rango de `x`
sobre el centro del sprite de Eric (no por `eric_near()` en un punto
fijo, que en pruebas resulto poco fiable para este caso concreto — ver
leccion en Backlog).

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
dentro de la misma época.

### 6.4 Rebobinado temporal

Eric dispone de una reserva limitada de tiempo rebobinable por zona. Al activarlo,
el estado completo del mundo vuelve atras: posicion de Eric, posicion de enemigos,
estado de objetos y modificaciones del mapa en todas las épocas simultaneamente.

El rebobinado es una mecanica exclusiva de puzzles, no de combate. Sirve para
deshacer acciones incorrectas como colocar un objeto en el lugar equivocado o
activar algo que no debia activarse.

La reserva se recarga al conseguir cada fragmento.

### 6.5 Enemigos

Cada época tiene un tipo de enemigo con patrones de movimiento fijos:

- **Prehistoria**: animales prehistoricos que patrullan zonas concretas.
- **Edad Media**: guardias con rutas fijas.
- **Futuro**: drones con trayectoria de patrulla horizontal; en P1 y P3
  ademas rebotan en vertical a la vez (patron en zigzag), simulando una
  esfera girando sobre su eje segun se desplaza.

El contacto con un enemigo hace perder una vida y reaparecer en el
lado de la pantalla (izquierda o derecha) en el que estaba Eric en el
momento del golpe, conservando los objetos que lleva.

---

## 7. Puzzles

### 7.1 Estructura

Cada época tiene 3 puzzles:

- **1 puzzle principal**: da acceso al fragmento del artefacto. Implica cruce
  temporal entre épocas.
- **2 puzzles intermedios**: desbloquean zonas del mapa u objetos necesarios.
  Pueden ser internos a la época o con cruce temporal menor.

Total: 9 puzzles. Ritmo aproximado de 1 puzzle cada 3 pantallas.

### 7.2 Ubicacion de los fragmentos

| Fragmento | Época | Pantalla |
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
5. El chaman se aparta, Eric recoge el fragmento 1
```

Objetos: tronco (Prehistoria)
Viajes temporales: ninguno, puzzle interno a la época
Dificultad: media, introduce elemento arcade

#### Puzzle Principal: La ofrenda al chaman (no implementado en esta versión inicial para el concurso)

**Objetivo**: conseguir la planta sagrada para entregarla al chaman que custodia el fragmento 1.

```
1. Eric llega a pantalla 1, encuentra el circulo de piedras
   El chaman custodia el fragmento y pide una planta sagrada como ofrenda
2. Viaje a Edad Media: Eric encuentra la planta (todavia existe en esta época)
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

#### Puzzle Simple: El puente levadizo

**Objetivo**: bajar el puente levadizo para acceder a la zona baja de la Edad Media.

```
1. Eric llega al foso en Edad Media
   El puente levadizo esta subido, mecanismo exterior sin manivela
   Justificacion: alguien levanto el puente desde el interior
   y nunca mas se bajo
2. Viaje a Futuro: entre las ruinas del castillo Eric encuentra
   la manivela original, oxidada pero intacta
3. Eric coge la manivela
4. Viaje a Edad Media: Eric acopla la manivela al mecanismo exterior
   Acciona la polea, el puente baja
   Zona baja de la Edad Media desbloqueada
```

Objetos: manivela (Futuro)
Viajes temporales: 2 (Edad Media, Futuro, Edad Media)
Dificultad: baja-media

#### Puzzle Medio: El huevo y el guardia

**Objetivo**: conseguir la llave de la torre espantando al guardia que la custodia.

```
1. Eric recoge el huevo de dinosaurio en zona baja de Prehistoria
   (pantalla 8, junto al rio, donde estaban los nidos)
2. Viaje a Edad Media: Eric deposita el huevo cerca del guardia
   que patrulla frente a la puerta de la torre
3. El guardia se acerca curioso al objeto extrano
4. El huevo eclosiona (animacion): sale un dinosaurio cachorro
5. El guardia huye despavorido, se le cae la llave
6. Eric recoge la llave
7. Eric abre la puerta de la torre
   Acceso al interior desbloqueado
```

Objetos: huevo de dinosaurio (Prehistoria), llave (Edad Media)
Viajes temporales: 2 (Prehistoria, Edad Media)
Dificultad: media

#### Puzzle Principal: La plataforma elevadora

**Objetivo**: acceder a la cuspide de la torre donde esta el fragmento 2.

```
1. Eric entra a la torre (puerta abierta con la llave)
   Interior oscuro, murcielagos bloquean la plataforma elevadora
2. Eric encuentra la antorcha apagada en otra pantalla de Edad Media
3. Eric enciende la antorcha en la forja medieval
4. Eric vuelve a la torre con la antorcha encendida
   El humo espanta a los murcielagos
   La plataforma elevadora queda despejada pero inactiva
5. Viaje a Futuro: entre las ruinas del castillo Eric encuentra
   el artilugio de levitacion magnetica
6. Eric coge el artilugio
7. Viaje a Edad Media: Eric acopla el artilugio a la plataforma
   La plataforma se eleva levitando
8. Eric sube a la cuspide de la torre
   Recoge el fragmento 2
```

Objetos: antorcha (Edad Media), artilugio de levitacion (Futuro)
Viajes temporales: 2 (Edad Media, Futuro, Edad Media)
Dificultad: alta, cadena de acciones multiple

### 7.6 Puzzles de Futuro

#### Puzzle Simple: La zona de radiacion

**Objetivo**: descontaminar la zona de radiacion que bloquea el acceso a pantallas clave del Futuro.

**Nota de diseno**: el alquimista y su laboratorio son visibles desde la primera visita
a la Edad Media. En esa primera interaccion menciona que necesita mineral volcanico.
El jugador no sabra para que hasta que llegue al Futuro y encuentre la zona de radiacion
(foreshadowing).

```
1. Eric llega al Futuro, zona de radiacion bloquea el acceso
2. Viaje a Edad Media: Eric localiza al alquimista
   El alquimista puede crear el sellante pero necesita
   mineral volcanico (lo pide desde la primera visita) y mercurio
   El mercurio ya esta en su laboratorio
3. Viaje a Prehistoria: Eric recoge el mineral volcanico
   en la zona volcanica o geologica del mapa
4. Viaje a Edad Media: Eric entrega el mineral volcanico al alquimista
   El alquimista crea el sellante y se lo entrega a Eric
5. Viaje a Futuro: Eric aplica el sellante a la fuente de radiacion
   La zona queda descontaminada
   Acceso a pantallas clave desbloqueado
```

Objetos: mineral volcanico (Prehistoria), sellante (Edad Media)
Viajes temporales: 4 (Futuro, Edad Media, Prehistoria, Edad Media, Futuro)
Dificultad: media-alta

#### Puzzle Medio: La puerta hermetica y los portales

**Objetivo**: acceder a la zona de escombros de pantalla 6 donde esta el fragmento 3.

```
1. Eric llega a pantalla 6 en Futuro
   Puerta hermetica con dispositivo de portales inactivo
   El dispositivo pide un codigo de activacion en dos partes
2. Viaje a Prehistoria: Eric examina los grabados primitivos
   en la roca junto al circulo de piedras (pantalla 1)
   Los simbolos se muestran en pantalla, el jugador los anota
   Primera mitad del codigo obtenida
3. Viaje a Edad Media: Eric examina el manuscrito del alquimista
   Una secuencia de simbolos aparece en pantalla
   El jugador los anota
   Segunda mitad del codigo obtenida
4. Viaje a Futuro: Eric introduce el codigo completo
   en el dispositivo de portales
   El dispositivo se activa
   Aparecen dos portales a ambos lados de la puerta hermetica
   (todo visible en la misma pantalla)
5. Eric entra por el portal izquierdo
   Aparece por el portal derecho al otro lado de la puerta
   Acceso a la zona de escombros desbloqueado
```

Objetos: ninguno, el codigo es informacion que el jugador anota
Viajes temporales: 3 (Futuro, Prehistoria, Edad Media, Futuro)
Dificultad: media-alta
Nota: primera vez en el juego que la solucion es informacion, no un objeto fisico

#### Puzzle Principal: El Mecha y el fragmento 3

**Objetivo**: activar el Mecha de la mina de uranio para despejar los escombros
y recuperar el fragmento 3.

```
1. Eric recoge el cristal de cuarzo en pantalla X de Prehistoria
2. Eric lleva el cristal a la pantalla de tormentas electricas
   Secuencia arcade: Eric esquiva rayos con patrones fijos
   hasta que uno impacta en el cristal
   El cristal queda cargado, aparece contador decreciente
   encima del sprite (aprox. 35 segundos)
3. Viaje INMEDIATO al Futuro con el cristal cargado
   El contador sigue decreciendo durante el fade de transicion
4. Eric corre hasta el Mecha en pantalla 9
   Inserta el cristal en el reactor
   El Mecha arranca (animacion: luces, sonido de motor)
   El contador sigue decreciendo alimentando al Mecha
5. Eric se sube al Mecha
   El control cambia de Eric al Mecha
   HUD especial muestra estado del Mecha (3 impactos max)
   y contador del cristal
6. Pantalla 9 a 8: drones en patrones fijos atacan al Mecha
   Maximo 3 impactos antes de destruccion del Mecha
7. Pantalla 8 a 7: escombros caen desde arriba
   El jugador avanza rapido calculando el timing
8. Pantalla 7 a 6: drones Y escombros simultaneamente
   Tramo mas dificil, maxima tension
   Dos presiones simultaneas: drones y contador decreciente
9. El Mecha llega a pantalla 6
   Brazo mecanico desplaza los escombros (animacion)
   El fragmento 3 queda visible y accesible
10. Eric baja del Mecha
    Camina hasta el fragmento
    Recoge el fragmento 3
    Guardado automatico
    Secuencia de fin del juego
```

Objetos: cristal de cuarzo (Prehistoria)
Viajes temporales: 1 (Prehistoria, Futuro)
Mecanicas nuevas: contador decreciente, conduccion del Mecha,
                  HUD especial, escombros cayendo desde arriba
Dificultad: maxima, culminacion del juego

Nota tecnica: el contador del cristal se define como constante ajustable:
CRISTAL_CARGA_TICKS (35 * 70) -- 35 segundos a 70 Hz, ajustable en pruebas

---

## 8. Layout de pantallas

*El layout de Edad Media y Futuro esta pendiente de definir.*

Para cada pantalla se describe: visual, plataformas, enemigos, objetos,
conexiones y eventos especiales.

Convencion de conexiones: izquierda, derecha, arriba, abajo.
Las conexiones verticales entre nivel medio y zona baja se realizan
mediante liana (Prehistoria), escalera de madera (Edad Media) y
escalera metalica oxidada (Futuro).

Mapa de conexiones:

```
[1]-[2]-[3]
          |
     [4]-[5]
     |
[6]-[7]-[8]-[9]
```

- Pantalla 1: derecha a 2
- Pantalla 2: izquierda a 1, derecha a 3
- Pantalla 3: izquierda a 2, abajo a 5
- Pantalla 4: derecha a 5, abajo por liana a 7
- Pantalla 5: izquierda a 4, arriba a 3
- Pantalla 6: derecha a 7
- Pantalla 7: izquierda a 6, derecha a 8, arriba por liana a 4
- Pantalla 8: izquierda a 7, derecha a 9
- Pantalla 9: izquierda a 8

### 8.1 Prehistoria

#### Pantalla 1: Cima de la colina

Visual: espacio abierto con cielo calido (amanecer o atardecer), terreno rocoso,
circulo de megalitos tipo Stonehenge en el centro. El fragmento 1 brilla
suavemente en el centro del circulo. El chaman esta de pie junto al fragmento.

Plataformas: terreno irregular con rocas como plataformas naturales.

Enemigos: ninguno. El chaman es un personaje interactivo, no un enemigo.

Objetos: fragmento 1 (inaccesible hasta entregar la ofrenda), chaman.

Conexiones: derecha a pantalla 2.

Eventos:
- Al acercarse al chaman sin la ofrenda: mensaje en pantalla
- Al entregar la ofrenda: animacion del chaman apartandose
- Al recoger el fragmento: guardado automatico y mensaje narrativo

#### Pantalla 2: Ladera de la colina

Visual: ladera con pendiente pronunciada, arboles primitivos de tronco grueso
y copa densa, vegetacion exuberante. Un arbol grande con una colmena colgando
de una rama, rodeada de puntitos animados (abejas). Cielo calido visible
entre las copas.

Plataformas: rocas salientes en la ladera como escalones naturales, irregulares
y de distintas alturas.

Enemigos: ninguno.

Objetos: colmena en el arbol, interactiva con el palo. Al agitar la colmena
con el palo empieza a gotear miel. Eric coloca la taza debajo para recogerla.

Conexiones: izquierda a pantalla 1, derecha a pantalla 3.

Eventos:
- Al usar el palo en la colmena: animacion de abejas saliendo y colmena goteando
- Al colocar la taza bajo la colmena: animacion de la taza llenandose

#### Pantalla 3: Pie de la colina

Visual: terreno que se aplana desde la ladera. Vegetacion menos densa.
Rocas dispersas. Se intuye el nivel medio hacia la derecha. El oso patrulla
de izquierda a derecha bloqueando el paso.

Plataformas: terreno mas llano con algunas rocas bajas. El oso ocupa todo
el paso hasta que Eric deposita la miel.

Enemigos: oso de las cavernas patrullando de izquierda a derecha con patron fijo.

Objetos:
- Palo en el suelo junto a una roca, accesible directamente.
- Zona de deposito de miel marcada visualmente donde Eric deposita la taza.

Conexiones: izquierda a pantalla 2, abajo a pantalla 5.

Eventos:
- Al depositar la taza con miel: el oso se acerca, animacion de comer,
  paso desbloqueado.
- La taza queda en el suelo tras el evento, Eric puede recuperarla.

#### Pantalla 4: Nivel medio zona izquierda

Visual: terreno llano del nivel medio. Vegetacion mixta entre arbustos y
arboles de mediana altura. Se empieza a ver el rio en la parte inferior.
Una liana larga cuelga desde un arbol alto en el borde inferior.

Plataformas: terreno relativamente llano con algunas plataformas naturales
de roca a media altura.

Enemigos: jabali gigante patrullando en el nivel inferior con patron fijo horizontal.

Objetos: ninguno relevante para los puzzles.

Conexiones: derecha a pantalla 5, abajo por liana a pantalla 7.

Eventos: ninguno especial.

#### Pantalla 5: Nivel medio zona central (punto de guardado)

Visual: claro abierto con una hoguera ceremonial en el centro, rodeada de
piedras en circulo pequeno. Humo animado subiendo de la hoguera. Arboles
a ambos lados enmarcando la pantalla.

Plataformas: terreno llano, la pantalla mas accesible del mapa. Sin obstaculos
elevados. Es un respiro visual y arcade.

Enemigos: ninguno. La hoguera es un lugar sagrado.

Objetos: hoguera ceremonial, punto de guardado manual.

Conexiones: izquierda a pantalla 4, arriba a pantalla 3.

Eventos:
- Al interactuar con la hoguera: mensaje de confirmacion de guardado (S/N)

#### Pantalla 6: Zona baja extremo izquierdo

Visual: orilla izquierda del rio caudaloso. Vegetacion densa y humeda,
helechos gigantes, arboles de raices expuestas. El rio ocupa la parte
inferior con agua en movimiento animada. Una formacion geologica destacada
(roca grande o formacion especial) que en el Futuro sera reconocible
entre los escombros que cubren el fragmento 3.

Plataformas: orilla irregular con plataformas naturales de roca y raices.
El rio en la parte inferior es infranqueable, sin rocas emergentes.

Objetos: ninguno relevante en Prehistoria.

Conexiones: derecha a pantalla 7.

Eventos: ninguno especial en Prehistoria.

#### Pantalla 7: Zona baja centro izquierda

Visual: zona baja junto al rio. Terreno humedo y fangoso. Vegetacion
exuberante con helechos gigantes y arboles de tronco grueso. La liana
de subida al nivel medio visible en el borde superior. Se intuyen las
rocas emergentes del rio hacia la derecha.

Plataformas: terreno irregular con barro y raices. Rocas planas como
plataformas naturales. La liana de subida accesible desde una roca elevada.

Enemigos: reptil grande patrullando el nivel inferior con patron fijo horizontal.

Objetos: ninguno relevante para los puzzles.

Conexiones: izquierda a pantalla 6, derecha a pantalla 8, arriba por liana a pantalla 4.

Eventos: ninguno especial.

#### Pantalla 8: Zona baja centro derecha (cruce del rio)

Visual: el rio en su punto mas estrecho y con mas actividad. Rocas emergentes
de distintos tamanos distribuidas a lo largo del rio formando un camino irregular.
Nido de dinosaurio visible en la orilla derecha con el huevo destacado visualmente.
Vegetacion menos densa para que el jugador vea bien las rocas y los peces.

Plataformas: orilla izquierda elevada, rocas emergentes estaticas en el rio
a distintas alturas, orilla derecha elevada.

Enemigos: peces prehistoricos saltando verticalmente entre las rocas con
patrones fijos. Cada pez tiene su propia frecuencia y altura de salto.

Objetos: huevo de dinosaurio en el nido junto a la orilla derecha,
accesible tras cruzar el rio.

Conexiones: izquierda a pantalla 7, derecha a pantalla 9.

Eventos:
- Secuencia arcade del cruce del rio con peces y rocas.
- Si Eric cae al rio retrocede al inicio de la pantalla conservando objetos.

#### Pantalla 9: Zona baja extremo derecho (roca monolitica)

Visual: zona baja derecha junto al rio. La roca monolitica domina la pantalla,
alta y prominente, con grabados primitivos en su superficie (primera parte del
codigo del dispositivo de portales del Futuro). Vegetacion menos densa.
El rio visible en la parte inferior sin rocas emergentes. El tronco apoyado
contra un arbol.

Plataformas: terreno relativamente llano con la roca monolitica como elemento
vertical dominante. Plataformas naturales de roca alrededor del monolito.

Enemigos: ninguno.

Objetos:
- Tronco apoyado contra un arbol, accesible directamente.
- Roca monolitica interactiva: al examinarla aparecen los simbolos primitivos
  en pantalla. El jugador debe anotarlos. Primera parte del codigo del
  dispositivo de portales del Futuro.

Conexiones: izquierda a pantalla 8.

Eventos:
- Al examinar el monolito: mensaje en pantalla con los simbolos que el
  jugador debe anotar.

### 8.2 Edad Media

Regla de enemigos para Edad Media:
- Pantallas 1, 2, 3: zona de colina, presencia humana tranquila. Sin enemigos.
- Pantalla 4: nivel medio con acceso a zona baja. Un guardia.
- Pantalla 5: capilla, lugar sagrado. Sin enemigos.
- Pantallas 6, 7, 8, 9: zona del foso y torre. Guardias presentes, zona defensiva.

Nota grafica general: los objetos clave que Eric necesita conseguir de otros
personajes llevan un pequeno brillo animado (destello sutil) para indicar
al jugador su importancia sin romper la inmersion. Primer uso: la llave
en el cinturon del guardia de pantalla 9.

#### Pantalla 1: Cima de la colina

Visual: cima con el circulo de megalitos casi oculto por la vegetacion.
Las piedras estan cubiertas de musgo e hiedra, algunas inclinadas por el
paso del tiempo. Arboles y arbustos han crecido entre las piedras. El lugar
transmite abandono y misterio. La silueta general del circulo sigue siendo
reconocible para el jugador que recuerda la Prehistoria.

Plataformas: terreno irregular similar a Prehistoria pero con mas vegetacion.

Enemigos: ninguno. El lugar es considerado maldito por los medievales.

Objetos: ninguno relevante para los puzzles.

Conexiones: derecha a pantalla 2.

Eventos: ninguno especial. Pantalla de continuidad visual con Prehistoria.

#### Pantalla 2: Ladera de la colina

Visual: ladera con vegetacion menos salvaje que en Prehistoria. Un sendero
bien marcado sube por la ladera. Una valla de madera rustica separa el sendero
del campo. El arbol de la colmena sigue en pie pero sin colmena. Alguna
construccion humilde en segundo plano.

Plataformas: ladera irregular con sendero accesible. La valla puede servir
como plataforma baja en algunos tramos.

Enemigos: ninguno.

Objetos: ninguno relevante para los puzzles.

Conexiones: izquierda a pantalla 1, derecha a pantalla 3.

Eventos: ninguno especial.

#### Pantalla 3: Pie de la colina

Visual: pie de la colina donde el terreno se aplana. El sendero continua
desde pantalla 2 con la valla de madera a un lado. Una casa de campo medieval
con techo de paja y paredes de piedra. La chimenea humea suavemente (animacion).
La taza de madera esta apoyada en el alfeizar de una ventana de la planta baja,
perfectamente visible para el jugador.

Plataformas: terreno llano con alguna plataforma baja.

Enemigos: ninguno.

Objetos: taza de madera en el alfeizar de la ventana, accesible directamente.

Conexiones: izquierda a pantalla 2, abajo a pantalla 5.

Eventos: la taza se recoge acercandose al alfeizar.

#### Pantalla 4: Nivel medio zona izquierda

Visual: nivel medio con camino mas definido que en Prehistoria. Terreno
parcialmente desbrozado. Una pequena herreria o taller secundario. La escalera
de madera que baja a la zona baja visible en el borde inferior.

Plataformas: terreno mas llano que en Prehistoria. Plataformas de piedra
construidas por el hombre, mas regulares que las rocas naturales.

Enemigos: un guardia patrullando de izquierda a derecha con patron fijo.

Objetos: ninguno relevante para los puzzles.

Conexiones: derecha a pantalla 5, abajo por escalera de madera a pantalla 7.

Eventos: ninguno especial.

#### Pantalla 5: Nivel medio zona central (punto de guardado)

Visual: nivel medio central con una capilla de piedra en el centro. La puerta
esta abierta invitando a entrar. Una cruz de piedra en la fachada. Arboles a
ambos lados enmarcando la pantalla. Ambiente tranquilo y recogido.

Plataformas: terreno llano, pantalla mas accesible del mapa medieval.

Enemigos: ninguno. La capilla es un lugar sagrado.

Objetos: interior de la capilla como punto de guardado manual.

Conexiones: izquierda a pantalla 4, arriba a pantalla 3.

Eventos:
- Al entrar a la capilla: mensaje de confirmacion de guardado (S/N)

#### Pantalla 6: Zona baja extremo izquierdo

Visual: orilla izquierda del foso. El rio natural de Prehistoria ha sido
canalizado y convertido en foso defensivo con paredes de piedra cortada.
El agua del foso es oscura y quieta. La formacion geologica de Prehistoria
sigue reconocible como parte del muro de contencion del foso.

Plataformas: terreno plano y regular. El borde del foso como plataforma elevada.

Enemigos: un guardia patrullando la orilla del foso con patron fijo.

Objetos: ninguno relevante para los puzzles.

Conexiones: derecha a pantalla 7.

Eventos: ninguno especial.

#### Pantalla 7: Zona baja centro izquierda

Visual: zona del foso centro izquierda. Paredes de piedra del foso visibles.
La escalera de madera que sube al nivel medio en el borde superior. Se intuye
el puente levadizo hacia la derecha. Antorchas en la pared con ambiente oscuro
y amenazante.

Plataformas: terreno regular con plataformas de piedra junto al foso.
La escalera de madera accesible desde una plataforma elevada.

Enemigos: un guardia patrullando de izquierda a derecha con patron fijo.

Objetos: ninguno relevante para los puzzles.

Conexiones: izquierda a pantalla 6, derecha a pantalla 8,
arriba por escalera de madera a pantalla 4.

Eventos: ninguno especial.

#### Pantalla 8: Zona baja centro derecha (puente levadizo)

Visual: el puente levadizo en posicion levantada domina la pantalla. Las
cadenas y poleas del mecanismo visibles a ambos lados. El foso bajo el puente
con agua oscura. El mecanismo exterior con el hueco de la manivela claramente
visible pero vacio. Antorchas en la pared.

Plataformas: orilla izquierda elevada, orilla derecha inaccesible hasta
bajar el puente. El foso en la parte inferior infranqueable.

Enemigos: un guardia patrullando la orilla izquierda custodiando el mecanismo.

Objetos: mecanismo del puente levadizo con el hueco de la manivela vacio.

Conexiones: izquierda a pantalla 7, derecha a pantalla 9 (bloqueada hasta
bajar el puente).

Eventos:
- Sin la manivela: mensaje al examinar el mecanismo
- Con la manivela: Eric la acopla, acciona la polea, el puente baja
- Puente bajado: conexion derecha a pantalla 9 desbloqueada

#### Pantalla 9: Zona baja extremo derecho (torre)

Visual exterior: la torre medieval se alza imponente, evolucion de la roca
monolitica de Prehistoria. Misma ubicacion, misma verticalidad, construccion
de piedra con ventanas estrechas y puerta de madera reforzada en la base.
El fragmento 2 brilla debilmente en lo alto. Antorchas en la base.

Visual interior (al entrar por la puerta): interior oscuro con la plataforma
elevadora en el centro. Murcielagos visibles bloqueando la plataforma.
La forja medieval en un rincon con brasas encendidas.

Plataformas exterior: terreno llano frente a la torre. Puerta bloqueada con llave.
Plataformas interior: plataforma elevadora central, forja en un lateral.

Enemigos: guardia patrullando frente a la puerta con patron fijo. Lleva la
llave colgada del cinturon con un pequeno brillo animado (destello sutil).
Al eclosionar el huevo el guardia huye y la llave cae al suelo.

Objetos:
- Puerta de la torre bloqueada con llave, interactiva.
- Zona de deposito del huevo marcada visualmente cerca del guardia.
- Llave: aparece en el suelo cuando el guardia huye.
- Antorcha apagada en el interior junto a la forja.
- Forja medieval con brasas: permite encender la antorcha.
- Plataforma elevadora: inactiva hasta acoplar el artilugio de levitacion.
- Fragmento 2: en la cuspide, accesible tras activar la plataforma.

Conexiones: izquierda a pantalla 8.

Eventos:
- Al depositar el huevo: el guardia se acerca curioso
- Animacion de eclosion: sale el dinosaurio cachorro, el guardia huye,
  cae la llave al suelo
- Al entrar a la torre: cambio de fondo BMP al interior
- Al encender la antorcha en la forja: animacion de llama
- Al volver con la antorcha encendida: animacion de murcielagos huyendo
- Al acoplar el artilugio de levitacion: la plataforma se eleva
- Al recoger el fragmento 2: guardado automatico y mensaje narrativo

### 8.3 Futuro

Nota grafica general para el Futuro:
Los fondos de todas las pantallas del Futuro eliminan cualquier atisbo de
vegetacion y cielos despejados. El ambiente es distopico al estilo Blade Runner:
ruinas urbanas o industriales en el horizonte, cielos cargados, estructuras
metalicas oxidadas, humo o niebla en segundo plano. Los fondos son ricos en
detalle pero usan una paleta de colores claros y desaturados para que Eric
y los enemigos en el plano principal sean siempre claramente visibles y no
se pierdan contra el escenario.

#### Pantalla 1: Cima de la colina

Visual: cima erosionada con el circulo de megalitos parcialmente derruido.
Algunas piedras caidas, otras en pie pero inclinadas, todas con el musgo
de la Edad Media ahora seco y gris. Vegetacion desaparecida, sustituida
por tierra arida y grietas. El circulo sigue siendo reconocible. En el
horizonte se intuyen ruinas de construcciones lejanas.

Plataformas: terreno irregular con las piedras caidas como plataformas naturales.

Enemigos: un dron de reconocimiento patrullando con trayectoria fija horizontal.

Objetos: el circulo es interactivo, al examinarlo Eric reflexiona sobre
el paso del tiempo (guino narrativo).

Conexiones: derecha a pantalla 2.

Eventos:
- Al examinar el circulo: mensaje narrativo de Eric.

#### Pantalla 2: Ladera de la colina

Visual: ladera arida y erosionada. El sendero medieval ha desaparecido,
sustituido por tierra reseca con grietas. Quedan algunos postes podridos
como unico rastro de la valla de madera medieval. El arbol de la colmena
sigue en pie pero seco y esqueletico, sin hojas. Ambiente desolado y silencioso.

Plataformas: ladera irregular con rocas sueltas. Menos plataformas que en
épocas anteriores por la erosion.

Enemigos: ninguno.

Objetos: ninguno relevante para los puzzles.

Conexiones: izquierda a pantalla 1, derecha a pantalla 3.

Eventos: ninguno especial.

#### Pantalla 3: Pie de la colina

Visual: pie de la colina arido y desolado. El empedrado del suelo medieval
todavia visible bajo polvo y tierra, evocando la casa y el camino que
existieron aqui. Donde estaba la valla de madera medieval ahora hay postes
metalicos oxidados con rayos laser entre ellos, pulsando con luz intensa.
Fondo distopico con estructuras lejanas y cielo cargado. Paleta clara y
desaturada.

Plataformas: terreno irregular con algunas rocas sueltas.

Enemigos: un dron patrullando con trayectoria fija horizontal.

Objetos: ninguno relevante para los puzzles.

Peligros: rayos laser entre los postes metalicos. Si Eric los toca retrocede
al inicio de la pantalla. Los rayos tienen un patron fijo de encendido y
apagado que el jugador puede aprender para encontrar la ventana de paso.

Conexiones: izquierda a pantalla 2, abajo a pantalla 5.

Eventos: animacion de pulso de los rayos laser indicando el patron de
encendido y apagado.

#### Pantalla 4: Nivel medio zona izquierda

Visual: nivel medio con restos de una instalacion industrial abandonada.
Estructuras metalicas oxidadas a distintas alturas. Tuberias rotas. Suelo
cubierto de polvo y escombros menores. La escalera metalica oxidada que
baja a la zona baja visible en el borde inferior, todavia funcional aunque
deteriorada. Fondo distopico. Paleta clara y desaturada.

Plataformas: estructuras metalicas y tuberias rotas como plataformas a
distintas alturas. Mas variedad vertical que en épocas anteriores.

Enemigos: un dron patrullando con trayectoria fija.

Objetos: ninguno relevante para los puzzles.

Conexiones: derecha a pantalla 5, abajo por escalera metalica oxidada a pantalla 7.

Eventos: ninguno especial.

#### Pantalla 5: Nivel medio zona central (punto de guardado)

Visual: nivel medio central con una terminal o capsula de hibernacion en
el centro, todavia operativa con una tenue luz parpadeante. Escombros menores
y estructuras metalicas caidas alrededor. El contraste entre la terminal
funcionando y el ambiente desolado refuerza el tono distopico. Fondo Blade
Runner. Paleta clara y desaturada.

Nota: la terminal debe estar en las mismas coordenadas de pixels que la
hoguera de Prehistoria y la capilla de Edad Media.

Plataformas: escombros menores como plataformas bajas. La terminal como
elemento vertical de referencia.

Enemigos: ninguno. Los drones evitan la zona por interferencias electromagneticas
de la terminal.

Objetos: terminal como punto de guardado manual.

Conexiones: izquierda a pantalla 4, arriba a pantalla 3.

Eventos:
- Al interactuar con la terminal: mensaje de confirmacion de guardado (S/N)
- Animacion de luz parpadeante en la terminal.

#### Pantalla 6: Zona baja extremo izquierdo (fragmento 3)

Visual: zona baja arida con el cauce seco del rio/foso visible como depresion
en el terreno. Grandes escombros dominan la pantalla con el fragmento 3
brillando debilmente bajo los restos. La puerta hermetica de metal en el
centro con el dispositivo de portales a un lado. Espacio marcado visualmente
a ambos lados de la puerta donde apareceran los portales. Fondo distopico.
Paleta clara y desaturada.

Plataformas: escombros de distintos tamanos como plataformas irregulares.
El cauce seco como zona deprimida transitable.

Enemigos: un robot pesado patrullando frente a los escombros con patron fijo.

Objetos:
- Fragmento 3 bajo los escombros, inaccesible hasta que el Mecha los despeje.
- Dispositivo de portales junto a la puerta hermetica, interactivo.
- Dos zonas marcadas donde apareceran los portales a ambos lados de la puerta.

Conexiones: derecha a pantalla 7.

Eventos:
- Sin el codigo: mensaje al examinar el dispositivo.
- Con el codigo completo: animacion de activacion, aparecen los dos portales.
- Al entrar por el portal izquierdo: Eric aparece por el portal derecho.
- Tras despejar el Mecha los escombros: fragmento 3 visible y accesible.
- Al recoger el fragmento 3: guardado automatico y secuencia de fin del juego.

#### Pantalla 7: Zona baja centro izquierda

Visual: zona baja centro izquierda con el cauce seco visible como depresion.
Ruinas industriales a ambos lados. Tuberias rotas emergiendo del suelo.
La escalera metalica oxidada que sube al nivel medio en el borde superior.
Residuos y escombros menores dispersos. Fondo distopico. Paleta clara y
desaturada.

Plataformas: cauce seco transitable. Tuberias rotas y estructuras metalicas
como plataformas a distintas alturas. Escalera metalica accesible desde
plataforma elevada.

Enemigos: un dron patrullando con trayectoria fija.

Objetos: ninguno relevante para los puzzles.

Conexiones: izquierda a pantalla 6, derecha a pantalla 8,
arriba por escalera metalica oxidada a pantalla 4.

Eventos: ninguno especial.

#### Pantalla 8: Zona baja centro derecha (zona de radiacion)

Visual: el cauce seco en su punto mas ancho. Los restos del puente medieval
caido y oxidado en el fondo del cauce, reconocibles como el mismo puente
que Eric bajo en la Edad Media. Las cadenas y poleas del mecanismo visibles
entre los escombros. Zona de radiacion marcada con simbolo universal de
radiactividad y suelo con color especial. Fondo distopico. Paleta clara y
desaturada.

Plataformas: cauce seco transitable. Restos del puente caido como plataformas
irregulares. La zona de radiacion ocupa parte de la pantalla bloqueando el paso.

Enemigos: un robot pesado patrullando con patron fijo.

Objetos: ninguno relevante para los puzzles. La zona de radiacion es el
bloqueo principal.

Conexiones: izquierda a pantalla 7, derecha a pantalla 9 (bloqueada por
zona de radiacion hasta aplicar el sellante).

Eventos:
- Sin el sellante: si Eric entra en la zona de radiacion retrocede al inicio.
- Con el sellante: Eric aplica el sellante, zona descontaminada, conexion
  derecha a pantalla 9 desbloqueada.

#### Pantalla 9: Zona baja extremo derecheeeo (Mecha y mina de uranio)

Visual: zona baja extremo derecho con las ruinas de la torre medieval
dominando la pantalla, reconocible como evolucion de la roca monolitica
de Prehistoria y la torre intacta de Edad Media. Solo queda un muro parcial
en pie. El cauce seco en la parte inferior. La entrada a la mina de uranio
integrada en las ruinas de la torre con una gran puerta metalica industrial
entreabierta. El Mecha visible en la entrada, imponente y con luces apagadas.
Fondo distopico. Paleta clara y desaturada.

Plataformas: terreno irregular con restos de la torre como plataformas elevadas.
La entrada a la mina a nivel del suelo.

Enemigos: dos drones patrullando con trayectorias fijas.

Objetos:
- El Mecha: inactivo hasta insertar el cristal de cuarzo cargado.
- Reactor del Mecha: hueco visible donde insertar el cristal.
- Ruinas de la torre: al examinarlas aparecen los simbolos del monolito
  de Prehistoria parcialmente borrados. Guino narrativo.

Conexiones: izquierda a pantalla 8.

Eventos:
- Al acercarse al Mecha sin el cristal: mensaje indicando que necesita energia.
- Al insertar el cristal cargado: animacion de arranque del Mecha.
- Al subirse al Mecha: cambio de control de Eric al Mecha, HUD especial
  con estado del Mecha (3 impactos max) y contador del cristal.
- Conduccion del Mecha desde pantalla 9 hasta pantalla 6:
  - Pantalla 9 a 8: drones atacan al Mecha, maximo 3 impactos antes de destruccion.
  - Pantalla 8 a 7: escombros caen desde arriba, el jugador calcula el timing.
  - Pantalla 7 a 6: drones y escombros simultaneamente, maxima tension.
- Al llegar a pantalla 6: brazo mecanico desplaza los escombros.
- Eric baja del Mecha y recoge el fragmento 3.

---

## 8. Sistema de guardado

| Evento | Tipo |
|---|---|
| Al iniciar el juego | Carga automatica si existe SAVEGAME.DAT |
| Al llegar a pantalla 5 de cada época | Guardado manual en objeto especial |
| Al conseguir cada fragmento | Guardado automatico |
| Al completar el juego | Guardado automatico |

El objeto de guardado manual en pantalla 5 es distinto en cada época:

- **Prehistoria**: hoguera ceremonial.
- **Edad Media**: capilla o posada.
- **Futuro**: terminal o capsula de hibernacion.

---

## 9. Arte grafico

*Pendiente de definir con la colaboradora artistica.*

Restricciones tecnicas: 320x200 pixeles, 256 colores indexados, paleta VGA.

Cada época debe tener una identidad visual inmediata y reconocible. El jugador
debe sentir el cambio de época antes de leer ningun texto.

### 9.1 Continuidad visual entre épocas

Uno de los requisitos artisticos mas importantes del juego es que el jugador
pueda reconocer que esta en el mismo lugar geografico en las tres épocas.
El arte debe transmitir la evolucion del tiempo de forma visual sin necesidad
de texto explicativo.

Recursos graficos para conseguirlo:

- **Siluetas reconocibles**: aunque una construccion este destruida en el Futuro,
  algun arco, muro parcial o contorno debe ser reconocible como la misma
  estructura vista en la Edad Media.
- **Elementos persistentes**: la roca monolitica de Prehistoria se convierte en
  cimiento de la torre medieval, que se convierte en el unico muro en pie del
  Futuro. El jugador la reconoce en las tres épocas.
- **Paleta de colores que evoluciona**: tonos calidos y vivos en Prehistoria,
  grises y ocres en Edad Media, tonos desaturados y frios en Futuro.
- **Detalles narrativos en el fondo**: en el Futuro, entre los escombros, restos
  reconocibles de épocas anteriores: un escudo heraldico partido, piedras con
  inscripciones medievales, restos oxidados de mecanismos.

### 9.2 Elementos a definir por época

- Paleta de colores predominante
- Estilo de los fondos (BMP de 320x200 por pantalla y época)
- Tileset de plataformas y elementos del entorno
- Sprites de Eric (spritesheet por época)
- Sprites de enemigos
- Objeto de guardado manual

### 9.3 Paleta VGA compartida por época (estado tecnico real y principio general)

Cada época tiene su PROPIA paleta VGA de 256 colores, compartida entre
sus 9 fondos y todos los sprites/objetos de esa época. El reparto de
indices descrito abajo es el ya implementado para Prehistoria, y debe
servir como plantilla para Edad Media y Futuro cuando llegue su turno
(18 pantallas restantes + sus respectivos sprites).

**Estado por época:**
- **Prehistoria**: ✅ implementado y verificado (detalle completo abajo).
- **Edad Media**: ⬜ pendiente. Cuando se generen los 9 fondos, aplicar
  el mismo proceso: verificar que comparten los mismos colores en su
  rango de fondo (igual que se hizo aqui comparando byte a byte
  PRE_P1..P9), y reservar rangos propios para Eric (puede que distinto
  spritesheet/paleta segun época), huevo si reaparece, y cualquier
  objeto interactivo propio de Edad Media (taza, antorcha, puente
  levadizo, escalera de madera, etc.).
- **Futuro**: ⬜ pendiente, mismo proceso. Objetos propios conocidos:
  escalera metalica, Mecha, elementos de la zona de radiacion.

**Reparto de indices implementado en Prehistoria (referencia):**

- **0-129 (130 colores)**: fondos de Prehistoria. Paleta COMPARTIDA
  entre las 9 pantallas (PRE_P1.BMP a PRE_P9.BMP) — verificada byte a
  byte, los 130 colores son identicos en los 9 BMP. Antes cada pantalla
  tenia su propia paleta de hasta 219 colores; ya no.
- **130-219 (90 colores)**: sprite de Eric. Basado en el spritesheet
  detallado de alta resolucion (ver seccion 3/anexos) reducido y
  cuantizado a 90 colores reales. Antes eran solo 16 colores en el
  rango 238-255.
- **220-231 (12 colores)**: huevo de dinosaurio (HUEVO.BMP, EGGICON.BMP).
  SIN TOCAR, paleta propia desde siempre, no afectada por nada de lo
  anterior.
- **232-241 (10 colores)**: palo (STICK.BMP, STKICON.BMP). Colores reales
  extraidos del BMP, cuantizados por frecuencia de pixel.
- **242-255 (14 colores)**: doble uso, sin conflicto real:
  - Roca de P1 (ROCK.BMP): 14 colores reales cuantizados. Se inyectan
    SOLO dentro de `screen_draw_rock()` (no en cambios de pantalla
    generales) para no pisar el agua de P8, ya que comparten rango.
  - Agua del rio de P8 (PRE_P8.BMP): 14 tonos azules propios, viven
    embebidos en la paleta del propio BMP (el motor carga el fondo con
    `set_palette(bmp.palette)`, asi que no hace falta inyeccion aparte
    en codigo).
  - Roca y agua nunca estan visibles a la vez (la roca solo en P1, el
    agua solo en P8), por eso compartir el rango no da problema.

**Patron a seguir para nuevos elementos graficos**, dentro de Prehistoria
(oso, miel, tronco, reptil, jabali, frames del chaman, etc.) y, cuando
toque, en Edad Media y Futuro desde cero: extraer los colores reales
del BMP final, cuantizarlos por frecuencia de pixel al numero de
colores que haga falta, y colocarlos en huecos libres del rango de la
época correspondiente si no chocan en pantalla con otros sprites que
ya usen ese rango, o reconsiderar el reparto (compartir tonos entre
sprites de paleta similar, por ejemplo tierra/marron entre tronco y
roca) si el rango se queda corto. Edad Media y Futuro empiezan con los
256 indices libres — el reparto exacto (cuantos colores para fondos,
cuantos para Eric, cuantos de margen para objetos) se decide al
generar su primer fondo y sprite, siguiendo la misma logica que aqui.

**Archivos de codigo afectados**: `screen.c` contiene las paletas
(`eric_palette`, `stick_palette`, `rock_palette`, `egg_palette`) y las
funciones `screen_inject_*_palette()` que las cargan en el hardware VGA.
`try_load_bmp()` (tambien en screen.c) es la funcion real que parsea
los BMP de 8 bits e ignora cualquier asuncion sobre el orden de paleta
que no este verificada contra el codigo.

---

## 10. Musica y sonido

*Pendiente de definir con la colaboradora musical.*

### Musica

Una pista por época en formato XM, en bucle. Debe poder escucharse durante
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
- Viaje temporal (fade entre épocas)
- Guardado
- Conseguir fragmento

---

## 11. Tecnologia

- **Plataforma**: MS-DOS 6.22, compatible con 486DX66 / 16MB RAM
- **Compilador**: OpenWatcom 2.0, C89
- **Video**: VGA modo 13h (320x200, 256 colores), double buffering
- **Sonido**: Judas Sound System (Sound Blaster, XM + WAV)
- **Emulacion**: DOSBox-X, PCem, 86Box
- **Repositorio**: https://github.com/LevoOne/Time-Daze (público)

---

## 12. HUD

### 12.1 HUD normal

El HUD es siempre visible durante el juego. Ocupa una franja fija de 16 pixels
en la parte inferior de la pantalla, dejando el area de juego en 320x184 pixels.

Distribucion de izquierda a derecha:

```
[VIDAS] [REBOBINADO] [OBJETO ACTUAL] ............. [F1] [F2] [F3]
```

- **Vidas** (extremo izquierdo): icono de Eric seguido del numero de vidas
  restantes. Tres iconos o corazones que se apagan al perder una vida.
- **Rebobinado** (centro izquierda): icono de rebobinado con indicador
  de reserva disponible. Puede ser una barra o un numero.
- **Objeto actual** (centro): icono del objeto que Eric lleva en ese momento.
  Vacio si no lleva ningun objeto. Permite al jugador saber en todo momento
  que lleva sin necesidad de abrir ningun menu.
- **Fragmentos** (extremo derecho): tres iconos de fragmentos del artilugio
  dibujados en color grisaceo tenue. Cuando Eric recoge cada fragmento el
  icono correspondiente se ilumina en un color concreto y distintivo.
  Los tres iluminados indican que el juego esta completado.

### 12.2 HUD del Mecha

Durante la secuencia de conduccion del Mecha el HUD normal se sustituye
temporalmente por un HUD especial que simula el panel de control del Mecha.
La transicion entre ambos HUDs se hace con una animacion rapida de barrido
horizontal.

Distribucion de izquierda a derecha:

```
[ESTADO: ***] [CRISTAL: 18s] [VELOCIDAD: >>]
```

- **Estado del Mecha** (izquierda): tres iconos (estrella, luz o simbolo
  mecanico) que se apagan con cada impacto recibido.
  Tres encendidos = intacto, dos = danado, uno = muy danado, cero = destruido.
- **Contador del cristal** (centro): segundos restantes del cristal de cuarzo,
  decreciendo en tiempo real. Cambia de color segun el tiempo restante:
  blanco en condiciones normales, amarillo por debajo de 15 segundos,
  rojo por debajo de 5 segundos.
- **Velocidad** (derecha): indicador visual de la velocidad del Mecha.
  Sin funcion de gameplay, refuerza la sensacion de movimiento pesado.

---

*Documento vivo. Se actualiza a medida que avanza el desarrollo.*

---

## TODO - Arte pendiente
Estado: ✅ Hecho | ⬜ Pendiente

### Indicadores visuales de transicion vertical

Todas las pantallas con conexion vertical necesitan un elemento visual
que indique al jugador que puede subir o bajar:

**Prehistoria (liana):**
 ✅ P1 eliminar la roca del BMP (ya cambiado en PRE_P1.BMP) para que sea un sprite
- P3 borde derecho (x=295): liana colgando, baja a P5
- P5 borde derecho (x=295): liana colgando, sube a P3
- P4 borde izquierdo (x=40): liana colgando, baja a P7, hacer que el gráfico de la liana baje hacia la parte de abajo, y se corte por arriba, como está ahora es confuso
- P7 borde izquierdo (x=40): liana colgando, sube a P4

**Edad Media (escalera de madera):**
- P3 borde derecho (x=295): escalera de madera, baja a P5
- P5 borde derecho (x=295): escalera de madera, sube a P3
- P4 borde izquierdo (x=40): escalera de madera, baja a P7
- P7 borde izquierdo (x=40): escalera de madera, sube a P4

**Futuro (escalera metalica):**
- P3 borde derecho (x=295): escalera metalica, baja a P5
- P5 borde derecho (x=295): escalera metalica, sube a P3
- P4 borde izquierdo (x=40): escalera metalica, baja a P7
- P7 borde izquierdo (x=40): escalera metalica, sube a P4

### Correciones gráficas pendientes

**Prehistoria:**
✅ P8: modificar el bitmap para que la roca descienda y coincida con la coordenada y de la plataforma
✅ P8: modificar el bitmap para que parezca que el río llega hasta la parte de abajo de la pantalla, fluyendo hacia abajo-izquierda
✅ P1: la roca redonda no puede estar ahí en el bitmap porque Eric aparece directamente en el lado del chamán, y eso debería ser posible sólo cuando se resuelve el primer puzzle P1
✅ STICK.BMP / STKICON.BMP: tenían paleta sin asignar correctamente (índices de píxel apuntando a huecos de la paleta de fondo en vez de colores propios). Resuelto: paleta propia real en índices 232-241 (ver sección 9.3).
✅ ROCK.BMP: mismo problema que el palo — sin paleta propia. Resuelto: paleta propia real en índices 242-255, inyectada solo al dibujar la roca (ver sección 9.3).
 - Modificar el gráfico / frames del chamán en P1, es ilegible
 - SHAMAN.BMP: sin confirmar todavía, pero pinta del mismo bug de paleta que tenían el palo y la roca (sin paleta propia en screen.c). Revisar al mismo tiempo que se rehaga el sprite del chamán.

**Edad Media:**

**Futuro:**

### Objetos interactivos visibles en el escenario

Los siguientes objetos deben ser visibles en el fondo o como sprites separados:

**Prehistoria:**
- P2: colmena accesible desde la plataforma central (x=160, y=130)
- P3: palo en el suelo junto a la roca izquierda (x=80, y=159)

**Edad Media:**
- P3: taza en el alfeizar de la casa (x=180, y=159)
- P9: antorcha apagada en la torre (x=100, y=160)

**Pendiente de revisar:** resto de objetos interactivos por pantalla
al integrar el arte final de cada época.

### Sprites animados pendientes

**Prehistoria P1:**
- **Roca sprite**: la roca redonda que bloquea el acceso al chaman debe ser un
  sprite independiente del BMP de fondo. Al resolver el puzzle del tronco/palanca,
  la roca debe animarse rodando hacia la derecha hasta salir de pantalla, dejando
  libre el paso al chaman.
  ✅ Roca ya es sprite independiente con paleta propia (ver seccion 9.3).
  ✅ Bloqueo de paso verificado: barrera invisible en logic.c (screen==0,
  PUZZLE_LEVER) ajustada a la posicion real del sprite (g_game.player.x
  no puede bajar de cierto valor mientras el puzzle no este resuelto).
  Render y colision confirmados correctos por el usuario.
  ✅ Animacion de rodar implementada: ROCKROLL.BMP (8 frames anti-horario,
  512x64), screen_trigger_rock_roll() + screen_draw_rock_rolling() en
  screen.c. Velocidad tunable con ROCK_ROLL_SPEED y ROCK_ROLL_ANIM_SPEED.
  ⚠️  PENDIENTE ELIMINAR: gatillo temporal en logic.c (bloque TODO TEST,
  screen==0) que activa la roca con ENTER sin necesitar el tronco.
  Eliminar cuando se implemente la logica real del puzzle tronco/palanca.
  ⬜ Sonido: arrancar efecto/musica de roca rodando al llamar a
  screen_trigger_rock_roll(), y detenerlo en screen_draw_rock_rolling()
  cuando s_roll_x < -64 (justo antes de resolver PUZZLE_LEVER).
- **Chaman sprite**: el chaman de P1 debe ser un sprite animado situado sobre la
  plataforma central (dolmen). No se desplaza. Realiza una animacion de baile
  chamanico en bucle (varios frames de movimiento ritual: brazos alzados, giros,
  invocaciones). Se detiene al interactuar con Eric.
  **TODO**: mejorar el sprite del chaman. Al reducirlo a 32x32 pixeles el nivel de
  detalle es insuficiente y el resultado es un amasijo de pixeles poco legible.
  Opciones: regenerar con Gemini a mayor resolucion (48x48 o 64x64) y ajustar
  el codigo de dibujado, o retocar manualmente en Aseprite.
  Cuando se regenere, aplicar tambien el patron de paleta propia descrito en
  9.3 (probablemente comparte el mismo bug que tenian palo/roca).

**Prehistoria P5:**
- **Hoguera sprite**: la hoguera del punto de guardado en P5 debe ser un sprite
  animado en lugar de una imagen estatica en el BMP. La animacion simula las llamas
  con varios frames (3-4 frames en bucle). La hoguera se dibuja encima del BMP de
  fondo en su posicion fija.

**Resto de elementos graficos de Prehistoria pendientes de generar e integrar**
(mismo patron de paleta propia en huecos libres de 242-255, ver seccion 9.3):
oso, animacion de miel cayendo, tronco, reptil, jabali, peces en P8.

**Futuro P1 y P3:**
- ✅ **Dron sprite** (`DRONE.BMP`, 3 frames de 32x32, paleta propia
  242-255 via `screen_inject_drone_palette()`) — esfera con nucleo
  verde, sin espejado por direccion (a diferencia de los animales, el
  propio giro del contenido interior ya transmite el movimiento). Dos
  tecnicas usadas segun el efecto buscado en cada version del sprite:
  rotacion 2D simple en el plano de la imagen, y una version posterior
  con desplazamiento tipo "esfera girando sobre su eje vertical"
  (remapeo columna a columna segun angulo esferico, con mascara
  circular fija identica en los 3 frames para que el contorno nunca
  tiemble entre frames, solo el contenido interior). Hitbox centrado
  con `ENEMY_DRONE_XOFF`/`YOFF` (32x32 visual sobre 20x12 de colision),
  mismo patron que jabali/reptil. Los pixeles del sprite usan indices
  242-255 directamente (no 0-13): `bmp_draw_tile()` copia el indice de
  pixel crudo al `back_buffer`, asi que debe coincidir con el rango
  real ya inyectado, y el 0 queda reservado para transparencia.

✅ **Secuencia de intro animada** — implementada en `show_presentation()`
(`timed.c`): logos (`contest.bmp` con jingle propio `CONTEST.WAV`,
`h3logo.bmp` en silencio, `timed.bmp`) seguidos de 8 paneles
(`INTRO1.BMP`-`INTRO8.BMP`) con `INTRO.XM` de fondo continuo. Narra la
reunion de ex-alumnos, el descubrimiento del artefacto, la activacion
accidental y la fragmentacion de Eric entre las tres épocas. Saltable
solo panel a panel con ESPACIO (no de golpe toda la secuencia).

### Animacion y movimiento del jugador

⬜ El ciclo de animacion al caminar se percibe un poco acelerado. Ajustable
en `ANIM_SPEED` (player.h, actualmente 8) — subir el valor ralentiza el
ciclo de piernas. El salto en si (trayectoria, no animacion) depende en
cambio de `PLAYER_GRAVITY` y `PLAYER_JUMP`, tambien en player.h.

### Sonido

✅ Sistema de sonido implementado con 5 canales SFX fijos/dinamicos
(`SFX_CHANNELS=5`, ampliado desde 4 para dar hueco a `SFX_HURT` sin
reciclar el slot dinamico):
- Indice 0 `SFX_JUMP`: salto, cargado una vez al arrancar.
- Indice 1: dinamico, recargado por pantalla/evento (roca rodando,
  pisadas del oso/jabali/reptil, goteo de miel, salpicadura de los
  peces, viaje entre épocas). `sfx_sync_dynamic_slot()` recarga el
  sonido correcto de la pantalla de destino tras cada viaje temporal,
  evitando que un sonido puntual (p.ej. `TRAVEL.WAV`) se quede pegado
  indefinidamente en pantallas que necesitan su propio sonido continuo
  (p.ej. el goteo de miel).
- Indice 2 `SFX_PICKUP` / Indice 3 `SFX_DROP`: coger/soltar objetos,
  cargados una vez al arrancar.
- Indice 4 `SFX_HURT`: perdida de vida, cargado una vez al arrancar.
  `player_hit()` incluye una pausa breve (~1.36s, la duracion real del
  WAV) trasladando el impacto, servida en pasos de 4 ticks con
  `sound_update()` intercalado para no distorsionar la musica de fondo
  durante la pausa.

**Leccion aprendida (bug real, resuelto)**: recargar el mismo canal
SFX repetidamente durante una partida larga (`sfx_free()`+`sfx_load()`
muchas veces sobre el mismo indice) puede acabar dejando el sample en
bucle indefinido de forma impredecible — posiblemente degradacion de
los punteros internos de Judas tras multiples recargas. La solucion
fiable para sonidos puntuales que no dependen de la pantalla (como el
golpe) es cargarlos **una sola vez** en un canal fijo propio, igual
que ya funcionaba `SFX_JUMP`, en vez de reciclar el slot dinamico.

Musica: XM (`PRETHEME.XM`, `MEDTHEME.XM`, `FUTTHEME.XM`, `fire.xm` para
P5) cargada con `music_load_xm()`, que libera el tema anterior
automaticamente. Memoria por época (`s_epoch_last_screen`) implementada:
al volver a una época tras viajar, Eric reaparece en la ultima pantalla
real en la que estuvo, no siempre en P1.

✅ **`audio_safe_wait(ticks)`** (`engine.c`/`engine.h`) — sustituye a
`timer_wait()` en cualquier espera que ocurra mientras suena musica o un
SFX largo (mas de 1-2s): sirve el mezclador en cada tick individual, en
vez de una sola vez al principio/final como hace `timer_wait()`. Bug
real resuelto: sin esto, cualquier `timer_wait()` largo con musica de
fondo sonando se oye entrecortado/distorsionado — se detecto con
`CONTEST.WAV` (jingle de `contest.bmp` en `show_presentation()`) y con
`INTRO.XM` (musica de los 9 paneles de intro, `timed.bmp`→`INTRO8.BMP`).
`vga_fade_in()`/`vga_fade_out()` (`engine.c`) tambien se corrigieron con
el mismo criterio (sirven audio cada tick dentro de cada paso del
fundido, no una vez por paso completo) — afecta a **todo** el juego, no
solo a la intro.

✅ **Jingles de la secuencia de presentacion**: `CONTEST.WAV` (logo del
concurso, canal dinamico 1, `sfx_set_loop(1,0)` explicito para
garantizar que no quede en bucle) e `INTRO.XM` (musica continua desde
`timed.bmp` hasta el final de `INTRO8.BMP`, un solo sample disparado por
`music_load_xm()`, sin necesitar `audio_safe_wait()` gracias al
mecanismo interno de Judas para musica vs. SFX sueltos). Fundido de
salida suave de `INTRO.XM` al final de `show_presentation()` (bajada
gradual de `judas_setmusicmastervolume()` en vez de `music_stop()` seco).

### Cuadro de dialogo

✅ Sistema implementado en `dialog.c`. Tres tipos:
- `DIALOG_HINT`: mensaje de 1-2 lineas. Se abre con `A` (pistas
  ambientales de pantalla, sin interaccion especifica) o con ESPACIO
  (chaman, monolito, aviso de limite de demo — siempre ligado a
  `eric_action()`); se cierra con **cualquiera de las dos** teclas.
  Bug real resuelto: al principio `A` abria y solo ESPACIO cerraba,
  poco ergonomico — `dialog_update()` ahora acepta `KEY_SPACE` o
  `KEY_A` indistintamente para cerrar cualquier `DIALOG_HINT`.
- `DIALOG_TALK`: conversacion real multi-pagina, cada ESPACIO avanza a
  la siguiente linea del array; en la ultima pagina el pie cambia a
  "Cerrar" en vez de "[ ESPACIO ]".
- `DIALOG_YESNO`: confirmacion S/N, usado en el punto de guardado de
  la hoguera y en la confirmacion de salida con ESC.

✅ **Separacion de tecla de accion y tecla de pista** — antes ambas
compartian ESPACIO: si Eric llevaba un objeto encima, pulsar ESPACIO
lo depositaba sin mostrar ninguna pista, generando confusion ("¿esto
suelta o pide ayuda?"). Ahora `A` esta dedicada en exclusiva a pedir la
pista ambiental de pantalla (`action_was_pressed`/`hint_was_pressed`
como flags de flanco independientes, ya no comparten deteccion).
`INSTR3.BMP` y `README.md` actualizados con la tecla nueva.

✅ **Enemigos congelados con cualquier dialogo abierto** — `enemies_update()`
y la comprobacion de colision en `game_loop()` (`timed.c`) envueltas en
`if (!dialog_is_open())`, igual que ya hacia `player_update()` para el
movimiento. Bug real resuelto: antes, leer una pista o hablar con el
chaman no pausaba a los enemigos moviles — podian golpear a Eric
mientras el jugador estaba leyendo texto en pantalla, sin poder
reaccionar.

Retrato: `ERICFRM.BMP`, recortado a su bbox real (42x37) y remapeado a
`eric_palette` (130-185, permanente) para que funcione en cualquier
pantalla sin conflicto de paleta.

**Colores del cuadro dependientes de la época activa**: `dialog_col_bg()`,
`dialog_col_border()`, `dialog_col_dim()` en `dialog.c` calculan el
indice segun `g_game.screen.current_epoch`, en vez de usar una
constante fija. Bug real resuelto: los colores originales usaban
indices del rango compartido de fondos (0-129), que varian segun que
imagen de fondo este activa (en una pantalla concreta el "negro" real
resultaba ser un naranja del cielo del fondo) — corregido apuntando a
tonos permanentes fiables (negro real = mas oscuro de `eric_palette`,
blanco/crema real = mas claro de `cup_palette`). Los casos de Edad
Media/Futuro estan con los mismos valores de Prehistoria como
placeholder, pendientes de ajustar cuando se defina la paleta
permanente de esas épocas.

### HUD

✅ **Pistas contextuales por pantalla** — implementado como
`g_hints[3][9][3]` (2 lineas + NULL por pantalla/época), mostrado via
`DIALOG_HINT` con el retrato de Eric al pulsar `A` sin ninguna
interaccion especifica pendiente ese frame. Las 27 frases de las 3
épocas ya estan escritas. Caso especial: P3 de Edad Media usa un
mensaje alternativo (`g_dialog_med_p3_empty`, "Nada por aqui...") una
vez recogida la taza, en vez del hint generico original que dejaba de
tener sentido ("Parece util... igual lo cojo").

✅ **Indicador de objeto cercano, aunque Eric lleve algo encima** — el
HUD muestra temporalmente el objeto del suelo (icono + nombre en gris)
tapando lo que Eric lleva, mientras este cerca; en cuanto se aleja,
vuelve a mostrar lo que lleva con normalidad. La deteccion se hace
siempre (no solo con manos libres), la recogida real sigue exigiendo
manos libres.

✅ **Fragmentos del artilugio en el HUD** — 3 iconos de 20x20 (ampliado
desde el indicador de 8x8 original), remapeados a dos paletas
permanentes ya existentes sin gastar presupuesto nuevo: estado "no
recogido" en tonos neutros de `eric_palette`, estado "recogido" en
tonos calidos de `cup_palette`.

⬜ Dejar el HUD en su forma definitiva una vez cerrado el resto del arte.

### Paleta VGA de Edad Media y Futuro

✅ **Aplicado a P1-P3 de ambas épocas** (las unicas alcanzables en esta
demo, ver 5.3) — mismo principio que Prehistoria: paleta compartida
entre los fondos de cada época, calculada a partir de **las pantallas
de esa época a la vez** (K-means conjunto sobre los pixeles combinados
de P1+P2+P3), no pantalla a pantalla ni remapeando una contra otra en
cadena — asi un color exclusivo de una sola pantalla (p.ej. el laser
rosa de Futuro P3, ausente en P1/P2) tiene su hueco garantizado desde
el reparto inicial en vez de arriesgarse a que el algoritmo lo
aproxime mal por no conocerlo de antemano.

⬜ P4-P9 de Edad Media y Futuro: fuera del alcance de esta demo (ver
limite real en 5.3), sin trabajar. Retomar el mismo proceso (seccion
9.3 como plantilla) si se amplia el alcance en una version posterior.

### Menu, instrucciones y flujo de partida

✅ **Menu principal** (`show_menu()`, `timed.c`) — pantalla estatica
tras la intro, seleccion directa por numero (1-4, sin cursor):
1 Instrucciones, 2 Cargar partida, 3 Partida nueva, 4 Salir al DOS.
Dos bitmaps (`MENUS.BMP` con partida guardada disponible, `MENUN.BMP`
con la opcion 2 atenuada en gris) segun `save_exists()`. Reutiliza el
mismo patron de dibujado que `show_presentation()`
(`vga_clear_screen`+`set_palette`+`draw_bitmap` centrado+`vga_fade_in`)
en vez del patron de doble buffer del resto del juego. Bug real
resuelto: `set_palette_silent()` no aplica la paleta al hardware por
si sola (solo actualiza la copia en memoria que usa `vga_fade_in()`) —
sin el fade posterior, el bitmap se veia con colores completamente
descuadrados.

✅ **Viaje entre épocas por seleccion directa** — sustituido el ciclo
con una sola tecla por 1/2/3 = Prehistoria/Edad Media/Futuro
directamente, sin viajar si ya se esta en esa época. Entrada siempre
por el lado izquierdo de la pantalla de destino (`x=4` fijo), no se
hereda la `x` de la época anterior (podia no caer sobre ninguna
plataforma valida del nuevo escenario).

✅ **Memoria de pantalla por época** — al volver a una época ya
visitada, Eric reaparece en la ultima pantalla real en la que estuvo
alli (`s_epoch_last_screen[EPOCH_COUNT]`), no siempre en P1.

✅ **Confirmacion de salida con ESC** — `DIALOG_YESNO` antes de volver
al menu principal. **No** es un cierre completo del programa: no llama
a `sound_shutdown()`/`engine_shutdown()` (eso solo ocurre al elegir
"Salir al DOS" desde el menu, o al cerrar `main()` con normalidad) —
simplemente pone `g_state = STATE_TITLE` y el bucle de `main()` vuelve
a mostrar `show_menu()`. Bug real resuelto: `dialog_got_yes()` no
consumia su propio flag al leerlo, dejando una respuesta "Si" fantasma
que podia disparar una salida inmediata en la siguiente partida sin que
el jugador tocara ESC — corregido para que `dialog_got_yes()` borre el
flag en el mismo momento en que se lee, no en `dialog_close()` (eso
rompia la confirmacion en curso, ver leccion en Backlog).

✅ **Pantalla de instrucciones** (`show_instructions()`) — 4 paginas
encadenadas, avance por ESPACIO: 1) Sinopsis, 2) Mecanica, 3) Controles
(teclado ilustrado, con `A` resaltada por separado de ESPACIO), 4) HUD
(captura real con llamadas a vidas/objeto/fragmentos).

✅ **Pantalla de game over** (`show_game_over()`, `GAMEOVER.BMP`) — se
muestra cuando `g_state == STATE_GAMEOVER` (vidas a 0), nunca al salir
voluntariamente con ESC (eso es `STATE_TITLE`, un estado distinto:
reutilizar `STATE_GAMEOVER` para ambos casos era la causa de un bug
real donde salir con ESC mostraba el aviso de derrota y luego, al
elegir partida nueva desde el menu, la volvia a mostrar automaticamente
sin que el jugador hiciera nada). El bucle de `main()` es ahora un
`do...while` que vuelve a `show_menu()` tras cada partida, terminada
por derrota o por salida voluntaria, sin cerrar el programa.

✅ **Puerta trasera de vidas infinitas** (tecla `7` en el menu,
`g_debug_infinite_lives`) — decision consciente: se deja en el build de
entrega. Las bases del concurso no prohiben explicitamente atajos de
desarrollador visibles en el codigo fuente publicado.

✅ **Bug de teclas compartidas entre menu y juego resuelto** —
`show_menu()` esperaba a que se soltara la tecla elegida (`1`/`2`/`3`/
`4`/`7`) antes de devolver el control: sin esto, la misma tecla fisica
podia seguir "pulsada" en el primer frame de la partida siguiente y
disparar una accion del juego que comparte esa tecla (p.ej. `2`/`3`
tambien viajan de época).

### Checklist para cerrar la fase de Prehistoria

1. ✅ **Probar puzzle taza/miel/oso** — flujo completo: recoger taza en P1
   (temporal), depositar en P2, golpear colmena con palo, recoger taza
   con miel, depositar en P3, oso se va a la izquierda, Eric puede bajar
   a P5.

2. ⬜ **BMPs y sprites pendientes**:
   -✅ Hoguera animada P5 (punto de guardado, 3-4 frames de llamas)
   -✅ Sprite de peces P8
   -✅ Sprite de reptil
   -✅ Sprite de jabali gigante P4
   -✅ Bitmap del fragmento 1 (y fragmentos 2/3 del HUD: diseno de
     astrolabio original partido en 3 cuñas, evitando deliberadamente
     cualquier parecido con IP de terceros)
   -✅ Bitmap del tronco a recoger en P9

3. ✅ **Codigos del monolito P9** — implementado via `DIALOG_HINT` +
   `g_dialog_monolith[]`. Contenido placeholder ("SOL - LUNA -
   ESTRELLA"), pendiente definir los simbolos reales del codigo del
   portal de Futuro y mantener coherencia con la segunda mitad del
   codigo en las ruinas de la torre de esa época.

4. ✅ **Frases de pistas por pantalla** — mostrar via ventana emergente.
   Decidir si se muestran al entrar en la pantalla o solo al pulsar
   accion cerca de algun elemento.

5. ✅ **Indicador de objeto en el suelo** — cuando Eric este sobre un
   objeto recogible, mostrar su icono y nombre en el HUD. Implementar
   en logic_update() comprobando si hay un inv_instance cerca de Eric.

6. ✅ **Lianas** — perfilar visualmente en algunas pantallas (usuario en
   Aseprite). Sin cambios en codigo salvo ajuste de coordenadas de
   transicion.

7. ✅ **Vidas con cabeza de Eric** — sustituir rectangulos del HUD por
   BMP con la cabeza de Eric recortada del spritesheet hi-res.
   Dejar para el HUD definitivo.

8. ✅ **Ventana emergente de dialogo** — ver detalle completo en la
   seccion "Cuadro de dialogo" mas arriba. Cubre los puntos 3, 4 y 8
   de esta lista.

9. ✅ **Prueba puzzle planta** — colocar la planta temporalmente en una
   pantalla accesible de Prehistoria para probar el flujo completo,
   igual que se hizo con la taza.

**Adicionales:**
- ✅ Eliminar gatillo temporal de la roca (TODO TEST en logic.c) y
  sustituir por logica real del tronco/palanca (la animacion de la
  roca rodando ya esta implementada).
- ✅ **SFX coger/depositar objetos** — `SFX_PICKUP`/`SFX_DROP`, canales
  fijos propios (indices 2/3), cargados una vez al arrancar.
- ✅ **SFX viaje entre épocas** — `TRAVEL.WAV` en el slot dinamico
  (indice 1), integrado en `screen_travel()`. Limitacion conocida sin
  resolver: queda un ligero solape/distorsion de audio en el instante
  de la carga de la nueva musica (`music_load_xm()` llama a
  `judas_loadxm()`, funcion cerrada de la libreria sin codigo fuente
  disponible, que no cede tiempo al mezclador durante la lectura del
  XM). Ver seccion de backlog al final del documento.
- ✅ **Ambiente sonoro hoguera P5** — implementado como musica XM en vez
  de SFX (loop nativo del sample, mas limpio que relanzar un WAV a mano).
  `FIREAMB.WAV` grabado normalizado (8-bit mono 22050Hz) y convertido a
  `FIRE.XM` en OpenMPT con loop activado sobre el sample completo.
  `screen_change()`, `screen_travel()`, `new_game()` y `load_game()`
  actualizados para cargar `fire.xm` al entrar en P5 (Prehistoria) y
  `PRETHEME.XM`/`MEDTHEME.XM`/tema de Futuro segun corresponda al salir.
  Bug resuelto: `fire.xm` se creo con los 32 canales por defecto de
  OpenMPT, pisando los 4 canales reservados para SFX (28-31) y silenciando
  `jump.wav` mientras sonaba; corregido reduciendo el modulo a los canales
  minimos necesarios. Probado y funcionando.
- ✅ **Logica punto de guardado P5** — al interactuar con la hoguera,
  mostrar confirmacion (S/N) y llamar a save_game() si el jugador
  confirma.
- ✅ Modificar P3 de prehistoria para que aparezca la liana en la derecha para bajar a P5

## Backlog

Limitaciones conocidas y lecciones tecnicas, sin impacto bloqueante en
la entrega pero utiles para retomar mas adelante:

- **Solape de audio en `music_load_xm()`** — `judas_loadxm()` (funcion
  cerrada de Judas, sin codigo fuente) no cede tiempo al mezclador
  mientras lee el XM, produciendo un ligero corte/distorsion en el
  instante exacto de cambio de musica (viaje entre épocas, cambio de
  pantalla). Sin solucion encontrada desde fuera de la libreria sin
  precargar todo el audio al arrancar.

- **`eric_near(x,y)` poco fiable para zonas de interaccion amplias** —
  usado en varios puntos (chaman, colmena/miel, escaleras bloqueadas de
  Edad Media/Futuro) con coordenadas fijas más `INTERACT_DIST=20` de
  margen. En la practica, varias de estas interacciones costaban
  activarse aunque el margen pareciera generoso sobre el papel — la
  causa mas probable es que `eric_near()` compara contra el **centro**
  del sprite de Eric, mientras que el jugador tiende a posicionarse por
  instinto usando el **borde** visible de su cuerpo, desajustando el
  punto real de activacion en varios pixeles. Solucion aplicada caso a
  caso: sustituir por una comprobacion directa de rango de `x` sobre
  `g_game.player.x + PLAYER_WIDTH/2` (el centro real), en vez de seguir
  ajustando coordenadas de `eric_near()` a ciegas. Si aparecen mas quejas
  de "cuesta activar esto" en pruebas futuras, aplicar el mismo patron
  antes que tocar `INTERACT_DIST` de forma global.

- **Flags de estado de dialogo (`dialog.c`) deben consumirse al leerse,
  no al cerrarse** — `dialog_got_yes()` guardaba la respuesta en un
  flag que `dialog_close()` no limpiaba; una confirmacion de "Si" con
  ESC (salir al menu) podia quedar pegada y disparar la misma accion en
  la siguiente partida sin que el jugador volviera a pulsar nada. El
  primer intento de arreglo (limpiar el flag dentro de `dialog_close()`)
  rompio la confirmacion normal, porque `dialog_close()` se llama en el
  mismo instante en que se confirma "Si" — el flag se borraba antes de
  que `game_loop()` llegara a leerlo. Arreglo correcto: `dialog_got_yes()`
  borra su propio flag al ser leido, no `dialog_close()`. Revisar si
  `s_space_was_pressed`/`s_yn_was_pressed` tienen el mismo riesgo si se
  añaden mas usos de `DIALOG_YESNO` en el futuro.

- **Estado `static` disperso en `screen.c` sin reiniciar entre partidas**
  — `s_epoch_last_screen[]`, el estado de la animacion de la roca
  (`s_rock_rolling` y relacionadas) y el de la miel goteando
  (`s_honey_*`) son variables de sesion que persisten mientras el
  proceso siga vivo, incluidas partidas nuevas dentro de la misma
  ejecucion. Se detecto inestabilidad real (época equivocada al
  empezar, musica que no suena, elementos graficos descolocados) tras
  la secuencia empezar → ESC → partida nueva en la misma ejecucion, sin
  llegar a aislar la causa exacta con certeza antes del cierre de la
  entrega — el comportamiento parecia parcialmente aleatorio segun el
  punto exacto de la sesion anterior en el que se salio. Se proyecto
  una funcion `screen_reset_session_state()` para reiniciar este estado
  disperso desde `new_game()`, pero **quedo sin implementar ni sin
  llamar en ningun sitio** — solo se redactaron sus comentarios de
  cabecera. Primera tarea a retomar si se revisita este bug.
