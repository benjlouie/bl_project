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
			map_[x][y] = CellData{ true, UNSET_OBSTACLE, false, nullptr };
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
	int adjOffset[8][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } };
	bool adjCellOpen[12];
	std::queue<Cell> visited;

	map_[startCell.row][startCell.col].obstacleGroupID = numObstacles_;
	visited.push(startCell);
	while (visited.size() > 0) {
		Cell curr = visited.front();
		visited.pop();

		memset(adjCellOpen, false, sizeof(bool) * 8); //reset array used in cornercount to zero

		unsigned numAdj = 8;
		for (unsigned i = 0; i < numAdj; i++) {
			unsigned nextRow = curr.row + adjOffset[i][0];
			unsigned nextCol = curr.col + adjOffset[i][1];

			//only check greater b/c of unsigned overflow
			if (nextRow >= rows_ || nextCol >= cols_) {
				continue;
			}

			//check if cell not part of obstacle or already ID'd
			CellData *nextCell = &map_[nextRow][nextCol];
			if (nextCell->open) {
				adjCellOpen[i] = true;
				continue;
			}
			if (nextCell->obstacleGroupID != UNSET_OBSTACLE) {
				continue;
			}

			nextCell->obstacleGroupID = numObstacles_;
			visited.push(Cell{ nextRow, nextCol });
		}

		//check if corner
		unsigned cornerCount = 0;
		memcpy(adjCellOpen + 8, adjCellOpen, sizeof(bool) * 4); //create a "loop" to correctly check for corners
		for (unsigned i = 0; i < 11; i++) { // 8 + 3 == 11, max rotation needed for 4+ open
			adjCellOpen[i] ? cornerCount++ : cornerCount = 0;
			if (cornerCount >= 4) { // 4+ consecutive open cells around curr (corner?)
				map_[curr.row][curr.col].corner = true;
				break;
			}
		}
	}
}

void RegionMap::ClearObstacleData(void)
{
	for (unsigned r = 0; r < rows_; r++) {
		for (unsigned c = 0; c < cols_; c++) {
			map_[r][c].obstacleGroupID = UNSET_OBSTACLE;
			map_[r][c].corner= false;
		}
	}
	numObstacles_ = 0;
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