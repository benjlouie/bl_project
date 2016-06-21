#include "gridSDL.h"

Grid::Grid(unsigned width, unsigned height, unsigned rows, unsigned columns)
{
	this->width = width;
	this->height = height;
	this->rows = rows;
	this->columns = columns;
	
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
	window = SDL_CreateWindow("Grid"
		, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
		, width, height
		, SDL_WINDOW_RESIZABLE);
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
			rect.x = rectWidth * col + outlinePx * col + outlinePx / 2;
			rect.y = rectHeight * row + outlinePx * row + outlinePx / 2;

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
	bool turnOn = true;

	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return;
			}
			else if (event.type == SDL_WINDOWEVENT) {
				switch (event.window.event) {
				//case SDL_WINDOWEVENT_SIZE_CHANGED:
				case SDL_WINDOWEVENT_RESIZED:
					width = event.window.data1;
					height = event.window.data2;
					rectWidth = (width - outlinePx * (columns - 1)) / columns;
					rectHeight = (height - outlinePx * (rows - 1)) / rows;
				}
				this->Render();
			}
			else if (event.button.button == SDL_BUTTON_LEFT) {
				unsigned x, y;
				unsigned row, col;
				x = event.button.x;
				y = event.button.y;
				row = y / (rectHeight + outlinePx);
				col = x / (rectWidth + outlinePx);
				if (row < rows && col < columns) {
					Color curr = grid[row][col];
					if (curr.r == 100 && curr.g == 255 && curr.b == 100) {
						if (event.button.state == SDL_PRESSED) {
							turnOn = false;
						}
						if (!turnOn) {
							grid[row][col] = Color{ 255, 255, 255, 255 };
						}
					}
					else {
						if (event.button.state == SDL_PRESSED) {
							turnOn = true;
						}
						if (turnOn) {
							grid[row][col] = Color{ 100, 255, 100, 255 };
						}
					}
				}
				this->Render();
			}
		}


		SDL_Delay(16); // ~60 updates per second
	}
}