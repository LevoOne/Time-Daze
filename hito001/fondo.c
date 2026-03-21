/* ----------------------------------------------------------------------------
 * Probamos toda la funcionalidad realizada hasta la fecha con el engine.
 * Usamos un fondo creado con Gemini y exportado con libreprite
 *
 * Prueba en conjunto:
 *   - Carga y muestra un BMP de fondo 320x200
 *   - Musica XM en bucle
 *   - Efecto WAV al saltar
 *   - Sprite con 5 frames de animacion y flip horizontal
 *   - Plataformas con colision AABB
 *   - Double buffer
 *
 * Necesita en el mismo directorio:
 *   - fondo.bmp  (320x200, 8 bits indexado)
 *   - music.xm
 *   - jump.wav
 *
 * Compilar:
 *   wcl386 -l=dos4g fondo.c engine.c judas.lib
 */

#include "engine.h"

/* ----------------------------------------------------------------
 * SPRITE DATA - 5 frames de 16x16
 * 0=reposo, 1-3=carrera, 4=salto
 * ---------------------------------------------------------------- */

#define PLAYER_W     16
#define PLAYER_H     16
#define FRAME_COUNT   4
#define ANIM_SPEED   10

static const unsigned char sprite_frames[5][PLAYER_W * PLAYER_H] = {
    /* Frame 0: reposo */
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
    /* Frame 1: paso medio derecha */
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
    /* Frame 2: pierna izquierda adelante */
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
    /* Frame 3: igual que frame 1 (ciclo simetrico) */
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
    /* Frame 4: salto (brazos arriba) */
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

/* ----------------------------------------------------------------
 * PLATAFORMAS
 * ---------------------------------------------------------------- */

typedef struct {
    int x, y, w, h;
    unsigned char color;
} Platform;

#define PLATFORM_COUNT  5

static const Platform platforms[PLATFORM_COUNT] = {
    {   0, 185, 320,  15,  2 },   /* suelo */
    {  40, 148,  64,   8, 10 },   /* plataforma cian */
    { 140, 120,  80,   8,  9 },   /* plataforma naranja */
    { 230, 148,  64,   8, 10 },   /* plataforma cian */
    { 110,  90,  48,   8,  6 },   /* plataforma marron */
};

static void draw_platforms(void)
{
    int p, x, y;
    for (p = 0; p < PLATFORM_COUNT; p++)
        for (y = platforms[p].y; y < platforms[p].y + platforms[p].h; y++)
            for (x = platforms[p].x; x < platforms[p].x + platforms[p].w; x++)
                vga_put_pixel(x, y, platforms[p].color);
}

/* ----------------------------------------------------------------
 * JUGADOR
 * ---------------------------------------------------------------- */

#define GRAVITY      0.35f
#define JUMP_FORCE  -5.5f
#define MOVE_SPEED   1.2f
#define SFX_JUMP     0

typedef struct {
    float x, y;
    float vel_x, vel_y;
    int   on_ground;
    int   facing;
    int   anim_frame;
    int   anim_timer;
    int   moving;
    int jump_was_pressed;
} Player;

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

        if (px + PLAYER_W <= plat->x) continue;
        if (px >= plat->x + plat->w)  continue;
        if (py + PLAYER_H <= plat->y) continue;
        if (py >= plat->y + plat->h)  continue;

        ov_top    = (py + PLAYER_H) - plat->y;
        ov_bottom = (plat->y + plat->h) - py;
        ov_left   = (px + PLAYER_W) - plat->x;
        ov_right  = (plat->x + plat->w) - px;

        min = ov_top; axis = 0;
        if (ov_bottom < min) { min = ov_bottom; axis = 1; }
        if (ov_left   < min) { min = ov_left;   axis = 2; }
        if (ov_right  < min) { min = ov_right;  axis = 3; }

        switch (axis)
        {
            case 0:
                p->y         = (float)(plat->y - PLAYER_H);
                p->vel_y     = 0.0f;
                p->on_ground = 1;
                break;
            case 1:
                p->y     = (float)(plat->y + plat->h);
                p->vel_y = 0.0f;
                break;
            case 2:
                p->x     = (float)(plat->x - PLAYER_W);
                p->vel_x = 0.0f;
                break;
            case 3:
                p->x     = (float)(plat->x + plat->w);
                p->vel_x = 0.0f;
                break;
        }
    }
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
    p->jump_was_pressed = 0;
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

    if (key_pressed(KEY_SPACE) && p->on_ground && !p->jump_was_pressed)
    {
        p->vel_y     = JUMP_FORCE;
        p->on_ground = 0;
        sfx_play(SFX_JUMP, 64, MIDDLE);
        p->jump_was_pressed = 1;
    }
    if (!key_pressed(KEY_SPACE))
        p->jump_was_pressed = 0;

    p->vel_y += GRAVITY;
    p->x     += p->vel_x;
    p->y     += p->vel_y;

    resolve_collisions(p);

    if (p->x < 0)                    p->x = 0;
    if (p->x > SCREEN_W - PLAYER_W)  p->x = (float)(SCREEN_W - PLAYER_W);
    if (p->y > SCREEN_H)           { p->y = -PLAYER_H; p->vel_y = 0.0f; }

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

    frame = (!p->on_ground) ? 4 : (p->moving ? p->anim_frame : 0);

    if (p->facing == 1)
    {
        draw_sprite(sprite_frames[frame], (int)p->x, (int)p->y,
                    PLAYER_W, PLAYER_H);
    }
    else
    {
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

/* ----------------------------------------------------------------
 * MAIN
 * ---------------------------------------------------------------- */

int main(void)
{
    Player player;
    BITMAP fondo;

    engine_init();
    sound_init();

    /* Cargar fondo */
    load_bmp("fondo.bmp", &fondo);
    set_palette(fondo.palette);

    /* Cargar sonido */
    sfx_load(SFX_JUMP, "jump.wav");
    music_load_xm("music.xm");
    music_play(0);

    player_init(&player);

    while (!key_pressed(KEY_ESC))
    {
        /* Logica */
        player_update(&player);

        /* Render */
        draw_bitmap_buf(&fondo, 0, 0);   /* fondo primero, cubre toda la pantalla */
        draw_platforms();
        player_draw(&player);

        /* Flip y sonido */
        vga_flip();
        sound_update();
        timer_wait(1);
    }

    /* Limpieza */
    free_bmp(&fondo);
    sfx_free(SFX_JUMP);
    sound_shutdown();
    engine_shutdown();

    return 0;
}