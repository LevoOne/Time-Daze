# Time Daze

**Concurso**: MS-DOS Club 2026 (tema: viajes en el tiempo)
**Deadline**: 30 de septiembre de 2026

---

## Sinopsis

Eric es un estudiante universitario que acude a una reunion de ex-alumnos en su
antiguo colegio. Mientras explora el edificio, descubre una vieja caja fuerte
olvidada. Al abrirla encuentra un extrano artefacto que activa sin querer,
fragmentandolo en tres partes que se dispersan por el tiempo.

Atrapado entre la Prehistoria, la Edad Media y un futuro post-apocaliptico, Eric
debe recuperar los tres fragmentos del artefacto para poder regresar a su epoca.

## Controles

- Flechas izquierda/derecha: mover a Eric
- SPACE: saltar
- ENTER: accion (recoger objeto, hablar, usar mecanismo, usar liana)
- ALT: viajar a la siguiente epoca
- ESC: salir

## Compilar

Requiere OpenWatcom 2.0 y DOS/4GW.

```
wcl386 -l=dos4g tempus.c game.c player.c enemies.c puzzles.c
        screen.c inventory.c save.c hud.c logic.c engine.c judas.lib
```

Para compilar con mensajes de debug:

```
wcl386 -l=dos4g -dDEBUG tempus.c game.c ...
```

## Ejecutar

Requiere DOSBox-X o un PC con MS-DOS. Todos los assets deben estar
en el mismo directorio que el ejecutable.

## Creditos

- Desarrollo: Javier
- Arte: en progreso
- Motor: engine propio sobre OpenWatcom 2.0 + Judas Sound System
- Inspirado en: Skool Daze (ZX Spectrum, 1984)

---

*Time Daze es un homenaje a "Skool Daze", clasico del ZX Spectrum,
del que toma prestado a su protagonista Eric.*
