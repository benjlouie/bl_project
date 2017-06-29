#include <iostream>
#include "Object.h"
#include "Space2D.h"

template<typename K>
void printRegion(Region<K> r);

int main(int argc, char** argv)
{
	Region<double> bounds{ 0,1,0,1 };
	Region<double> hit1{ 0.25,0.75,0.25,0.75 };
	Region<double> hit2{ 0,1,0,0.5 };
	std::vector<Region<double>> hits{ hit1, hit2 };
	Object o1({ 0,2,0,2 });
	Object o2(bounds, { hit1,{ 0,1,0,0.5 } }); //calls on rvalue, uses move constructor
	Object o3(bounds, { { 0.25,0.75,0.25,0.75 },hit2 });
	Object o4(bounds, { hit1,hit2 });
	Object o5(bounds, hits);
	Object o7(bounds, std::move(hits));
	Object o9(bounds);

	Object o10(o4);
	Object o11(std::move(o4));
	o1 = o3;
	o1 = std::move(o3);

	Object o12({ 0,0,2,2 }, { hit1,hit2 });

	o1.checkCollision(hit1);

	Region<double> space_bounds{ DBL_MIN, DBL_MAX, DBL_MIN, DBL_MAX };
	Space2D space(space_bounds);

	Region<double> space_position{ 100,200,100,200 };
	Object space_object(bounds);
	//space.insertObject(1, space_object, space_position);

	return EXIT_SUCCESS;
}

template<typename K>
void printRegion(Region<K> r)
{
	std::cout << r.yLow_ << ", " << r.yHigh_ << "; " << r.xLow_ << ", " << r.xHigh_ << "\n";
}