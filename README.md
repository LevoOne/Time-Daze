# Tempus Fugit

Juego de plataformas para MS-DOS desarrollado en C con OpenWatcom 2.0.
Presentado a la segunda edición del concurso de programacion C:\DOS\CONTEST del MS-DOS Club.

## Descripcion

Tempus Fugit es un juego de plataformas 2D en el que el protagonista
viaja entre tres epocas distintas (prehistoria, edad media y futuro)
para recuperar las piezas de un artefacto temporal. Cada epoca
comparte el mismo mapa geografico pero en momentos distintos de la
historia, y las acciones realizadas en una epoca tienen consecuencias
en las demas.

## Requisitos

- MS-DOS 6.22 o compatible
- PC 486DX66 con 16MB de RAM
- Tarjeta VGA
- Sound Blaster (opcional)
- DOSBox-X para emulacion

## Compilacion

wcl386 -l=dos4g main.c engine.c judas.lib

## Tecnologia

- Compilador: OpenWatcom 2.0
- Grafico: VGA modo 13h (320x200, 256 colores)
- Sonido: Judas Sound System (Sound Blaster, XM/WAV)
- Extensor DOS: DOS/4GW

## Estructura del repositorio

- hito01/ Motor base, subsistemas y pruebas
- juego/  Codigo fuente del juego

## Creditos

- Desarrollo: Javier
- Motor grafico: basado en tutoriales de David Brackeen
- Sonido: Judas Sound System por Cadaver/Yehar
- Musica: (pendiente)
- Efectos: (pendiente)
