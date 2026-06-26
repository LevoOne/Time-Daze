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
 * Indices 238-255 reservados para los colores de Eric.
 * Se inyectan en la paleta activa despues de cada cambio de fondo.
 * Formato: R,G,B en rango 0..63 para VGA.
 * ---------------------------------------------------------------- */
static const byte eric_palette[18 * 3] =
{
     0,  0,  0,  /* indice 238: negro */
    11, 13, 48,  /* indice 239: azul oscuro jersey */
     0,  5, 34,  /* indice 240: azul muy oscuro */
     0, 36, 63,  /* indice 241: azul claro */
    21, 11,  8,  /* indice 242: marron muy oscuro piel */
    26, 14, 11,  /* indice 243: marron oscuro piel */
    33, 18, 14,  /* indice 244: marron piel */
    33, 26, 29,  /* indice 245: gris rosado */
    59, 50, 47,  /* indice 246: piel clara */
    44, 25, 21,  /* indice 247: marron rojizo */
    48, 33, 26,  /* indice 248: marron medio */
    56, 36, 30,  /* indice 249: salmon */
    58, 41, 36,  /* indice 250: salmon claro */
    63, 49, 45,  /* indice 251: rosa claro */
    59, 44, 38,  /* indice 252: salmon medio */
    50, 50, 50,  /* indice 253: gris claro */
    58, 57, 50,  /* indice 254: blanco hueso */
     0,  0,  0,  /* indice 255: negro */
};

/* Inyecta los colores de Eric en la paleta VGA activa */
static void screen_inject_eric_palette(void)
{
    int i;
    outp(0x3C8, 238);
    for (i = 0; i < 18 * 3; i++)
        outp(0x3C9, eric_palette[i]);
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
 * VARIABLES GLOBALES
 * ---------------------------------------------------------------- */
ScreenData g_screen_data;
BITMAP     g_spritesheet;
BITMAP     g_rock_sprite;
BITMAP     g_shaman_sprite;
BITMAP     g_stick_sprite;
BITMAP     g_stick_icon;
BITMAP     g_egg_sprite;
BITMAP     g_egg_icon;

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
        { 29,  116,  50, 2 },   /* dolmen izquierdo */
        { 136, 110,  60, 2 },   /* roca plana central, chaman */
        { 0, 0, 0, 0 }
    },
    /* P2: ladera, colmena */
    {
        { 0,   148, 320, 8 },   /* suelo principal */
        { 27,  115,  54, 2 },   /* roca izquierda */
        { 134, 110,  64, 2 },   /* roca central, colmena */
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

    /* Cargar el sprite del chamán de P1 */
    if (!try_load_bmp("SHAMAN.BMP", &g_shaman_sprite))
    g_shaman_sprite.data = NULL;

    /* Cargar el sprite del palo (objeto recogible) */
    if (!try_load_bmp("STICK.BMP", &g_stick_sprite))
        g_stick_sprite.data = NULL;

    /* Cargar el icono del palo para el HUD */
    if (!try_load_bmp("STKICON.BMP", &g_stick_icon))
        g_stick_icon.data = NULL;

    /* Cargar el sprite del huevo de dinosaurio (objeto recogible) */
    if (!try_load_bmp("HUEVO.BMP", &g_egg_sprite))
        g_egg_sprite.data = NULL;

    /* Cargar el icono del huevo para el HUD */
    if (!try_load_bmp("EGGICON.BMP", &g_egg_icon))
        g_egg_icon.data = NULL;
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
}

/* ----------------------------------------------------------------
 * SCREEN DRAW ROCK
 * Dibuja el sprite de la roca en P1 de Prehistoria mientras
 * el puzzle PUZZLE_LEVER no este resuelto.
 * ---------------------------------------------------------------- */
void screen_draw_rock(void)
{
    if (g_game.screen.current_epoch  != EPOCH_PREHISTORY) return;
    if (g_game.screen.current_screen != 0)                return;
    if (puzzle_is_solved(PUZZLE_LEVER))                   return;
    if (g_rock_sprite.data == NULL)                       return;

    draw_bitmap_buf_t(&g_rock_sprite, 108, 86);
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
    palette_inject(238, eric_palette, 18);
    screen_inject_eric_palette();
    palette_inject(220, egg_palette, 12);
    screen_inject_egg_palette();

    /* Fade in de la nueva pantalla */
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

    vga_fade_out(16, 4);
    vga_clear(0);
    vga_flip();
    screen_load(new_epoch, screen);

    /* Reposicionar a Eric a una Y segura para que caiga al suelo */
    player_place((int)g_game.player.x, 100);

    screen_draw();
    vga_flip();
    palette_inject(238, eric_palette, 18);
    screen_inject_eric_palette();
    palette_inject(220, egg_palette, 12);
    screen_inject_egg_palette();
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

    bmp_draw_tile(&g_shaman_sprite, s_frame, 0, 32, 32, 36, 84);
}
