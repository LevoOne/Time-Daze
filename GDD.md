# Time Daze - Game Design Document

**Version**: 1.0  
**Fecha**: Abril 2026  
**Autor**: Javier  
**Concurso**: MS-DOS Club 2026 (tema: viajes en el tiempo)  
**Deadline**: 30 de septiembre de 2026  

---

## 1. Concepto

**Time Daze** es un juego de plataformas 2D para MS-DOS en el que el protagonista
viaja entre tres epocas historicas distintas para recuperar los fragmentos de un
artefacto temporal. La mecanica central combina plataformas clasicas de 8 bits con
puzzles basados en el paso del tiempo al estilo Day of the Tentacle: las acciones
realizadas en una epoca tienen consecuencias en las epocas futuras.

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

Enemigos: peces prehistoricos saltando desde el rio con patrones fijos.

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
epocas anteriores por la erosion.

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
distintas alturas. Mas variedad vertical que en epocas anteriores.

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

### 9.1 Continuidad visual entre epocas

Uno de los requisitos artisticos mas importantes del juego es que el jugador
pueda reconocer que esta en el mismo lugar geografico en las tres epocas.
El arte debe transmitir la evolucion del tiempo de forma visual sin necesidad
de texto explicativo.

Recursos graficos para conseguirlo:

- **Siluetas reconocibles**: aunque una construccion este destruida en el Futuro,
  algun arco, muro parcial o contorno debe ser reconocible como la misma
  estructura vista en la Edad Media.
- **Elementos persistentes**: la roca monolitica de Prehistoria se convierte en
  cimiento de la torre medieval, que se convierte en el unico muro en pie del
  Futuro. El jugador la reconoce en las tres epocas.
- **Paleta de colores que evoluciona**: tonos calidos y vivos en Prehistoria,
  grises y ocres en Edad Media, tonos desaturados y frios en Futuro.
- **Detalles narrativos en el fondo**: en el Futuro, entre los escombros, restos
  reconocibles de epocas anteriores: un escudo heraldico partido, piedras con
  inscripciones medievales, restos oxidados de mecanismos.

### 9.2 Elementos a definir por epoca

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
 - Modificar el gráfico / frames del chamán en P1, es ilegible

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
al integrar el arte final de cada epoca.

### Sprites animados pendientes

**Prehistoria P1:**
- **Roca sprite**: la roca redonda que bloquea el acceso al chaman debe ser un
  sprite independiente del BMP de fondo. Al resolver el puzzle del tronco/palanca,
  la roca debe animarse rodando hacia la derecha hasta salir de pantalla, dejando
  libre el paso al chaman.
- **Chaman sprite**: el chaman de P1 debe ser un sprite animado situado sobre la
  plataforma central (dolmen). No se desplaza. Realiza una animacion de baile
  chamanico en bucle (varios frames de movimiento ritual: brazos alzados, giros,
  invocaciones). Se detiene al interactuar con Eric.
  **TODO**: mejorar el sprite del chaman. Al reducirlo a 32x32 pixeles el nivel de
  detalle es insuficiente y el resultado es un amasijo de pixeles poco legible.
  Opciones: regenerar con Gemini a mayor resolucion (48x48 o 64x64) y ajustar
  el codigo de dibujado, o retocar manualmente en Aseprite.

**Prehistoria P5:**
- **Hoguera sprite**: la hoguera del punto de guardado en P5 debe ser un sprite
  animado en lugar de una imagen estatica en el BMP. La animacion simula las llamas
  con varios frames (3-4 frames en bucle). La hoguera se dibuja encima del BMP de
  fondo en su posicion fija.

Implementar una secuencia de intro animada que introduzca al jugador en la historia
antes de que comience el juego. La secuencia debe narrar brevemente:

- La reunion de ex-alumnos en el colegio
- Eric descubriendo la caja fuerte y el artefacto
- La activacion accidental del artefacto y su fragmentacion
- Eric atrapado entre las tres epocas

Formato sugerido: serie de pantallas estaticas de 320x200 con texto superpuesto
y fade entre ellas, al estilo de las intros de juegos DOS de la epoca.
La secuencia debe ser saltable con cualquier tecla.

La intro se insertaria en show_presentation() en tempus.c, despues de los logos
y antes de que comience el juego.