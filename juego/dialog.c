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
const char *g_hints[3][9][3] =
{
    /* PREHISTORIA */
    {
        { "El anciano guarda",        "un secreto...",         NULL },  /* P1 */
        { "La colmena gotea",         "cuando se agita",       NULL },  /* P2 */
        { "El oso goloso me",         "cierra el paso",        NULL },  /* P3 */
        { "Algo grande ronda",        "por aqui",              NULL },  /* P4 */
        { "La hoguera es un buen",    "sitio para descansar",  NULL },  /* P5 */
        { "El viento arrastra olor",  "a tierra mojada",       NULL },  /* P6 */
        { "Las sombras se mueven",    "solas aqui",            NULL },  /* P7 */
        { "El rio no perdona a",      "quien lo cruza solo",   NULL },  /* P8 */
        { "Las marcas en la piedra",  "dicen algo",            NULL },  /* P9 */
    },
    /* EDAD MEDIA */
    {
        { "Las murallas ocultan mas", "de lo que muestran",    NULL },  /* P1 */
        { "El puente es viejo pero",  "aun resiste",           NULL },  /* P2 */
        { "Un guardia descuidado es", "una oportunidad",       NULL },  /* P3 */
        { "La fragua trabaja dia",    "y noche",               NULL },  /* P4 */
        { "La capilla guarda",        "secretos del pasado",   NULL },  /* P5 */
        { "El mercado oculta mas",    "de lo que vende",       NULL },  /* P6 */
        { "Las mazmorras no",         "estan vacias",          NULL },  /* P7 */
        { "El laboratorio del",       "alquimista humea",      NULL },  /* P8 */
        { "La torre mas alta guarda", "lo mas valioso",        NULL },  /* P9 */
    },
    /* FUTURO */
    {
        { "Las maquinas hablan si",   "sabes escuchar",        NULL },  /* P1 */
        { "La radiacion no",          "perdona errores",       NULL },  /* P2 */
        { "El Mecha duerme...",       "por ahora",             NULL },  /* P3 */
        { "Los drones patrullan",     "sin descanso",          NULL },  /* P4 */
        { "El nucleo late en las",    "profundidades",         NULL },  /* P5 */
        { "La puerta hermetica",      "lleva semanas cerrada", NULL },  /* P6 */
        { "El aire aqui tiene",       "sabor metalico",        NULL },  /* P7 */
        { "La terminal todavia",      "funciona",              NULL },  /* P8 */
        { "El portal espera",         "ser activado",          NULL },  /* P9 */
    },
};

/* ----------------------------------------------------------------
 * DIALOGOS DE PERSONAJES
 * Arrays de strings terminados en NULL.
 * ---------------------------------------------------------------- */
const char *g_dialog_shaman[] =
{
    "Forastero... has llegado lejos.",
    "Toma...un fragmento del tiempo roto.",
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

/* Colores del dialogo, dependientes de la epoca actual: los indices de
 * paleta "permanente" (130-185, 186-199...) apuntan a colores distintos
 * segun que epoca tenga su paleta inyectada en cada momento. Cuando se
 * definan las paletas de Edad Media y Futuro, actualizar los casos
 * correspondientes con sus propios indices oscuro/claro/medio. */
static unsigned char dialog_col_bg(void)
{
    switch (g_game.screen.current_epoch)
    {
        case EPOCH_PREHISTORY: return 130;  /* eric_palette, mas oscuro (4,1,1) */
        case EPOCH_MEDIEVAL:   return 130;  /* TODO: ajustar con la paleta de Edad Media */
        case EPOCH_FUTURE:     return 130;  /* TODO: ajustar con la paleta de Futuro */
        default:                return 130;
    }
}

static unsigned char dialog_col_border(void)
{
    switch (g_game.screen.current_epoch)
    {
        case EPOCH_PREHISTORY: return 37;  /* cup_palette, mas claro (54,52,49) */
        case EPOCH_MEDIEVAL:   return 199;  /* TODO: ajustar con la paleta de Edad Media */
        case EPOCH_FUTURE:     return 199;  /* TODO: ajustar con la paleta de Futuro */
        default:                return 199;
    }
}

static unsigned char dialog_col_dim(void)
{
    switch (g_game.screen.current_epoch)
    {
        case EPOCH_PREHISTORY: return 37;  /* eric_palette, gris medio (31,32,32) */
        case EPOCH_MEDIEVAL:   return 174;  /* TODO: ajustar con la paleta de Edad Media */
        case EPOCH_FUTURE:     return 174;  /* TODO: ajustar con la paleta de Futuro */
        default:                return 174;
    }
}

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
    int i, x, y;
    unsigned char col_bg, col_border, col_text, col_dim; 
    
    /* Coordenadas del texto dentro de la ventana */
    int tx, ty;

    
    if (s_type == DIALOG_NONE) return;

    /* Parámetros de color del diálogo según paleta de época */
    col_bg     = dialog_col_bg();
    col_border = dialog_col_border();
    col_text   = col_border;
    col_dim    = dialog_col_dim();
    
    ty = DLG_Y + DLG_PAD + 5;

    /* Fondo de la ventana */
    for (i = DLG_Y; i < DLG_Y + DLG_H; i++)
        memset(&back_buffer[i * SCREEN_W + DLG_X], col_bg, DLG_W);

    /* Borde */
    draw_line(DLG_X,           DLG_Y,            DLG_X + DLG_W - 1, DLG_Y,            col_border);
    draw_line(DLG_X + DLG_W-1, DLG_Y,            DLG_X + DLG_W - 1, DLG_Y + DLG_H-1, col_border);
    draw_line(DLG_X + DLG_W-1, DLG_Y + DLG_H-1, DLG_X,             DLG_Y + DLG_H-1, col_border);
    draw_line(DLG_X,           DLG_Y + DLG_H-1, DLG_X,             DLG_Y,            col_border);

    /* Retrato */
    tx = DLG_X + DLG_PAD;
    if (s_portrait != NULL && s_portrait->data != NULL)
    {
        draw_bitmap_buf_t(s_portrait, tx, DLG_Y + DLG_PAD + 7);
        tx += DLG_PORT_W + DLG_PAD;
    }

    /* Texto segun tipo */
    if (s_type == DIALOG_HINT)
    {
        if (s_lines != NULL && s_lines[0] != NULL)
            draw_string(s_lines[0], tx+7, ty+13, col_text);
        if (s_lines != NULL && s_lines[1] != NULL)
            draw_string(s_lines[1], tx+7, ty+13+10, col_text);
        //draw_string("[ ESPACIO ]", tx, DLG_Y + DLG_H - 14, col_dim);
    }
    else if (s_type == DIALOG_YESNO)
    {
        if (s_lines != NULL && s_lines[0] != NULL)
            draw_string(s_lines[0], tx+7, ty+13, col_text);
        draw_string("[S] Si    [N] No", tx, DLG_Y + DLG_H - 14, col_dim);
    }
    else if (s_type == DIALOG_TALK)
    {
        if (s_lines != NULL && s_lines[s_line] != NULL)
            draw_string(s_lines[s_line], tx+7, ty+13, col_text);
        if (s_lines != NULL && s_lines[s_line + 1] != NULL)
            draw_string("[ ESPACIO ]", tx, DLG_Y + DLG_H - 14, col_dim);
        else
            draw_string("[ ESPACIO ] Cerrar", tx, DLG_Y + DLG_H - 14, col_dim);
    }

    vga_flip();
}
