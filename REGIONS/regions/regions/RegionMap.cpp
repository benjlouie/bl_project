#include "RegionMap.h"

#define UNSET_OBSTACLE -1

RegionMap::RegionMap(unsigned rows, unsigned cols)
{
	rows_ = rows;
	cols_ = cols;
	numObstacles_ = 0;

	map_ = new CellData*[rows];
	for (unsigned x = 0; x < rows_; x++) {
		map_[x] = new CellData[cols_];
		for (unsigned y = 0; y < cols_; y++) {
			//TODO: make sure struct fill here is correct
			map_[x][y] = CellData{ true, UNSET_OBSTACLE, nullptr };
		}
	}
}

void RegionMap::SetCell(Cell cell, bool cellOpen, void *data)
{
	map_[cell.row][cell.col].open = cellOpen;
	map_[cell.row][cell.col].userData = data;

	//TODO: add to connected obstacle if aplicable
}

void RegionMap::SetAll(bool cellOpen, void *data)
{
	for (unsigned r = 0; r < rows_; r++) {
		for (unsigned c = 0; c < cols_; c++) {
			map_[r][c].open = cellOpen;
			map_[r][c].userData = data;
		}
	}
}

void RegionMap::IdentifyObstacles(void)
{
	//loop through each cell
	for (unsigned r = 0; r < rows_; r++) {
		for (unsigned c = 0; c < cols_; c++) {
			if (map_[r][c].open || map_[r][c].obstacleGroupID != -1) {
				continue;
			}
			//if cell is closed, breadth first search from there and identify obstacle
			MarkObstacle(Cell{ r, c });
			//increase numObstacles
			numObstacles_++;
		}
	}
}

// breadth first search to identify and mark (ID) an obstacle
void RegionMap::MarkObstacle(Cell startCell)
{
	//TODO: make options for if diagonals still connect
	int adjOffset[4][2] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
	std::queue<Cell> visited;

	map_[startCell.row][startCell.col].obstacleGroupID = numObstacles_;
	visited.push(startCell);
	while (visited.size() > 0) {
		Cell curr = visited.front();
		visited.pop();
		for (unsigned i = 0; i < 4; i++) {
			int nextRow = curr.row + adjOffset[i][0];
			int nextCol = curr.col + adjOffset[i][1];
			CellData *nextCell = &map_[nextRow][nextCol];

			if (nextCell->open || nextCell->obstacleGroupID != UNSET_OBSTACLE
				|| (nextRow < 0 || nextRow > rows_)
				|| (nextCol < 0 || nextCol > cols_)
				) {
				continue; //cell not part of obstacle or already ID'd, or out of bounds
			}
			nextCell->obstacleGroupID = numObstacles_;
			visited.push(Cell{ nextRow, nextCol });
		}
	}
}

RegionMap::CellData RegionMap::GetCellData(Cell cell)
{
	return map_[cell.row][cell.col];
}

unsigned RegionMap::GetRows(void)
{
	return rows_;
}

unsigned RegionMap::GetCols(void)
{
	return cols_;
}

RegionMap::~RegionMap(void)
{
	for (unsigned x = 0; x < rows_; x++) {
		delete[] map_[x];
	}
	delete[] map_;
}