/*
 * main.c - Demo de animación por frames y colisiones con plataformas
 *
 * Controles:
 *   Izquierda / Derecha : mover
 *   ESPACIO             : saltar
 *   ESC                 : salir
 *
 * Compila con OpenWatcom:
 *   wcl386 -l=dos4g main.c engine.c
 */

#include "engine.h"

/* ═══════════════════════════════════════════════════════════════
 * SPRITE DATA
 * Cada frame es de 16x16 píxeles = 256 bytes.
 * Colores VGA por defecto:
 *   0  = transparente
 *   14 = amarillo  (cuerpo)
 *   4  = rojo      (ojos/boca)
 *   6  = marrón    (piernas)
 * ═══════════════════════════════════════════════════════════════ */

#define PLAYER_W        16
#define PLAYER_H        16
#define FRAME_COUNT     4    /* frames del ciclo de carrera */

/*
 * Frame 0: reposo
 * Frame 1: paso medio (pierna derecha adelante)
 * Frame 2: pierna izquierda adelante
 * Frame 3: igual que 1 (ciclo simétrico)
 * Frame 4: salto (brazos arriba)
 *
 * Animación de carrera: cicla 0->1->2->3->0...
 * En el aire: siempre frame 4.
 */
static const unsigned char sprite_frames[5][PLAYER_W * PLAYER_H] = {

    /* ── Frame 0: reposo ─────────────────────────────────────── */
    {
        0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
        0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
       14,14,14,14, 4,14,14,14,14,14,14, 4,14,14,14,14,
       14,14,14,14,14, 4, 4, 4, 4, 4, 4,14,14,14,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
        0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
        0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
        0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0,
        0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
        0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
        0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
        0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
        0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
    },

    /* ── Frame 1: paso medio (pierna derecha adelante) ────────── */
    {
        0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
        0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
       14,14,14,14, 4,14,14,14,14,14,14, 4,14,14,14,14,
       14,14,14,14,14, 4, 4, 4, 4, 4, 4,14,14,14,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
        0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
        0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
        0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0,
        0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0,
        0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0,
        0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0,
        0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },

    /* ── Frame 2: pierna izquierda adelante ───────────────────── */
    {
        0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
        0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
       14,14,14,14, 4,14,14,14,14,14,14, 4,14,14,14,14,
       14,14,14,14,14, 4, 4, 4, 4, 4, 4,14,14,14,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
        0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
        0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
        0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0,
        0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0,
        0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
        0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
    },

    /* ── Frame 3: igual que frame 1 (ciclo simétrico) ─────────── */
    {
        0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
        0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
       14,14,14,14, 4,14,14,14,14,14,14, 4,14,14,14,14,
       14,14,14,14,14, 4, 4, 4, 4, 4, 4,14,14,14,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
        0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
        0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
        0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0,
        0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0,
        0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0,
        0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0,
        0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },

    /* ── Frame 4: salto (brazos arriba) ───────────────────────── */
    {
        0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
        0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
       14,14,14,14, 4,14,14,14,14,14,14, 4,14,14,14,14,
       14,14,14,14,14, 4, 4, 4, 4, 4, 4,14,14,14,14,14,
       14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
       14, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0,14,
       14, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0,14,
        0, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0,
        0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 6, 6, 0, 0, 6, 6, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 6, 6, 0, 0, 6, 6, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 6, 6, 0, 0, 6, 6, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
};

/* ═══════════════════════════════════════════════════════════════
 * PLATAFORMAS
 * ═══════════════════════════════════════════════════════════════ */

typedef struct {
    int x, y, w, h;
    unsigned char color;
} Platform;

#define PLATFORM_COUNT  5

static const Platform platforms[PLATFORM_COUNT] = {
    {   0, 185, 320,  15,  2 },   /* suelo - verde oscuro  */
    {  40, 148,  64,   8, 10 },   /* plataforma - cian     */
    { 140, 120,  80,   8,  9 },   /* plataforma - naranja  */
    { 230, 148,  64,   8, 10 },   /* plataforma - cian     */
    { 110,  90,  48,   8,  6 },   /* plataforma - marrón   */
};

static void draw_platforms(void)
{
    int p, x, y;
    for (p = 0; p < PLATFORM_COUNT; p++)
        for (y = platforms[p].y; y < platforms[p].y + platforms[p].h; y++)
            for (x = platforms[p].x; x < platforms[p].x + platforms[p].w; x++)
                vga_put_pixel(x, y, platforms[p].color);
}

/* ═══════════════════════════════════════════════════════════════
 * JUGADOR
 * ═══════════════════════════════════════════════════════════════ */

#define GRAVITY        0.35f
#define JUMP_FORCE    -5.5f
#define MOVE_SPEED     1.0f
#define ANIM_SPEED     10      /* ticks por frame de animación (~12 fps) */

typedef struct {
    float x, y;
    float vel_x, vel_y;
    int   on_ground;
    int   facing;        /* 1=derecha, -1=izquierda */
    int   anim_frame;    /* frame actual 0-3 */
    int   anim_timer;    /* ticks hasta el siguiente frame */
    int   moving;
} Player;

static int player_get_draw_frame(const Player *p)
{
    if (!p->on_ground) return 4;
    if (!p->moving)    return 0;
    return p->anim_frame;
}

static void player_init(Player *p)
{
    p->x          = 20.0f;
    p->y          = 185.0f - PLAYER_H;
    p->vel_x      = 0.0f;
    p->vel_y      = 0.0f;
    p->on_ground  = 1;
    p->facing     = 1;
    p->anim_frame = 0;
    p->anim_timer = 0;
    p->moving     = 0;
}

/* ── Colisión AABB ───────────────────────────────────────────── */
static void resolve_collisions(Player *p)
{
    int i;
    const Platform *plat;
    int px, py;
    int ov_top, ov_bottom, ov_left, ov_right;
    int min, axis;

    p->on_ground = 0;

    for (i = 0; i < PLATFORM_COUNT; i++)
    {
        plat = &platforms[i];
        px   = (int)p->x;
        py   = (int)p->y;

        /* Test AABB */
        if (px + PLAYER_W <= plat->x) continue;
        if (px >= plat->x + plat->w)  continue;
        if (py + PLAYER_H <= plat->y) continue;
        if (py >= plat->y + plat->h)  continue;

        /* Penetración por cada lado */
        ov_top    = (py + PLAYER_H) - plat->y;
        ov_bottom = (plat->y + plat->h) - py;
        ov_left   = (px + PLAYER_W) - plat->x;
        ov_right  = (plat->x + plat->w) - px;

        /* Resolver por el eje de menor penetración */
        min = ov_top; axis = 0;
        if (ov_bottom < min) { min = ov_bottom; axis = 1; }
        if (ov_left   < min) { min = ov_left;   axis = 2; }
        if (ov_right  < min) { min = ov_right;  axis = 3; }

        switch (axis)
        {
            case 0:  /* aterriza encima */
                p->y = (float)(plat->y - PLAYER_H);
                p->vel_y = 0.0f;
                p->on_ground = 1;
                break;
            case 1:  /* golpea por abajo */
                p->y = (float)(plat->y + plat->h);
                p->vel_y = 0.0f;
                break;
            case 2:  /* choca por la izquierda */
                p->x = (float)(plat->x - PLAYER_W);
                p->vel_x = 0.0f;
                break;
            case 3:  /* choca por la derecha */
                p->x = (float)(plat->x + plat->w);
                p->vel_x = 0.0f;
                break;
        }
    }
}

static void player_update(Player *p)
{
    p->moving = 0;

    if (key_pressed(KEY_LEFT))
    {
        p->vel_x  = -MOVE_SPEED;
        p->facing = -1;
        p->moving = 1;
    }
    else if (key_pressed(KEY_RIGHT))
    {
        p->vel_x  = MOVE_SPEED;
        p->facing = 1;
        p->moving = 1;
    }
    else
    {
        p->vel_x = 0.0f;
    }

    if (key_pressed(KEY_SPACE) && p->on_ground)
    {
        p->vel_y    = JUMP_FORCE;
        p->on_ground = 0;
    }

    p->vel_y += GRAVITY;
    p->x     += p->vel_x;
    p->y     += p->vel_y;

    resolve_collisions(p);

    /* Límites laterales */
    if (p->x < 0)                     p->x = 0;
    if (p->x > SCREEN_W - PLAYER_W)   p->x = (float)(SCREEN_W - PLAYER_W);

    /* Caída fuera de pantalla: reaparece arriba */
    if (p->y > SCREEN_H) { p->y = -PLAYER_H; p->vel_y = 0.0f; }

    /* Animación */
    if (p->on_ground && p->moving)
    {
        if (++p->anim_timer >= ANIM_SPEED)
        {
            p->anim_timer = 0;
            p->anim_frame = (p->anim_frame + 1) % FRAME_COUNT;
        }
    }
    else
    {
        p->anim_frame = 0;
        p->anim_timer = 0;
    }
}

static void player_draw(const Player *p)
{
    int frame, sx, sy, dx, dy;
    unsigned char c;

    frame = player_get_draw_frame(p);

    if (p->facing == 1)
    {
        draw_sprite(sprite_frames[frame], (int)p->x, (int)p->y,
                    PLAYER_W, PLAYER_H);
    }
    else
    {
        /* Flip horizontal manual */
        for (sy = 0; sy < PLAYER_H; sy++)
        {
            dy = (int)p->y + sy;
            if (dy < 0 || dy >= SCREEN_H) continue;
            for (sx = 0; sx < PLAYER_W; sx++)
            {
                dx = (int)p->x + (PLAYER_W - 1 - sx);
                if (dx < 0 || dx >= SCREEN_W) continue;
                c = sprite_frames[frame][sy * PLAYER_W + sx];
                if (c == 0) continue;
                back_buffer[dy * SCREEN_W + dx] = c;
            }
        }
    }
}

/* ═══════════════════════════════════════════════════════════════
 * MAIN
 * ═══════════════════════════════════════════════════════════════ */

int main(void)
{
    Player player;

    vga_set_mode13h();
    keyboard_install();
    timer_install();
    player_init(&player);

    while (!key_pressed(KEY_ESC))
    {
        player_update(&player);

        vga_clear(1);
        draw_platforms();
        player_draw(&player);
        vga_flip();

        timer_wait(1);
    }

    timer_uninstall();
    keyboard_uninstall();
    vga_set_text_mode();
    return 0;
}