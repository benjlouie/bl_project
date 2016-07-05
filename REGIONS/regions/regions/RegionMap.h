#include <queue>
#include <unordered_map>
#include <string>

class RegionMap {
public:
	struct Cell {
		unsigned row, col;
	};
	struct CellData {
		bool open;
		int obstacleGroupID;
		bool corner;
		void *userData;
	};


public:
	RegionMap(unsigned rows, unsigned cols, bool diagObstacles = false);
	void SetCell(Cell cell, bool cellOpen, void *data = nullptr);
	void SetAll(bool cellOpen, void *data);
	void IdentifyObstacles(void);
	void ClearObstacleData(void);
	void AllowDiagonalObstacles(bool diags);
	void IdentifyCorners(void);
	CellData GetCellData(Cell cell);
	unsigned GetRows(void);
	unsigned GetCols(void);

	~RegionMap(void);
private:
	CellData **map_;
	unsigned rows_;
	unsigned cols_;
	unsigned numObstacles_;
	bool diagonalObstacles_;

	void MarkObstacle(Cell startCell);
	void MarkCorners(Cell startCell);
};