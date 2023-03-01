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
#define FLAG 3


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

	
	displayGrid(tableau, window, renderer);
	while (program_launched)
	{
		//EVENT
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					bombPlacing(tableau, (event.button.x - 250) / 50, event.button.y / 50);
					if (play(tableau, (event.button.x - 250) / 50, event.button.y / 50) == 3) {

					}
					displayGrid(tableau, window, renderer);
				}
				
				break;

			case SDL_QUIT:
				program_launched = SDL_FALSE;
				break;

			default:
				
				break;
			}
		}

		//UPDATE

		//DISPLAY
		

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
			//printf("%d", i);
			switch (tableau[i][j]) {
			case HIDDEN_CELL:
				if ((i + j) % 2 == 0) 
					Tile = SDL_LoadBMP("img/herbe1.bmp");
				else  
					Tile = SDL_LoadBMP("img/herbe2.bmp"); 
				break;
			
			case BOMB_CELL:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/herbe1.bmp");
				else
					Tile = SDL_LoadBMP("img/herbe2.bmp");
				break;

			case DISCOVERED_CELL:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/herbe-automne1.bmp");
				else
					Tile = SDL_LoadBMP("img/herbe-automne2.bmp");
				break;

			case 5:
				Tile = SDL_LoadBMP("img/egg1.bmp");
				break;

			case 6:
				Tile = SDL_LoadBMP("img/egg2.bmp");
				break;

			case 7:
				Tile = SDL_LoadBMP("img/egg3.bmp");
				break;

			case 8:
				Tile = SDL_LoadBMP("img/egg4.bmp");
				break;

			case 9:
				Tile = SDL_LoadBMP("img/egg5.bmp");
				break;

			case 10:
				Tile = SDL_LoadBMP("img/egg6.bmp");
				break;

			case 11:
				Tile = SDL_LoadBMP("img/egg7.bmp");
				break;

			case 12:
				Tile = SDL_LoadBMP("img/egg8.bmp");
				break;

			case 13:
				Tile = SDL_LoadBMP("img/Bombe.bmp");
				break;

			case FLAG:
				break;
			default:
				Tile = SDL_LoadBMP("img/NumberTwo.bmp");
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


int play(int tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
	{

	int bombs = bombsAround(tableau, x, y);

	if (tableau[y][x] == BOMB_CELL) {
		for (int i = 0; i < GRID_LENGTH; i++) {
			for (int j = 0; j < GRID_LENGTH; j++) {
				if (tableau[i][j] == BOMB_CELL)
					tableau[i][j] = 13;
			}
		}
		return 3;
	}

	if (bombs != 0)
	{
		tableau[y][x] = bombs + 4;
		return 1;
	}

	tableau[y][x] = DISCOVERED_CELL;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i - 1 != 0 || j - 1 != 0) {
				if ((x + i - 1 >= 0 && x + i - 1 < GRID_LENGTH && y + j - 1 >= 0 && y + j - 1 < GRID_LENGTH) && tableau[y + j - 1][x + i - 1] == HIDDEN_CELL) {
					play(tableau,x + i - 1,y + j - 1);
				}
			}
		}
	}
}

int bombsAround(int tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
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

int victory(int tableau[GRID_LENGTH][GRID_LENGTH])
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

int bombPlacing(int tableau[GRID_LENGTH][GRID_LENGTH], int startPosX, int startPosY)
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
				removeAt(&freeIndex, (startPosY) * 10 + (j - 1) * 10 + (startPosX) + i - 1);
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