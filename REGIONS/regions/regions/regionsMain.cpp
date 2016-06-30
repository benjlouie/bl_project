#include "gridSDL.h"
#include <Windows.h>
#include <string>

void EventHandler(bool *loop, bool *render);

Grid *g_grid;
bool g_colorToggle = true;

int main(int argc, char **argv)
{
	unsigned sideLength = 10;
	unsigned rows = 70;
	unsigned cols = 70;

	g_grid = new Grid(sideLength * cols, sideLength * rows, rows, cols);

	cout << "test string" << endl;

	bool loop = true;
	while (loop) {
		bool render = false;
		EventHandler(&loop, &render);
		if (render) {
			g_grid->Render();
		}
		SDL_Delay(16); // ~60 updates per second
	}

	g_grid->Close();
	delete g_grid;
	return EXIT_SUCCESS;
}

void EventHandler(bool *loop, bool *render)
{
	SDL_Color curr;
	Grid::Cell cell;
	unsigned x, y;
	string tmp; //todo: delete

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			*loop = false; //TODO: close all windows here
			return;
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			case SDL_WINDOWEVENT_CLOSE:
				//TODO: only close window with same windowID
				*loop = false;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				*render = true;
				break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEMOTION:
			x = event.button.x;
			y = event.button.y;
			cell = g_grid->getCellFromCoordinate(x, y);
			curr = g_grid->getCellColor(cell);

			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
				//TODO: these are test colors, change eventually
				if (curr.r == 100 && curr.g == 255 && curr.b == 100) {
					if (event.button.state == SDL_PRESSED) {
						g_colorToggle = false;
					}
					if (!g_colorToggle) {
						g_grid->SetCell(cell, SDL_Color{ 255, 255, 255, SDL_ALPHA_OPAQUE });
					}
				}
				else {
					if (event.button.state == SDL_PRESSED) {
						g_colorToggle = true;
					}
					if (g_colorToggle) {
						g_grid->SetCell(cell, SDL_Color{ 100, 255, 100, SDL_ALPHA_OPAQUE });
					}
				}
				*render = true;
				break;
			case SDL_BUTTON_RIGHT:
			case SDL_BUTTON_X1: //TODO: find out why holding RMB outputs 4 to event
				//TODO: these are test colors, change eventually
				if (curr.r == 100 && curr.g == 255 && curr.b == 100) {
					if (event.button.state == SDL_PRESSED) {
						g_colorToggle = false;
					}
					if (!g_colorToggle) {
						g_grid->SetRow(cell.row, SDL_Color{ 255, 255, 255, SDL_ALPHA_OPAQUE });
					}
				}
				else {
					if (event.button.state == SDL_PRESSED) {
						g_colorToggle = true;
					}
					if (g_colorToggle) {
						g_grid->SetRow(cell.row, SDL_Color{ 100, 255, 100, SDL_ALPHA_OPAQUE });
					}
				}
				*render = true;
				break;
			}
		}
	}
}