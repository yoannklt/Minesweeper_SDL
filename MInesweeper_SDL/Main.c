#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// FICHIER(S) PRESENT(S) DANS HEADER FILES, CONTIENT LA DECLARATION DE PLUSIEURS FONCTIONS ET/OU STRUCTURES UTILES AU CODE
#include "Main.h"
#include "Array.h"
#define GRID_LENGTH 10
#define BOMB_NUMBER 14
#define HIDDEN_CELL 0
#define BOMB_CELL 1
#define DISCOVERED_CELL 2


int main(int argc, char** argv)
{
	SDL_Renderer* renderer = NULL;
	SDL_Window* window = NULL;

	int tableau[GRID_LENGTH][GRID_LENGTH] = { HIDDEN_CELL };

	// INITIALISATION VIDEO : PEUT ÊTRE APPELE AVEC D'AUTRES FLAGS (ARGUMENTS) COMME SDL_INIT_AUDIO.
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_ExitWithError("Initialisation SDL");
	
	SDL_bool program_launched = SDL_TRUE;
	// CREATION DE LA FENETRE WINDOW PUIS VERIFICATION DE L'INITIALISATION
	window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 750, 500, 0);
	if (window == NULL)
		SDL_ExitWithError("Creation window");

	renderer = SDL_CreateRenderer(window, -1, 1);
	if (renderer == NULL)
		SDL_ExitWithError("Creation renderer");

	//bombPlacing(tableau, 5, 5);

	while (program_launched)
	{
		//EVENT
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				program_launched = SDL_FALSE;
				break;

			default:
				
				break;
			}
		}

		//UPDATE

		//DISPLAY
		displayGrid(tableau, window, renderer);

	}
	
	// LIBERATION DE LA MEMOIRE PUIS DESTRUCTION DE LA FENETRES, DES RENDUS ETC
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return EXIT_SUCCESS; // return 0;
}

// FONCTION (CREEE MANUELLEMENT) QUI S'OCCUPE DE LA GESTION DES ERREURS DE CREATION/ INITIALISATION
void SDL_ExitWithError(const char* message)
{
	SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
	SDL_Quit();
	exit(EXIT_FAILURE);
}

// FONCTION (CREEE MANUELLEMENT) QUI GERE LES EVENEMENTS AU SEIN DE NOTRE FENETRE SDL
//void SDL_EventHandler(SDL_bool program_launched)
//{
//	// CODE MINIMAL
//	while (program_launched)
//	{
//		SDL_Event event;
//
//		while (SDL_PollEvent(&event))
//		{
//			switch (event.type)
//			{
//			case SDL_QUIT:
//				program_launched = SDL_FALSE;
//				break;
//
//			default:
//				break;
//			}
//		}
//	}
//}

void displayGrid(int tableau[GRID_LENGTH][GRID_LENGTH],SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_RenderClear(renderer);
	SDL_Texture* texture = NULL;
	SDL_Surface* Tile = NULL;
	SDL_Rect rectangle;

	for (int i = 0; i < GRID_LENGTH; i++) {
		rectangle.y = 50 * i;
		for (int j = 0; j < GRID_LENGTH; j++) {
			Tile = NULL;
			switch (tableau[i][j]) {
			case HIDDEN_CELL:
				if ((i + j) % 2 == 0) 
					Tile = SDL_LoadBMP("img/herbe1.bmp");
				else  
					Tile = SDL_LoadBMP("img/herbe2.bmp"); 
				break;
			}
			//------------------------------------//
			if (Tile == NULL)
				DestroyWindowAndRenderer(window, renderer);

			texture = SDL_CreateTextureFromSurface(renderer, Tile);
			SDL_FreeSurface(Tile);

			if (texture == NULL)
				DestroyWindowAndRenderer(window, renderer);

			if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
				DestroyWindowAndRenderer(window, renderer);

			rectangle.x = 50 * j + 250; // oubli pas le "rectangleTest.y" aussi mon reuf

			if (SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0)
				DestroyWindowAndRenderer(window, renderer);
		}
	}
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(texture);
}

void DestroyWindowAndRenderer(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(&renderer);
	SDL_DestroyWindow(&window);
	SDL_Quit();
}

int play(char tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
{
	tableau[x][y] = DISCOVERED_CELL;

	int bombs = bombsAround(tableau, x, y);

	if (tableau[y][x] == BOMB_CELL) {
		for (int i = 0; i < GRID_LENGTH; i++) {
			for (int j = 0; j < GRID_LENGTH; j++) {
				if (tableau[i][j] == BOMB_CELL)
					tableau[i][j] = 9;
			}
		}
		return 3;
	}
	else
		tableau[y][x] = DISCOVERED_CELL;

	if (bombs != 0) {
		tableau[x][y] = DISCOVERED_CELL;
		return 1;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i - 1 != 0 || j - 1 != 0) {
				if ((x + i - 1 >= 0 && x + i - 1 < GRID_LENGTH && y + j - 1 >= 0 && y + j - 1 < GRID_LENGTH) && tableau[y + j - 1][x + i - 1] == BOMB_CELL) {
					tableau[i][j] = DISCOVERED_CELL;
					play(tableau, i, j);
				}
			}
		}
	}
}

int bombsAround(char tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
{
	int bombsAround = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i - 1 != 0 || j - 1 != 0) {
				if ((x + i - 1 >= 0 && x + i - 1 < GRID_LENGTH && y + j - 1 >= 0 && y + j - 1 < GRID_LENGTH) && tableau[y + j - 1][x + i - 1] == BOMB_CELL) {
					bombsAround++;
				}
			}
		}
	}

	return bombsAround;

}

int victory(char tableau[GRID_LENGTH][GRID_LENGTH])
{
	for (int i = 0; i < GRID_LENGTH; i++) {
		for (int j = 0; j < GRID_LENGTH; j++) {
			if (tableau[i][j] == HIDDEN_CELL)
				return 1;
		}
	}
	return 0;
}

// FONCTION QUI MET DE LA COULEUR DANS MA VIE
void Color(int couleurDuTexte, int couleurDeFond)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, couleurDeFond * 16 + couleurDuTexte);
}

int bombPlacing(char tableau[GRID_LENGTH][GRID_LENGTH], int startPosX, int startPosY)
{
	srand(time(NULL));
	const int FREE_CASE_COUNT = (GRID_LENGTH) * (GRID_LENGTH);
	if (FREE_CASE_COUNT < BOMB_NUMBER)
		return 1;
	stArray freeIndex = createTab(FREE_CASE_COUNT);
	int randomPos, i = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (startPosX + i - 1 >= 0 && startPosX + i - 1 < GRID_LENGTH && startPosY + j - 1 >= 0 && startPosY < GRID_LENGTH) {
				removeAt(&freeIndex, (startPosY) * 10 + (j - 1) * 10 + (startPosX)+i - 1);
			}
		}
	}


	for (i = 0; i < BOMB_NUMBER; i++)
	{
		randomPos = rand() % (freeIndex.size - i - 9);
		printf("RandomPos : %d    Coordonnes :%d | %d\n", randomPos, freeIndex.point[randomPos] / 10, freeIndex.point[randomPos] % 10);
		tableau[freeIndex.point[randomPos] / 10][freeIndex.point[randomPos] % 10] = BOMB_CELL;
		removeAt(&freeIndex, randomPos);
	}

	free(freeIndex.point);
	return 0;
}