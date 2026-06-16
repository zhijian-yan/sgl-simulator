#include "sgl/sgl.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// Here you can set the screen size and frame rate.
#define HOR_RES 1280
#define VER_RES 720
#define SET_FPS 60

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static sgl_screen_t scr;
static uint16_t buffer[HOR_RES * VER_RES];
static void sgl_paint(sgl_screen_t *scr);
static int fps;
static int need_refresh;

static Uint32 timer_fps_callback(Uint32 interval, void *param) {
    fps = sgl_get_fcount(&scr);
    sgl_reset_fcount(&scr);
    return interval;
}

static Uint32 timer_user_callback(Uint32 interval, void *param) {
    need_refresh = 1;
    return interval;
}

static void sgl_flush(void *buffer, sgl_rect_t *visible) {
    (void)visible;
    SDL_UpdateTexture(texture, NULL, buffer, HOR_RES * sizeof(uint16_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    // init sdl
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("sgl simulator", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, HOR_RES, VER_RES, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                                SDL_TEXTUREACCESS_STREAMING, HOR_RES, VER_RES);
    SDL_AddTimer(1000, timer_fps_callback, NULL);
    SDL_AddTimer((Uint32)(1000.0 / SET_FPS), timer_user_callback, NULL);
    // init sgl
    sgl_init(&scr, buffer, HOR_RES * VER_RES * 2, HOR_RES, VER_RES);
    sgl_set_draw_pixel(&scr, sgl_draw_pixel_rgb565);
    sgl_set_flush(&scr, sgl_flush);
    sgl_set_paint(&scr, sgl_paint);
    sgl_set_screen_rotation(&scr, SGL_ROTATE_0);
    // loop
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            }
        }
        if(need_refresh) {
            need_refresh = 0;
            sgl_handler(&scr);
        }
        SDL_Delay(1);
    }
    // exit
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

static void sgl_paint(sgl_screen_t *scr) {
    sgl_clear_screen(scr, 0x00);
    // Unleash Your Imagination, Create Without Limits!
    sgl_show_format(scr, HOR_RES - 1, 0, SGL_ALIGN_UP_RIGHT, SGL_DIR_DEFAULT,
                    0x07f0, "%dfps", fps);
}
