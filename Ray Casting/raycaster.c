#include<SDL2/SDL.h>
#include <stdbool.h>

// Base Var
#define WIDTH 900
#define HEIGHT 600

#define PLAYER_FOV 80

#define COLOR_WHITE 0xffffffff
#define PLAYER_COLOR 0xff00ff00
#define CELL_SIZE 50

// Map
int map[5][5] = {
    {0,0,0,1,0},
    {0,0,1,1,0},
    {1,0,0,1,1},
    {1,0,0,0,1},
    {1,1,1,0,1}
};

typedef struct {
    float x, y, angle;

} Player;

Player player = {0, 0, 45};

bool app_running = true;

void DrawPlayer(SDL_Surface *psurface, Player player)
{
    SDL_Rect player_rect = {player.x, player.y, 10, 10};
    SDL_FillRect(psurface, &player_rect, PLAYER_COLOR);
}

/* Returns the distance to the next wall 
 * assuming the player looks in given direction 
 */
void GetDistance(Player player, double angle)
{

}

// Main Loop
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *pwindow = SDL_CreateWindow("RayCaster", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

    int counter = 0;
    while (app_running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                app_running = false;
                break;
            }
        }

        DrawPlayer(psurface, player);
        
        SDL_UpdateWindowSurface(pwindow);
        SDL_Delay(10);
    }

    return 0;
}