# Time Daze

**Concurso**: C:\DOS\CONTEST 2 (tema: viajes en el tiempo)
**Plazo**: 30 de septiembre de 2026

---

## Nota importante

Por cuestiones de tiempo, la versión del juego que se presenta a la edición del concurso
es una versión demo con funcionalidad limitada. La fase de prehistoria está completamente 
implementada, mientras que el mundo medieval y futuro lo están parcialmente. Se publicarán
versiones sucesivas en este repositorio según vaya implementando nueva funcionalidad.

## Sinopsis

Eric es un estudiante universitario que acude a una reunion de ex-alumnos en su
antiguo colegio. Mientras explora el edificio, descubre una vieja caja fuerte
olvidada. Al abrirla encuentra un extrano artefacto que activa sin querer,
fragmentandolo en tres partes que se dispersan por el tiempo.

Atrapado entre la Prehistoria, la Edad Media y un futuro post-apocaliptico, Eric
debe recuperar los tres fragmentos del artefacto para poder regresar a su epoca.

## Instrucciones

Eric quedo atrapado entre tres épocas al activar por accidente un artefacto que su abuelo le había dicho que no tocara. Recuperar sus tres fragmentos, esparcidos por la Prehistoria, Edad Media y un Futuro distópico, es la unica forma de volver a casa -- y el camino rara vez es tan directo como parece a primera vista.

Si en algún momento una pantalla no da pistas claras sobre que hacer, pulsa A para que Eric comente algo que le llama la atención del sitio donde se enceuntraencuentra. No es la solución, pero suele da pistas para saber qué hacer o mirar.

Cada epoca reta a Eric de una forma distinta. Hay puzles que resolver, objetos que recoger, objetos que mover, tecnología que activar, enemigos a despistas, animales a esquivar, necesidad de objetos que no tenemos, etc.

Pero el verdadero motor del juego es el propio viaje en el tiempo: un objeto recogido en una época no se transforma ni desaparece al saltar a otra, y a menudo es justo lo que hace falta para superar algo que en su época de origen no serviría de nada. Si Eric se queda atascado, casi siempre merece la pena repasar que lleva encima -- y de donde lo trajo.

Esta demo se detiene tras las primeras pantallas de Edad Media y Futuro, con un aviso claro en pantalla al llegar a ese limite; no es un error, es hasta donde llega esta versión, no me ha dado tiepo a más. Conviene guardar la partida con cierta frecuencia, sobre todo nada más pisar una época nueva: todo el progreso -- fragmentos, inventario, puzles resueltos -- se recoge en un único fichero, así que una partida guardada nos a ahorrar mucho tiempo y frustración.

## Controles

- O / P: mover a Eric a la izquierda / derecha
- Q: saltar
- ESPACIO: interactuar (recoger objeto, hablar, usar mecanismo, usar liana) / avanzar dialogo
- A: obtener pista en la pantalla actual
- 1 / 2 / 3: viajar directamente a Prehistoria / Edad Media / Futuro
- ESC: salir (pide confirmacion antes de cerrar)

## Compilar

Requiere OpenWatcom 2.0 y DOS/4GW:

```
wcl386 -l=dos4g -w4 timed.c player.c enemies.c puzzles.c inventory.c
        screen.c save.c hud.c logic.c engine.c game.c dialog.c judas.lib
```

Para compilar con mensajes de debug:

```
wcl386 -l=dos4g -w4 -dDEBUG timed.c player.c enemies.c puzzles.c inventory.c
        screen.c save.c hud.c logic.c engine.c game.c dialog.c judas.lib
```

## Ejecutar

Requiere DOSBox-X o un PC con MS-DOS. Todos los assets deben estar
en el mismo directorio que el ejecutable.

En el repositorio hay un directorio 'RELEASE' donde se incluye ejecutable y assets. Basta ejecutar 'JUGAR.BAT' para lanzar el juego en DOSBox-x.

## Creditos

- Código: Javier
- Arte: Javier, Noelia, Claude y Gemini
- Testing: Noelia, Paula, Lara, Daniel y Carmen
- Motor: engine propio desarrollado en Watcom C (OpenWatcom 2.0) y librería Judas Sound System
- Inspirado en: Skool Daze (ZX Spectrum, Microsphere 1984), Day of the Tentacle (MS-DOS, LucasArts 1993)

