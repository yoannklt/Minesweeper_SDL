#ifndef MAIN
#define MAIN
#define GRID_LENGTH 10
#define TEXTURE_COUNT 34

typedef struct Cell {
	int state;
	float animationElapsedTime;
	int isAnimating;
}Cell;

void SDL_ExitWithError(const char* message);
void DestroyWindowAndRenderer(SDL_Window* window, SDL_Renderer* renderer);
void displayGrid(Cell tableau[GRID_LENGTH][GRID_LENGTH], SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* textures[TEXTURE_COUNT], float deltaTime);
int play(Cell tableau[GRID_LENGTH][GRID_LENGTH], int x, int y);
int bombsAround(Cell tableau[GRID_LENGTH][GRID_LENGTH], int x, int y);
int victory(Cell tableau[GRID_LENGTH][GRID_LENGTH]);
void Color(int couleurDuTexte, int couleurDeFond);
int bombPlacing(Cell tableau[GRID_LENGTH][GRID_LENGTH], int startPosX, int startPosY);
void placeFlag(Cell tableau[GRID_LENGTH][GRID_LENGTH], int x, int y);
void DisplayImage(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect rectangle);
void InitializeTexture(SDL_Renderer* renderer, SDL_Texture* textures[TEXTURE_COUNT]);


#endif 
