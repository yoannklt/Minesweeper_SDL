#ifndef MAIN
#define MAIN
#define GRID_LENGTH 10

void SDL_ExitWithError(const char* message);
void DestroyWindowAndRenderer(SDL_Window* window, SDL_Renderer* renderer);
void displayGrid(int* tableau[GRID_LENGTH], SDL_Window* window, SDL_Renderer* renderer);
int play(char tableau[GRID_LENGTH][GRID_LENGTH], int x, int y);
int bombsAround(char tableau[GRID_LENGTH][GRID_LENGTH], int x, int y);
int victory(char tableau[GRID_LENGTH][GRID_LENGTH]);
void Color(int couleurDuTexte, int couleurDeFond);
int bombPlacing(char tableau[GRID_LENGTH][GRID_LENGTH], int startPosX, int startPosY);


#endif 
