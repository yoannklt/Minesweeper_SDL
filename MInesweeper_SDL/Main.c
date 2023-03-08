#include <SDL.h>
//#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// FICHIER(S) PRESENT(S) DANS HEADER FILES, CONTIENT LA DECLARATION DE PLUSIEURS FONCTIONS ET/OU STRUCTURES UTILES AU CODE
#include "Main.h"
#include "Array.h"

#define ANIMATION_DURATION 1000
#define TEXTURE_COUNT 34
#define GRID_LENGTH 10
#define BOMB_NUMBER 17
#define HIDDEN_CELL 0
#define BOMB_CELL 1
#define DISCOVERED_CELL 2
#define FLAG 3
#define FLAG_BOMB 4


int main(int argc, char** argv)
{
	SDL_Renderer* renderer = NULL;
	SDL_Window* window = NULL;
	SDL_Texture* textures[TEXTURE_COUNT];


	Cell tableau[GRID_LENGTH][GRID_LENGTH] = { HIDDEN_CELL, 0.0, 0 };

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

	InitializeTexture(renderer, textures);

	int nbTours = 0;
	float deltaTime = 0;

	int FPSTarget = 60;
	float deltaTimeTarget = 1000 / (float) FPSTarget;
	SDL_bool game = SDL_FALSE;
	SDL_bool isGameOver = SDL_FALSE;

	while (program_launched)
	{
		int time = SDL_GetTicks();
		//EVENT
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{

			case SDL_MOUSEMOTION:
				// printf("X: %d  Y: %d\n", event.motion.x, event.motion.y);
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (game == SDL_FALSE) {
						if ((event.button.x >= 273 && event.button.y >= 315) && (event.button.x <= 473 && event.button.y <= 386))
							game = SDL_TRUE;
					}
					else if ((event.button.x >= 250 && event.button.y >= 0) && (event.button.x <= 750 && event.button.y <= 500)) {
						if (nbTours == 0 && game == SDL_TRUE) {
							bombPlacing(tableau, (event.button.x - 250) / 50, event.button.y / 50);
							if (play(tableau, (event.button.x - 250) / 50, event.button.y / 50) == 3) {
								isGameOver = SDL_TRUE;
								printf("OUAIS");
							}
							nbTours++;
						}
						else
							play(tableau, (event.button.x - 250) / 50, event.button.y / 50);
					}

				}
				
				if (event.button.button == SDL_BUTTON_RIGHT && game == SDL_TRUE)
						placeFlag(tableau, (event.button.x - 250) / 50, event.button.y / 50);
				
				if (isGameOver == SDL_TRUE)
					gameOver(window, renderer);

				break;

			case SDL_QUIT:
				program_launched = SDL_FALSE;
				break;

			default:
				
				break;
			}
		}

		//RENDER
		if (game == SDL_TRUE) {
			displayGrid(tableau, window, renderer, &textures, deltaTime);
		}
		else displayMenu(window, renderer);
			

		deltaTime = SDL_GetTicks() - time;

		if (deltaTime < deltaTimeTarget) {
			SDL_Delay(deltaTimeTarget - deltaTime);
			deltaTime = deltaTimeTarget;
		}
		else
			printf("%f\n", deltaTime);
	}
	
	// LIBERATION DE LA MEMOIRE PUIS DESTRUCTION DE LA FENETRES, DES RENDUS ETC
	DestroyWindowAndRenderer(window, renderer);
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

void displayGrid(Cell tableau[GRID_LENGTH][GRID_LENGTH],SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* textures[TEXTURE_COUNT], float deltaTime)
{
	SDL_RenderClear(renderer);
	SDL_Surface* Tile = NULL;
	SDL_Rect rectangle;
	
	for (int i = 0; i < GRID_LENGTH; i++) {
		rectangle.y = 50 * i;
		for (int j = 0; j < GRID_LENGTH; j++) {
			float animationElapsedTime = tableau[i][j].animationElapsedTime += deltaTime;
			int index = (int) ( animationElapsedTime / (ANIMATION_DURATION / 6.0) ) % 6;

			//------------------------------------//
			//textures[0] //clair
			//textures[17] //sombre
			int factorI = i % 2;
			int factorJ = ((j + factorI) % 2);
			int start = factorJ * 17;

			switch (tableau[i][j].state)
			{
				case BOMB_CELL:
					break;

				case DISCOVERED_CELL:
					start += 2;
					break;

				case FLAG:
					start += 1;
					break;

				case FLAG_BOMB:
					start += 1;
					break;

				case 6:
					start += 3;
					break;

				case 7:
					start += 4;
					break;

				case 8:
					start += 5;
					break;

				case 9:
					start += 6;
					break;

				case 10:
					start += 7;
					break;

				case 11:
					start += 8;
					break;

				case 12:
					start += 9;
					break;

				case 13:
					start += 10;
					break;

				case 14:
					start += 11 + index;
					break;

				default:
					
					break;
			}

			//------------------------------------//

			rectangle.x = 50 * j + 250; // oubli pas le "rectangleTest.y" aussi mon reuf
			DisplayImage(renderer, textures[start], rectangle);

		}
	}
	rectangle.x = 0;
	rectangle.y = 0;
	displaySideMenu(window, renderer, rectangle);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(textures);
}

void displaySideMenu(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect rectangle)
{
	SDL_Texture* texture;
	SDL_Surface* image = SDL_LoadBMP("img/Pannel.bmp");

	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	DisplayImage(renderer, texture, rectangle);
	SDL_DestroyTexture(texture);
}

void displayMenu(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_RenderClear(renderer);
	SDL_Texture* texture;
	SDL_Surface* image = SDL_LoadBMP("img/ecrantitre.bmp");
	SDL_Rect rectangle;
	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = image->w;
	rectangle.h = image->h;

	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	DisplayImage(renderer, texture, rectangle);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(texture);
}

void gameOver(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_Texture* texture;
	SDL_Surface* image = SDL_LoadBMP("img/gameover.bmp");
	SDL_Rect rectangle;
	rectangle.x = 175;
	rectangle.y = 50;

	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	DisplayImage(renderer, texture, rectangle);
	SDL_DestroyTexture(texture);
}

void DestroyWindowAndRenderer(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(&renderer);
	SDL_DestroyWindow(&window);
	SDL_Quit();
}

void InitializeTexture(SDL_Renderer* renderer, SDL_Texture* textures[TEXTURE_COUNT])
{

	const char* PATH[] =
	{
/* 0  */		"img/herbe2.bmp",
/* 1  */		"img/flag.bmp",
/* 2  */		"img/herbe-automne2.bmp",
/* 3  */		"img/oeuf1clair.bmp",
/* 4  */		"img/oeuf2clair.bmp",
/* 5  */		"img/oeuf3clair.bmp",
/* 6  */		"img/oeuf4clair.bmp",
/* 7  */		"img/oeuf5clair.bmp",
/* 8  */		"img/oeuf6clair.bmp",
/* 9  */		"img/oeuf7clair.bmp",
/* 10 */		"img/oeuf8clair.bmp",
/* 11 */		"img/bombe1clair.bmp",
/* 12 */		"img/bombe2clair.bmp",
/* 13 */		"img/bombe3clair.bmp",
/* 14 */		"img/bombe4clair.bmp",
/* 15 */		"img/bombe5clair.bmp",
/* 16 */		"img/bombe6clair.bmp",
	
/* 0  */		"img/herbe1.bmp",
/* 1  */		"img/flagfonce.bmp",
/* 2  */		"img/herbe-automne1.bmp",
/* 3  */		"img/oeuf1fonce.bmp",
/* 4  */		"img/oeuf2fonce.bmp",
/* 5  */		"img/oeuf3fonce.bmp",
/* 6  */		"img/oeuf4fonce.bmp",
/* 7  */		"img/oeuf5fonce.bmp",
/* 8  */		"img/oeuf6fonce.bmp",
/* 9  */		"img/oeuf7fonce.bmp",
/* 10 */		"img/oeuf8fonce.bmp",
/* 11 */		"img/bombe1fonce.bmp",
/* 12 */		"img/bombe2fonce.bmp",
/* 13 */		"img/bombe3fonce.bmp",
/* 14 */		"img/bombe4fonce.bmp",
/* 15 */		"img/bombe5fonce.bmp",
/* 16 */		"img/bombe6fonce.bmp"
	};

	//Load de tous les BMP vers des SDLSurface
	for (int i = 0; i < TEXTURE_COUNT; i++) {
		SDL_Surface* surface = SDL_LoadBMP(PATH[i]);
		textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

}

void DisplayImage(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect rectangle)
{
	if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
		SDL_ExitWithError("QueryTexture failed");

	if (SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0)
		SDL_ExitWithError("RenderCopy failed");
}

void placeFlag(Cell tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
{
	switch (tableau[y][x].state)
	{
	case BOMB_CELL:
		tableau[y][x].state = FLAG_BOMB;
		break;

	case HIDDEN_CELL:
		tableau[y][x].state = FLAG;
		break;

	case FLAG:
		tableau[y][x].state = HIDDEN_CELL;
		break;

	case FLAG_BOMB:
		tableau[y][x].state = BOMB_CELL;
		break;

	default:
		break;
	}
}

int play(Cell tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
{

	int bombs = bombsAround(tableau, x, y);

	if (tableau[y][x].state == FLAG || tableau[y][x].state == FLAG_BOMB)
		return 2;

	if (tableau[y][x].state == BOMB_CELL || tableau[y][x].state == FLAG_BOMB) {
		for (int i = 0; i < GRID_LENGTH; i++) {
			for (int j = 0; j < GRID_LENGTH; j++) {
				if (tableau[i][j].state == BOMB_CELL)
					tableau[i][j].state = 14;
			}
		}
		return 3;
	}

	if (bombs != 0)
	{
		tableau[y][x].state = bombs + 5;
		return 1;
	}

	tableau[y][x].state = DISCOVERED_CELL;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i - 1 != 0 || j - 1 != 0) {
				if ((x + i - 1 >= 0 && x + i - 1 < GRID_LENGTH && y + j - 1 >= 0 && y + j - 1 < GRID_LENGTH) && tableau[y + j - 1][x + i - 1].state == HIDDEN_CELL) {
					play(tableau,x + i - 1,y + j - 1);
				}
			}
		}
	}
	return 0;
}

int bombsAround(Cell tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
{
	int bombsAround = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i - 1 != 0 || j - 1 != 0) {
				if ((x + i - 1 >= 0 && x + i - 1 < GRID_LENGTH && y + j - 1 >= 0 && y + j - 1 < GRID_LENGTH) && (tableau[y + j - 1][x + i - 1].state == BOMB_CELL || tableau[y + j - 1][x + i - 1].state == FLAG_BOMB)) {
					bombsAround++;
				}
			}
		}
	}

	return bombsAround;

}

int victory(Cell tableau[GRID_LENGTH][GRID_LENGTH])
{
	for (int i = 0; i < GRID_LENGTH; i++) {
		for (int j = 0; j < GRID_LENGTH; j++) {
			if (tableau[i][j].state == HIDDEN_CELL)
				return 1;
		}
	}
	return 0;
}

int bombPlacing(Cell tableau[GRID_LENGTH][GRID_LENGTH], int startPosX, int startPosY)
{
	srand(time(NULL));
	const int FREE_CASE_COUNT = (GRID_LENGTH) * (GRID_LENGTH);
	if (FREE_CASE_COUNT < BOMB_NUMBER)
		return 1;
	stArray freeIndex = createTab(FREE_CASE_COUNT);
	int randomPos, i, j = 0;

	for (i = 3; i > 0; i--)
	{
		for (j = 3; j > 0; j--)
		{
			if (startPosX + i - 2 >= 0 && startPosX + i - 2 < GRID_LENGTH && startPosY + j - 2 >= 0 && startPosY + j - 2 < GRID_LENGTH)
			{
				//printf("indice [%d]: %d\n", (startPosY + i - 2) * 10 + startPosX + j - 2, freeIndex.point[(startPosY + i - 2) * 10 + startPosX + j - 2]);
				removeAt(&freeIndex, (startPosY + i - 2) * 10 + startPosX + j - 2);
			}

		}
	}

	for (i = 0; i < BOMB_NUMBER; i++)
	{
		randomPos = rand() % (freeIndex.size - i - 9);
		//printf("RandomPos : %d    Coordonnes :%d | %d\n", randomPos ,freeIndex.point[randomPos] / 10, freeIndex.point[randomPos] % 10);
		tableau[freeIndex.point[randomPos] / 10][freeIndex.point[randomPos] % 10].state = BOMB_CELL;
		removeAt(&freeIndex, randomPos);
	}

	free(freeIndex.point);
	return 0;
}