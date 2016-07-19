#include <vector>
#include <SDL2/SDL.h>


using namespace std;

class Grid {
public:
	struct Cell {
		unsigned row, col;
	};

	Grid(unsigned window_width, unsigned window_height, unsigned rows, unsigned columns, unsigned outlinePx = 0);
	void SetBackground(SDL_Color color);
	void SetOutlinePx(unsigned outlinePx);
	void SetCell(Cell cell, SDL_Color color);
	void SetRow(unsigned row, SDL_Color color);
	void SetColumn(unsigned column, SDL_Color color);
	void SetAll(SDL_Color color);
	SDL_Color GetCellColor(Cell cell);
	Cell GetCellFromCoordinate(int xCoordinate, int yCoordinate);
	void Render(void);
	void RenderAll(void);
	~Grid(void);

	//TODO: remove debug func
	void drawLine(Cell start, Cell end, SDL_Color color);
	void Present(void);

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
	unsigned outlinePx;
	SDL_Color background;

	void RenderCell(Cell cell);
	void RenderRow(unsigned row);
	void RenderColumn(unsigned col);
};