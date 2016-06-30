#include <iostream>
#include <thread>
#include <vector>
#include <SDL2/SDL.h>


using namespace std;

class Grid {
public:
	struct Cell {
		unsigned row, col;
	};

	Grid(unsigned window_width, unsigned window_height, unsigned rows, unsigned columns);
	void SetBackground(SDL_Color color);
	void SetCell(Cell cell, SDL_Color color);
	void SetRow(unsigned row, SDL_Color color);
	void SetColumn(unsigned column, SDL_Color color);
	void SetAll(SDL_Color color);
	Cell cellFromCoordinate(int xCoordinate, int yCoordinate);
	void Render(void);
	void Close(void);

private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Color **grid;
	vector<Cell> changedCells;
	vector<unsigned> changedRows;
	vector<unsigned> changedColumns;
	bool renderAll;
	int width;
	int height;
	unsigned rows;
	unsigned columns;
	unsigned rectWidth;
	unsigned rectHeight;
	unsigned outlinePx = 1;
	SDL_Color background;

	void RenderAll(void);
	void RenderCell(Cell cell);
	void RenderRow(unsigned row);
	void RenderColumn(unsigned col);
	//void EventHandler(void);
};