#include "gridSDL.h"

Grid::Grid(unsigned window_width, unsigned window_height, unsigned rows, unsigned columns, unsigned outlinePx)
{
	this->width = window_width;
	this->height = window_height;
	this->rows = rows;
	this->columns = columns;
	this->outlinePx = outlinePx;
	
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
		, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
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
}

void Grid::SetBackground(SDL_Color color)
{
	background = color;
	renderAll = true;
}

void Grid::SetOutlinePx(unsigned outlinePx)
{
	this->outlinePx = outlinePx;
	renderAll = true;
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
		for (unsigned c = 0; c < columns; c++) {
			grid[row][c] = color;
		}
		changedRows.push_back(row);
	}
}

void Grid::SetColumn(unsigned column, SDL_Color color)
{
	if (column < columns) {
		for (unsigned r = 0; r < rows; r++) {
			grid[r][column] = color;
		}
		changedColumns.push_back(column);
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

SDL_Color Grid::GetCellColor(Cell cell)
{
	if (cell.row < rows && cell.col < columns) {
		return grid[cell.row][cell.col];
	}
	return SDL_Color{ 0, 0, 0, SDL_ALPHA_TRANSPARENT }; //TODO: add error somewhere to show this
}

Grid::Cell Grid::GetCellFromCoordinate(int xCoordinate, int yCoordinate)
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

Grid::~Grid(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	for (unsigned r = 0; r < rows; r++) {
		delete[] grid[r];
	}
	delete[] grid;
}