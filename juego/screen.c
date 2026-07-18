/*
 * screen.c - Sistema de pantallas de Tempus Fugit
 *
 * Gestiona:
 *   - Tabla de conexiones entre pantallas
 *   - Tabla de plataformas por pantalla y epoca
 *   - Carga y dibujado del fondo BMP
 *   - Transiciones entre pantallas y epocas (fade)
 *   - Spritesheet de Eric
 *
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include <string.h>
#include "engine.h"
#include "game.h"
#include "screen.h"
#include "player.h"
#include "puzzles.h"

/* Animación del chamán */
#define SHAMAN_FRAMES     4
#define SHAMAN_ANIM_SPEED 12

/* Prototipo interno */
static int try_load_bmp(char *file, BITMAP *b);

/* ----------------------------------------------------------------
 * PALETA DE ERIC
 * Indices 130-219 reservados para los colores de Eric (90 colores).
 * Los fondos de Prehistoria ahora comparten una paleta comun en los
 * indices 0-129 (en vez de hasta 219 cada uno por separado), lo que
 * libero este bloque de 90 indices para dar mucho mas detalle al
 * sprite de Eric. El hueco 220-231 sigue siendo del huevo, sin tocar.
 * Se inyectan en la paleta activa despues de cada cambio de fondo.
 * Formato: R,G,B en rango 0..63 para VGA.
 * ---------------------------------------------------------------- */
static const byte eric_palette[56 * 3] =
{
     4,  1,  1,  /* indice 130 */
     1,  1,  5,  /* indice 131 */
     4,  4,  6,  /* indice 132 */
     6,  4,  2,  /* indice 133 */
     9,  3,  2,  /* indice 134 */
     5,  5,  8,  /* indice 135 */
     5,  6, 14,  /* indice 136 */
    12,  4,  2,  /* indice 137 */
    11,  7,  5,  /* indice 138 */
    14,  7,  4,  /* indice 139 */
     7,  8, 15,  /* indice 140 */
    11,  9, 12,  /* indice 141 */
    15,  9,  6,  /* indice 142 */
    10, 11, 20,  /* indice 143 */
    11, 12, 16,  /* indice 144 */
    20, 10,  6,  /* indice 145 */
    17, 12,  9,  /* indice 146 */
    11, 13, 23,  /* indice 147 */
    15, 14, 16,  /* indice 148 */
    21, 13,  9,  /* indice 149 */
    12, 16, 17,  /* indice 150 */
    13, 15, 26,  /* indice 151 */
    25, 15, 11,  /* indice 152 */
    16, 18, 20,  /* indice 153 */
    20, 18, 17,  /* indice 154 */
    16, 18, 27,  /* indice 155 */
    25, 17, 12,  /* indice 156 */
    17, 20, 22,  /* indice 157 */
    23, 19, 16,  /* indice 158 */
    18, 20, 29,  /* indice 159 */
    31, 18, 11,  /* indice 160 */
    18, 21, 32,  /* indice 161 */
    21, 22, 22,  /* indice 162 */
    24, 21, 20,  /* indice 163 */
    21, 23, 24,  /* indice 164 */
    29, 22, 15,  /* indice 165 */
    34, 21, 16,  /* indice 166 */
    22, 26, 26,  /* indice 167 */
    32, 25, 18,  /* indice 168 */
    24, 28, 29,  /* indice 169 */
    40, 25, 18,  /* indice 170 */
    26, 30, 31,  /* indice 171 */
    33, 29, 25,  /* indice 172 */
    38, 29, 21,  /* indice 173 */
    31, 32, 32,  /* indice 174 */
    44, 29, 23,  /* indice 175 */
    41, 34, 22,  /* indice 176 */
    42, 35, 26,  /* indice 177 */
    41, 37, 33,  /* indice 178 */
    50, 34, 26,  /* indice 179 */
    45, 38, 30,  /* indice 180 */
    54, 38, 30,  /* indice 181 */
    47, 41, 38,  /* indice 182 */
    60, 42, 32,  /* indice 183 */
    52, 46, 44,  /* indice 184 */
    61, 45, 37,  /* indice 185 */
};

/* ----------------------------------------------------------------
 * PALETA DE LA TAZA (todas las pantallas de Prehistoria)
 * Indices 186-199: rango DEDICADO que no se solapa con roca/agua/
 * oso/miel (todos en 242-255). La taza aparece en el HUD en todas
 * las pantallas, por eso necesita su propio rango permanente.
 * Colores madera/marron extraidos de CUP.BMP y CUPHONEY.BMP.
 * ---------------------------------------------------------------- */
static const byte cup_palette[14 * 3] =
{
    13,  8,  3,  /* indice 186 */
     8,  9, 11,  /* indice 187 */
    22, 13,  5,  /* indice 188 */
    15, 15, 18,  /* indice 189 */
    26, 16,  6,  /* indice 190 */
    28, 20, 10,  /* indice 191 */
    22, 22, 21,  /* indice 192 */
    33, 24, 14,  /* indice 193 */
    28, 29, 29,  /* indice 194 */
    41, 30, 17,  /* indice 195 */
    47, 36, 18,  /* indice 196 */
    37, 38, 38,  /* indice 197 */
    50, 47, 35,  /* indice 198 */
    54, 52, 49,  /* indice 199 */
};

/* Inyecta los colores de Eric en la paleta VGA activa */
static void screen_inject_eric_palette(void)
{
    int i;
    outp(0x3C8, 130);
    for (i = 0; i < 56 * 3; i++)
        outp(0x3C9, eric_palette[i]);
}

/* Inyecta los colores de la taza en la paleta VGA activa */
void screen_inject_cup_palette(void)
{
    int i;
    outp(0x3C8, 186);
    for (i = 0; i < 14 * 3; i++)
        outp(0x3C9, cup_palette[i]);
}

/* ----------------------------------------------------------------
 * PALETA DEL HUEVO DE DINOSAURIO
 * Indices 220-231 reservados para los colores del huevo.
 * Se inyectan en la paleta activa antes de dibujar el huevo.
 * ---------------------------------------------------------------- */
static const byte egg_palette[12 * 3] =
{
    16, 14, 15,  /* indice 220 */
    17, 16, 15,  /* indice 221 */
    15, 14, 16,  /* indice 222 */
    17, 15, 17,  /* indice 223 */
    24, 23, 24,  /* indice 224 */
    34, 28, 30,  /* indice 225 */
    31, 32, 31,  /* indice 226 */
    34, 32, 31,  /* indice 227 */
    30, 28, 33,  /* indice 228 */
    35, 30, 35,  /* indice 229 */
    52, 30, 37,  /* indice 230 */
    51, 43, 46,  /* indice 231 */
};

/* Inyecta los colores del huevo en la paleta VGA activa */
static void screen_inject_egg_palette(void)
{
    int i;
    outp(0x3C8, 220);
    for (i = 0; i < 12 * 3; i++)
        outp(0x3C9, egg_palette[i]);
}

/* ----------------------------------------------------------------
 * PALETA DEL CHAMAN (P1, Prehistoria)
 * Indices 200-219 reservados para los colores del chaman.
 * Estos indices quedaron libres al reducir eric_palette de 90 a 70
 * colores (130-199). El chaman y la roca son simultaneos en P1
 * (antes y despues de resolver el puzzle), por lo que NO pueden
 * compartir el rango 242-255 que usa la roca. 200-219 es el hueco
 * correcto: no choca ni con Eric (130-199) ni con huevo (220-231)
 * ni con palo (232-241) ni con roca/agua (242-255).
 * Sprite: SHAMAN.BMP, 256x64, 4 frames de 64x64.
 * Colores extraidos del BMP real (cuantizacion ponderada, 20 tonos
 * tierra/marron/piel, ordenados de mas oscuro a mas claro).
 * ---------------------------------------------------------------- */
static const byte shaman_palette[20 * 3] =
{
     5,  1,  0,  /* indice 200 */
     4,  4,  4,  /* indice 201 */
    10,  4,  2,  /* indice 202 */
    13,  7,  5,  /* indice 203 */
    16, 10,  8,  /* indice 204 */
    22, 10,  6,  /* indice 205 */
    18, 15, 12,  /* indice 206 */
    25, 14, 10,  /* indice 207 */
    23, 17, 14,  /* indice 208 */
    30, 16,  8,  /* indice 209 */
    29, 20, 14,  /* indice 210 */
    36, 21, 15,  /* indice 211 */
    31, 25, 20,  /* indice 212 */
    40, 24, 10,  /* indice 213 */
    40, 28, 20,  /* indice 214 */
    45, 32, 20,  /* indice 215 */
    50, 33, 22,  /* indice 216 */
    42, 37, 30,  /* indice 217 */
    51, 35, 26,  /* indice 218 */
    53, 49, 40,  /* indice 219 */
};

/* Inyecta los colores del chaman en la paleta VGA activa.
 * Se llama desde screen_draw_shaman() antes de dibujar. */
static void screen_inject_shaman_palette(void)
{
    int i;
    outp(0x3C8, 200);
    for (i = 0; i < 20 * 3; i++)
        outp(0x3C9, shaman_palette[i]);
}

/* ----------------------------------------------------------------
 * PALETA DEL PALO (P3, Prehistoria)
 * Indices 232-241 reservados para los colores del palo.
 * ANTES el palo no tenia paleta propia: se dibujaba con sus indices
 * de pixel crudos apoyandose en lo que tuviera la paleta de fondo
 * activa en ese momento (igual que Eric/huevo se dibujan, pero SIN
 * el palette_inject que ellos si tienen). Mientras cada pantalla de
 * Prehistoria tenia su propia paleta dedicada, coincidia mas o menos
 * por casualidad. Al pasar P3 a la paleta compartida de 130 colores,
 * esos mismos indices ya apuntaban a otros colores -> el palo se veia
 * con la paleta equivocada. Solucionado: STICK.BMP real subido y
 * cuantizado a sus 10 colores reales en 232-241.
 *
 * Valores extraidos directamente de STICK.BMP real (cuantizacion
 * ponderada por frecuencia de pixel de sus colores reales, ordenados
 * de mas oscuro a mas claro). STICK.BMP ya viene regenerado para que
 * sus indices de pixel apunten a 232-241 en vez de a los indices
 * viejos de fondo.
 * ---------------------------------------------------------------- */
static const byte stick_palette[10 * 3] =
{
     9,  3,  0,  /* indice 232: contorno (marron muy oscuro) */
    14,  7,  3,  /* indice 233: sombra oscura rojiza */
    13,  9,  7,  /* indice 234: sombra oscura neutra */
    18,  8,  2,  /* indice 235: marron oscuro rojizo */
    18, 12,  7,  /* indice 236: marron medio-oscuro */
    25, 14,  6,  /* indice 237: marron medio */
    28, 19, 13,  /* indice 238: marron */
    35, 21, 11,  /* indice 239: marron claro calido */
    35, 23, 15,  /* indice 240: marron claro */
    39, 27, 19,  /* indice 241: tierra clara (brillo) */
};

/* Inyecta los colores del palo en la paleta VGA activa */
static void screen_inject_stick_palette(void)
{
    int i;
    outp(0x3C8, 232);
    for (i = 0; i < 10 * 3; i++)
        outp(0x3C9, stick_palette[i]);
}

/* ----------------------------------------------------------------
 * PALETA DE LA ROCA (P1, Prehistoria)
 * Indices 242-255 reservados para los colores de la roca.
 * Mismo problema que tenia el palo: ROCK.BMP nunca tuvo paleta
 * propia, se dibujaba con sus indices de pixel crudos apoyandose
 * en la paleta de fondo activa. Al pasar a la paleta compartida de
 * 130 colores esos indices ya no coinciden -> la roca se ve con
 * colores equivocados.
 *
 * OJO: 242-255 es el mismo rango libre que PRE_P8.BMP usa para sus
 * propios tonos de agua (ver RESUMEN_PALETA_ERIC.md). No hay
 * conflicto porque la roca solo se dibuja en P1 y el agua solo
 * existe en P8 (nunca estan activas a la vez), pero por eso esta
 * paleta NO se inyecta en screen_change/screen_travel como las
 * demas (eso pisaria los colores del agua al cambiar de pantalla
 * hacia cualquier lado). En vez de eso se inyecta solo dentro de
 * screen_draw_rock(), que ya esta condicionado a P1 con el puzzle
 * de la palanca sin resolver.
 * Valores extraidos directamente de ROCK.BMP real (cuantizacion
 * ponderada por frecuencia de pixel, ordenados de mas oscuro a
 * mas claro). ROCK.BMP ya viene regenerado para que sus indices de
 * pixel apunten a 242-255.
 * ---------------------------------------------------------------- */
static const byte rock_palette[14 * 3] =
{
     6,  1,  0,  /* indice 242 */
    10,  6,  5,  /* indice 243 */
    13,  7,  5,  /* indice 244 */
    11,  9,  7,  /* indice 245 */
    20,  8,  2,  /* indice 246 */
    14, 10,  9,  /* indice 247 */
    16, 13, 12,  /* indice 248 */
    22, 12,  8,  /* indice 249 */
    21, 15, 13,  /* indice 250 */
    29, 13,  6,  /* indice 251 */
    27, 19, 15,  /* indice 252 */
    30, 23, 19,  /* indice 253 */
    42, 22, 11,  /* indice 254 */
    33, 26, 22,  /* indice 255 */
};

/* Inyecta los colores de la roca en la paleta VGA activa.
 * Solo se llama desde screen_draw_rock(), nunca en cambios de
 * pantalla generales, para no pisar los colores del agua de P8
 * que tambien viven en el rango 242-255 (ver comentario arriba). */
static void screen_inject_rock_palette(void)
{
    int i;
    outp(0x3C8, 242);
    for (i = 0; i < 14 * 3; i++)
        outp(0x3C9, rock_palette[i]);
}

/* ----------------------------------------------------------------
/* ----------------------------------------------------------------
 * PALETA DE LA MIEL (P2, Prehistoria)
 * Indices 242-249 — mismo rango libre que roca/agua/oso.
 * Sin conflicto: miel solo en P2, roca en P1, agua en P8, oso en P3.
 * 8 colores amber/dorado extraidos de HONEY.BMP.
 * ---------------------------------------------------------------- */
/* ----------------------------------------------------------------
 * PALETA DEL OSO (P3, Prehistoria)
 * Indices 242-255 — mismo rango libre que roca/agua/miel.
 * Sin conflicto: oso solo en P3, roca en P1, agua en P8, miel en P2.
 * 14 colores marron/tierra extraidos de BEAR.BMP.
 * ---------------------------------------------------------------- */
static const byte bear_palette[14 * 3] =
{
     3,  0,  0,  /* indice 242 */
     7,  3,  4,  /* indice 243 */
     8,  4,  4,  /* indice 244 */
     9,  5,  5,  /* indice 245 */
    10,  6,  6,  /* indice 246 */
    12,  8,  6,  /* indice 247 */
    14,  9,  8,  /* indice 248 */
    15, 10,  8,  /* indice 249 */
    16, 10,  8,  /* indice 250 */
    17, 12, 10,  /* indice 251 */
    18, 12, 10,  /* indice 252 */
    20, 14, 12,  /* indice 253 */
    22, 14, 12,  /* indice 254 */
    24, 16, 13,  /* indice 255 */
};

void screen_inject_bear_palette(void)
{
    int i;
    outp(0x3C8, 242);
    for (i = 0; i < 14 * 3; i++)
        outp(0x3C9, bear_palette[i]);
}

static const byte honey_palette[8 * 3] =
{
    17, 10, 15,  /* indice 242 */
    25, 14, 12,  /* indice 243 */
    34, 27, 12,  /* indice 244 */
    55, 28,  9,  /* indice 245 */
    55, 28,  9,  /* indice 246 */
    54, 40, 25,  /* indice 247 */
    59, 48, 38,  /* indice 248 */
    62, 60, 13,  /* indice 249 */
};

static void screen_inject_honey_palette(void)
{
    int i;
    outp(0x3C8, 242);
    for (i = 0; i < 8 * 3; i++)
        outp(0x3C9, honey_palette[i]);
}

/* ----------------------------------------------------------------
 * ANIMACION MIEL GOTEANDO (P2, Prehistoria)
 * Activada cuando Eric golpea la colmena con el palo.
 * Desactivada cuando Eric recoge la miel con la taza.
 *
 * Ciclo:
 *   Frames 0-1: gota formandose en x=175, y=97 (alternando)
 *   Frame 2:    gota cayendo, y va de 97 a 128
 *   Frame 3:    salpicadura en y=128, luego reinicia ciclo
 *
 * HONEY_DRIP_X    = 175  (181 centro - 6 semiancho)
 * HONEY_DRIP_TOP  = 97   (y donde empieza la gota)
 * HONEY_DRIP_BOT  = 128  (148 - 20 alto sprite = y donde cae)
 * HONEY_FRAME_SPD = ticks por fase de animacion
 * ---------------------------------------------------------------- */
#define HONEY_DRIP_X    180
#define HONEY_DRIP_TOP   97
#define HONEY_DRIP_BOT  133
#define HONEY_FRAME_SPD   20

static int s_honey_active = 0;
static int s_honey_phase  = 0;  /* 0=form0, 1=form1, 2=caida, 3=splash */
static int s_honey_timer  = 0;
static int s_honey_y      = 0;

void screen_trigger_honey_drip(void)
{
    sfx_free(SFX_BEAR_STEP);
    sfx_load(SFX_BEAR_STEP, "drops.wav");
    s_honey_active = 1;
    s_honey_phase  = 0;
    s_honey_timer  = 0;
    s_honey_y      = HONEY_DRIP_TOP;
}

void screen_stop_honey_drip(void)
{
    sfx_free(SFX_BEAR_STEP);
    sfx_load(SFX_BEAR_STEP, "woso.wav");
    s_honey_active = 0;
}

void screen_draw_honey_drip(void)
{
    if (!s_honey_active)                                    return;
    if (g_game.screen.current_epoch  != EPOCH_PREHISTORY)  return;
    if (g_game.screen.current_screen != 1)                 return;
    if (g_honey_sprite.data == NULL)                        return;

    screen_inject_honey_palette();

    switch (s_honey_phase)
    {
        case 0:  /* gota formandose frame 0 */
            bmp_draw_tile(&g_honey_sprite, 0, 0, 12, 20, HONEY_DRIP_X, HONEY_DRIP_TOP);
            if (++s_honey_timer >= HONEY_FRAME_SPD)
            {
                s_honey_timer = 0;
                s_honey_phase = 1;
            }
            break;

        case 1:  /* gota formandose frame 1 */
            bmp_draw_tile(&g_honey_sprite, 1, 0, 12, 20, HONEY_DRIP_X, HONEY_DRIP_TOP);
            if (++s_honey_timer >= HONEY_FRAME_SPD)
            {
                s_honey_timer = 0;
                s_honey_phase = 2;
                s_honey_y     = HONEY_DRIP_TOP;
            }
            break;

        case 2:  /* gota cayendo */
            bmp_draw_tile(&g_honey_sprite, 2, 0, 12, 20, HONEY_DRIP_X+3, s_honey_y);
            if (++s_honey_timer >= 4)
            {
                s_honey_timer = 0;
                s_honey_y++;
                if (s_honey_y >= HONEY_DRIP_BOT)
                {
                    s_honey_phase = 3;
                    s_honey_timer = 0;
                }
            }
            break;

        case 3:  /* salpicadura */
            if (s_honey_timer == 0)
                sfx_play(SFX_BEAR_STEP, 56, MIDDLE);
            bmp_draw_tile(&g_honey_sprite, 3, 0, 12, 20, HONEY_DRIP_X+1, HONEY_DRIP_BOT+3);
            if (++s_honey_timer >= HONEY_FRAME_SPD * 2)
            {
                s_honey_timer = 0;
                s_honey_phase = 0;  /* reinicia ciclo */
            }
            break;
    }
}

/* ----------------------------------------------------------------
 * VARIABLES GLOBALES
 * ---------------------------------------------------------------- */
ScreenData g_screen_data;
BITMAP     g_spritesheet;
BITMAP     g_rock_sprite;
BITMAP     g_rock_roll_sprite;
BITMAP     g_shaman_sprite;
BITMAP     g_stick_sprite;
BITMAP     g_egg_sprite;
BITMAP     g_cup_sprite;
BITMAP     g_cuphoney_sprite;
BITMAP     g_egg_icon;
BITMAP     g_honey_sprite;
BITMAP     g_bear_sprite;
BITMAP      g_cup_icon;
BITMAP      g_cuphoney_icon;
BITMAP     g_eric_head;

/* Fondos BMP: uno por pantalla y epoca (27 en total)      */
/* Se cargan bajo demanda y se cachean en memoria          */
static BITMAP g_backgrounds[EPOCH_COUNT][SCREEN_COUNT];
static int    g_bg_loaded[EPOCH_COUNT][SCREEN_COUNT];

/* ----------------------------------------------------------------
 * NOMBRES DE FICHEROS DE FONDO
 * Formato: PRE_P1.BMP, MED_P1.BMP, FUT_P1.BMP
 * ---------------------------------------------------------------- */
static const char *bg_names[EPOCH_COUNT][SCREEN_COUNT] =
{
    /* PREHISTORIA */
    {
        "PRE_P1.BMP", "PRE_P2.BMP", "PRE_P3.BMP",
        "PRE_P4.BMP", "PRE_P5.BMP", "PRE_P6.BMP",
        "PRE_P7.BMP", "PRE_P8.BMP", "PRE_P9.BMP"
    },
    /* EDAD MEDIA */
    {
        "MED_P1.BMP", "MED_P2.BMP", "MED_P3.BMP",
        "MED_P4.BMP", "MED_P5.BMP", "MED_P6.BMP",
        "MED_P7.BMP", "MED_P8.BMP", "MED_P9.BMP"
    },
    /* FUTURO */
    {
        "FUT_P1.BMP", "FUT_P2.BMP", "FUT_P3.BMP",
        "FUT_P4.BMP", "FUT_P5.BMP", "FUT_P6.BMP",
        "FUT_P7.BMP", "FUT_P8.BMP", "FUT_P9.BMP"
    }
};

/* ----------------------------------------------------------------
 * TABLA DE CONEXIONES
 * [epoca][pantalla][direccion]
 * Pantallas indexadas 0..8 (pantalla 1 = indice 0)
 * ---------------------------------------------------------------- */
static const int connections[EPOCH_COUNT][SCREEN_COUNT][DIR_COUNT] =
{
    /* PREHISTORIA */
    {
        /* P1 */ { NO_SCREEN, 1, NO_SCREEN, NO_SCREEN },
        /* P2 */ { 0, 2, NO_SCREEN, NO_SCREEN },
        /* P3 */ { 1, NO_SCREEN, NO_SCREEN, 4 },
        /* P4 */ { NO_SCREEN, 4, NO_SCREEN, 6 },
        /* P5 */ { 3, NO_SCREEN, 2, NO_SCREEN },
        /* P6 */ { NO_SCREEN, 6, NO_SCREEN, NO_SCREEN },
        /* P7 */ { 5, 7, 3, NO_SCREEN },
        /* P8 */ { 6, 8, NO_SCREEN, NO_SCREEN },
        /* P9 */ { 7, NO_SCREEN, NO_SCREEN, NO_SCREEN }
    },
    /* EDAD MEDIA */
    {
        /* P1 */ { NO_SCREEN, 1, NO_SCREEN, NO_SCREEN },
        /* P2 */ { 0, 2, NO_SCREEN, NO_SCREEN },
        /* P3 */ { 1, NO_SCREEN, NO_SCREEN, 4 },
        /* P4 */ { NO_SCREEN, 4, NO_SCREEN, 6 },
        /* P5 */ { 3, NO_SCREEN, 2, NO_SCREEN },
        /* P6 */ { NO_SCREEN, 6, NO_SCREEN, NO_SCREEN },
        /* P7 */ { 5, 7, 3, NO_SCREEN },
        /* P8 */ { 6, 8, NO_SCREEN, NO_SCREEN },
        /* P9 */ { 7, NO_SCREEN, NO_SCREEN, NO_SCREEN }
    },
    /* FUTURO */
    {
        /* P1 */ { NO_SCREEN, 1, NO_SCREEN, NO_SCREEN },
        /* P2 */ { 0, 2, NO_SCREEN, NO_SCREEN },
        /* P3 */ { 1, NO_SCREEN, NO_SCREEN, 4 },
        /* P4 */ { NO_SCREEN, 4, NO_SCREEN, 6 },
        /* P5 */ { 3, NO_SCREEN, 2, NO_SCREEN },
        /* P6 */ { NO_SCREEN, 6, NO_SCREEN, NO_SCREEN },
        /* P7 */ { 5, 7, 3, NO_SCREEN },
        /* P8 */ { 6, 8, NO_SCREEN, NO_SCREEN },
        /* P9 */ { 7, NO_SCREEN, NO_SCREEN, NO_SCREEN }
    }
};

/* ----------------------------------------------------------------
 * TABLAS DE PLATAFORMAS
 * Valores aproximados para pruebas con placeholders.
 * Se ajustaran con el arte final.
 * Fin de lista marcado con w=0.
 * ---------------------------------------------------------------- */
static const Platform platforms_pre[SCREEN_COUNT][MAX_PLATFORMS] =
{
    /* P1: cima, megalitos */
    {
        { 0,   148, 320, 8 },   /* suelo principal */
        { 40,  116,  20, 2 },   /* dolmen izquierdo */
        { 136, 110,  60, 2 },   /* roca plana central, chaman */
        { 0, 0, 0, 0 }
    },
    /* P2: ladera, colmena */
    {
        { 0,   148, 320, 8 },   /* suelo principal */
        { 40,  116,  28, 2 },   /* roca izquierda */
        /*{ 134, 110,  64, 2 },    roca central, colmena */
        { 245, 133,  59, 2 },   /* roca derecha */
        { 0, 0, 0, 0 }
    },
    /* P3: pie colina, oso */
    {
        { 0,   148, 320, 8 },   /* suelo principal */
        { 5,  105, 73, 1 },   /* roca izquierda */
        { 0, 0, 0, 0 }
    },
    /* P4: nivel medio */
    {
        { 0,   148, 320, 8 },   /* suelo principal */
        { 4,  105, 75, 2 },    /* plataforma izquierda */
        { 0, 0, 0, 0 }
    },
    /* P5: hoguera */
    {
        { 0,   148, 320, 8 },   /* suelo principal */
        { 0, 0, 0, 0 }
    },
    /* P6: zona baja izquierda */
    {
        { 0,   148, 320, 8 },   /* suelo principal */
        { 0, 112, 33, 2 },   /* plataforma izquierda */
        { 0, 0, 0, 0 }
    },
    /* P7: zona baja centro */
    {
         { 0, 148, 320, 8 },   /* suelo principal */
        { 0, 87, 35, 2 },   /* plataforma izquierda */
        { 90, 101, 54, 2 },   /* plataforma derecha */
        { 0, 0, 0, 0 }
    },
    /* P8: cruce del rio */
    {
        { 0, 87, 36, 8 },     /* plataforma izquierda */
        { 39, 101, 41, 8 },   /* 2da plataforma  izquierda */
        { 0,   148, 60, 8 },    /* orilla izquierda */
        { 231, 148, 89, 8 },    /* orilla derecha */
        { 102,  112, 10, 2 },   /* roca 1 */
        { 136, 107, 5, 2 },     /* roca 2 */
        { 176, 100, 5, 2 },     /* roca 3 */
        { 206, 105, 8, 2 },     /* roca 4 */
        { 0, 0, 0, 0 }
    },
    /* P9: monolito */
    {
        { 0,   148, 320, 8 },   /* suelo principal */
        { 234, 120, 51, 2 },   /* tronco caido */
        { 0, 0, 0, 0 }
    }
};

static const Platform platforms_med[SCREEN_COUNT][MAX_PLATFORMS] =
{
    /* P1: megalitos cubiertos de musgo */
    {
        { 0,   175, 320, 8 },
        { 40,  150,  60, 8 },
        { 180, 140,  80, 8 },
        { 260, 155,  50, 8 },
        { 0, 0, 0, 0 }
    },
    /* P2: ladera con sendero */
    {
        { 0,   175, 320, 8 },
        { 20,  158,  50, 8 },
        { 100, 140,  60, 8 },
        { 200, 120,  70, 8 },
        { 0, 0, 0, 0 }
    },
    /* P3: casa con taza */
    {
        { 0,   175, 320, 8 },
        { 60,  155,  40, 8 },
        { 0, 0, 0, 0 }
    },
    /* P4: herreria, escalera */
    {
        { 0,   175, 320, 8 },
        { 50,  148,  60, 8 },
        { 180, 152,  70, 8 },
        { 0, 0, 0, 0 }
    },
    /* P5: capilla */
    {
        { 0,   175, 320, 8 },
        { 0, 0, 0, 0 }
    },
    /* P6: orilla izquierda del foso */
    {
        { 0,   175, 320, 8 },
        { 30,  155,  40, 8 },
        { 0, 0, 0, 0 }
    },
    /* P7: foso centro izquierda */
    {
        { 0,   175, 320, 8 },
        { 40,  152,  50, 8 },
        { 180, 148,  60, 8 },
        { 0, 0, 0, 0 }
    },
    /* P8: puente levadizo */
    {
        { 0,   175, 140, 8 },
        { 180, 175, 140, 8 },
        { 60,  155,  40, 8 },
        { 0, 0, 0, 0 }
    },
    /* P9: torre */
    {
        { 0,   175, 320, 8 },
        { 100, 148,  50, 8 },
        { 0, 0, 0, 0 }
    }
};

static const Platform platforms_fut[SCREEN_COUNT][MAX_PLATFORMS] =
{
    /* P1: megalitos derruidos */
    {
        { 0,   175, 320, 8 },
        { 40,  158,  40, 8 },
        { 160, 148,  50, 8 },
        { 260, 162,  40, 8 },
        { 0, 0, 0, 0 }
    },
    /* P2: ladera arida */
    {
        { 0,   175, 320, 8 },
        { 30,  158,  40, 8 },
        { 150, 145,  50, 8 },
        { 0, 0, 0, 0 }
    },
    /* P3: empedrado, postes laser */
    {
        { 0,   175, 320, 8 },
        { 60,  158,  40, 8 },
        { 0, 0, 0, 0 }
    },
    /* P4: ruinas industriales */
    {
        { 0,   175, 320, 8 },
        { 40,  145,  50, 8 },
        { 160, 130,  60, 8 },
        { 250, 150,  50, 8 },
        { 0, 0, 0, 0 }
    },
    /* P5: terminal */
    {
        { 0,   175, 320, 8 },
        { 80,  158,  40, 8 },
        { 200, 155,  40, 8 },
        { 0, 0, 0, 0 }
    },
    /* P6: escombros, puerta hermetica */
    {
        { 0,   175, 320, 8 },
        { 40,  155,  50, 8 },
        { 160, 148,  60, 8 },
        { 260, 158,  40, 8 },
        { 0, 0, 0, 0 }
    },
    /* P7: cauce seco, ruinas */
    {
        { 0,   175, 320, 8 },
        { 50,  152,  40, 8 },
        { 180, 148,  60, 8 },
        { 0, 0, 0, 0 }
    },
    /* P8: restos del puente, zona radiacion */
    {
        { 0,   175, 140, 8 },
        { 180, 175, 140, 8 },
        { 80,  158,  50, 8 },
        { 0, 0, 0, 0 }
    },
    /* P9: Mecha, ruinas de la torre */
    {
        { 0,   175, 320, 8 },
        { 80,  148,  60, 8 },
        { 220, 155,  70, 8 },
        { 0, 0, 0, 0 }
    }
};

/* ----------------------------------------------------------------
 * COLORES DE FONDO PLACEHOLDER
 * Mientras no hay BMPs, cada epoca usa un color base distinto
 * ---------------------------------------------------------------- */
static const unsigned char bg_colors[EPOCH_COUNT] =
{
    6,    /* Prehistoria: marron */
    8,    /* Edad Media: gris    */
    1     /* Futuro: azul oscuro */
};

/* ----------------------------------------------------------------
 * INIT
 * ---------------------------------------------------------------- */
void screen_init(void)
{
    int i, j;

    /* Marcar todos los fondos como no cargados */
    for (i = 0; i < EPOCH_COUNT; i++)
        for (j = 0; j < SCREEN_COUNT; j++)
            g_bg_loaded[i][j] = 0;

    /* Inicializar screen_data */
    g_screen_data.platform_count = 0;

    /* Cargar el spritesheet de Eric si existe */
    if (!try_load_bmp("PLAYER.BMP", &g_spritesheet))
        g_spritesheet.data = NULL;

    /* Cargar el sprite de la roca de P1 */
    if (!try_load_bmp("ROCK.BMP", &g_rock_sprite))
        g_rock_sprite.data = NULL;

    if (!try_load_bmp("ROCKROLL.BMP", &g_rock_roll_sprite))
        g_rock_roll_sprite.data = NULL;

    /* Cargar el sprite del chamán de P1 */
    if (!try_load_bmp("SHAMAN.BMP", &g_shaman_sprite))
    g_shaman_sprite.data = NULL;

    /* Cargar el sprite del palo (objeto recogible) */
    if (!try_load_bmp("STICK.BMP", &g_stick_sprite))
        g_stick_sprite.data = NULL;

    /* Cargar el sprite del huevo de dinosaurio (objeto recogible) */
    if (!try_load_bmp("HUEVO.BMP", &g_egg_sprite))
        g_egg_sprite.data = NULL;

    /* Cargar sprites de la taza */
    if (!try_load_bmp("CUP16.BMP", &g_cup_sprite))
        g_cup_sprite.data = NULL;

    if (!try_load_bmp("CUPH16.BMP", &g_cuphoney_sprite))
        g_cuphoney_sprite.data = NULL;

    if (!try_load_bmp("EGGICON.BMP", &g_egg_icon))
        g_egg_icon.data = NULL;

    if (!try_load_bmp("HONEY.BMP", &g_honey_sprite))
        g_honey_sprite.data = NULL;

    if (!try_load_bmp("BEAR.BMP", &g_bear_sprite))
        g_bear_sprite.data = NULL;

    /* Cargar bitmaps de taza de miel */
    if (!try_load_bmp("CUPICON.BMP", &g_cup_icon))
        g_cup_icon.data = NULL;
    if (!try_load_bmp("CUPHICON.BMP", &g_cuphoney_icon))
        g_cuphoney_icon.data = NULL;

    if (!try_load_bmp("HEAD.BMP", &g_eric_head))
        g_eric_head.data = NULL;
}

/* ----------------------------------------------------------------
 * TRY LOAD BMP
 * Intenta cargar un BMP sin terminar el programa si no existe.
 * Devuelve 1 si OK, 0 si el fichero no existe o es invalido.
 * ---------------------------------------------------------------- */
static int try_load_bmp(char *file, BITMAP *b)
{
    FILE *fp;
    long  index;
    word  num_colors;
    int   x;
    int   row_size;
    int   padding;

    fp = fopen(file, "rb");
    if (fp == NULL) return 0;

    if (fgetc(fp) != 'B' || fgetc(fp) != 'M')
    {
        fclose(fp);
        return 0;
    }

    fskip(fp, 16);
    fread(&b->width,  sizeof(word), 1, fp);
    fskip(fp, 2);
    fread(&b->height, sizeof(word), 1, fp);
    fskip(fp, 22);
    fread(&num_colors, sizeof(word), 1, fp);
    fskip(fp, 6);

    if (num_colors == 0) num_colors = 256;

    b->data = (byte *)malloc((word)(b->width * b->height));
    if (b->data == NULL)
    {
        fclose(fp);
        return 0;
    }

    for (index = 0; index < num_colors; index++)
    {
        b->palette[(int)(index * 3 + 2)] = fgetc(fp) >> 2;
        b->palette[(int)(index * 3 + 1)] = fgetc(fp) >> 2;
        b->palette[(int)(index * 3 + 0)] = fgetc(fp) >> 2;
        x = fgetc(fp);
    }

    /* Las filas de un BMP estan alineadas a multiplos de 4 bytes.
     * Si el ancho no es multiplo de 4, hay bytes de padding al
     * final de cada fila que hay que saltar. */
    row_size = ((b->width + 3) / 4) * 4;
    padding  = row_size - b->width;

    for (index = (b->height - 1) * b->width; index >= 0; index -= b->width)
    {
        for (x = 0; x < b->width; x++)
            b->data[(word)(index + x)] = (byte)fgetc(fp);
        for (x = 0; x < padding; x++)
            fgetc(fp);
    }

    fclose(fp);
    return 1;
}

/* ----------------------------------------------------------------
 * SCREEN LOAD
 * Carga plataformas y fondo de una pantalla concreta
 * ---------------------------------------------------------------- */
void screen_load(int epoch, int screen)
{
    int i;
    const Platform *src;

    /* Seleccionar tabla de plataformas segun epoca */
    switch (epoch)
    {
        case EPOCH_PREHISTORY: src = platforms_pre[screen]; break;
        case EPOCH_MEDIEVAL:   src = platforms_med[screen]; break;
        case EPOCH_FUTURE:     src = platforms_fut[screen]; break;
        default:               src = platforms_pre[screen]; break;
    }

    /* Copiar plataformas a g_screen_data */
    g_screen_data.platform_count = 0;
    for (i = 0; i < MAX_PLATFORMS; i++)
    {
        if (src[i].w == 0) break;
        g_screen_data.platforms[i] = src[i];
        g_screen_data.platform_count++;
    }

    /* Actualizar ScreenManager */
    g_game.screen.current_epoch  = epoch;
    g_game.screen.current_screen = screen;

    /* Cargar BMP de fondo bajo demanda si no esta cargado */
    if (!g_bg_loaded[epoch][screen])
    {
        if (try_load_bmp((char *)bg_names[epoch][screen],
                         &g_backgrounds[epoch][screen]))
        {
            g_bg_loaded[epoch][screen] = 1;
        }
        else
        {
            /* BMP no disponible: usar placeholder de color */
            g_backgrounds[epoch][screen].data = NULL;
            g_bg_loaded[epoch][screen] = 1;
        }
    }

    /* Aplicar paleta solo si el BMP esta cargado */
    if (g_bg_loaded[epoch][screen] &&
        g_backgrounds[epoch][screen].data != NULL)
        set_palette_silent(g_backgrounds[epoch][screen].palette);
}

/* ----------------------------------------------------------------
 * SCREEN APPLY PALETTE
 * Aplica la paleta de la pantalla actual al hardware VGA.
 * Usar al arrancar el juego o cargar una partida guardada,
 * cuando no hay transicion de fade.
 * ---------------------------------------------------------------- */
void screen_apply_palette(void)
{
    int epoch;
    int screen;

    epoch  = g_game.screen.current_epoch;
    screen = g_game.screen.current_screen;

    if (g_bg_loaded[epoch][screen] &&
        g_backgrounds[epoch][screen].data != NULL)
        set_palette(g_backgrounds[epoch][screen].palette);

    screen_inject_eric_palette();
    screen_inject_egg_palette();
    screen_inject_stick_palette();
    screen_inject_cup_palette();
}

/* ----------------------------------------------------------------
 * SCREEN DRAW ROCK
 * Dibuja el sprite de la roca en P1 de Prehistoria mientras
 * el puzzle PUZZLE_LEVER no este resuelto.
 * ---------------------------------------------------------------- */
 static int s_rock_rolling;
void screen_draw_rock(void)
{
    if (g_game.screen.current_epoch  != EPOCH_PREHISTORY)
        return;
    if (g_game.screen.current_screen != 0)
        return;
    if (puzzle_is_solved(PUZZLE_LEVER))
        return;
    if (s_rock_rolling)
        return;
    if (g_rock_sprite.data == NULL)
        return;

    screen_inject_rock_palette();
    draw_bitmap_buf_t(&g_rock_sprite, 108, 86);
}

/* ----------------------------------------------------------------
 * ANIMACION ROCA RODANDO
 * Estado interno: la roca rueda hacia la izquierda al activarse.
 * Velocidad de animacion: ROCK_ROLL_ANIM_SPEED frames de juego
 *   por frame de sprite.
 * Velocidad de desplazamiento: ROCK_ROLL_SPEED pixels por frame.
 * Ajusta estos dos valores para tunear la animacion.
 * ---------------------------------------------------------------- */
#define ROCK_ROLL_FRAMES     8
#define ROCK_ROLL_ANIM_SPEED 6    /* frames de juego por frame de sprite */
#define ROCK_ROLL_SPEED      1    /* pixels por frame hacia la izquierda */

static int s_rock_rolling = 0;
static int s_roll_frame    = 0;  /* frame actual del spritesheet (0-7) */
static int s_roll_timer    = 0;  /* contador para cambio de frame      */
static int s_roll_x        = 0;  /* posicion X actual de la roca       */
static int s_roll_sfx_timer = 0;

#define ROCK_SFX_INTERVAL  46      /* ~0.66s a 70Hz */


void screen_trigger_rock_roll(void)
{
    s_rock_rolling   = 1;
    s_roll_frame     = 0;
    s_roll_timer     = 0;
    s_roll_x         = 108;
    s_roll_sfx_timer = 0;
    sfx_play(SFX_ROCK_ROLL, 64, MIDDLE);  /* primera vez */
}

void screen_draw_rock_rolling(void)
{
    if (!s_rock_rolling)
        return;
    if (g_game.screen.current_epoch  != EPOCH_PREHISTORY)
        return;
    if (g_game.screen.current_screen != 0)
        return;
    if (g_rock_roll_sprite.data == NULL)
        return;
    
    /* Avanzar posicion */
    s_roll_x -= ROCK_ROLL_SPEED;

    /* Avanzar frame de animacion */
    if (++s_roll_timer >= ROCK_ROLL_ANIM_SPEED)
    {
        s_roll_timer = 0;
        s_roll_frame = (s_roll_frame + 1) % ROCK_ROLL_FRAMES;
    }

     /* Repetir SFX cuando el anterior ha terminado (~0.66s = 46 ticks) */
    if (++s_roll_sfx_timer >= ROCK_SFX_INTERVAL)
    {
        s_roll_sfx_timer = 0;
        sfx_play(SFX_ROCK_ROLL, 64, MIDDLE);
    }

    /* Si la roca salio por el borde izquierdo, resolver puzzle y parar */
    if (s_roll_x < -64)
    {
        s_rock_rolling = 0;
        puzzle_solve(PUZZLE_LEVER);
        return;
    }

    screen_inject_rock_palette();
    bmp_draw_tile(&g_rock_roll_sprite, s_roll_frame, 0, 64, 64, s_roll_x, 86);
}

/* ----------------------------------------------------------------
 * SCREEN DRAW
 * Dibuja el fondo de la pantalla actual en el back buffer
 * ---------------------------------------------------------------- */
void screen_draw(void)
{
    int epoch;
    int screen;

    epoch  = g_game.screen.current_epoch;
    screen = g_game.screen.current_screen;

    if (g_bg_loaded[epoch][screen] &&
        g_backgrounds[epoch][screen].data != NULL)
    {
        /* BMP cargado: dibujar en el back buffer */
        draw_bitmap_buf(&g_backgrounds[epoch][screen], 0, 0);
    }
    else
    {
        /* Placeholder: fondo de color solido por epoca */
        vga_clear(bg_colors[epoch]);
    }
}

/* ----------------------------------------------------------------
 * GET CONNECTION
 * Devuelve la pantalla conectada en una direccion.
 * Tiene en cuenta los bloqueos por puzzles.
 * ---------------------------------------------------------------- */
int screen_get_connection(int dir)
{
    int next;
    int epoch;
    int screen;

    epoch  = g_game.screen.current_epoch;
    screen = g_game.screen.current_screen;
    next   = connections[epoch][screen][dir];

    if (next == NO_SCREEN) return NO_SCREEN;

    /* Bloqueos por puzzles */

    /* Edad Media: p9 bloqueada hasta bajar el puente */
    if (epoch == EPOCH_MEDIEVAL &&
        screen == 7 && dir == DIR_RIGHT &&
        !puzzle_is_solved(PUZZLE_BRIDGE))
        return NO_SCREEN;

    /* Futuro: p9 bloqueada hasta sellar la radiacion */
    if (epoch == EPOCH_FUTURE &&
        screen == 7 && dir == DIR_RIGHT &&
        !puzzle_is_solved(PUZZLE_RADIATION))
        return NO_SCREEN;

    return next;
}

/* ----------------------------------------------------------------
 * SCREEN CHANGE
 * Cambia de pantalla en una direccion con fade
 * ---------------------------------------------------------------- */
int screen_change(int dir)
{
    int next;
    int py;

    next = screen_get_connection(dir);
    if (next == NO_SCREEN) return 0;

    py = (int)g_game.player.y;

    /* Fade out de la pantalla actual */
    vga_fade_out(16, 4);

    /* Limpiar el back buffer antes de cargar la nueva pantalla */
    /* para evitar parpadeo durante el cambio de paleta         */
    vga_clear(0);
    vga_flip();

    
   /* Cargar SFX especifico de la pantalla */
   sfx_free(1);
    if(next == 0)
        sfx_load(SFX_ROCK_ROLL,  "moverock.wav");
    else if(next == 1 && s_honey_active)
        sfx_load(SFX_HONEY_DROP, "drops.wav");
    else if(next == 2)
        sfx_load(SFX_BEAR_STEP,  "woso.wav");        

    /* Cargar la nueva pantalla y su paleta */
    screen_load(g_game.screen.current_epoch, next);

    /* Reposicionar a Eric segun la direccion de entrada */
    switch (dir)
    {
        case DIR_LEFT:
            player_place(SCREEN_W - PLAYER_WIDTH - 4, py);
            break;
        case DIR_RIGHT:
            player_place(4, py);
            break;
        case DIR_UP:
            player_place((int)g_game.player.x, SCREEN_H - PLAYER_HEIGHT - 40);
            break;
        case DIR_DOWN:
            player_place((int)g_game.player.x, 8);
            break;
        default:
            break;
    }

    /* Dibujar el nuevo fondo en el back buffer y hacer flip */
    /* antes del fade in para que se vea la nueva pantalla   */
    screen_draw();
    vga_flip();

    /* Inyectar paleta de Eric en saved_palette y en el hardware */
    palette_inject(130, eric_palette, 56);
    screen_inject_eric_palette();
    palette_inject(186, cup_palette, 14);
    screen_inject_cup_palette();
    palette_inject(220, egg_palette, 12);
    screen_inject_egg_palette();
    palette_inject(232, stick_palette, 10);
    screen_inject_stick_palette();
    vga_fade_in(16, 4);

    return 1;
}

/* ----------------------------------------------------------------
 * SCREEN TRAVEL
 * Viaje temporal a otra epoca con fade
 * Eric mantiene su posicion en el mapa
 * ---------------------------------------------------------------- */
void screen_travel(int new_epoch)
{
    int screen;

    screen = g_game.screen.current_screen;

    /* Parar musica antes del fade para vaciar el buffer */
    music_free();
    timer_wait(3);

    vga_fade_out(16, 4);
    vga_clear(0);
    vga_flip();

    /* Cargar nueva musica con pantalla en negro */
    if (new_epoch == EPOCH_MEDIEVAL)
        music_load_xm("MEDTHEME.XM");
    else
        music_load_xm("PRETHEME.XM");
    music_play(0);

    screen_load(new_epoch, screen);
    player_place((int)g_game.player.x, 100);
    screen_draw();
    vga_flip();
    palette_inject(130, eric_palette, 56);
    screen_inject_eric_palette();
    palette_inject(186, cup_palette, 14);
    screen_inject_cup_palette();
    palette_inject(220, egg_palette, 12);
    screen_inject_egg_palette();
    palette_inject(232, stick_palette, 10);
    screen_inject_stick_palette();
    vga_fade_in(16, 4);
}

/* ----------------------------------------------------------------
 * Draw Chamán
 * Animación del chamán en P1 de pre
 * ---------------------------------------------------------------- */
void screen_draw_shaman(void)
{
    static int s_frame = 0;
    static int s_timer = 0;

    if (g_game.screen.current_epoch  != EPOCH_PREHISTORY) return;
    if (g_game.screen.current_screen != 0)                return;
    if (puzzle_is_solved(PUZZLE_SHAMAN))                  return;
    if (g_shaman_sprite.data == NULL)                     return;

    if (++s_timer >= 30)
    {
        s_timer = 0;
        s_frame = (s_frame + 1) % 4;
    }

    screen_inject_shaman_palette();
    bmp_draw_tile(&g_shaman_sprite, s_frame, 0, 64, 64, 28, 53);
}

/* -----------------------------------------------------------------------------------------
 * SELECTOR screen_is_honey_dripping()
 *   Devuelve 1 si la animacion de goteo de miel esta activa, 0 si no.
 * -----------------------------------------------------------------------------------------*/
int screen_is_honey_dripping(void)
{
    return s_honey_active;
}
