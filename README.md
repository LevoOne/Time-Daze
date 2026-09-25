# Time Daze

**Concurso**: C:\DOS\CONTEST 2 (tema: viajes en el tiempo)
**Plazo**: 30 de septiembre de 2026

---

## Sinopsis

Eric es un estudiante universitario que acude a una reunion de ex-alumnos en su
antiguo colegio. Mientras explora el edificio, descubre una vieja caja fuerte
olvidada. Al abrirla encuentra un extrano artefacto que activa sin querer,
fragmentandolo en tres partes que se dispersan por el tiempo.

Atrapado entre la Prehistoria, la Edad Media y un futuro post-apocaliptico, Eric
debe recuperar los tres fragmentos del artefacto para poder regresar a su epoca.

## Instrucciones

Eric debe recorrer las tres epocas y recuperar los tres fragmentos del
artefacto para completar su viaje de vuelta.

Cada pantalla ofrece una pista contextual: pulsa ESPACIO para obtener pistas.
Estas pistas no resuelven los puzles por si solas, pero orientan sobre donde fijarse o que probar
a continuacion.

La mecanica central del juego es el viaje en el tiempo: un objeto
recogido en una epoca puede ser lo que hace falta para avanzar en
otra. Nada se transforma ni desaparece al viajar
-- si Eric encuentra un obstaculo que no puede superar con lo que lleva
encima, puede merecer la pena volver a una epoca ya visitada, coger algo
alli, y probar de nuevo en la epoca donde se quedo atascado.

Se recomienda guardar la partida con cierta frecuencia, y en particular
al alcanzar cada nueva epoca por primera vez: el progreso (fragmentos
recuperados, objetos en el inventario, puzles ya resueltos) se guarda de
forma conjunta para las tres epocas en un unico fichero de partida ('SAVEGAME.DAT'), asi
que una partida guardada en cualquier momento del recorrido
evita tener que rehacer tramos ya completados si algo se tuerce.

## Controles

- O / P: mover a Eric a la izquierda / derecha
- Q: saltar
- ESPACIO: interactuar (recoger objeto, hablar, usar mecanismo) / pistas en pantalla
- 1 / 2 / 3: viajar directamente a Prehistoria / Edad Media / Futuro
- ESC: salir al DOS

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

## Creditos

- Código: Javier
- Arte: Javier, Noelia, Claude y Gemini
- Testing: Noelia, Paula, Lara, Daniel y Carmen
- Motor: engine propio desarrollado en Watcom C (OpenWatcom 2.0) y librería Judas Sound System
- Inspirado en: Skool Daze (ZX Spectrum, Microsphere 1984), Day of the Tentacle (MS-DOS, LucasArts 1993)

