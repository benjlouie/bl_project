#include "gridSDL.h"
#include "RegionMap.h"

void EventHandler(bool *loop, bool *render);
void EventHandler_Mouse(SDL_Event *event, bool *loop, bool *render);

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

	delete g_grid;
	return EXIT_SUCCESS;
}

void EventHandler(bool *loop, bool *render)
{
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
			EventHandler_Mouse(&event, loop, render);
			break;
		}
	}
}

void EventHandler_Mouse(SDL_Event *event, bool *loop, bool *render)
{
	//TODO: these are test colors, change eventually
	SDL_Color colorOn = SDL_Color{ 100, 255, 100, SDL_ALPHA_OPAQUE };
	SDL_Color colorOff = SDL_Color{ 255, 255, 255, SDL_ALPHA_OPAQUE };

	SDL_Color curr;
	Grid::Cell cell;
	unsigned x, y;

	x = event->button.x;
	y = event->button.y;
	cell = g_grid->GetCellFromCoordinate(x, y);
	curr = g_grid->GetCellColor(cell);

	switch (event->button.button) {
	case SDL_BUTTON_LEFT:
		if (curr.r == 100 && curr.g == 255 && curr.b == 100) {
			if (event->button.state == SDL_PRESSED) {
				g_colorToggle = false;
			}
			if (!g_colorToggle) {
				g_grid->SetCell(cell, colorOff);
			}
		}
		else {
			if (event->button.state == SDL_PRESSED) {
				g_colorToggle = true;
			}
			if (g_colorToggle) {
				g_grid->SetCell(cell, colorOn);
			}
		}
		*render = true;
		break;
	case SDL_BUTTON_RIGHT:
	case SDL_BUTTON_X1: //TODO: find out why holding RMB outputs 4 to event
		if (curr.r == 100 && curr.g == 255 && curr.b == 100) {
			if (event->button.state == SDL_PRESSED) {
				g_colorToggle = false;
			}
			if (!g_colorToggle) {
				g_grid->SetRow(cell.row, colorOff);
			}
		}
		else {
			if (event->button.state == SDL_PRESSED) {
				g_colorToggle = true;
			}
			if (g_colorToggle) {
				g_grid->SetRow(cell.row, colorOn);
			}
		}
		*render = true;
		break;
	case SDL_BUTTON_MIDDLE:
		if (curr.r == 100 && curr.g == 255 && curr.b == 100) {
			if (event->button.state == SDL_PRESSED) {
				g_colorToggle = false;
			}
			if (!g_colorToggle) {
				g_grid->SetColumn(cell.col, colorOff);
			}
		}
		else {
			if (event->button.state == SDL_PRESSED) {
				g_colorToggle = true;
			}
			if (g_colorToggle) {
				g_grid->SetColumn(cell.col, colorOn);
			}
		}
		*render = true;
		break;
	}
}