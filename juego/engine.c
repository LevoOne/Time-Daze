/*
 * engine.c - Motor base VGA para MS-DOS con OpenWatcom 2.0
 */

#include "engine.h"

/* Declaracion externa para evitar distorsion de musica durante fades */
extern void sound_update(void);

/* ----------------------------------------------------------------
 * VARIABLES GLOBALES
 * ---------------------------------------------------------------- */

byte *VGA = (byte *)0xA0000;

unsigned char          back_buffer[SCREEN_SIZE];
volatile unsigned char keys[128];
volatile unsigned long timer_ticks = 0;

static void (__interrupt __far *old_keyboard_handler)();
static void (__interrupt __far *old_timer_handler)();
static unsigned long last_tick = 0;

/* Paleta guardada para fade: 256 colores x 3 componentes (0..63) */
static unsigned char saved_palette[256 * 3];

/* ----------------------------------------------------------------
 * INIT / SHUTDOWN
 * ---------------------------------------------------------------- */

void engine_init(void)
{
    vga_set_mode13h();
    keyboard_install();
    timer_install();
}

void engine_shutdown(void)
{
    timer_uninstall();
    keyboard_uninstall();
    vga_set_text_mode();
}

/* ----------------------------------------------------------------
 * VIDEO
 * ---------------------------------------------------------------- */

void vga_set_mode13h(void)
{
    union REGS regs;
    regs.w.ax = 0x0013;
    int386(0x10, &regs, &regs);
}

void vga_set_text_mode(void)
{
    union REGS regs;
    regs.w.ax = 0x0003;
    int386(0x10, &regs, &regs);
}

void vga_flip(void)
{
    memcpy((unsigned char *)0xA0000, back_buffer, SCREEN_SIZE);
}

void vga_clear(unsigned char color)
{
    memset(back_buffer, color, SCREEN_SIZE);
}

void vga_clear_screen(unsigned char color)
{
    memset(VGA, color, SCREEN_SIZE);
}

void vga_put_pixel(int x, int y, unsigned char color)
{
    if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H) return;
    back_buffer[y * SCREEN_W + x] = color;
}

/* Lee la paleta VGA actual al array saved_palette */
static void vga_read_palette(void)
{
    int i;
    outp(0x3C7, 0);
    for (i = 0; i < 256 * 3; i++)
        saved_palette[i] = (unsigned char)inp(0x3C9);
}

/* Escribe una paleta escalada al hardware VGA */
/* scale: 64 = paleta completa, 0 = todo negro */
static void vga_write_palette_scaled(int scale)
{
    int i;
    outp(0x3C8, 0);
    for (i = 0; i < 256 * 3; i++)
        outp(0x3C9, (saved_palette[i] * scale) / 64);
}

void vga_fade_out(int steps, unsigned long ticks_per_step)
{
    int s;
    int step_size;

    step_size = 64 / steps;
    vga_read_palette();

    for (s = 64; s >= 0; s -= step_size)
    {
        wait_for_retrace();
        vga_write_palette_scaled(s);
        sound_update();
        timer_wait(ticks_per_step);
    }
    vga_write_palette_scaled(0);
}

void vga_fade_in(int steps, unsigned long ticks_per_step)
{
    int s;
    int step_size;

    step_size = 64 / steps;

    for (s = 0; s <= 64; s += step_size)
    {
        wait_for_retrace();
        vga_write_palette_scaled(s);
        sound_update();
        timer_wait(ticks_per_step);
    }
    vga_write_palette_scaled(64);
}

/* ----------------------------------------------------------------
 * SPRITES
 * ---------------------------------------------------------------- */

void draw_sprite(const unsigned char *data, int x, int y, int w, int h)
{
    draw_sprite_keyed(data, x, y, w, h, COLOR_TRANSPARENT);
}

void draw_sprite_keyed(const unsigned char *data, int x, int y,
                       int w, int h, unsigned char key)
{
    int sx, sy, dx, dy;
    unsigned char c;

    for (sy = 0; sy < h; sy++)
    {
        dy = y + sy;
        if (dy < 0 || dy >= SCREEN_H) continue;
        for (sx = 0; sx < w; sx++)
        {
            dx = x + sx;
            if (dx < 0 || dx >= SCREEN_W) continue;
            c = data[sy * w + sx];
            if (c == key) continue;
            back_buffer[dy * SCREEN_W + dx] = c;
        }
    }
}

/* ----------------------------------------------------------------
 * BMP
 * ---------------------------------------------------------------- */

void fskip(FILE *fp, int num_byte)
{
    int i;
    for (i = 0; i < num_byte; i++)
        fgetc(fp);
}

void load_bmp(char *file, BITMAP *b)
{
    FILE *fp;
    long  index;
    word  num_colors;
    int   x;

    if ((fp = fopen(file, "rb")) == NULL)
    {
        printf("Error al abrir %s\n", file);
        exit(1);
    }

    if (fgetc(fp) != 'B' || fgetc(fp) != 'M')
    {
        fclose(fp);
        printf("%s no es un BMP\n", file);
        exit(1);
    }

    fskip(fp, 16);
    fread(&b->width,  sizeof(word), 1, fp);
    fskip(fp, 2);
    fread(&b->height, sizeof(word), 1, fp);
    fskip(fp, 22);
    fread(&num_colors, sizeof(word), 1, fp);
    fskip(fp, 6);

    if (num_colors == 0) num_colors = 256;

    if ((b->data = (byte *)malloc((word)(b->width * b->height))) == NULL)
    {
        fclose(fp);
        printf("Error de memoria cargando %s\n", file);
        exit(1);
    }

    /* Paleta: formato BMP es B,G,R,0 -> guardamos como R,G,B escalado a 0..63 */
    for (index = 0; index < num_colors; index++)
    {
        b->palette[(int)(index * 3 + 2)] = fgetc(fp) >> 2;  /* B */
        b->palette[(int)(index * 3 + 1)] = fgetc(fp) >> 2;  /* G */
        b->palette[(int)(index * 3 + 0)] = fgetc(fp) >> 2;  /* R */
        fgetc(fp);                                            /* padding */
    }

    /* Pixeles: BMP guarda las filas de abajo a arriba */
    for (index = (b->height - 1) * b->width; index >= 0; index -= b->width)
        for (x = 0; x < b->width; x++)
            b->data[(word)index + x] = (byte)fgetc(fp);

    fclose(fp);
}

void free_bmp(BITMAP *b)
{
    if (b->data)
    {
        free(b->data);
        b->data  = NULL;
        b->width = 0;
        b->height = 0;
    }
}

void bmp_draw_tile(const BITMAP *bmp,
                   int tile_x, int tile_y,
                   int tile_w, int tile_h,
                   int dst_x,  int dst_y)
{
    int sx, sy, dx, dy;
    int src_x_base, src_y_base;
    unsigned char c;

    src_x_base = tile_x * tile_w;
    src_y_base = tile_y * tile_h;

    for (sy = 0; sy < tile_h; sy++)
    {
        dy = dst_y + sy;
        if (dy < 0 || dy >= SCREEN_H) continue;
        for (sx = 0; sx < tile_w; sx++)
        {
            dx = dst_x + sx;
            if (dx < 0 || dx >= SCREEN_W) continue;
            c = bmp->data[(src_y_base + sy) * bmp->width + (src_x_base + sx)];
            if (c == 0) continue;
            back_buffer[dy * SCREEN_W + dx] = c;
        }
    }
}

void bmp_draw_tile_opaque(const BITMAP *bmp,
                          int tile_x, int tile_y,
                          int tile_w, int tile_h,
                          int dst_x,  int dst_y)
{
    int sx, sy, dx, dy;
    int src_x_base, src_y_base;

    src_x_base = tile_x * tile_w;
    src_y_base = tile_y * tile_h;

    for (sy = 0; sy < tile_h; sy++)
    {
        dy = dst_y + sy;
        if (dy < 0 || dy >= SCREEN_H) continue;
        for (sx = 0; sx < tile_w; sx++)
        {
            dx = dst_x + sx;
            if (dx < 0 || dx >= SCREEN_W) continue;
            back_buffer[dy * SCREEN_W + dx] =
                bmp->data[(src_y_base + sy) * bmp->width + (src_x_base + sx)];
        }
    }
}

void set_palette(byte *palette)
{
    int i;
    /* Guardar en saved_palette para que vga_fade_in use la paleta correcta */
    for (i = 0; i < 256 * 3; i++)
        saved_palette[i] = palette[i];
    outp(0x3C8, 0);
    for (i = 0; i < 256 * 3; i++)
        outp(0x3C9, palette[i]);
}

/* Inyecta colores en saved_palette sin tocar el hardware.
 * start_index: indice de inicio (0..255)
 * colors: array de bytes en formato R,G,B (0..63)
 * count: numero de colores a inyectar */
void palette_inject(int start_index, const byte *colors, int count)
{
    int i;
    int base;

    base = start_index * 3;
    for (i = 0; i < count * 3; i++)
        saved_palette[base + i] = colors[i];
}

void set_palette_silent(byte *palette)
{
    int i;
    for (i = 0; i < 256 * 3; i++)
        saved_palette[i] = palette[i];
}

void rotate_palette(byte *palette)
{
    int  i;
    byte red, green, blue;

    /* Guarda el primer color (indice 1, saltamos el 0 que es transparente) */
    red   = palette[3];
    green = palette[4];
    blue  = palette[5];

    /* Desplaza todos los colores un paso hacia la izquierda */
    for (i = 3; i < 256 * 3 - 3; i++)
        palette[i] = palette[i + 3];

    /* El ultimo color toma el valor del primero (rotacion circular) */
    palette[256 * 3 - 3] = red;
    palette[256 * 3 - 2] = green;
    palette[256 * 3 - 1] = blue;

    set_palette(palette);
}

void wait_for_retrace(void)
{
    /* Espera a que termine el retrace actual */
    while  ( inp(0x3DA) & 0x08) {}
    /* Espera al inicio del siguiente retrace */
    while (!(inp(0x3DA) & 0x08)) {}
}

/* ----------------------------------------------------------------
 * PRIMITIVAS DE DIBUJO
 * ---------------------------------------------------------------- */

void draw_line(int x1, int y1, int x2, int y2, unsigned char color)
{
    int dx, dy, sx, sy, err, e2;

    dx  = x2 - x1;
    if (dx < 0) dx = -dx;
    dy  = y2 - y1;
    if (dy < 0) dy = -dy;

    sx  = (x1 < x2) ? 1 : -1;
    sy  = (y1 < y2) ? 1 : -1;
    err = dx - dy;

    while (1)
    {
        vga_put_pixel(x1, y1, color);

        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

void pixel_plot(int x, int y, int color)
{
    /* Escribe directamente en VRAM sin usar el back buffer */
    if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H) return;
    VGA[y * SCREEN_W + x] = (byte)color;
}

void pixel_plot_fast(int x, int y, int color)
{
    /* Acceso directo sin comprobacion de limites, maximo rendimiento */
    VGA[SCREEN_W * y + x] = (byte)color;
}

/* ----------------------------------------------------------------
 * TEXTO EN MODO GRAFICO - FUENTE 8x8 DEL BIOS
 * ---------------------------------------------------------------- */

static unsigned char *bios_font = NULL;

void font_init(void)
{
    /* El vector 0x43 apunta a la fuente de caracteres del BIOS        */
    /* En DOS/4GW el primer megabyte esta mapeado en el espacio flat   */
    /* El vector esta en 0x0000:0x010C (0x43 * 4 = 0x10C)             */
    unsigned long seg;
    unsigned long off;
    unsigned long *ivt;

    ivt = (unsigned long *)0x0000010CUL;
    off = (*ivt) & 0x0000FFFFUL;
    seg = ((*ivt) >> 16) & 0x0000FFFFUL;

    bios_font = (unsigned char *)(seg * 16 + off);
}

void draw_char(char c, int x, int y, unsigned char color)
{
    int           row, col;
    unsigned char *glyph;
    unsigned char bits;
    int           px, py;

    if (bios_font == NULL) return;

    glyph = bios_font + (unsigned char)c * 8;

    for (row = 0; row < 8; row++)
    {
        bits = glyph[row];
        py   = y + row;
        if (py < 0 || py >= SCREEN_H) continue;

        for (col = 0; col < 8; col++)
        {
            if (bits & (0x80 >> col))
            {
                px = x + col;
                if (px >= 0 && px < SCREEN_W)
                    back_buffer[py * SCREEN_W + px] = color;
            }
        }
    }
}

void draw_string(const char *s, int x, int y, unsigned char color)
{
    int i;
    for (i = 0; s[i] != '\0'; i++)
        draw_char(s[i], x + i * 8, y, color);
}

void draw_int(int n, int x, int y, unsigned char color)
{
    char buf[12];
    itoa(n, buf, 10);
    draw_string(buf, x, y, color);
}

/* Dibuja en VRAM directamente (sin back buffer) */
void draw_bitmap(BITMAP *bmp, int x, int y)
{
    int  j;
    word screen_offset;
    word bitmap_offset;

    screen_offset = (word)((y << 8) + (y << 6) + x);
    bitmap_offset = 0;

    for (j = 0; j < bmp->height; j++)
    {
        memcpy(&VGA[screen_offset], &bmp->data[bitmap_offset], bmp->width);
        bitmap_offset  += bmp->width;
        screen_offset  += SCREEN_W;
    }
}

/* Dibuja en VRAM directamente con transparencia (color 0) */
void draw_transparent_bitmap(BITMAP *bmp, int x, int y)
{
    int  i, j;
    word screen_offset;
    word bitmap_offset;
    byte data;

    screen_offset = (word)((y << 8) + (y << 6));
    bitmap_offset = 0;

    for (j = 0; j < bmp->height; j++)
    {
        for (i = 0; i < bmp->width; i++, bitmap_offset++)
        {
            data = bmp->data[bitmap_offset];
            if (data) VGA[screen_offset + x + i] = data;
        }
        screen_offset += SCREEN_W;
    }
}

/* Dibuja en back buffer */
void draw_bitmap_buf(BITMAP *bmp, int x, int y)
{
    int j;
    int dst, src;

    /* Caso optimizado: bitmap del mismo tamano que la pantalla en (0,0) */
    if (x == 0 && y == 0 && bmp->width == SCREEN_W && bmp->height == SCREEN_H)
    {
        memcpy(back_buffer, bmp->data, SCREEN_SIZE);
        return;
    }

    /* Caso general */
    src = 0;
    for (j = 0; j < bmp->height; j++)
    {
        if (y + j >= 0 && y + j < SCREEN_H)
        {
            dst = (y + j) * SCREEN_W + x;
            memcpy(&back_buffer[dst], &bmp->data[src], bmp->width);
        }
        src += bmp->width;
    }
}

/* Igual que draw_bitmap_buf pero el indice 0 es transparente */
void draw_bitmap_buf_t(BITMAP *bmp, int x, int y)
{
    int i, j;
    int dx;
    byte c;

    for (j = 0; j < bmp->height; j++)
    {
        if (y + j < 0 || y + j >= SCREEN_H) continue;
        for (i = 0; i < bmp->width; i++)
        {
            dx = x + i;
            if (dx < 0 || dx >= SCREEN_W) continue;
            c = bmp->data[j * bmp->width + i];
            if (c == 0) continue;
            back_buffer[(y + j) * SCREEN_W + dx] = c;
        }
    }
}

/* Dibuja en back buffer con transparencia (color 0) */
void draw_transparent_bitmap_buf(BITMAP *bmp, int x, int y)
{
    int  i, j;
    int  dst_row, src;
    byte data;

    src = 0;
    for (j = 0; j < bmp->height; j++)
    {
        dst_row = (y + j) * SCREEN_W;
        if (y + j >= 0 && y + j < SCREEN_H)
        {
            for (i = 0; i < bmp->width; i++, src++)
            {
                data = bmp->data[src];
                if (data && (x + i) >= 0 && (x + i) < SCREEN_W)
                    back_buffer[dst_row + x + i] = data;
            }
        }
        else
        {
            src += bmp->width;
        }
    }
}

/* ----------------------------------------------------------------
 * TECLADO
 * ---------------------------------------------------------------- */

static void __interrupt __far keyboard_handler(void)
{
    unsigned char scancode;
    scancode = inp(0x60);
    if (scancode & 0x80)
        keys[scancode & 0x7F] = 0;
    else if (scancode < 128)
        keys[scancode] = 1;
    outp(0x20, 0x20);
}

void keyboard_install(void)
{
    memset((void *)keys, 0, sizeof(keys));
    old_keyboard_handler = _dos_getvect(9);
    _dos_setvect(9, keyboard_handler);
}

void keyboard_uninstall(void)
{
    _dos_setvect(9, old_keyboard_handler);
}

int key_pressed(unsigned char scancode)
{
    return keys[scancode] != 0;
}

/* ----------------------------------------------------------------
 * TIMER
 * ---------------------------------------------------------------- */

#define TIMER_DIVISOR   17045
#define BIOS_CALL_RATE  4

static unsigned char bios_counter = 0;

static void __interrupt __far timer_handler(void)
{
    timer_ticks++;
    bios_counter++;
    if (bios_counter >= BIOS_CALL_RATE)
    {
        bios_counter = 0;
        _chain_intr(old_timer_handler);
    }
    else
    {
        outp(0x20, 0x20);
    }
}

void timer_install(void)
{
    old_timer_handler = _dos_getvect(8);
    _dos_setvect(8, timer_handler);
    outp(0x43, 0x36);
    outp(0x40, TIMER_DIVISOR & 0xFF);
    outp(0x40, (TIMER_DIVISOR >> 8) & 0xFF);
    last_tick = timer_ticks;
}

void timer_uninstall(void)
{
    outp(0x43, 0x36);
    outp(0x40, 0);
    outp(0x40, 0);
    _dos_setvect(8, old_timer_handler);
}

void timer_wait(unsigned long ticks_per_frame)
{
    while (timer_ticks - last_tick < ticks_per_frame) {}
    last_tick = timer_ticks;
}

/* ----------------------------------------------------------------
 * PC SPEAKER
 * ---------------------------------------------------------------- */

void speaker_beep(unsigned int freq, unsigned int duration_ms)
{
    unsigned int divisor;

    if (freq == 0) return;

    divisor = (unsigned int)(1193180UL / freq);

    /* Activar el speaker y conectarlo al PIT canal 2 */
    outp(0x43, 0xB6);
    outp(0x42, divisor & 0xFF);
    outp(0x42, (divisor >> 8) & 0xFF);
    outp(0x61, inp(0x61) | 0x03);

    delay(duration_ms);

    speaker_stop();
}

void speaker_stop(void)
{
    outp(0x61, inp(0x61) & 0xFC);
}

/* ----------------------------------------------------------------
 * SONIDO - JUDAS
 * ---------------------------------------------------------------- */

static int          sound_ready  = 0;
static SAMPLE_t    *sfx_samples[SFX_MAX];
int                 music_format = MUSIC_NONE;

int sound_init(void)
{
    int i;

    for (i = 0; i < SFX_MAX; i++)
        sfx_samples[i] = NULL;

    music_format = MUSIC_NONE;

    judas_config();

    if (!judas_init(22050, FASTMIXER, STEREO | SIXTEENBIT, 0))
    {
        sound_ready = 0;
        return 0;
    }

    judas_preventdistortion(SFX_CHANNELS);
    sound_ready = 1;
    return 1;
}

void sound_update(void)
{
    if (sound_ready)
        judas_update();
}

void sound_shutdown(void)
{
    int i;

    if (!sound_ready) return;

    music_stop();
    music_free();

    for (i = 0; i < SFX_MAX; i++)
        sfx_free(i);

    judas_uninit();
    sound_ready = 0;
}

int sound_available(void)
{
    return sound_ready;
}

int sfx_load(int index, char *filename)
{
    if (!sound_ready)          return 0;
    if (index < 0 || index >= SFX_MAX) return 0;

    if (sfx_samples[index])
        judas_freesample(sfx_samples[index]);

    sfx_samples[index] = judas_loadwav(filename);
    return (sfx_samples[index] != NULL) ? 1 : 0;
}

void sfx_free(int index)
{
    if (index < 0 || index >= SFX_MAX) return;
    if (sfx_samples[index])
    {
        judas_freesample(sfx_samples[index]);
        sfx_samples[index] = NULL;
    }
}

void sfx_play(int index, unsigned short volume, unsigned char panning)
{
    int channel;
    if (!sound_ready)                return;
    if (index < 0 || index >= SFX_MAX) return;
    if (!sfx_samples[index])         return;

    /*
     * Judas volume range: 0..64*256 (16384 = maximo volumen)
     * El parametro volume de esta funcion acepta 0..64 para
     * compatibilidad intuitiva, y lo escalamos internamente.
     */
    channel = SFX_FIRST + (index % SFX_CHANNELS);
    judas_playsample(sfx_samples[index], channel,
                     judas_mixrate,
                     (unsigned short)(volume * 256),
                     panning);
}

void sfx_stop(int index)
{
    if (!sound_ready) return;
    if (index < 0 || index >= SFX_MAX) return;
    judas_stopsample(SFX_FIRST + (index % SFX_CHANNELS));
}

int music_load_xm(char *filename)
{
    if (!sound_ready) return 0;
    music_free();
    if (!judas_loadxm(filename)) return 0;
    music_format = MUSIC_XM;
    return 1;
}

int music_load_mod(char *filename)
{
    if (!sound_ready) return 0;
    music_free();
    if (!judas_loadmod(filename)) return 0;
    music_format = MUSIC_MOD;
    return 1;
}

int music_load_s3m(char *filename)
{
    if (!sound_ready) return 0;
    music_free();
    if (!judas_loads3m(filename)) return 0;
    music_format = MUSIC_S3M;
    return 1;
}

void music_play(int rounds)
{
    if (!sound_ready) return;
    switch (music_format)
    {
        case MUSIC_XM:  judas_playxm(rounds);  break;
        case MUSIC_MOD: judas_playmod(rounds); break;
        case MUSIC_S3M: judas_plays3m(rounds); break;
    }
}

void music_stop(void)
{
    if (!sound_ready) return;
    switch (music_format)
    {
        case MUSIC_XM:  judas_stopxm();  break;
        case MUSIC_MOD: judas_stopmod(); break;
        case MUSIC_S3M: judas_stops3m(); break;
    }
}

void music_free(void)
{
    if (!sound_ready) return;
    switch (music_format)
    {
        case MUSIC_XM:  judas_freexm();  break;
        case MUSIC_MOD: judas_freemod(); break;
        case MUSIC_S3M: judas_frees3m(); break;
    }
    music_format = MUSIC_NONE;
}
