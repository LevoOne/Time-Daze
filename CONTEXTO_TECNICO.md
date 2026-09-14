# Time Daze — Contexto técnico acumulado
### Documento de traspaso para continuar en un chat nuevo

---

## 1. Presupuesto de paleta VGA (256 colores) — REGLA CRÍTICA

| Rango | Uso | Tipo |
|---|---|---|
| 0-128 | Fondos (paisaje de cada pantalla) | Por pantalla, hasta 129 colores |
| **129** | **Reservado para el negro del HUD** (`memset` en `hud.c` usa índice 129) | Nunca usarlo para paisaje |
| 130-185 | Eric (56 colores) | Permanente, inyectado en cada `screen_change()`/`screen_travel()` |
| 186-199 | Taza (14 colores) | Permanente |
| 200-219 | Chamán / peces (comparten, nunca coinciden en pantalla) | Dinámico |
| 220-231 | Huevo (12 colores) | Permanente |
| 232-241 | Palo (10 colores) | Permanente |
| 242-255 | Roca / miel / oso-jabalí-reptil / hoguera / agua P8 | Dinámico, nunca coinciden en pantalla entre sí |

**Regla de oro**: cualquier fondo de pantalla **de gameplay real** (`PRE_PX.BMP`, `MED_PX.BMP`...) solo puede usar índices **0-128** para su paisaje real. El índice **129** se reserva siempre para negro puro (HUD). Los rangos 130-255 se pisan automáticamente por Eric/objetos/enemigos sea cual sea el contenido del fondo ahí.

**Esta regla NO aplica a pantallas estáticas aisladas** (logos de `show_presentation()`, menú, instrucciones, paneles de la intro): esas no comparten la paleta con ninguna otra capa dibujada encima (nada de Eric/HUD/enemigos simultáneo), inyectan su propia paleta completa vía `set_palette_silent()` sin que nadie más la toque mientras están en pantalla — pueden usar hasta 256 colores libremente, sin ninguna restricción de rango.

**Para Edad Media**: todas las pantallas de la misma época deben compartir **la misma paleta 0-128 byte a byte** (igual que las 9 de Prehistoria) — si no, cambiar de pantalla dentro de la época provoca parpadeos de color. Flujo de trabajo:
1. Generas el fondo con Gemini/Aseprite sin preocuparte del límite de colores.
2. Lo subes en un `.zip` (los `.bmp` sueltos llegan corruptos/convertidos a jpg si se arrastran directo al chat).
3. Se remapea contra la paleta ya establecida de la época (129 colores + negro en 129), reutilizando siempre la misma paleta base en vez de generar una nueva cada vez.

## 2. Reglas de audio (Judas)

- **Formato objetivo**: WAV, 8-bit mono, 22050Hz, cabecera mínima (`RIFF > fmt > data`, sin metadatos extra que puedan confundir a Judas).
- **Conversión segura**: nunca convertir directo de 32-bit a 8-bit con ffmpeg (bug conocido, produce silencio). Pasar siempre por un intermedio de 16-bit.
- **Comprobar volumen antes de convertir** (`ffmpeg -af volumedetect`) — si el pico está muy por debajo de 0dB, aplicar ganancia antes de reducir a 8 bits, o se pierde la señal.
- **Canales SFX**: Judas da 4 canales de SFX (`SFX_CHANNELS=4`, canales 28-31). Reparto actual:
  - Índice 0: `SFX_JUMP`, siempre cargado.
  - Índice 1: dinámico, se recarga en cada `screen_change()`/`screen_travel()` según la pantalla/acción (roca, oso-jabalí-reptil, salpicadura, viaje entre épocas...).
  - Índice 2: `SFX_PICKUP`.
  - Índice 3: `SFX_DROP`.
- **Música**: formato XM (tracker), vía OpenMPT. Bucles ambientales (`FIRE.XM`) con loop activado en el sample; piezas con principio/fin (`JINGLE.XM`) sin loop.
- **Cuidado con OpenMPT**: los módulos nuevos se crean con 32 canales por defecto — recórtalos a los que realmente uses, o roban los canales de SFX de Judas (bug ya encontrado con `fire.xm`).

## 3. Sistema de diálogo (`dialog.c`)

- Tipos: `DIALOG_HINT` (1-2 líneas, se cierra con un solo ESPACIO — usar para mensajes cortos/pistas), `DIALOG_TALK` (múltiples páginas separadas, cada ESPACIO avanza — reservar solo para conversaciones de verdad multi-paso), `DIALOG_YESNO`.
- Colores del cuadro (`dialog_col_bg()`, `dialog_col_border()`, `dialog_col_dim()`) son funciones dependientes de `g_game.screen.current_epoch`, NO constantes fijas — el índice 0 del rango compartido de fondos varía según la pantalla, así que nunca hay que asumir que un índice bajo es negro/blanco fiable. Actualmente los 3 casos de época devuelven los mismos valores de Prehistoria con `TODO` — hay que rellenarlos cuando se diseñe la paleta permanente de Edad Media/Futuro.
- `g_hints[3][9][3]`: 2 líneas + `NULL` por pantalla/época. Se pasa directo a `dialog_open()`, sin variable intermedia.
- Retrato de Eric: `ERICFRM.BMP`, remapeado a `eric_palette` (130-185), recortado a su bbox real (42×37) para no desbordar el cuadro.

## 4. Workflow de entrega de código — IMPORTANTE

Tras cualquier cambio de código, **entregar siempre el archivo completo modificado para descarga** (no solo describir el cambio en el chat) — esto ya está guardado como instrucción permanente vía `memory_user_edits`, debería aplicar automáticamente en el chat nuevo también.

Antes de editar cualquier archivo, **pedir que se re-suba a Projects primero** — el usuario modifica localmente sin subir siempre al momento, así que la copia en Projects puede estar desactualizada.

## 5. Estado del proyecto (resumen)

- **Alcance recortado a shareware**: Prehistoria completa + Edad Media P1-P3 (hasta la taza), por limitación de tiempo hasta el 30 de septiembre.
- **Prehistoria**: prácticamente cerrada (puzzles, enemigos, audio, diálogos, HUD de fragmentos). Pendiente menor: arte de marco de diálogo (de momento sin marco, solo fondo negro), SFX de viaje entre épocas con un ligero solape de audio sin resolver del todo (aceptado, no se sigue puliendo).
- **Edad Media**: P1 y P2 con fondo generado, en proceso de corrección de paleta compartida. P3 pendiente de generar.
- **Pendiente aparcado**: revertir `inv_place(ITEM_CUP, ...)` de Prehistoria P1 a su ubicación real en Edad Media P3 (chapuza temporal para poder probar el puzzle del oso sin esperar a tener Edad Media lista).
- **Intro (`show_presentation()`)**: guión completo de 8 paneles ya escrito y calibrado a la duración de `JINGLE.XM` (54.7s), pendiente de implementar en código. Ver `INTRO_TIME_DAZE.md` (documento aparte) para el guión panel a panel con las descripciones visuales de cada uno.
- **Barrido de marcadores TEMP/TODO/DEBUG**: pendiente, aparcado varias veces.

## 6. Criterios para generar prompts de Gemini

Al pedir un prompt de generación de imagen (fondo de pantalla, sprite, panel de la intro...), me baso en estas fuentes combinadas, por orden de prioridad:

1. **Fuente de la descripción visual**: `GDD.md` para fondos/pantallas/enemigos del juego; `INTRO_TIME_DAZE.md` para los paneles de la secuencia de introducción. Traduzco la descripción existente a inglés técnico de prompt — no me la invento si ya está escrita ahí.

2. **Restricciones técnicas fijas**:
   - Fondos de pantalla: 320×200, paleta indexada ≤129 colores reales (ver punto 1 de este documento), reservando el índice 129 para negro puro del HUD.
   - Sprites/objetos: tamaño final calculado a partir del hitbox real de colisión (`ENEMY_X_W`/`ENEMY_X_H` en `enemies.h`, o el tamaño de icono del HUD que corresponda), con un factor de escala visual razonable sobre ese hitbox (ejemplo: jabalí, hitbox 24×20 → sprite final 64×64, factor ~2.7x).
   - Fondo magenta sólido (`#FF00FF`) siempre en sprites/objetos recortables, para poder limpiarlo y cuantizarlo después.
   - Pedir explícitamente "same technical style as [asset ya existente]" para mantener coherencia visual entre encargos.
   - Generar a resolución alta (por ejemplo 768×256 o similar) para reducir con calidad después, no directamente al tamaño final.

3. **Lecciones aprendidas de fallos anteriores en esta sesión**:
   - Evitar verbos como "shattered/broken into N pieces" — el modelo tiende a interpretarlo como "N objetos, cada uno partido por la mitad" en vez de "un objeto partido en N trozos". Mejor describir directamente la forma final deseada de cada trozo, sin narrar el proceso de rotura.
   - Si un intento sale mal, identificar qué palabra/concepto concreto pudo confundir al modelo y sustituirla, en vez de repetir el mismo prompt.
   - Especificar objetos/formas concretas y reconocibles (ej. "a circular medallion split into wedges" en vez de "an ancient device") ayuda a que el modelo entienda mejor la composición esperada.

4. **Restricciones de derechos de autor**: comprobar que el diseño pedido no se acerque a IP reconocible de terceros (caso real: el Giratiempo de Harry Potter) — si hay riesgo, señalarlo y proponer una alternativa de diseño original con el mismo espíritu antes de generar nada.

5. **Licencia creativa**: solo cuando el usuario la pide explícitamente (ejemplo: colmillos de mamut en el jabalí "prehistórico") — no añadir interpretación creativa por iniciativa propia si el GDD ya es específico.

6. **Imágenes de referencia subidas por el usuario**: si se adjunta una imagen de referencia de tono/color/estilo (ejemplo: `MED_P1.png` para el tono nocturno de Edad Media), usarla como referencia explícita en el prompt en vez de inventar una paleta nueva, y recomendar adjuntarla junto al prompt de texto en Gemini (no solo describir el estilo por escrito).

**Tras generar cada imagen**: pasa por el mismo pipeline de limpieza ya establecido — detectar y limpiar fondo/fleco (firma `B>G` para fondos magenta, o umbral de blanco/negro para fondos oscuros), separar por componentes conectados si hay varios elementos en una sola hoja, cuantizar a la paleta/rango de índices correspondiente, y verificar agujeros interiores de índice 0 antes de dar el asset por bueno.

---

*Para retomar en un chat nuevo: sube este documento a Projects, y al empezar la conversación menciona en qué punto concreto quieres seguir (por ejemplo "sigamos generando fondos de Edad Media, aplicando la regla de paleta compartida del punto 1").*
