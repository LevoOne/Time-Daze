# Secuencia de introducción — Time Daze
### Guión estructurado para `show_presentation()` — sustituye al placeholder `intro.bmp`
### v3 — tiempos recalibrados para sincronizar con JINGLE.WAV/XM (54.7s)

---

## 0. El concepto en una frase

El GDD ya deja el guiño a *Skool Daze* por escrito, en negro sobre blanco: Eric es **"heredero espiritual del Eric de Skool Daze"**, un universitario de ~25 años, **tercera generación** de su familia en pasar por el mismo colegio. No es un escolar actual — es un adulto que vuelve a pisar los pasillos donde de crío seguramente se metió en líos parecidos a los del Eric original.

Eso cambia el planteamiento de la intro respecto a la primera versión: **el guiño a Skool Daze no puede ser la trama presente** (Eric ya no va al colegio) — tiene que ser un **destello de nostalgia**, breve y de pasada, mientras la trama real avanza: la reunión de antiguos alumnos, el encargo del abuelo, la caja fuerte, el artilugio que se rompe en tres fragmentos.

---

## 1. Aviso importante antes de producir ningún asset

**No recreéis gráficos reales de Microsphere.** El guiño funciona por *situación y nombre*, no por copiar píxeles ajenos — sigue aplicando igual que antes, aunque ahora el momento de homenaje ocupe mucho menos metraje (un solo panel, no un acto entero).

---

## 2. Estructura general

- **8 paneles**, una sola línea narrativa (ya no dos actos separados).
- Mismo patrón técnico que ya tenéis: `load_bmp → fade_in → wait → fade_out → free`.
- **Música**: `JINGLE.XM` (convertido de `JINGLE.WAV`, 54.7s), sonando de principio a fin de toda la secuencia — desde el logo del concurso hasta el fundido final. Sin loop: es una pieza con principio y fin, se deja sonar una sola vez.
- Tecla de salto global para todo el bloque.
- **Los tiempos de esta versión están recalibrados para que la duración total de la secuencia visual (3 logos + 8 paneles + fundido final) sume ~54.7s**, la duración real del jingle.

---

## Guión

### Panel 1 — "Reencuentro"
**Visual:** Fachada o vestíbulo del colegio, con una pancarta de aniversario ("20 años de la promoción", o el texto que prefiráis). Eric, ya adulto, entrando entre otros ex-alumnos. En una vitrina de trofeos/fotos de promociones antiguas que se ve de pasada, **un guiño pequeño y discreto**: una foto de clase antigua con un crío revoltoso señalado o con cara de trastada — el único momento "Skool Daze" visual de toda la intro, y de pasada, no protagonista.

**Texto:**
> *"Tres generaciones de su familia pasaron por este colegio. Eric no venía por nostalgia."*

**Duración:** ~230 ticks (3.3s)

**Prompt para Gemini:**
```
Create a 320x200 pixel illustration for a DOS-era 2D game intro 
sequence ("Time Daze"), panel 1 of 8. Pixel art style, clear black 
outlines, no gradients, no dithering, warm earthy color palette 
matching the game's established look.

Scene: the entrance/lobby of an old secondary school, decorated with 
a modest anniversary banner (something like "Promocion del 85" on a 
simple cloth banner). Eric, now an adult in his mid-20s, walks in 
among a handful of other adult former classmates dressed casually/
semi-formally, mid-conversation, nostalgic mood. In the background, 
partially visible, a trophy/photo display cabinet on the wall — inside 
it, barely noticeable among the framed class photos, one small old 
photo shows a mischievous-looking kid making a face or caught mid-
prank (a subtle nostalgic easter egg, not the focus of the scene).

Reserve a solid black band across the bottom ~40 pixels of the image 
for a text caption. Within that black band, render this exact text in 
a clean, legible pixel-art font, off-white/cream color, centered:
"Tres generaciones de su familia pasaron por este colegio. Eric no
venia por nostalgia."
(wrap across 2 lines if needed to fit the width, keep it readable at 
this resolution)

Use a MAXIMUM of 256 colors in the image palette (this is a standalone 
screen, no palette-sharing constraint).
Output as a 320x200 BMP file with 256-color indexed palette.
```

---

### Panel 2 — "El encargo"
**Visual:** Primer plano de Eric, pensativo, quizás con el móvil en la mano o mirando al infinito — visualizando/recordando la petición de su abuelo (puede resolverse solo con su expresión, no hace falta dibujar al abuelo).

**Texto:**
> *"— Recupera lo que escondí en la caja fuerte del colegio. No preguntes por qué — le había dicho su abuelo."*

**Duración:** ~280 ticks (4.0s) — hay una cita textual, dale tiempo de lectura.

**Prompt para Gemini:**
```
Create a 320x200 pixel illustration for a DOS-era 2D game intro 
sequence ("Time Daze"), panel 2 of 8. Pixel art style, clear black 
outlines, no gradients, no dithering, warm earthy color palette 
matching the game's established look and the previous intro panel.

Scene: close-up/medium shot of Eric alone, stepped slightly away from 
the reunion crowd, looking thoughtful and distant — remembering a 
conversation. His expression is pensive, maybe looking down at a phone 
in his hand or just staring into the middle distance. No other 
characters visible; this is an internal, quiet moment amid the 
reunion's background noise (a blurred/softened hint of the lobby scene 
behind him is fine).

Reserve a solid black band across the bottom ~48 pixels of the image 
for a text caption (this one is longer, needs 2-3 lines). Within that 
black band, render this exact text in a clean, legible pixel-art font, 
off-white/cream color, centered:
"— Recupera lo que escondi en la caja fuerte del colegio. No preguntes
por que — le habia dicho su abuelo."
(wrap naturally across 2-3 lines to fit the width, keep it readable at 
this resolution)

Use a MAXIMUM of 256 colors in the image palette (standalone screen, 
no palette-sharing constraint).
Output as a 320x200 BMP file with 256-color indexed palette.
```

---

### Panel 3 — "El discurso"
**Visual:** Salón de actos, el director dando un discurso soporífero desde un atril, filas de ex-alumnos adultos con cara de aburrimiento. Eric, al fondo, mirando hacia una puerta lateral.

**Texto:**
> *"Mientras el director se perdía en su décimo agradecimiento, Eric encontró su salida."*

**Duración:** ~230 ticks (3.3s)

**Prompt para Gemini:**
```
Create a 320x200 pixel illustration for a DOS-era 2D game intro 
sequence ("Time Daze"), panel 3 of 8. Pixel art style, clear black 
outlines, no gradients, no dithering, warm earthy color palette 
matching the game's established look and the previous intro panels.

Scene: a school assembly hall, seen from behind the seated rows of 
adult former classmates. The headmaster/director stands at a podium 
at the front, mid-speech, visibly droning on (exaggerated long-winded 
body language is fine). Most of the seated crowd looks politely bored. 
Eric is seated near the back or an aisle, turned slightly, glancing 
toward a side door — clearly about to slip away unnoticed.

Reserve a solid black band across the bottom ~40 pixels of the image 
for a text caption. Within that black band, render this exact text in 
a clean, legible pixel-art font, off-white/cream color, centered:
"Mientras el director se perdia en su decimo agradecimiento, Eric
encontro su salida."
(wrap across 2 lines if needed to fit the width, keep it readable at 
this resolution)

Use a MAXIMUM of 256 colors in the image palette (standalone screen, 
no palette-sharing constraint).
Output as a 320x200 BMP file with 256-color indexed palette.
```

---

### Panel 4 — "Los pasillos"
**Visual:** Eric solo, recorriendo pasillos vacíos del colegio — taquillas, vitrinas, silencio. Atmósfera de "conoce este sitio de memoria".

**Texto:**
> *"Los pasillos no habían cambiado tanto como él."*

**Duración:** ~190 ticks (2.7s)

**Prompt para Gemini:**
```
Create a 320x200 pixel illustration for a DOS-era 2D game intro 
sequence ("Time Daze"), panel 4 of 8. Pixel art style, clear black 
outlines, no gradients, no dithering, warm earthy color palette 
matching the game's established look and the previous intro panels.

Scene: Eric walking alone down an empty school hallway — rows of 
lockers on one side, a trophy display case or bulletin board on the 
other, quiet and still, nobody else around. Soft late-afternoon light 
through a window at the end of the hall. Atmosphere of quiet 
familiarity, like he could walk this route with his eyes closed.

Reserve a solid black band across the bottom ~40 pixels of the image 
for a text caption. Within that black band, render this exact text in 
a clean, legible pixel-art font, off-white/cream color, centered:
"Los pasillos no habian cambiado tanto como el."

Use a MAXIMUM of 256 colors in the image palette (standalone screen, 
no palette-sharing constraint).
Output as a 320x200 BMP file with 256-color indexed palette.
```

---

### Panel 5 — "La caja fuerte"
**Visual:** Eric frente a una caja fuerte antigua (despacho del director, sala de trofeos, sótano — el sitio que mejor encaje con vuestro arte), manipulándola.

**Texto:**
> *"La combinación seguía siendo la misma de siempre. Algunas cosas, en este colegio, nunca cambian."*

**Duración:** ~230 ticks (3.3s)

**Prompt para Gemini:**
```
Create a 320x200 pixel illustration for a DOS-era 2D game intro 
sequence ("Time Daze"), panel 5 of 8. Pixel art style, clear black 
outlines, no gradients, no dithering, warm earthy color palette 
matching the game's established look and the previous intro panels.

Scene: Eric crouched in front of an old-fashioned combination safe 
(dial-style, built into an office wall or a storage room), one hand on 
the dial, focused expression, mid-turn. The room around him (a 
headmaster's office, trophy room, or basement storage — pick whichever 
best matches the school setting) is dim and slightly cluttered with 
old furniture/boxes, lit mostly by a single desk lamp or window light.

Reserve a solid black band across the bottom ~48 pixels of the image 
for a text caption (this one may need 2 lines). Within that black 
band, render this exact text in a clean, legible pixel-art font, 
off-white/cream color, centered:
"La combinacion seguia siendo la misma de siempre. Algunas cosas, en
este colegio, nunca cambian."
(wrap naturally across 2 lines to fit the width, keep it readable at 
this resolution)

Use a MAXIMUM of 256 colors in the image palette (standalone screen, 
no palette-sharing constraint).
Output as a 320x200 BMP file with 256-color indexed palette.
```

---

### Panel 6 — "El artilugio"
**Visual:** La caja abierta, un artefacto extraño y anticuado dentro — el objeto que el abuelo escondió. Eric lo saca, lo examina con curiosidad.

**Texto:**
> *"Ni idea de qué era. Ni idea de por qué su abuelo lo quería de vuelta."*

**Duración:** ~230 ticks (3.3s)

**Prompt para Gemini:**
```
Create a 320x200 pixel illustration for a DOS-era 2D game intro 
sequence ("Time Daze"), panel 6 of 8. Pixel art style, clear black 
outlines, no gradients, no dithering, warm earthy color palette 
matching the game's established look and the previous intro panels.

Scene: close-up on the now-open safe from the previous panel, with an 
old, strange, ornate device resting inside — an antique brass/bronze 
mechanical artifact (astrolabe-like, with visible gears and a small 
crystal or hourglass-like element), clearly out of place among the 
otherwise mundane contents of the safe. Eric's hand reaches in, about 
to lift it out; his face shows curiosity and confusion, not recognition.

Reserve a solid black band across the bottom ~48 pixels of the image 
for a text caption (this one may need 2 lines). Within that black 
band, render this exact text in a clean, legible pixel-art font, 
off-white/cream color, centered:
"Ni idea de que era. Ni idea de por que su abuelo lo queria de vuelta."
(wrap naturally across 2 lines to fit the width, keep it readable at 
this resolution)

Use a MAXIMUM of 256 colors in the image palette (standalone screen, 
no palette-sharing constraint).
Output as a 320x200 BMP file with 256-color indexed palette.
```

---

### Panel 7 — "El error"
**Visual:** Eric toquetea el artilugio por curiosidad — estalla en un flash de luz. Aquí es donde metéis vuestra paleta VGA plena con más fuerza, como clímax visual de la secuencia.

**Texto:**
> *"Y entonces Eric hizo lo único que no debía hacer: activarlo."*

**Duración:** ~170 ticks (2.4s) — corto, es el golpe de efecto.

**Prompt para Gemini:**
```
Create a 320x200 pixel illustration for a DOS-era 2D game intro 
sequence ("Time Daze"), panel 7 of 8 — the dramatic turning point of 
the sequence. Pixel art style, clear black outlines, no dithering.

Unlike the previous panels (which use a warm, muted, slightly dim 
earthy palette), THIS panel should break that mood deliberately: 
Eric, holding the brass artifact from the previous panel, has just 
pressed/turned something on it, and it is erupting in a sudden burst 
of bright, saturated light — vivid golds, whites, and electric blues 
radiating outward from the device, silhouetting Eric's startled 
expression and posture (leaning back, eyes wide) against the flash. 
This should feel like a jarring visual climax compared to the calm 
tone of the earlier panels — full vivid color saturation, strong 
contrast, dynamic light rays.

Reserve a solid black band across the bottom ~40 pixels of the image 
for a text caption. Within that black band, render this exact text in 
a clean, legible pixel-art font, off-white/cream color, centered:
"Y entonces Eric hizo lo único que no debía hacer: activarlo."

Use a MAXIMUM of 256 colors in the image palette (standalone screen, 
no palette-sharing constraint) — use this generous color budget for 
the bright flash effect specifically.
Output as a 320x200 BMP file with 256-color indexed palette.
```

---

### Panel 8 — "Atrapado en el tiempo"
**Visual:** El artilugio estallando en tres fragmentos que salen disparados en direcciones distintas a través de un vórtice, arrastrando a Eric con ellos. Puede resolverse en una sola imagen dinámica (los tres fragmentos + Eric siendo absorbido) sin necesidad de partirlo en más paneles.

**Texto:**
> *"Tres fragmentos, tres épocas. Y Eric, atrapado entre medias, sin más opción que ir a buscarlos."*

**Duración:** ~260 ticks (3.7s), después el **fundido final prolongado** (ver punto 2.1 más abajo) antes de pasar al menú del juego.

**Prompt para Gemini:**
```
Create a 320x200 pixel illustration for a DOS-era 2D game intro 
sequence ("Time Daze"), panel 8 of 8 — the final panel before the 
game begins. Pixel art style, clear black outlines, no dithering.

Scene: a swirling vortex of light and energy fills most of the frame, 
in the same vivid gold/white/electric-blue palette as the previous 
panel's flash. Within the vortex, three distinct glowing fragments of 
the brass artifact spin outward in different directions, each with a 
faint colored tint hinting at a different era (one with a faint mossy 
green tint, one with a cool grey-stone tint, one with a faint neon 
cyan tint). Eric's silhouette is caught in the middle of the vortex, 
arms out, being pulled off-balance, clearly being swept away rather 
than standing on solid ground anymore.

Reserve a solid black band across the bottom ~48 pixels of the image 
for a text caption (this one may need 2 lines). Within that black 
band, render this exact text in a clean, legible pixel-art font, 
off-white/cream color, centered:
"Tres fragmentos, tres epocas. Y Eric, atrapado entre medias, sin mas
opcion que ir a buscarlos."
(wrap naturally across 2 lines to fit the width, keep it readable at 
this resolution)

Use a MAXIMUM of 256 colors in the image palette (standalone screen, 
no palette-sharing constraint).
Output as a 320x200 BMP file with 256-color indexed palette.
```

---

### 2.1 — Fundido final prolongado

A diferencia de las transiciones normales entre paneles (fundido rápido + 1s de hueco), el cierre de la intro necesita un **fundido a negro sostenido de ~819 ticks (11.7s)** antes de mostrar el menú — es lo que hace que la duración total de la secuencia visual cuadre con los 54.7s del jingle, dejando que la música termine de sonar sobre pantalla en negro en vez de cortarse de golpe.

```c
vga_fade_out(16, 4);
vga_clear(0);
vga_flip();
timer_wait(819);   /* deja sonar el resto del jingle sobre negro */
/* -> transicion al menu / new_game() */
```

**Aviso de UX**: 11.7s de pantalla en negro es bastante tiempo si el jugador no sabe que es intencionado. La tecla de salto global (punto 2) cubre este riesgo — quien no quiera esperar, salta directo al menú. Si al probarlo se siente demasiado largo incluso sabiendo que es a propósito, las opciones son: alargar más los paneles (menos negro, secuencia visual más larga) o recortar el propio `JINGLE.WAV` a ~45s antes de convertirlo a XM.

---

## 3. Lista de assets nuevos necesarios

| Archivo | Contenido |
|---|---|
| `INTRO1.BMP` | Vestíbulo del colegio, vitrina con el guiño a Skool Daze |
| `INTRO2.BMP` | Primer plano de Eric, pensativo |
| `INTRO3.BMP` | Salón de actos, discurso del director |
| `INTRO4.BMP` | Pasillos vacíos |
| `INTRO5.BMP` | La caja fuerte |
| `INTRO6.BMP` | El artilugio en las manos de Eric |
| `INTRO7.BMP` | El flash de activación (paleta VGA plena) |
| `INTRO8.BMP` | Fragmentación + vórtice arrastrando a Eric |

8 encargos de arte — ninguno reutilizable esta vez (a diferencia de la versión anterior, ya no hay panel de "Eric cayendo en Prehistoria" que pudiera compartir fondo con P1, porque ese momento ahora ocurre ya dentro del propio panel 8 o justo al fundir con el juego).

---

## 4. Nota de música

`JINGLE.WAV` → convertido a `JINGLE.XM` (mismo proceso que `FIREAMB.WAV`→`FIRE.XM`, pero **sin activar el loop del sample** — esta vez es una pieza con principio y fin, no un bucle ambiental). Suena de principio a fin de toda la secuencia (desde el logo del concurso hasta el fundido final de 11.7s), sin necesidad de que cambie de tema en ningún punto intermedio. `PRETHEME.XM` entra ya directamente al arrancar la partida real en Prehistoria, después del fundido final — no hay solape ni transición que gestionar entre ambos.

---

## 5. Nota técnica

Sin cambios respecto a la propuesta anterior: encadenar 8 bloques `load_bmp → fade_in → wait → fade_out → free` idénticos a los que ya usáis para los logos, sin abstracciones nuevas. `draw_string()` para el texto superpuesto, no hace falta el sistema de `dialog.c`.

---

## 6. Si el tiempo aprieta: versión mínima

Los paneles 3-6 (discurso, pasillos, caja fuerte, artilugio) son la parte más "de trámite" — se pueden fusionar en 2 paneles en vez de 4 sin perder nada esencial de la trama (por ejemplo, un panel que combine "se escabulle del discurso" con "encuentra la caja fuerte" en una sola imagen y caption más largo). El panel 1 (el guiño) y el panel 7-8 (la activación y fragmentación) son los que yo no tocaría — son el gancho emocional y el gancho de gameplay, respectivamente.
