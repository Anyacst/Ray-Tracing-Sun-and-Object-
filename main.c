#include </opt/homebrew/include/SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define COLOR_WHITE 0xFFFFFFFF
#define WIDTH 1000
#define HEIGHT 900
#define COLOR_BLACK 0x00000000
#define RAY_NUMBER 100
#define COLOR_GRAY 0xefefefef
#define COLOR_YELLOW 0xFFEE8C
#define M_PIE 3.14
#define SPEED 3

struct Circle {
    int x;
    int y;
    int r;
};

void draw_circle(SDL_Surface *surface, struct Circle circle, Uint32 color);
void Generate_rays(SDL_Surface *surface, struct Circle circle, Uint32 color);
void MovingSecondCircle(SDL_Surface *surface, int *upward, int *downward, struct Circle *circle, Uint32 color);
void FillRays(SDL_Surface *surface, struct Circle circle, struct Circle secondcircle, Uint32 color);

void draw_circle(SDL_Surface *surface, struct Circle circle, Uint32 color) {
  int radius = circle.r * circle.r;

  for (int y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
    for (int x = circle.x - circle.r; x <= circle.x + circle.r; x++) {

      //Calculating the distance from the centre
      int dx = x - circle.x;
      int dy = y - circle.y;

      if (dx * dx + dy * dy <= radius) {
        SDL_Rect pixel = {x, y, 1, 1};  // Define 1x1 pixel rectangle
        SDL_FillRect(surface, &pixel, color);
      }
    }
  }
}

void FillRays(SDL_Surface *surface, struct Circle circle , struct Circle secondcircle, Uint32 color) {
    
  for(double theta=0; theta <= 2 * M_PIE; theta += 0.001) { 
        double step = 1.0;

        for (double t = circle.r; ; t += step) {
            double x_draw = circle.x + t * cos(theta);
            double y_draw = circle.y + t * sin(theta);

            if((x_draw - secondcircle.x)* (x_draw - secondcircle.x)+ (y_draw - secondcircle.y)* (y_draw - secondcircle.y) < secondcircle.r * secondcircle.r) {
              break;
            } 

            SDL_Rect pixel = {x_draw, y_draw, 4, 4};
            SDL_FillRect(surface, &pixel, color);

            if (x_draw < 0 || x_draw >= WIDTH || y_draw < 0 || y_draw >= HEIGHT) {
                break;  // Stop when the ray reaches the screen boundary
            }
        }
    }
}

void MovingSecondCircle(SDL_Surface *surface, int *upward, int *downward, struct Circle *circle, Uint32 color) {
    if (*upward) { 
        circle->y -= SPEED;
        if (circle->y <= 0) {
            *upward = 0;
            *downward = 1;
        }
    } 
    else if (*downward) {  
        circle->y += SPEED;
        if (circle->y >= HEIGHT) {
            *downward = 0;
            *upward = 1;
        }
    }
    draw_circle(surface, *circle, color);
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("SDL Window",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        WIDTH, HEIGHT,
                                        SDL_WINDOW_SHOWN);
  
  if (!window) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Surface *surface = SDL_GetWindowSurface(window);

  // Define the circle
  struct Circle circle = {400, 300, 80};
  struct Circle secondcircle = {600, 400, 140};


  // Draw the circle **before** the event loop
  draw_circle(surface, circle, COLOR_WHITE);
  draw_circle(surface, secondcircle, COLOR_WHITE);
  SDL_UpdateWindowSurface(window);

  SDL_Rect erase_rect = {0, 0, WIDTH, HEIGHT};
  SDL_Event event;
  int running = 1;
  int downward = 0;
  int upward = 1;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
          running = 0;
      }
      if (event.type == SDL_MOUSEMOTION) {
        circle.x = event.motion.x;
        circle.y = event.motion.y;
      }
    }
    SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
    draw_circle(surface, secondcircle, COLOR_WHITE);
    draw_circle(surface, circle, COLOR_WHITE);
    FillRays(surface , circle, secondcircle, COLOR_YELLOW);
    MovingSecondCircle(surface,&upward,&downward,&secondcircle,COLOR_WHITE);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

