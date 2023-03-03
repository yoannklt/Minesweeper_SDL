#include <SDL.h>
//#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// FICHIER(S) PRESENT(S) DANS HEADER FILES, CONTIENT LA DECLARATION DE PLUSIEURS FONCTIONS ET/OU STRUCTURES UTILES AU CODE
#include "Main.h"
#include "Array.h"
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
	SDL_Texture* textures[21];


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

	InitializeTexture(renderer, textures);

	int nbTours = 0;

	displayGrid(tableau, window, renderer, textures);
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
					if (nbTours == 0) {
						bombPlacing(tableau, (event.button.x - 250) / 50, event.button.y / 50);
						play(tableau, (event.button.x - 250) / 50, event.button.y / 50) == 3;
						nbTours++;
					}
					
					if (play(tableau, (event.button.x - 250) / 50, event.button.y / 50) == 3) {
						printf("BAKA");
					}
					displayGrid(tableau, window, renderer, &textures);
				}
				
				if (event.button.button == SDL_BUTTON_RIGHT) {
					placeFlag(tableau, (event.button.x - 250) / 50, event.button.y / 50);
					displayGrid(tableau, window, renderer, &textures);
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

void displayGrid(int tableau[GRID_LENGTH][GRID_LENGTH],SDL_Window* window, SDL_Renderer* renderer, SDL_Texture** textures)
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

			case FLAG:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/flagfonce.bmp");
				else 
					Tile = SDL_LoadBMP("img/flag.bmp");
				break;

			case FLAG_BOMB:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/flagfonce.bmp");
				else
					Tile = SDL_LoadBMP("img/flag.bmp");
				break;

			case 6:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/oeuf1clair.bmp");
				else
					Tile = SDL_LoadBMP("img/oeuf1fonce.bmp");
				break;

			case 7:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/oeuf2clair.bmp");
				else
					Tile = SDL_LoadBMP("img/oeuf2fonce.bmp");
				break;

			case 8:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/oeuf3clair.bmp");
				else
					Tile = SDL_LoadBMP("img/oeuf3fonce.bmp");
				break;

			case 9:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/oeuf4clair.bmp");
				else
					Tile = SDL_LoadBMP("img/oeuf4fonce.bmp");
				break;

			case 10:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/oeuf5clair.bmp");
				else
					Tile = SDL_LoadBMP("img/oeuf5fonce.bmp");
				break;

			case 11:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/oeuf6clair.bmp");
				else
					Tile = SDL_LoadBMP("img/oeuf6fonce.bmp");
				break;

			case 12:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/oeuf7clair.bmp");
				else
					Tile = SDL_LoadBMP("img/oeuf7fonce.bmp");
				break;

			case 13:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/oeuf8clair.bmp");
				else
					Tile = SDL_LoadBMP("img/oeuf8fonce.bmp");
				break;

			case 14:
				if ((i + j) % 2 == 0)
					Tile = SDL_LoadBMP("img/bombeclair.bmp");
				else
					Tile = SDL_LoadBMP("img/bombefonce.bmp");
				break;

			default:
				Tile = SDL_LoadBMP("img/NumberTwo.bmp");
				break;
			}
			//------------------------------------//

			rectangle.x = 50 * j + 250; // oubli pas le "rectangleTest.y" aussi mon reuf
			DisplayImage(window, renderer, Tile, texture, rectangle);

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

void InitializeTexture(SDL_Renderer* renderer, SDL_Texture* textures[20])
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
/* 11 */		"img/bombeclair.bmp",	
	
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
/* 10  */		"img/oeuf8fonce.bmp",
/* 11 */		"img/bombefonce.bmp"
	};

	//Load de tous les BMP vers des SDLSurface
	for (int i = 0; i < 21; i++) {
		SDL_Surface* surface = (i % 2 == 0) ? SDL_LoadBMP(PATH[i]) : SDL_LoadBMP(PATH[i]);
		textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

}

void DisplayImage(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* Tile, SDL_Texture* texture, SDL_Rect rectangle)
{
	if (Tile == NULL)
		DestroyWindowAndRenderer(window, renderer);

	texture = SDL_CreateTextureFromSurface(renderer, Tile);
	SDL_FreeSurface(Tile);

	if (texture == NULL)
		DestroyWindowAndRenderer(window, renderer);

	if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
		DestroyWindowAndRenderer(window, renderer);


	if (SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0)
		DestroyWindowAndRenderer(window, renderer);
}

void placeFlag(int tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
{
	switch (tableau[y][x])
	{
	case BOMB_CELL:
		tableau[y][x] = FLAG_BOMB;
		break;

	case HIDDEN_CELL:
		tableau[y][x] = FLAG;
		break;

	case FLAG:
		tableau[y][x] = HIDDEN_CELL;
		break;

	case FLAG_BOMB:
		tableau[y][x] = BOMB_CELL;
		break;

	default:
		break;
	}
}

int play(int tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
{

	int bombs = bombsAround(tableau, x, y);

	if (tableau[y][x] == FLAG || tableau[y][x] == FLAG_BOMB)
		return 2;

	if (tableau[y][x] == BOMB_CELL || tableau[y][x] == FLAG_BOMB) {
		for (int i = 0; i < GRID_LENGTH; i++) {
			for (int j = 0; j < GRID_LENGTH; j++) {
				if (tableau[i][j] == BOMB_CELL)
					tableau[i][j] = 14;
			}
		}
		return 3;
	}

	if (bombs != 0)
	{
		tableau[y][x] = bombs + 5;
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
	return 0;
}

int bombsAround(int tableau[GRID_LENGTH][GRID_LENGTH], int x, int y)
{
	int bombsAround = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i - 1 != 0 || j - 1 != 0) {
				if ((x + i - 1 >= 0 && x + i - 1 < GRID_LENGTH && y + j - 1 >= 0 && y + j - 1 < GRID_LENGTH) && (tableau[y + j - 1][x + i - 1] == BOMB_CELL || tableau[y + j - 1][x + i - 1] == FLAG_BOMB)) {
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
		tableau[freeIndex.point[randomPos] / 10][freeIndex.point[randomPos] % 10] = BOMB_CELL;
		removeAt(&freeIndex, randomPos);
	}

	free(freeIndex.point);
	return 0;
}