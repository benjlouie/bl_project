#include <queue>

class RegionMap {
public:
	struct Cell {
		unsigned row, col;
	};
	struct CellData {
		bool open;
		int obstacleGroupID;
		void *userData;
	};


public:
	RegionMap(unsigned rows, unsigned cols);
	void SetCell(Cell cell, bool cellOpen, void *data = nullptr);
	void SetAll(bool cellOpen, void *data);
	void IdentifyObstacles(void);

	CellData GetCellData(Cell cell);
	unsigned GetRows(void);
	unsigned GetCols(void);

	~RegionMap(void);
private:
	CellData **map_;
	unsigned rows_;
	unsigned cols_;
	unsigned numObstacles_;

	void MarkObstacle(Cell cell);
};