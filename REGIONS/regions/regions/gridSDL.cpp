#include "gridSDL.h"

Grid::Grid(unsigned width, unsigned height, unsigned rows, unsigned columns)
{
	this->width = width;
	this->height = height;
	this->rows = rows;
	this->columns = columns;
	
	// one pixel between each rectangle
	rectWidth = (width - outlinePx * (columns - 1)) / columns;
	rectHeight = (height - outlinePx * (rows - 1)) / rows;

	Uint32 subsystemMask = SDL_WasInit(SDL_INIT_EVERYTHING);
	if (subsystemMask == 0) {
		SDL_Init(SDL_INIT_VIDEO);
	}
	else if (!(subsystemMask & SDL_INIT_VIDEO)) {
		SDL_InitSubSystem(SDL_INIT_VIDEO);
	}
	//TODO: add method for changing window name
	window = SDL_CreateWindow("Grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, NULL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//init each elm with a color
	grid = new Color* [rows];
	for (unsigned i = 0; i < rows; i++) {
		grid[i] = new Color[columns];
		memset(grid[i], 255, sizeof(Color) * columns); //default to white
	}

	//start event handler
	Render();
	EventHandler();
}

void Grid::SetBackground(Color color)
{
	background = color;
}

void Grid::SetColor(unsigned row, unsigned col, Color color)
{
	grid[row][col] = color;
}

void Grid::SetRow(unsigned row, Color color)
{
	for (unsigned i = 0; i < rows; i++) {
		grid[row][i] = color;
	}
}

void Grid::SetColumn(unsigned column, Color color)
{
	for (unsigned i = 0; i < rows; i++) {
		grid[i][column] = color;
	}
}

void Grid::SetAll(Color color)
{
	for (unsigned r = 0; r < rows; r++) {
		for (unsigned c = 0; c < columns; c++) {
			grid[r][c] = color;
		}
	}
}

void Grid::Render(void)
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderClear(renderer);

	for (unsigned row = 0; row < rows; row++) {
		for (unsigned col = 0; col < columns; col++) {
			Uint8 r, g, b, a;
			r = grid[row][col].r;
			g = grid[row][col].g;
			b = grid[row][col].b;
			a = grid[row][col].a;
			SDL_Rect rect;
			rect.h = rectHeight;
			rect.w = rectWidth;
			rect.x = rectWidth * col + outlinePx * col;
			rect.y = rectHeight * row + outlinePx * row;

			SDL_SetRenderDrawColor(renderer, r, g, b, a);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}

void Grid::close(void)
{
	SDL_DestroyWindow(window);

	for (unsigned c = 0; c < columns; c++) {
		delete[] grid[c];
	}
	delete[] grid;
}

void Grid::EventHandler(void)
{
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return;
			}
			else if (event.button.button == SDL_BUTTON_LEFT) {
				unsigned x, y;
				unsigned row, col;
				x = event.button.x;
				y = event.button.y;
				row = y / (rectHeight + outlinePx);
				col = x / (rectWidth + outlinePx);
				grid[row][col] = Color{ 100, 255, 100, 255 };
				this->Render();
			}
		}


		SDL_Delay(16); // ~30 updates per sec
	}
}