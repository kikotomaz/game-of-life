// //////////////////////////////////////////////////////////////////////// //
// C implementation if Conway's Game of Life using SDL2                     //
// By: Kiko                                                                 //
//                                                                          //
// https://en.wikipedia.org/wiki/Conway's_Game_of_Life                      //
//                                                                          //
// The world is initialized with random values of 0-1 at runtime.           //
// The seed / initial state can be modified in the InitWorld() function     //
// //////////////////////////////////////////////////////////////////////// //

#include <SDL2/SDL.h>
#include <time.h>

#define ALIVE 1
#define DEAD 0

static const int HEIGHT = 112;
static const int WIDTH = 184;
static const int SCALE = 8;
static const int TICK_DELAY_MS = 70;

void InitWorld(int* world);
void Tick(int** world);
void Draw(SDL_Renderer* renderer, const int* world);

int main(void){

    int* world = malloc(WIDTH * HEIGHT * sizeof(int));
    InitWorld(world);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(
            WIDTH * SCALE, HEIGHT * SCALE,
            0, &window, &renderer);

    SDL_RenderSetScale(renderer, SCALE, SCALE);

    SDL_Event e;
    int running = 0;
    while(running == 0)
    {
        Tick(&world);
        Draw(renderer, world);
        SDL_Delay(TICK_DELAY_MS);
        while( SDL_PollEvent( &e ) > 0 )
        { 
            if( e.type == SDL_QUIT ){
                printf("Registered QUIT cmd, exiting now\n"); 
                running = 1;
            }
        }
    }
 
    free(world);

    return 0;
}

void InitWorld(int* world)
{
    srandom(time(NULL));
    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH; x++)
        {
            world[y*WIDTH + x] = random() % 2;
        }
    }
}

int GetCoords(const int x, const int y)
{
    return WIDTH * (y % HEIGHT) + (x % WIDTH);
}

void Draw(SDL_Renderer* renderer, const int* world)
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);

    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH; x++)
        {
            if(world[WIDTH * y + x] == 1)
            {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int EvaluateState(const int self, const int neighbors)
{

    if(self == ALIVE)
    {
        if(neighbors == 2 || neighbors == 3)
        {
            return ALIVE;
        }
    }
    else
    {
        if(neighbors == 3)
        {
            return ALIVE;
        }
    }

    return DEAD;
}

void Tick(int** world)
{
    int* nextWorld = calloc(WIDTH * HEIGHT, sizeof(int));

    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH; x++)
        {
            const int currentPos = GetCoords(x,y);

            int neighbors = 0;
            for(int i = -1; i <= 1; i++)
            {
                for(int j = -1; j <= 1; j++)
                {
                    neighbors += (*world)[GetCoords(x + i, y + j)];
                }
            }
            neighbors -= (*world)[currentPos];

            nextWorld[currentPos] = EvaluateState((*world)[currentPos], neighbors);
        }
    }

    free(*world);

    *world = nextWorld;
}
