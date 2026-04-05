#include<SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

// Base Var
#define WIDTH 900
#define HEIGHT 600

#define PLAYER_FOV 80

#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_BLUE 0xff0000ff
#define COLOR_RED 0xffff0000
#define COLOR_GREEN 0xff00ff00
#define PLAYER_COLOR 0xff00ff00
#define CELL_SIZE 50

#define MAP_HEIGHT 5
#define MAP_WIDTH 5

#define SCENE_WIDTH MAP_WIDTH*CELL_SIZE
#define SCENE_HEIGHT MAP_HEIGHT*CELL_SIZE
#define RAY_STEP_SIZE 0.1
#define RENDER_DISTANCE 250

#define PLAYER_ROT_SPD 3
#define PLAYER_MVT_SPD 10

#define PI 3.141592653589793
#define DEG2RAD PI/180
// Map
int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0,0,0,1,0},
    {0,0,2,2,0},
    {3,0,0,1,1},
    {1,0,0,0,1},
    {1,1,1,0,1}
};

typedef struct {
    double x, y, angle;
} Player;

typedef struct {
    double distance;
    int box_type;
} DistanceResult;

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
DistanceResult GetDistance(Player player, double angle)
{
    double ray_x = player.x;
    double ray_y = player.y;

    while (1)
    {
        double ray_distance = sqrt(pow(ray_x - player.x, 2) + pow(ray_y - player.y, 2));
        if (ray_distance > RENDER_DISTANCE)
            return (DistanceResult){-1, -1};

        int cell_x = (int)(ray_x / CELL_SIZE);
        int cell_y = (int)(ray_y / CELL_SIZE);

        if (cell_x < 0 || cell_y < 0 || cell_x >= MAP_WIDTH || cell_y >= MAP_HEIGHT)
            return (DistanceResult){-1, -1};

        if (map[cell_y][cell_x] > 0)
            return (DistanceResult){ray_distance, map[cell_y][cell_x]};

        ray_x += cos(angle) * RAY_STEP_SIZE;
        ray_y += sin(angle) * RAY_STEP_SIZE;
    }
}

void DrawVerticalLine(SDL_Surface *psurface, double height, double x, Uint32 color)
{
    SDL_Rect vertical_rect = {x, HEIGHT/2 - height/2, 1, height};
    SDL_FillRect(psurface, &vertical_rect, color);
}

void DrawFOV(SDL_Surface *psurface, Player player)
{
    double scale = WIDTH /PLAYER_FOV;
    double offset = player.angle -PLAYER_FOV /2;
    for(double angle=player.angle -PLAYER_FOV/2; angle <= player.angle + PLAYER_FOV/2; angle += 0.1)
    {
        // check distance for every angle in field of view
        DistanceResult result = GetDistance(player, angle*DEG2RAD);
        if (result.distance < 0)
        {
            //distance calcul went wrong, or out of render distance
            continue;
        }
        // base on distance, draw a line
        double visual_height = (CELL_SIZE * HEIGHT)/result.distance;
        double x_coord_of_VL = (angle - (player.angle - PLAYER_FOV / 2.0)) * (WIDTH / (double)PLAYER_FOV);
        Uint32 color;
        switch (result.box_type)
        {
        case 1:
            /* code */
            color = COLOR_WHITE;
            break;
        case 2:
            color = COLOR_BLUE;
            break;
        case 3:
            color = COLOR_RED;
            break;        
        default:
            color = COLOR_WHITE;
            break;
        }
        DrawVerticalLine(psurface, visual_height, x_coord_of_VL, color);
    }
}

void handle_key(SDL_Event event)
{
    SDL_Keycode code = event.key.keysym.sym;
    if (code == SDLK_LEFT)
    {
        player.angle -= PLAYER_ROT_SPD;
    }
    if (code == SDLK_RIGHT)
    {
        player.angle += PLAYER_ROT_SPD;
    }
    if (code == SDLK_UP)
    {
        player.x += cos(player.angle*DEG2RAD)*PLAYER_MVT_SPD;
        player.y += sin(player.angle*DEG2RAD)*PLAYER_MVT_SPD;
    }    
    if (code == SDLK_DOWN)
    {
        player.x -= cos(player.angle*DEG2RAD)*PLAYER_MVT_SPD;
        player.y -= sin(player.angle*DEG2RAD)*PLAYER_MVT_SPD;
    }
}

// Main Loop
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *pwindow = SDL_CreateWindow("RayCaster", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

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
                case SDL_KEYDOWN:
                handle_key(event);
                break;
            }
        }
        SDL_FillRect(psurface, NULL, COLOR_BLACK);

        //DrawPlayer(psurface, player);
        DrawFOV(psurface, player);
        /*
        * double distance = GetDistance(player, 45);
        * printf("Got distance: %d\n", distance);
        * player.x += 0.01;
        * player.y += 0.01,
        **/
        SDL_UpdateWindowSurface(pwindow);
        SDL_Delay(5);
    }

    return 0;
}