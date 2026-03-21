/*
 * main.c - Ejemplo de uso del motor
 *
 * Un cuadrado de 16x16 que:
 *   - Se mueve con las flechas izquierda/derecha
 *   - Salta con ESPACIO
 *   - Tiene gravedad y colisiona con el suelo
 *   - Sale con ESC
 *
 * Compila con OpenWatcom:
 *   wcl386 -l=dos4g main.c engine.c
 */

#include "engine.h"

/* ─── Dimensiones del sprite del jugador ────────────────────── */
#define PLAYER_W   16
#define PLAYER_H   16

/* ─── Física ─────────────────────────────────────────────────── */
#define GRAVITY        0.4f
#define JUMP_FORCE    -12.0f
#define MOVE_SPEED     3.0f
#define GROUND_Y      (SCREEN_H - PLAYER_H - 1)

/* ─── Sprite del jugador (16x16, color 14=amarillo, 0=transparente) ── */
/*
 * En tu juego real cargarás esto desde un BMP.
 * Aquí lo definimos inline para que el ejemplo sea autocontenido.
 */
static const unsigned char player_sprite[PLAYER_W * PLAYER_H] = {
    0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
    0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
   14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
   14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
   14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
   14,14,14,14, 4,14,14,14,14,14,14, 4,14,14,14,14,
   14,14,14,14,14, 4, 4, 4, 4, 4, 4,14,14,14,14,14,
   14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
   14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
   14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
   14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
   14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
    0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
    0, 0,14,14, 0, 0,14,14,14,14, 0, 0,14,14, 0, 0,
    0, 0,14,14, 0, 0, 0, 0, 0, 0, 0, 0,14,14, 0, 0,
    0, 0,14,14, 0, 0, 0, 0, 0, 0, 0, 0,14,14, 0, 0,
};

/* ─── Estado del jugador ─────────────────────────────────────── */
typedef struct {
    float x, y;          /* posición (usamos float para la física) */
    float vel_y;         /* velocidad vertical */
    int   on_ground;     /* 1 si está tocando el suelo */
} Player;

static void player_init(Player *p)
{
    p->x        = SCREEN_W / 2 - PLAYER_W / 2;
    p->y        = GROUND_Y;
    p->vel_y    = 0.0f;
    p->on_ground = 1;
}

static void player_update(Player *p)
{
    /* Movimiento horizontal */
    if (key_pressed(KEY_LEFT))
        p->x -= MOVE_SPEED;
    if (key_pressed(KEY_RIGHT))
        p->x += MOVE_SPEED;

    /* Salto: solo si está en el suelo */
    if (key_pressed(KEY_SPACE) && p->on_ground)
    {
        p->vel_y    = JUMP_FORCE;
        p->on_ground = 0;
    }

    /* Gravedad */
    p->vel_y += GRAVITY;
    p->y     += p->vel_y;

    /* Colisión con el suelo */
    if (p->y >= GROUND_Y)
    {
        p->y        = GROUND_Y;
        p->vel_y    = 0.0f;
        p->on_ground = 1;
    }

    /* Límites laterales de pantalla */
    if (p->x < 0)               p->x = 0;
    if (p->x > SCREEN_W - PLAYER_W) p->x = SCREEN_W - PLAYER_W;
}

static void player_draw(const Player *p)
{
    draw_sprite(player_sprite, (int)p->x, (int)p->y, PLAYER_W, PLAYER_H);
}

/* ─── Main ───────────────────────────────────────────────────── */

int main(void)
{
    Player player;

    /* Inicializar subsistemas */
    vga_set_mode13h();
    keyboard_install();
    timer_install();

    player_init(&player);

    /* ── Game loop ──────────────────────────────────────────── */
    while (!key_pressed(KEY_ESC))
    {
        /* 1. Lógica */
        player_update(&player);

        /* 2. Render al back buffer */
        vga_clear(1);                        /* fondo azul oscuro (índice 1) */

        /* Suelo: una línea horizontal verde */
        {
            int i;
            for (i = 0; i < SCREEN_W; i++)
                vga_put_pixel(i, GROUND_Y + PLAYER_H, 2);  /* color 2 = verde */
        }

        player_draw(&player);

        /* 3. Volcar a pantalla */
        vga_flip();

        /* 4. Limitar a ~70 fps (1 tick = 1 frame a 70 Hz) */
        timer_wait(1);
    }

    /* Limpiar y salir */
    timer_uninstall();
    keyboard_uninstall();
    vga_set_text_mode();

    return 0;
}
