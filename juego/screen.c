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


/* Prototipo interno */
static int try_load_bmp(char *file, BITMAP *b);


/* ----------------------------------------------------------------
 * VARIABLES GLOBALES
 * ---------------------------------------------------------------- */
ScreenData g_screen_data;
BITMAP     g_spritesheet;

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
    /* P1: cima, circulo de megalitos */
    {
        { 0,   175, 320, 8 },
        { 40,  150,  60, 8 },
        { 180, 140,  80, 8 },
        { 260, 155,  50, 8 },
        { 0, 0, 0, 0 }
    },
    /* P2: ladera, colmena */
    {
        { 0,   175, 320, 8 },
        { 20,  155,  50, 8 },
        { 100, 135,  60, 8 },
        { 200, 115,  70, 8 },
        { 0, 0, 0, 0 }
    },
    /* P3: pie colina, oso */
    {
        { 0,   175, 320, 8 },
        { 60,  155,  40, 8 },
        { 200, 160,  60, 8 },
        { 0, 0, 0, 0 }
    },
    /* P4: nivel medio, jabali */
    {
        { 0,   175, 320, 8 },
        { 50,  145,  60, 8 },
        { 180, 150,  70, 8 },
        { 0, 0, 0, 0 }
    },
    /* P5: hoguera */
    {
        { 0,   175, 320, 8 },
        { 0, 0, 0, 0 }
    },
    /* P6: zona baja izquierda */
    {
        { 0,   175, 160, 8 },
        { 30,  155,  40, 8 },
        { 0, 0, 0, 0 }
    },
    /* P7: zona baja centro */
    {
        { 0,   175, 320, 8 },
        { 40,  150,  50, 8 },
        { 180, 145,  60, 8 },
        { 0, 0, 0, 0 }
    },
    /* P8: cruce del rio */
    {
        { 0,   175,  60, 8 },
        { 260, 175,  60, 8 },
        { 80,  160,  30, 8 },
        { 130, 155,  30, 8 },
        { 180, 160,  30, 8 },
        { 230, 158,  25, 8 },
        { 0, 0, 0, 0 }
    },
    /* P9: monolito */
    {
        { 0,   175, 320, 8 },
        { 100, 145,  40, 8 },
        { 220, 155,  60, 8 },
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

    for (index = (b->height - 1) * b->width; index >= 0; index -= b->width)
        for (x = 0; x < b->width; x++)
            b->data[(word)(index + x)] = (byte)fgetc(fp);

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
        set_palette(g_backgrounds[epoch][screen].palette);
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

    /* Prehistoria: zona baja bloqueada hasta cruzar el rio */
    if (epoch == EPOCH_PREHISTORY &&
        screen == 7 && dir == DIR_RIGHT &&
        !puzzle_is_solved(PUZZLE_RIVER))
        return NO_SCREEN;

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

    vga_fade_out(16, 4);
    screen_load(g_game.screen.current_epoch, next);
    vga_fade_in(16, 4);

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
            player_place((int)g_game.player.x, SCREEN_H - PLAYER_HEIGHT - 8);
            break;
        case DIR_DOWN:
            player_place((int)g_game.player.x, 8);
            break;
        default:
            break;
    }

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
    screen_load(new_epoch, screen);
    vga_fade_in(16, 4);
}
