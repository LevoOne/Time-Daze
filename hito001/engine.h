/*
 * engine.h - Motor base VGA para MS-DOS con OpenWatcom 2.0
 *
 * Subsistemas:
 *   - Video VGA modo 13h (320x200, NUM_COLORS colores)
 *   - Double buffering
 *   - Sprites con transparencia y clipping
 *   - BMP indexado de NUM_COLORS colores
 *   - Teclado por interrupciones (puerto 0x60)
 *   - Timer a 70 Hz (PIT reprogramado)
 *   - PC Speaker (efectos simples)
 *   - Judas (efectos WAV y musica XM/MOD/S3M)
 *
 * Uso del sonido:
 *   1. Llama a sound_init() despues de vga_set_mode13h()
 *   2. Carga efectos con sfx_load(), musica con music_load()
 *   3. Llama a sound_update() una vez por tick en el game loop
 *   4. Llama a sound_shutdown() antes de salir
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <dos.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "judas.h"

/* ----------------------------------------------------------------
 * TIPOS
 * ---------------------------------------------------------------- */
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

/* ----------------------------------------------------------------
 * VIDEO - CONSTANTES
 * ---------------------------------------------------------------- */
#define SCREEN_W        320
#define SCREEN_H        200
#define SCREEN_SIZE     (SCREEN_W * SCREEN_H)
#define NUM_COLORS      256
#define VRETRACE        0x08
#define COLOR_TRANSPARENT  0

/* ----------------------------------------------------------------
 * PALETA - CONSTANTES
 * ---------------------------------------------------------------- */
#define PALETTE_INDEX   0x03c8
#define PALETTE_DATA    0x03c9
#define INPUT_STATUS    0x03da

/* Puntero directo a VRAM (usado por draw_bitmap) */
extern byte *VGA;

/* Back buffer: todo se dibuja aqui, vga_flip() lo vuelca a VRAM */
extern unsigned char back_buffer[SCREEN_SIZE];

/* ----------------------------------------------------------------
 * INIT / SHUTDOWN
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * engine_init()
 *   Inicializa los subsistemas principales en el orden correcto:
 *   modo VGA 13h, teclado y timer.
 *   El sonido se inicializa por separado con sound_init().
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void engine_init(void);

/* -----------------------------------------------------------------------------------------
 * engine_shutdown()
 *   Libera los subsistemas principales en orden inverso al de init:
 *   timer, teclado y modo texto.
 *   El sonido debe liberarse antes con sound_shutdown().
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void engine_shutdown(void);

/* ----------------------------------------------------------------
 * VIDEO - FUNCIONES
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * vga_set_mode13h()
 *   Establece el modo de video 13h (320x200, 256 colores).
 *   Debe llamarse al inicio del programa antes de cualquier operacion grafica.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void vga_set_mode13h(void);

/* -----------------------------------------------------------------------------------------
 * vga_set_text_mode()
 *   Restaura el modo texto 80x25.
 *   Debe llamarse antes de salir del programa.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void vga_set_text_mode(void);

/* -----------------------------------------------------------------------------------------
 * vga_flip()
 *   Vuelca el contenido del back buffer a la VRAM de una sola vez.
 *   Llamar una vez al final de cada frame, despues de dibujar todo.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void vga_flip(void);

/* -----------------------------------------------------------------------------------------
 * vga_clear()
 *   Limpia el back buffer rellenandolo con un color uniforme.
 *   Llamar al inicio del bloque de render de cada frame.
 * Entrada: color (byte) = indice de color VGA (0..255)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void vga_clear(unsigned char color);

/* -----------------------------------------------------------------------------------------
 * vga_put_pixel()
 *   Dibuja un pixel en el back buffer con comprobacion de limites.
 * Entrada: x, y  (int)  = coordenadas en pantalla
 *          color (byte) = indice de color VGA (0..255)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void vga_put_pixel(int x, int y, unsigned char color);

/* -----------------------------------------------------------------------------------------
 * vga_fade_out()
 *   Oscurece la paleta gradualmente hasta negro.
 *   Guarda internamente la paleta actual antes de oscurecer.
 * Entrada: steps          (int)           = numero de pasos (16 = suave, 8 = rapido)
 *          ticks_per_step (unsigned long)  = ticks entre pasos (7 = ~0.1s a 70Hz)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void vga_fade_out(int steps, unsigned long ticks_per_step);

/* -----------------------------------------------------------------------------------------
 * vga_fade_in()
 *   Aclara la paleta desde negro hasta la paleta guardada por vga_fade_out().
 *   Debe llamarse despues de vga_fade_out() para restaurar los colores.
 * Entrada: steps          (int)           = numero de pasos (16 = suave, 8 = rapido)
 *          ticks_per_step (unsigned long)  = ticks entre pasos (7 = ~0.1s a 70Hz)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void vga_fade_in(int steps, unsigned long ticks_per_step);

/* ----------------------------------------------------------------
 * SPRITES
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * draw_sprite()
 *   Dibuja un sprite en el back buffer usando el color 0 como transparente.
 * Entrada: data    (byte*) = puntero a los datos del sprite (w*h bytes)
 *          x, y   (int)   = posicion destino (puede ser negativa, se recorta)
 *          w, h   (int)   = dimensiones del sprite en pixeles
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void draw_sprite(const unsigned char *data, int x, int y, int w, int h);

/* -----------------------------------------------------------------------------------------
 * draw_sprite_keyed()
 *   Dibuja un sprite en el back buffer con color de transparencia personalizado.
 * Entrada: data    (byte*)  = puntero a los datos del sprite
 *          x, y   (int)    = posicion destino
 *          w, h   (int)    = dimensiones del sprite
 *          key    (byte)   = indice de color que se tratara como transparente
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void draw_sprite_keyed(const unsigned char *data, int x, int y,
                       int w, int h, unsigned char key);

/* ----------------------------------------------------------------
 * BMP
 * ---------------------------------------------------------------- */
typedef struct
{
    word  width;
    word  height;
    byte  palette[NUM_COLORS * 3];
    byte *data;
} BITMAP;

/* -----------------------------------------------------------------------------------------
 * load_bmp()
 *   Carga un fichero BMP de 8 bits indexado (256 colores) desde disco.
 *   Reserva memoria para los pixeles. Llamar a free_bmp() cuando ya no se necesite.
 * Entrada: file (char*)   = nombre del fichero BMP
 *          b    (BITMAP*) = puntero a la estructura donde cargar el bitmap
 * Salida:  ninguna (termina el programa si hay error)
 * -----------------------------------------------------------------------------------------*/
void load_bmp(char *file, BITMAP *b);

/* -----------------------------------------------------------------------------------------
 * free_bmp()
 *   Libera la memoria reservada por load_bmp().
 * Entrada: b (BITMAP*) = puntero al bitmap a liberar
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void free_bmp(BITMAP *b);

/* -----------------------------------------------------------------------------------------
 * fskip()
 *   Salta un numero de bytes en un fichero abierto.
 *   Uso interno de load_bmp().
 * Entrada: fp       (FILE*) = puntero al fichero
 *          num_byte (int)   = numero de bytes a saltar
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void fskip(FILE *fp, int num_byte);

/* -----------------------------------------------------------------------------------------
 * draw_bitmap()
 *   Dibuja un bitmap completo directamente en la VRAM (sin back buffer).
 *   Usar solo fuera del ciclo de double buffering.
 * Entrada: bmp  (BITMAP*) = puntero al bitmap
 *          x, y (int)     = posicion destino en pantalla
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void draw_bitmap(BITMAP *bmp, int x, int y);

/* -----------------------------------------------------------------------------------------
 * draw_transparent_bitmap()
 *   Dibuja un bitmap en la VRAM con el color 0 como transparente.
 *   Usar solo fuera del ciclo de double buffering.
 * Entrada: bmp  (BITMAP*) = puntero al bitmap
 *          x, y (int)     = posicion destino en pantalla
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void draw_transparent_bitmap(BITMAP *bmp, int x, int y);

/* -----------------------------------------------------------------------------------------
 * draw_bitmap_buf()
 *   Dibuja un bitmap completo en el back buffer.
 *   Usar dentro del ciclo de double buffering.
 * Entrada: bmp  (BITMAP*) = puntero al bitmap
 *          x, y (int)     = posicion destino en pantalla
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void draw_bitmap_buf(BITMAP *bmp, int x, int y);

/* -----------------------------------------------------------------------------------------
 * draw_transparent_bitmap_buf()
 *   Dibuja un bitmap en el back buffer con el color 0 como transparente.
 *   Usar dentro del ciclo de double buffering.
 * Entrada: bmp  (BITMAP*) = puntero al bitmap
 *          x, y (int)     = posicion destino en pantalla
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void draw_transparent_bitmap_buf(BITMAP *bmp, int x, int y);

/* -----------------------------------------------------------------------------------------
 * bmp_draw_tile()
 *   Dibuja un tile extraido de un spritesheet en el back buffer con transparencia (color 0).
 *   Los tiles se organizan en filas de izquierda a derecha.
 *   Indice = tile_col + tile_row * (bmp->width / tile_w)
 * Entrada: bmp    (BITMAP*) = puntero al spritesheet
 *          tile_x (int)    = columna del tile en la cuadricula (0, 1, 2...)
 *          tile_y (int)    = fila del tile en la cuadricula (0, 1, 2...)
 *          tile_w (int)    = ancho del tile en pixeles
 *          tile_h (int)    = alto del tile en pixeles
 *          dst_x  (int)    = posicion destino x en pantalla
 *          dst_y  (int)    = posicion destino y en pantalla
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void bmp_draw_tile(const BITMAP *bmp,
                   int tile_x, int tile_y,
                   int tile_w, int tile_h,
                   int dst_x,  int dst_y);

/* -----------------------------------------------------------------------------------------
 * bmp_draw_tile_opaque()
 *   Igual que bmp_draw_tile pero sin transparencia.
 *   Util para tiles de fondo que rellenan toda la celda.
 * Entrada: bmp    (BITMAP*) = puntero al spritesheet
 *          tile_x (int)    = columna del tile en la cuadricula
 *          tile_y (int)    = fila del tile en la cuadricula
 *          tile_w (int)    = ancho del tile en pixeles
 *          tile_h (int)    = alto del tile en pixeles
 *          dst_x  (int)    = posicion destino x en pantalla
 *          dst_y  (int)    = posicion destino y en pantalla
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void bmp_draw_tile_opaque(const BITMAP *bmp,
                          int tile_x, int tile_y,
                          int tile_w, int tile_h,
                          int dst_x,  int dst_y);

/* -----------------------------------------------------------------------------------------                          
 * Los valores deben estar en rango 0..63 (6 bits por componente).
 * Entrada: palette (byte*) = array de 256*3 bytes en formato R,G,B
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void set_palette(byte *palette);

/* -----------------------------------------------------------------------------------------
 * rotate_palette()
 *   Rota los colores de la paleta un paso hacia la izquierda y la aplica al hardware.
 *   Util para efectos de agua, fuego o animaciones de paleta ciclica.
 *   El indice 0 (transparente) no se rota.
 * Entrada: palette (byte*) = array de 256*3 bytes que se modifica in-place
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void rotate_palette(byte *palette);

/* -----------------------------------------------------------------------------------------
 * wait_for_retrace()
 *   Espera al inicio del siguiente pulso de retrace vertical del monitor (60 Hz).
 *   Usar antes de set_palette() o rotate_palette() para evitar flickering de colores.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void wait_for_retrace(void);

/* ----------------------------------------------------------------
 * PRIMITIVAS DE DIBUJO
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * draw_line()
 *   Dibuja una linea en el back buffer usando el algoritmo de Bresenham.
 * Entrada: x1, y1 (int)  = coordenadas del punto inicial
 *          x2, y2 (int)  = coordenadas del punto final
 *          color  (byte) = indice de color VGA (0..255)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void draw_line(int x1, int y1, int x2, int y2, unsigned char color);

/* -----------------------------------------------------------------------------------------
 * pixel_plot()
 *   Dibuja un pixel directamente en la VRAM con comprobacion de limites.
 *   No usa el back buffer. Compatible con el codigo del tutorial de Brackeen.
 * Entrada: x, y  (int) = coordenadas en pantalla
 *          color (int) = indice de color VGA (0..255)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void pixel_plot(int x, int y, int color);

/* -----------------------------------------------------------------------------------------
 * pixel_plot_fast()
 *   Dibuja un pixel directamente en la VRAM sin comprobacion de limites.
 *   Maximo rendimiento. Asegurate de que x e y estan dentro de pantalla.
 * Entrada: x, y  (int) = coordenadas en pantalla (0..319, 0..199)
 *          color (int) = indice de color VGA (0..255)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void pixel_plot_fast(int x, int y, int color);

/* ----------------------------------------------------------------
 * TECLADO
 * ---------------------------------------------------------------- */
#define KEY_ESC        0x01
#define KEY_1          0x02
#define KEY_2          0x03
#define KEY_LEFT       0x4B
#define KEY_RIGHT      0x4D
#define KEY_UP         0x48
#define KEY_DOWN       0x50
#define KEY_SPACE      0x39
#define KEY_ENTER      0x1C
#define KEY_CTRL       0x1D
#define KEY_ALT        0x38

extern volatile unsigned char keys[128];

/* -----------------------------------------------------------------------------------------
 * keyboard_install()
 *   Instala el manejador de teclado por interrupciones (IRQ1, puerto 0x60).
 *   Llamar al inicio del programa, despues de vga_set_mode13h().
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void keyboard_install(void);

/* -----------------------------------------------------------------------------------------
 * keyboard_uninstall()
 *   Desinstala el manejador de teclado y restaura el original del BIOS.
 *   Llamar antes de salir del programa.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void keyboard_uninstall(void);

/* -----------------------------------------------------------------------------------------
 * key_pressed()
 *   Devuelve 1 si la tecla con el scancode indicado esta actualmente pulsada.
 * Entrada: scancode (byte) = scancode de la tecla (usar las constantes KEY_*)
 * Salida:  1 si pulsada, 0 si libre
 * -----------------------------------------------------------------------------------------*/
int key_pressed(unsigned char scancode);

/* ----------------------------------------------------------------
 * TIMER
 * ---------------------------------------------------------------- */
extern volatile unsigned long timer_ticks;

/* -----------------------------------------------------------------------------------------
 * timer_install()
 *   Instala el timer reprogramando el PIT a ~70 Hz.
 *   Llamar al inicio del programa.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void timer_install(void);

/* -----------------------------------------------------------------------------------------
 * timer_uninstall()
 *   Desinstala el timer y restaura el PIT a su frecuencia original (~18.2 Hz).
 *   Llamar antes de salir del programa.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void timer_uninstall(void);

/* -----------------------------------------------------------------------------------------
 * timer_wait()
 *   Espera hasta que hayan transcurrido ticks_per_frame ticks desde la ultima llamada.
 *   Usar al final del game loop para limitar la velocidad del juego.
 *   A 70 Hz: ticks_per_frame=1 para 70 fps, ticks_per_frame=2 para 35 fps.
 * Entrada: ticks_per_frame (unsigned long) = ticks a esperar
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void timer_wait(unsigned long ticks_per_frame);

/* ----------------------------------------------------------------
 * PC SPEAKER
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * speaker_beep()
 *   Emite un pitido por el PC Speaker durante un tiempo determinado.
 * Entrada: freq        (unsigned int) = frecuencia en Hz
 *          duration_ms (unsigned int) = duracion en milisegundos
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void speaker_beep(unsigned int freq, unsigned int duration_ms);

/* -----------------------------------------------------------------------------------------
 * speaker_stop()
 *   Para el PC Speaker inmediatamente.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void speaker_stop(void);

/* ----------------------------------------------------------------
 * SONIDO - JUDAS
 *
 * Canales 0..(32-SFX_CHANNELS-1) : usados por la musica XM/MOD/S3M
 * Canales SFX_FIRST..31          : reservados para efectos WAV
 *
 * Flujo de uso:
 *   sound_init()
 *   sfx_load(SFX_JUMP, "jump.wav")
 *   music_load_xm("level1.xm")
 *   music_play(0)
 *   -- game loop --
 *   sfx_play(SFX_JUMP, 64, MIDDLE)
 *   sound_update()
 *   -- fin --
 *   sound_shutdown()
 * ---------------------------------------------------------------- */

#define SFX_CHANNELS   4
#define SFX_FIRST      (32 - SFX_CHANNELS)
#define SFX_MAX        16

/* -----------------------------------------------------------------------------------------
 * sound_init()
 *   Inicializa Judas: detecta la Sound Blaster via variable de entorno BLASTER,
 *   inicializa el mixer a 22050 Hz estereo 16 bits y prepara los canales.
 *   Si falla, el juego continua en modo NOSOUND sin sonido.
 * Entrada: ninguna
 * Salida:  1 si Sound Blaster detectada e inicializada, 0 si modo NOSOUND
 * -----------------------------------------------------------------------------------------*/
int sound_init(void);

/* -----------------------------------------------------------------------------------------
 * sound_update()
 *   Rellena el buffer DMA de la Sound Blaster con nuevos datos de audio.
 *   Debe llamarse una vez por tick en el game loop para evitar cortes en el sonido.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void sound_update(void);

/* -----------------------------------------------------------------------------------------
 * sound_shutdown()
 *   Para toda la musica y efectos, libera los samples y desinicializa Judas.
 *   Llamar antes de salir del programa.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void sound_shutdown(void);

/* -----------------------------------------------------------------------------------------
 * sound_available()
 *   Indica si el sonido esta activo (Sound Blaster detectada correctamente).
 * Entrada: ninguna
 * Salida:  1 si sonido disponible, 0 si modo NOSOUND
 * -----------------------------------------------------------------------------------------*/
int sound_available(void);

/* -----------------------------------------------------------------------------------------
 * sfx_load()
 *   Carga un fichero WAV y lo asocia a un indice de efecto (0..SFX_MAX-1).
 * Entrada: index    (int)   = indice del efecto (usar constantes SFX_* propias)
 *          filename (char*) = nombre del fichero WAV
 * Salida:  1 si OK, 0 si error
 * -----------------------------------------------------------------------------------------*/
int sfx_load(int index, char *filename);

/* -----------------------------------------------------------------------------------------
 * sfx_free()
 *   Libera la memoria de un efecto de sonido cargado.
 * Entrada: index (int) = indice del efecto a liberar
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void sfx_free(int index);

/* -----------------------------------------------------------------------------------------
 * sfx_play()
 *   Reproduce un efecto de sonido previamente cargado con sfx_load().
 * Entrada: index   (int)   = indice del efecto
 *          volume  (short) = volumen (0..64, donde 64 es el maximo)
 *          panning (byte)  = paneo (LEFT=0, MIDDLE=128, RIGHT=255)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void sfx_play(int index, unsigned short volume, unsigned char panning);

/* -----------------------------------------------------------------------------------------
 * sfx_stop()
 *   Para la reproduccion de un efecto de sonido.
 * Entrada: index (int) = indice del efecto a parar
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void sfx_stop(int index);

/* -----------------------------------------------------------------------------------------
 * music_load_xm()
 *   Carga un fichero XM (FastTracker 2) para reproduccion.
 *   Llama a music_free() automaticamente si habia musica cargada.
 * Entrada: filename (char*) = nombre del fichero XM
 * Salida:  1 si OK, 0 si error
 * -----------------------------------------------------------------------------------------*/
int music_load_xm(char *filename);

/* -----------------------------------------------------------------------------------------
 * music_load_mod()
 *   Carga un fichero MOD para reproduccion.
 * Entrada: filename (char*) = nombre del fichero MOD
 * Salida:  1 si OK, 0 si error
 * -----------------------------------------------------------------------------------------*/
int music_load_mod(char *filename);

/* -----------------------------------------------------------------------------------------
 * music_load_s3m()
 *   Carga un fichero S3M (ScreamTracker 3) para reproduccion.
 * Entrada: filename (char*) = nombre del fichero S3M
 * Salida:  1 si OK, 0 si error
 * -----------------------------------------------------------------------------------------*/
int music_load_s3m(char *filename);

/* -----------------------------------------------------------------------------------------
 * music_play()
 *   Inicia la reproduccion de la musica cargada.
 * Entrada: rounds (int) = 0 para bucle infinito, 1 para reproducir una vez
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void music_play(int rounds);

/* -----------------------------------------------------------------------------------------
 * music_stop()
 *   Para la reproduccion de la musica sin liberarla de memoria.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void music_stop(void);

/* -----------------------------------------------------------------------------------------
 * music_free()
 *   Libera de memoria la musica cargada actualmente.
 *   Llamar antes de cargar una nueva pista o al salir.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void music_free(void);

#define MUSIC_NONE  0
#define MUSIC_XM    1
#define MUSIC_MOD   2
#define MUSIC_S3M   3

extern int music_format;

#endif /* ENGINE_H */
