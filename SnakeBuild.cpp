#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <cmath>
#include <Windows.h>
#include <SDL_ttf.h>
#include <sstream> 

//Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 900;


SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* snakeRender = NULL;
SDL_Surface* loadSurface(std::string path);
SDL_Rect snakePosition;
SDL_Rect fruitPosition;
SDL_Rect tailPosition;


int xTail[1000], yTail[1000]; //array for tail pieces
int nTail; //length of the snake


void snakeTail() {
	int xPrev = xTail[0]; //prev initialized to array 0
	int yPrev = yTail[0];
	int xPrev2, yPrev2;
	xTail[0] = snakePosition.x; //snakehead set to array 0
	yTail[0] = snakePosition.y;

	for (int i = 1; i < nTail; i++) //for the length of tail do this
	{
		xPrev2 = xTail[i]; //2nd prev is set to the current tail piece
		yPrev2 = yTail[i];
		xTail[i] = xPrev; //tail is set to the one infront
		yTail[i] = yPrev;
		xPrev = xPrev2; //updates the prev so it works down the line
		yPrev = yPrev2;

		SDL_Rect fillRect = { xPrev, yPrev, 20, 20 };
		SDL_SetRenderDrawColor(snakeRender, 255, 145, 164, 0xFF);
		SDL_RenderFillRect(snakeRender, &fillRect);
	}
}

bool checkCollision() {
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of snake head
	leftA = snakePosition.x;
	rightA = snakePosition.x + 10;
	topA = snakePosition.y;
	bottomA = snakePosition.y + 10;

	//Calculate the sides of fruit 
	leftB = fruitPosition.x;
	rightB = fruitPosition.x + 10;
	topB = fruitPosition.y;
	bottomB = fruitPosition.y + 10;

	    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return true;
    }

    if( topA >= bottomB )
    {
        return true;
    }

    if( rightA <= leftB )
    {
        return true;
    }

    if( leftA >= rightB )
    {
        return true;
    }

    //If none of the sides from A are outside B
    return false;
}

void game_Structure() { //sets up initial spot for snake and for the fruit
	snakePosition.x = (280);
	snakePosition.y = (480);
	fruitPosition.x = (rand() % 30) * 20;
	fruitPosition.y = ((rand() % 39) + 6) * 20;
}

void snakeSnake(int x, int y) { //renders the snake rect
	SDL_SetRenderDrawColor(snakeRender, 102, 176, 50, 0xFF); //colors background green
	SDL_RenderClear(snakeRender); //clears the screen

	SDL_Rect fillRect = {x, y, 20, 20}; //position of snake head
	SDL_SetRenderDrawColor(snakeRender, 251, 96, 127, 0xFF); //colors pink
	SDL_RenderFillRect(snakeRender, &fillRect); //renders
}


void fruit(int x, int y) { //renders fruit and makes it purple
	SDL_Rect fillRect = {x, y, 20, 20};
	SDL_SetRenderDrawColor(snakeRender, 106, 13, 173, 0xFF);
	SDL_RenderFillRect(snakeRender, &fillRect);
}

/*FIX ME: i want a scoreboard*/
void displayBoard(int score) { //gray display area
	SDL_Rect fillRect = {0, 0, SCREEN_WIDTH, 120 };
	SDL_SetRenderDrawColor(snakeRender, 61, 72, 73, 0xFF);

	std::stringstream ss;
	ss << (score);
	std::string str = ss.str();
	const char* scoreChar = str.c_str();

	TTF_Init();
	int texW = 0;
	int texH = 0;

	TTF_Font* font = TTF_OpenFont("Schluber.ttf", 30);
	SDL_Color color = { 255, 255, 255 };

	SDL_Surface* surface = TTF_RenderText_Solid(font,
		"Welcome to a remake of the classic game Snake!", color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(snakeRender, surface);
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { 40, 30, texW, texH };
	

	SDL_Surface* surfaceTwo = TTF_RenderText_Solid(font,
		"Score: ", color);
	SDL_Texture* textureTwo = SDL_CreateTextureFromSurface(snakeRender, surfaceTwo);
	SDL_QueryTexture(textureTwo, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectTwo = { 230, 70, texW, texH };
	

	SDL_Surface* surfaceThree = TTF_RenderText_Solid(font,
		scoreChar, color);
	SDL_Texture* textureThree = SDL_CreateTextureFromSurface(snakeRender, surfaceThree);
	SDL_QueryTexture(textureThree, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectThree = { 310, 70, texW, texH };

	SDL_RenderFillRect(snakeRender, &fillRect);

    SDL_RenderCopy(snakeRender, texture, NULL, &dstrect);
	SDL_RenderCopy(snakeRender, textureTwo, NULL, &dstrectTwo);
	SDL_RenderCopy(snakeRender, textureThree, NULL, &dstrectThree);
}


void close(){ //closes everything properly
	SDL_DestroyRenderer(snakeRender);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


bool game_Screen() { //creates the game surface and the render as wll
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;	
	}
	else{
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else{
			screenSurface = SDL_GetWindowSurface(window);
			snakeRender = SDL_CreateRenderer(window, -1, 0);
		}
	}
	return success;
}


SDL_Surface* loadSurface(std::string path){ //optomizes the screen surface
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL){
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
		if (optimizedSurface == NULL){
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

int score = 0;
void gameLogic(int dir){
	int snakeVelocity = 20; //speed of the snake set to the size of the snake 
	snakeSnake(snakePosition.x, snakePosition.y); //passes the positon of the snake head
	
	if (dir == 1) { snakePosition.x -= snakeVelocity; } //depending on the direction choosen velocity is applied in that direction
	if (dir == 2) { snakePosition.x += snakeVelocity; }
	if (dir == 3) { snakePosition.y -= snakeVelocity; }
	if (dir == 4) { snakePosition.y += snakeVelocity; }
	if (!checkCollision()) { //if a collision with fruit happens make a new fruit add a tail
		fruitPosition.x = (rand() % 30) * 20;
		fruitPosition.y = ((rand() % 39) + 6) * 20;
		nTail++;
		score = score + 10;
		if (nTail == 1) { nTail++; } //for the first fruit add two tails makes it so a tail pops up on the first fruit eaten
	}
	snakeTail(); 
	//checks if snake is outside of screen
	if (snakePosition.x == (SCREEN_WIDTH + 20) || snakePosition.x < 0 || snakePosition.y == (SCREEN_HEIGHT + 20) || snakePosition.y < 120)
		close();

	//checks if snake bit himself
	for (int i = 1; i < nTail; i++)
		if (xTail[i] == snakePosition.x && yTail[i] == snakePosition.y)
			close();

	fruit(fruitPosition.x, fruitPosition.y);
	displayBoard(score);
	SDL_RenderPresent(snakeRender);
	SDL_Delay(90); //slows things down a bit
}


int main(int argc, char* args[]) {
	int dir = 0;
	if (!game_Screen()) {
		printf("Failed to initialize!\n");
	}
	else {
		bool quit = false;
		SDL_Event e;

		game_Structure(); 
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else {
					switch (e.type) {
						/* Look for a keypress */
					case SDL_KEYDOWN:
						/* Check the SDLKey values and move change the coords */
						switch (e.key.keysym.sym) {
						case SDLK_LEFT:
							dir = 1;
							break;
						case SDLK_RIGHT:
							dir = 2;;
							break;
						case SDLK_UP:
							dir = 3;;
							break;
						case SDLK_DOWN:
							dir = 4;;
							break;
						default:
							break;
						}
						break;

					default:
						break;
					}
				}
			}
			gameLogic(dir); //passes the direction choosen into the main logic
		}
	}
	close();
	return 0;
}
