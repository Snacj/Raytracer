#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>

#define WIDTH 1200
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_RAY 0xebe134

struct Circle {
    double x;
    double y;
    double radius;
};

void drawCircle(SDL_Surface* surface, struct Circle circle, Uint32 color) {
    double radius_squared = pow(circle.radius, 2);
    for (double x = circle.x - circle.radius; x<=circle.x + circle.radius; x++) {
        for (double y = circle.y - circle.radius; y<=circle.y + circle.radius; y++) {
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if ( distance_squared < radius_squared ) {
                SDL_Rect pixel = (SDL_Rect) {x,y,1,1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

void gen_rays(SDL_Surface* surface, struct Circle circle, struct Circle obstacle, double density, Uint32 color) {
    double rad_sq = pow(circle.radius, 2);
    for (double i = 0; i < density; i++) {
        double angle = i * (2 * M_PI / density);
        for (double step = 0; step < WIDTH; step++) {
            double x = circle.x + step * cos(angle);
            double y = circle.y + step * sin(angle);
            double dist_sq = pow(x - obstacle.x, 2) + pow(y - obstacle.y, 2);
            if (dist_sq <= pow(obstacle.radius, 2)) {
                break;
            }
            SDL_Rect pixel = (SDL_Rect) {x,y,1,1};
            SDL_FillRect(surface, &pixel, color);
        }
    }


}

int main (int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    struct Circle circle = {300,300,80};
    struct Circle obstacle = {850,300, 120};
    SDL_Rect erase_rect = {0,0,WIDTH,HEIGHT};

    int sim_running = 1;
    SDL_Event event;
    while (sim_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                sim_running = 0;
            } 
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0) {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
            } 
        }
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
        gen_rays(surface, circle, obstacle, 400, COLOR_RAY);
        drawCircle(surface, circle, COLOR_WHITE);
        drawCircle(surface, obstacle, COLOR_WHITE);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }


    return 0;
}
