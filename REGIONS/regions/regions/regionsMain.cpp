#include "gridSDL.h"

void EventHandler(bool *loop, bool *render);

Grid *grid;

int main(int argc, char **argv)
{
	unsigned sideLength = 10;
	unsigned rows = 70;
	unsigned cols = 70;

	grid = new Grid(sideLength * cols, sideLength * rows, rows, cols);

	cout << "test string" << endl;

	bool loop = true;
	while (loop) {
		bool render = false;
		EventHandler(&loop, &render);
		if (render) {
			grid->Render();
		}
		SDL_Delay(16); // ~60 updates per second
	}

	delete grid;
	return 0;
}

void EventHandler(bool *loop, bool *render)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			*loop = false; //TODO: close all windows here
			break;
		}
		else if (event.type == SDL_WINDOWEVENT) {
			switch (event.window.event) {
			case SDL_WINDOWEVENT_CLOSE:
				//TODO: only close window with same windowID
				grid->Close();
				break;
			case SDL_WINDOWEVENT_RESIZED:
				*render = true;
				break;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				unsigned x, y;
				x = event.button.x;
				y = event.button.y;
				Grid::Cell cell = grid->cellFromCoordinate(x, y);

				grid->SetCell(cell, SDL_Color{ 100, 180, 0, SDL_ALPHA_OPAQUE });
			}
			*render = true;
		}
	}


}