#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

// FICHIER(S) PRESENT(S) DANS HEADER FILES, CONTIENT LA DECLARATION DE PLUSIEURS FONCTIONS ET/OU STRUCTURES UTILES AU CODE
#include "Main.h"


int main(int argc, char** argv)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = NULL;
	SDL_Rect rectangle;
	// INITIALISATION VIDEO : PEUT ÊTRE APPELE AVEC D'AUTRES FLAGS (ARGUMENTS) COMME SDL_INIT_AUDIO.
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_ExitWithError("Initialisation SDL");
	
	SDL_bool program_launched = SDL_TRUE;

	// CREATION DE LA FENETRE WINDOW PUIS VERIFICATION DE L'INITIALISATION
	SDL_Window* window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (window == NULL)
		SDL_ExitWithError("Creation window");

	SDL_EventHandler(program_launched);
	
	// LIBERATION DE LA MEMOIRE PUIS DESTRUCTION DE LA FENETRES, DES RENDUS ETC
	SDL_DestroyWindow(window);
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
void SDL_EventHandler(SDL_bool program_launched)
{
	// CODE MINIMAL
	while (program_launched)
	{
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
	}
}