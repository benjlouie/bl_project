#include "RegionMap.h"

#define UNSET_OBSTACLE -1
#define UNSET_CORNER -1
#define UNSET_REGION -1

RegionMap::RegionMap(unsigned rows, unsigned cols)
{
	rows_ = rows;
	cols_ = cols;
	numObstacles_ = 0;
	numCorners_ = 0;
	numRegions_ = 0;

	map_ = new CellData*[rows];
	for (unsigned x = 0; x < rows_; x++) {
		map_[x] = new CellData[cols_];
		for (unsigned y = 0; y < cols_; y++) {
			//TODO: make sure struct fill here is correct
			map_[x][y] = CellData{ true
				, UNSET_OBSTACLE, UNSET_CORNER, UNSET_REGION
				, Cell{ -1, -1 }, Cell{ -1, -1 }
				, false, false, false
				, nullptr };
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

			//check if next cell not part of obstacle or already ID'd
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
				map_[curr.row][curr.col].cornerID = numCorners_;
				numCorners_++;
				cornerCells_.push_back(curr);
				map_[curr.row][curr.col].cornerCell = curr;
				break;
			}
		}
	}
}

//TODO: look diagonal, not between two walls
//TODO: go through adjOffset randomly (or differently) each time
//TODO: find way to get good nodes from edge (-1 connects) nodes (maybe go halfway between corner and node?)
//breadth first search from every corner, find node positions and identify region separators
void RegionMap::IdentifyNodes(void)
{
	//TODO: find out if 4 or 8 adjcency is better
	//TODO: find optimal adj pattern (star, circular, etc)
	//int adjOffset[8][2] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
	int adjOffset[8][2] = { { 0, 1 }, { 0, -1 }, { -1, 0 }, { 1, 0 }, { 1, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 } };
	std::queue<Cell> visited;

	//start queue with all the corners
	for (Cell cell : cornerCells_) {
		visited.push(cell);
		int cornerId = map_[cell.row][cell.col].cornerID;
		//cornerConnects_[cornerId]; //add all corner IDs to the map
	}
	//cornerConnects_[-1]; //set out of bounds as a "corner"

	while (visited.size() > 0) {
		Cell curr = visited.front();
		visited.pop();
		int currCornerId = map_[curr.row][curr.col].cornerID;
		bool isNode = false;

		for (int i = 0; i < 8; i++) {
			unsigned nextRow = curr.row + adjOffset[i][0];
			unsigned nextCol = curr.col + adjOffset[i][1];

			if (nextRow >= rows_ || nextCol >= cols_) {
				//out of bounds, use -1 for this "corner"
				if (cornerConnects_[currCornerId].count(-1) == 0) {
					cornerConnects_[currCornerId].insert(-1);
					//cornerConnects_[-1].insert(currCornerId);
					map_[curr.row][curr.col].node = true;
					nodeCells_.push_back(curr);
				}
				continue;
			}

			CellData *nextCell = &map_[nextRow][nextCol];
			if (nextCell->open == false) {
				continue; //ignore walls and other corners
			}

			if (nextCell->cornerID == UNSET_CORNER) {
				//open node, mark it and add look at it later
				nextCell->cornerCell = map_[curr.row][curr.col].cornerCell; //TODO: check
				nextCell->prevCell = curr;
				nextCell->cornerID = currCornerId;
				visited.push(Cell{ nextRow, nextCol });
				continue;
			}
			else if (nextCell->cornerID == currCornerId) {
				continue;
			} else {
				//encountered path from other corner, add curr node list
				//only add first encounter with other corner
				if (cornerConnects_[currCornerId].count(nextCell->cornerID) == 0) {
					//corners not connected yet, this is node position
					//add cornerID's to each list, set both cells as nodes
					cornerConnects_[currCornerId].insert(nextCell->cornerID);
					cornerConnects_[nextCell->cornerID].insert(currCornerId);

					//TODO: check this
					nodeCells_.push_back(Cell{ nextRow, nextCol });
					nextCell->node = true;

					isNode = true;

					//TODO: delete debug
					std::string tmp = std::to_string(currCornerId) + " " + std::to_string(nextCell->cornerID) 
						+ "\t\t" + std::to_string(curr.row) + "," + std::to_string(curr.col)
						+ "\t" + std::to_string(nextRow) + "," + std::to_string(nextCol)
						+ "\n";
					OutputDebugStringA(tmp.c_str());
				}
			}

			if (isNode && map_[curr.row][curr.col].open) { //only open cells can be nodes
				map_[curr.row][curr.col].node = true;
				nodeCells_.push_back(curr);
			}
		}
	}
}

//follow node paths back and set those paths
//remove corner/node data from all other cells
void RegionMap::SanitizeNodeData(void)
{
	//follow node paths
	for (Cell node : nodeCells_) {
		CellData *curr = &map_[node.row][node.col];
		while (curr->prevCell != Cell{ -1, -1 }) {
			curr->nodePath = true;
			Cell prev = curr->prevCell;
			curr = &map_[prev.row][prev.col];
		}
	}

	//clear node data from other cells
	for (unsigned r = 0; r < rows_; r++) {
		for (unsigned c = 0; c < cols_; c++) {
			CellData *data = &map_[r][c];
			if (!data->open || data->nodePath) {
				continue;
			}

			data->prevCell = Cell{ -1, -1 };
			data->cornerCell = Cell{ -1, -1 };
			data->cornerID = UNSET_CORNER;
		}
	}
}

void RegionMap::IdentifyRegions(void)
{
	//loop through each cell
	for (unsigned r = 0; r < rows_; r++) {
		for (unsigned c = 0; c < cols_; c++) {
			CellData &data = map_[r][c];
			if (!data.open 
				|| data.nodePath
				|| data.regionID != UNSET_REGION) {
				continue;
			}

			//search out and identify that region
			MarkRegion(Cell{ r, c });
			numRegions_++;
		}
	}
}

//TODO: make the adj 8 directions, don't allow diags that go between walls
void RegionMap::MarkRegion(Cell startCell)
{
	int adjOffset[4][2] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
	std::queue<Cell> visited;

	map_[startCell.row][startCell.col].regionID = numRegions_;
	visited.push(startCell);
	while (visited.size() > 0) {
		Cell curr = visited.front();
		visited.pop();

		for (unsigned i = 0; i < 4; i++) {
			unsigned nextRow = curr.row + adjOffset[i][0];
			unsigned nextCol = curr.col + adjOffset[i][1];

			//only check greater b/c of unsigned overflow
			if (nextRow >= rows_ || nextCol >= cols_) {
				continue;
			}

			CellData &nextCell = map_[nextRow][nextCol];
			if (!nextCell.open || nextCell.nodePath || nextCell.regionID != UNSET_REGION) {
				continue;
			}

			nextCell.regionID = numRegions_;
			visited.push(Cell{ nextRow, nextCol });
		}
	}
}

void RegionMap::ClearObstacleData(void)
{
	//TODO: make sure everything is cleared properly
	for (unsigned r = 0; r < rows_; r++) {
		for (unsigned c = 0; c < cols_; c++) {
			map_[r][c].obstacleGroupID = UNSET_OBSTACLE;
			map_[r][c].cornerID = UNSET_CORNER;
			map_[r][c].regionID = UNSET_REGION;
			map_[r][c].prevCell = Cell{ -1, -1 };
			map_[r][c].cornerCell = Cell{ -1, -1 };
			map_[r][c].corner = false;
			map_[r][c].node = false;
			map_[r][c].nodePath = false;
		}
	}
	numObstacles_ = 0;
	numCorners_ = 0;
	cornerCells_.clear();
	nodeCells_.clear();
	cornerConnects_.clear();
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