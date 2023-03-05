#ifndef MAIN
#define MAIN
#define GRID_LENGTH 10

void SDL_ExitWithError(const char* message);
void DestroyWindowAndRenderer(SDL_Window* window, SDL_Renderer* renderer);
void displayGrid(int tableau[GRID_LENGTH][GRID_LENGTH], SDL_Window* window, SDL_Renderer* renderer, SDL_Texture** textures);
int play(int tableau[GRID_LENGTH][GRID_LENGTH], int x, int y);
int bombsAround(int tableau[GRID_LENGTH][GRID_LENGTH], int x, int y);
int victory(int tableau[GRID_LENGTH][GRID_LENGTH]);
void Color(int couleurDuTexte, int couleurDeFond);
int bombPlacing(int tableau[GRID_LENGTH][GRID_LENGTH], int startPosX, int startPosY);
void placeFlag(int tableau[GRID_LENGTH][GRID_LENGTH], int x, int y);
void DisplayImage(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* Tile, SDL_Texture* texture, SDL_Rect rectangle);
void InitializeTexture(SDL_Renderer* renderer, SDL_Texture* textures[20]);
void DisplayMenu(SDL_Window* window, SDL_Renderer* renderer);


#endif 
