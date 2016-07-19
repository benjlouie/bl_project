#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

//TODO: delete debug
#include <Windows.h>
//

class RegionMap {
public:
	struct Cell {
		unsigned row, col;

		bool operator==(const Cell &cell) const
		{
			return (this->row == cell.row
				&& this->col == cell.col);
		}
	};

	struct CellData {
		bool open;
		int obstacleGroupID;
		int cornerID;
		Cell prevCell;
		Cell cornerCell; //TODO: check if this works
		bool corner;
		bool node;
		void *userData;
	};


public:
	RegionMap(unsigned rows, unsigned cols);
	void SetCell(Cell cell, bool cellOpen, void *data = nullptr);
	void SetAll(bool cellOpen, void *data);
	void IdentifyObstacles(void);
	void IdentifyNodes(void);
	void ClearObstacleData(void);
	CellData GetCellData(Cell cell);
	unsigned GetRows(void);
	unsigned GetCols(void);

	~RegionMap(void);
private:
	CellData **map_;
	unsigned rows_;
	unsigned cols_;
	unsigned numObstacles_;
	unsigned numCorners_;
	std::vector<Cell> cornerCells_;
	std::vector<Cell> nodeCells_;
	std::unordered_map<int, std::unordered_set<int>> cornerConnects_; //CellData.cornerId type

	void MarkObstacle(Cell startCell);
	void MarkCorners(Cell startCell);
};

