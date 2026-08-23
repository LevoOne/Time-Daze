# Secuencia de introducción — Time Daze
### Guión estructurado para `show_presentation()` — sustituye al placeholder `intro.bmp`
### v2 — corregido para alinearse con la sección 2 (Historia) del GDD

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
- Música: un único tema de fondo, con el mismo arco que comentamos (más ligero al principio, más tenso en la activación, se asienta en `PRETHEME.XM` en cuanto Eric cae en Prehistoria).
- Tecla de salto global para todo el bloque.

---

## Guión

### Panel 1 — "Reencuentro"
**Visual:** Fachada o vestíbulo del colegio, con una pancarta de aniversario ("20 años de la promoción", o el texto que prefiráis). Eric, ya adulto, entrando entre otros ex-alumnos. En una vitrina de trofeos/fotos de promociones antiguas que se ve de pasada, **un guiño pequeño y discreto**: una foto de clase antigua con un crío revoltoso señalado o con cara de trastada — el único momento "Skool Daze" visual de toda la intro, y de pasada, no protagonista.

**Texto:**
> *"Tres generaciones de su familia pasaron por este colegio. Eric no venía por nostalgia."*

**Duración:** ~180 ticks (2.5s)

---

### Panel 2 — "El encargo"
**Visual:** Primer plano de Eric, pensativo, quizás con el móvil en la mano o mirando al infinito — visualizando/recordando la petición de su abuelo (puede resolverse solo con su expresión, no hace falta dibujar al abuelo).

**Texto:**
> *"— Recupera lo que escondí en la caja fuerte del colegio. No preguntes por qué — le había dicho su abuelo."*

**Duración:** ~220 ticks (3.1s) — hay una cita textual, dale tiempo de lectura.

---

### Panel 3 — "El discurso"
**Visual:** Salón de actos, el director dando un discurso soporífero desde un atril, filas de ex-alumnos adultos con cara de aburrimiento. Eric, al fondo, mirando hacia una puerta lateral.

**Texto:**
> *"Mientras el director se perdía en su décimo agradecimiento, Eric encontró su salida."*

**Duración:** ~180 ticks (2.5s)

---

### Panel 4 — "Los pasillos"
**Visual:** Eric solo, recorriendo pasillos vacíos del colegio — taquillas, vitrinas, silencio. Atmósfera de "conoce este sitio de memoria".

**Texto:**
> *"Los pasillos no habían cambiado tanto como él."*

**Duración:** ~150 ticks (2.1s)

---

### Panel 5 — "La caja fuerte"
**Visual:** Eric frente a una caja fuerte antigua (despacho del director, sala de trofeos, sótano — el sitio que mejor encaje con vuestro arte), manipulándola.

**Texto:**
> *"La combinación seguía siendo la misma de siempre. Algunas cosas, en este colegio, nunca cambian."*

**Duración:** ~180 ticks (2.5s)

---

### Panel 6 — "El artilugio"
**Visual:** La caja abierta, un artefacto extraño y anticuado dentro — el objeto que el abuelo escondió. Eric lo saca, lo examina con curiosidad.

**Texto:**
> *"Ni idea de qué era. Ni idea de por qué su abuelo lo quería de vuelta."*

**Duración:** ~180 ticks (2.5s)

---

### Panel 7 — "El error"
**Visual:** Eric toquetea el artilugio por curiosidad — estalla en un flash de luz. Aquí es donde metéis vuestra paleta VGA plena con más fuerza, como clímax visual de la secuencia.

**Texto:**
> *"Y entonces Eric hizo lo único que no debía hacer: activarlo."*

**Duración:** ~130 ticks (1.9s) — corto, es el golpe de efecto.

---

### Panel 8 — "Atrapado en el tiempo"
**Visual:** El artilugio estallando en tres fragmentos que salen disparados en direcciones distintas a través de un vórtice, arrastrando a Eric con ellos. Puede resolverse en una sola imagen dinámica (los tres fragmentos + Eric siendo absorbido) sin necesidad de partirlo en más paneles.

**Texto:**
> *"Tres fragmentos, tres épocas. Y Eric, atrapado entre medias, sin más opción que ir a buscarlos."*

**Duración:** ~200 ticks (2.9s), después fade a negro y directo al primer frame jugable de Prehistoria — mismo criterio de fusión sin costuras que ya comentamos.

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

Igual que en la versión anterior: un tema con arco simple, tono ligero/cotidiano en los paneles 1-6, gira a tenso en el panel 7 (la activación), y resuelve en `PRETHEME.XM` en cuanto se fusiona con el arranque real de Prehistoria — sin necesidad de componer nada exclusivo para la intro si no queréis.

---

## 5. Nota técnica

Sin cambios respecto a la propuesta anterior: encadenar 8 bloques `load_bmp → fade_in → wait → fade_out → free` idénticos a los que ya usáis para los logos, sin abstracciones nuevas. `draw_string()` para el texto superpuesto, no hace falta el sistema de `dialog.c`.

---

## 6. Si el tiempo aprieta: versión mínima

Los paneles 3-6 (discurso, pasillos, caja fuerte, artilugio) son la parte más "de trámite" — se pueden fusionar en 2 paneles en vez de 4 sin perder nada esencial de la trama (por ejemplo, un panel que combine "se escabulle del discurso" con "encuentra la caja fuerte" en una sola imagen y caption más largo). El panel 1 (el guiño) y el panel 7-8 (la activación y fragmentación) son los que yo no tocaría — son el gancho emocional y el gancho de gameplay, respectivamente.
