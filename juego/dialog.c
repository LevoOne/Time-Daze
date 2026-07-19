/* ================================================================
 * dialog.c - Sistema de ventanas emergentes de dialogo
 * ================================================================ */

#include <string.h>
#include "engine.h"
#include "game.h"
#include "dialog.h"

/* ----------------------------------------------------------------
 * TEXTOS: hints por pantalla [epoca][pantalla]
 * ---------------------------------------------------------------- */
const char *g_hints[3][9] =
{
    /* PREHISTORIA */
    {
        "El anciano guarda un secreto...",           /* P1 */
        "La colmena gotea cuando se agita",          /* P2 */
        "Los osos no comparten su territorio",       /* P3 */
        "Algo grande ronda por aqui",                /* P4 */
        "El fuego nunca se apaga del todo",          /* P5 */
        "El viento arrastra olor a tierra mojada",   /* P6 */
        "Las sombras se mueven solas aqui",          /* P7 */
        "El rio no perdona a quien lo cruza solo",   /* P8 */
        "Las marcas en la piedra dicen algo",        /* P9 */
    },
    /* EDAD MEDIA */
    {
        "Las murallas ocultan mas de lo que muestran",  /* P1 */
        "El puente es viejo pero aun resiste",          /* P2 */
        "Un guardia descuidado es una oportunidad",     /* P3 */
        "La fragua trabaja dia y noche",                /* P4 */
        "La capilla guarda secretos del pasado",        /* P5 */
        "El mercado oculta mas de lo que vende",        /* P6 */
        "Las mazmorras no estan vacias",                /* P7 */
        "El laboratorio del alquimista humea",          /* P8 */
        "La torre mas alta guarda lo mas valioso",      /* P9 */
    },
    /* FUTURO */
    {
        "Las maquinas hablan si sabes escuchar",        /* P1 */
        "La radiacion no perdona errores",              /* P2 */
        "El Mecha duerme... por ahora",                 /* P3 */
        "Los drones patrullan sin descanso",            /* P4 */
        "El nucleo late en las profundidades",          /* P5 */
        "La puerta hermetica lleva semanas cerrada",    /* P6 */
        "El aire aqui tiene sabor metalico",            /* P7 */
        "La terminal todavia funciona",                 /* P8 */
        "El portal espera ser activado",                /* P9 */
    },
};

/* ----------------------------------------------------------------
 * DIALOGOS DE PERSONAJES
 * Arrays de strings terminados en NULL.
 * ---------------------------------------------------------------- */
const char *g_dialog_shaman[] =
{
    "Forastero... has llegado lejos.",
    "La planta sagrada crece donde",
    "el fuego y el agua se tocan.",
    "Traemela y te abriré el paso.",
    NULL
};

const char *g_dialog_monolith[] =
{
    "Los simbolos dicen:",
    "SOL - LUNA - ESTRELLA",
    NULL
};

/* ----------------------------------------------------------------
 * ESTADO INTERNO
 * ---------------------------------------------------------------- */
static int          s_type      = DIALOG_NONE;
static BITMAP      *s_portrait  = NULL;
static const char **s_lines     = NULL;
static int          s_line      = 0;    /* linea actual en DIALOG_TALK */
static int          s_got_yes   = 0;
static int          s_space_was_pressed = 0;
static int          s_yn_was_pressed    = 0;

/* Dimensiones de la ventana */
#define DLG_W       260
#define DLG_H        70
#define DLG_X       ((SCREEN_W - DLG_W) / 2)   /* 30 */
#define DLG_Y        55
#define DLG_PAD       6
#define DLG_PORT_W   48
#define DLG_PORT_H   48
#define DLG_COL_BG    0    /* negro de la paleta compartida */
#define DLG_COL_BORDER 15  /* blanco */
#define DLG_COL_TEXT   15
#define DLG_COL_DIM    8   /* gris para instrucciones */

/* ----------------------------------------------------------------
 * FUNCIONES PUBLICAS
 * ---------------------------------------------------------------- */

void dialog_open(int type, BITMAP *portrait, const char **lines)
{
    s_type              = type;
    s_portrait          = portrait;
    s_lines             = lines;
    s_line              = 0;
    s_got_yes           = 0;
    s_space_was_pressed = 1;   /* evitar cierre inmediato si ESPACIO sigue pulsado */
    s_yn_was_pressed    = 0;
}

void dialog_close(void)
{
    s_type = DIALOG_NONE;
}

int dialog_is_open(void)
{
    return s_type != DIALOG_NONE;
}

int dialog_got_yes(void)
{
    return s_got_yes;
}

void dialog_update(void)
{
    if (s_type == DIALOG_NONE) return;

    if (s_type == DIALOG_HINT)
    {
        if (key_pressed(KEY_SPACE))
        {
            if (!s_space_was_pressed)
            {
                s_space_was_pressed = 1;
                dialog_close();
            }
        }
        else
        {
            s_space_was_pressed = 0;
        }
    }
    else if (s_type == DIALOG_YESNO)
    {
        /* S = scancode 0x1F, N = scancode 0x31 */
        if (!s_yn_was_pressed)
        {
            if (key_pressed(0x1F))   /* S */
            {
                s_yn_was_pressed = 1;
                s_got_yes = 1;
                dialog_close();
            }
            else if (key_pressed(0x31))   /* N */
            {
                s_yn_was_pressed = 1;
                s_got_yes = 0;
                dialog_close();
            }
        }
    }
    else if (s_type == DIALOG_TALK)
    {
        if (key_pressed(KEY_SPACE))
        {
            if (!s_space_was_pressed)
            {
                s_space_was_pressed = 1;
                if (s_lines == NULL || s_lines[s_line + 1] == NULL)
                    dialog_close();
                else
                    s_line++;
            }
        }
        else
        {
            s_space_was_pressed = 0;
        }
    }
}

void dialog_draw(void)
{
    int i, x, y, tx, ty;

    if (s_type == DIALOG_NONE) return;

    /* Fondo de la ventana */
    for (i = DLG_Y; i < DLG_Y + DLG_H; i++)
        memset(&back_buffer[i * SCREEN_W + DLG_X], DLG_COL_BG, DLG_W);

    /* Borde */
    draw_line(DLG_X,           DLG_Y,            DLG_X + DLG_W - 1, DLG_Y,            DLG_COL_BORDER);
    draw_line(DLG_X + DLG_W-1, DLG_Y,            DLG_X + DLG_W - 1, DLG_Y + DLG_H-1, DLG_COL_BORDER);
    draw_line(DLG_X + DLG_W-1, DLG_Y + DLG_H-1, DLG_X,             DLG_Y + DLG_H-1, DLG_COL_BORDER);
    draw_line(DLG_X,           DLG_Y + DLG_H-1, DLG_X,             DLG_Y,            DLG_COL_BORDER);

    /* Retrato (si hay) */
    tx = DLG_X + DLG_PAD;
    if (s_portrait != NULL && s_portrait->data != NULL)
    {
        draw_bitmap_buf_t(s_portrait, tx, DLG_Y + DLG_PAD);
        tx += DLG_PORT_W + DLG_PAD;
    }

    /* Texto segun tipo */
    ty = DLG_Y + DLG_PAD + 2;

    if (s_type == DIALOG_HINT)
    {
        if (s_lines != NULL && s_lines[0] != NULL)
            draw_string(s_lines[0], tx, ty, DLG_COL_TEXT);
        draw_string("[ ESPACIO ]", tx, DLG_Y + DLG_H - 14, DLG_COL_DIM);
    }
    else if (s_type == DIALOG_YESNO)
    {
        if (s_lines != NULL && s_lines[0] != NULL)
            draw_string(s_lines[0], tx, ty, DLG_COL_TEXT);
        draw_string("[ S ] Si    [ N ] No", tx, DLG_Y + DLG_H - 14, DLG_COL_DIM);
    }
    else if (s_type == DIALOG_TALK)
    {
        if (s_lines != NULL && s_lines[s_line] != NULL)
            draw_string(s_lines[s_line], tx, ty, DLG_COL_TEXT);
        if (s_lines != NULL && s_lines[s_line + 1] != NULL)
            draw_string("[ ESPACIO ]", tx, DLG_Y + DLG_H - 14, DLG_COL_DIM);
        else
            draw_string("[ ESPACIO ] Cerrar", tx, DLG_Y + DLG_H - 14, DLG_COL_DIM);
    }

    vga_flip();
}
