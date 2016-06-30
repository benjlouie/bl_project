#include "gridSDL.h"

Grid::Grid(unsigned window_width, unsigned window_height, unsigned rows, unsigned columns)
{
	this->width = window_width;
	this->height = window_height;
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
	grid = new SDL_Color*[rows];
	for (unsigned i = 0; i < rows; i++) {
		grid[i] = new SDL_Color[columns];
		memset(grid[i], 255, sizeof(SDL_Color) * columns); //default to white
	}

	renderAll = true;
	Render();
	//TODO: set up event handler in main somehow
	//EventHandler();
}

void Grid::SetBackground(SDL_Color color)
{
	background = color;
}

void Grid::SetCell(Cell cell, SDL_Color color)
{
	unsigned row = cell.row;
	unsigned col = cell.col;
	if (row < rows && col < columns) {
		grid[row][col] = color;
		changedCells.push_back(Cell{ row, col });
	}
}

void Grid::SetRow(unsigned row, SDL_Color color)
{
	if (row < rows) {
		for (unsigned i = 0; i < rows; i++) {
			grid[row][i] = color;
		}
		changedRows.push_back(row);
	}
}

void Grid::SetColumn(unsigned column, SDL_Color color)
{
	if (column < columns) {
		for (unsigned i = 0; i < rows; i++) {
			grid[i][column] = color;
		}
		changedRows.push_back(column);
	}
}

void Grid::SetAll(SDL_Color color)
{
	for (unsigned r = 0; r < rows; r++) {
		for (unsigned c = 0; c < columns; c++) {
			grid[r][c] = color;
		}
	}
}

SDL_Color Grid::getCellColor(Grid::Cell cell)
{
	if (cell.row < rows && cell.col < columns) {
		return grid[cell.row][cell.col];
	}
	return SDL_Color{ 0, 0, 0, SDL_ALPHA_TRANSPARENT }; //TODO: add error somewhere to show this
}

Grid::Cell Grid::getCellFromCoordinate(int xCoordinate, int yCoordinate)
{
	unsigned row, col;
	row = yCoordinate / (rectHeight + outlinePx);
	col = xCoordinate / (rectWidth + outlinePx);
	return Cell{ row, col };
}

void Grid::Render(void)
{
	//account for window resizes
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	if (w != width || h != height) {
		width = w;
		height = h;
		renderAll = true;
		rectWidth = (width - outlinePx * (columns - 1)) / columns;
		rectHeight = (height - outlinePx * (rows - 1)) / rows;
	}

	if (renderAll) {
		RenderAll();
		SDL_RenderPresent(renderer);
		renderAll = false;
		return;
	}

	for (unsigned row : changedRows) {
		RenderRow(row);
	}
	changedRows.clear();
	for (unsigned col : changedColumns) {
		RenderColumn(col);
	}
	changedColumns.clear();
	for (Cell cell : changedCells) {
		RenderCell(cell);
	}
	changedCells.clear();

	SDL_RenderPresent(renderer);
}

//TODO: make it only render the changed cells, or all of them (maybe have renderAll() function?)
void Grid::RenderAll(void)
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
}

void Grid::RenderCell(Cell cell)
{
	unsigned row = cell.row;
	unsigned col = cell.col;
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

void Grid::RenderRow(unsigned row)
{
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

void Grid::RenderColumn(unsigned col)
{
	for (unsigned row = 0; row < rows; row++) {
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

void Grid::Close(void)
{
	SDL_DestroyWindow(window);

	for (unsigned r = 0; r < rows; r++) {
		delete[] grid[r];
	}
	delete[] grid;
}

//TODO: redo or delete
/*
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
					SDL_Color curr = grid[row][col];
					if (curr.r == 100 && curr.g == 255 && curr.b == 100) {
						if (event.button.state == SDL_PRESSED) {
							turnOn = false;
						}
						if (!turnOn) {
							grid[row][col] = SDL_Color{ 255, 255, 255, 255 };
						}
					}
					else {
						if (event.button.state == SDL_PRESSED) {
							turnOn = true;
						}
						if (turnOn) {
							grid[row][col] = SDL_Color{ 100, 255, 100, 255 };
						}
					}
				}
				this->Render();
			}
		}

		SDL_Delay(16); // ~60 updates per second
	}
}
*/