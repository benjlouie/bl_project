#include "gridSDL.h"
#include "RegionMap.h"

void TransferGrid(unsigned rows, unsigned cols);
void EventHandler(bool *loop, bool *render, bool *transfer);
void EventHandler_Mouse(SDL_Event *event, bool *loop, bool *render, bool *transfer);

Grid *g_grid;
RegionMap *g_map;
bool g_colorToggle = true;

int main(int argc, char **argv)
{
	unsigned sideLength = 10;
	unsigned rows = 70;
	unsigned cols = 70;

	g_grid = new Grid(sideLength * cols, sideLength * rows, rows, cols, 1);
	g_map = new RegionMap(rows, cols, true);

	g_map->IdentifyObstacles();
	TransferGrid(rows, cols);
	g_grid->Render();

	bool loop = true;
	while (loop) {
		bool render = false;
		bool transfer = false;
		EventHandler(&loop, &render, &transfer);
		if (transfer) {
			g_map->ClearObstacleData();
			g_map->IdentifyObstacles();
			g_map->IdentifyCorners();
			TransferGrid(rows, cols);
		}
		if (render) {
			g_grid->Render();
		}
		SDL_Delay(16); // ~60 updates per second
	}

	delete g_grid;
	return EXIT_SUCCESS;
}

void TransferGrid(unsigned rows, unsigned cols)
{
	for (unsigned r = 0; r < rows; r++) {
		for (unsigned c = 0; c < cols; c++) {
			RegionMap::Cell cell = RegionMap::Cell{ r, c };
			RegionMap::CellData data = g_map->GetCellData(cell);
			
			if (!data.open) {
				//color different obstacles
				SDL_Color obstacleColor = SDL_Color{ 50 * (data.obstacleGroupID % 5)
					, 50 * (data.obstacleGroupID % 25) / 5
					, 50 * (data.obstacleGroupID % 125) / 25
					, SDL_ALPHA_OPAQUE };
				SDL_Color cornerColor = SDL_Color{ 255, 140, 0, SDL_ALPHA_OPAQUE };
				if (data.corner) {
					g_grid->SetCell(Grid::Cell{ r, c }, cornerColor);
				}
				else {
					g_grid->SetCell(Grid::Cell{ r, c }, obstacleColor);
				}
				continue;
			}
			//TODO: colors for open cells
		}
	}
}

void EventHandler(bool *loop, bool *render, bool *transfer)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			*loop = false;
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
			EventHandler_Mouse(&event, loop, render, transfer);
			break;
		}
	}
}

void EventHandler_Mouse(SDL_Event *event, bool *loop, bool *render, bool *transfer)
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

	RegionMap::Cell mapCell = RegionMap::Cell{ cell.row, cell.col };

	switch (event->button.button) {
	case SDL_BUTTON_LEFT:
		if (curr.r == 100 && curr.g == 255 && curr.b == 100) {
			if (event->button.state == SDL_PRESSED) {
				g_colorToggle = false;
			}
			if (!g_colorToggle) {
				g_grid->SetCell(cell, colorOff);
				g_map->SetCell(mapCell, true);
			}
		}
		else {
			if (event->button.state == SDL_PRESSED) {
				g_colorToggle = true;
			}
			if (g_colorToggle) {
				g_grid->SetCell(cell, colorOn);
				g_map->SetCell(mapCell, false);
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
		*transfer = true;
		*render = true;
		break;
	}
}