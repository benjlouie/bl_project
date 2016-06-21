#include <iostream>
#include <thread>
#include <SDL2/SDL.h>


using namespace std;

struct Color
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};

class Grid {
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	Color **grid;
	unsigned width;
	unsigned height;
	unsigned rows;
	unsigned columns;
	unsigned rectWidth;
	unsigned rectHeight;
	unsigned outlinePx = 2;
	Color background;

	void EventHandler(void);
public:
	Grid(unsigned window_width, unsigned window_height, unsigned grid_width, unsigned grid_height);
	void SetBackground(Color color);
	void SetColor(unsigned row, unsigned col, Color color);
	void SetRow(unsigned row, Color color);
	void SetColumn(unsigned column, Color color);
	void SetAll(Color color);
	void Render(void);
	void close(void);
};