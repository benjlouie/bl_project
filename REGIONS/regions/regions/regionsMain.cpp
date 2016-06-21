#include "gridSDL.h"

int main(int argc, char **argv)
{
	unsigned sideLength = 20;
	unsigned rows = 30;
	unsigned cols = 30;

	Grid grid(sideLength * cols, sideLength * rows, rows, cols);


	return 0;
}