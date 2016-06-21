#include "gridSDL.h"

int main(int argc, char **argv)
{
	unsigned sideLength = 50;
	unsigned rows = 20;
	unsigned cols = 20;

	Grid grid(sideLength * cols, sideLength * rows, rows, cols);
	/*
	grid.SetBackground(Color{ 0, 0, 0, 255 });
	grid.SetRow(2, Color{ 34, 134, 100, 255 });
	grid.SetColumn(5, Color{ 134, 65, 12, 255 });
	grid.SetColor(12, 16, Color{ 255, 20, 90, 255 });
	grid.Render();
	*/

	return 0;
}