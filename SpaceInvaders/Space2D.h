#pragma once

#include <set>
#include <unordered_map>
#include "../Quadtree/Quadtree.h"
#include "Object.h"

struct SpaceObject
{
	uint64_t id;
	Object object;
	//TODO: ptr to
};

class Space2D
{
public:
	Space2D(Region<double>& space_bounds);
	Space2D(Region<double>&& space_bounds);
	~Space2D();

	void insertObject(uint64_t id, Object& object, Region<double>& position);
	void moveObject(uint64_t id, double change_x, double change_y);

private:
	Quadtree<double, SpaceObject*>* space_;
	std::unordered_map<uint64_t, Object*> object_reference_;
	std::unordered_map<uint64_t, std::set<uint64_t>> object_collisions_; //TODO: think about if this is worth it
};

Space2D::Space2D(Region<double>& space_bounds)
{
	space_ = new Quadtree<double, SpaceObject*>(space_bounds);
}

Space2D::Space2D(Region<double>&& space_bounds)
{
	space_ = new Quadtree<double, SpaceObject*>(space_bounds);
}

Space2D::~Space2D()
{
	for (auto object_pair : object_reference_) {
		delete object_pair.second;
	}
	delete space_;
}

void Space2D::insertObject(uint64_t id, Object& object, Region<double>& position)
{
	SpaceObject* space_object = new	SpaceObject;
	space_object->id = id;
	space_object->object = object;

	object_reference_.emplace(id, &space_object->object);
	space_->insert(position, space_object);

	//keep track of intersections for each object
	auto intersectors = space_->getRegionsIntersect(position);
	for (auto space_obj : intersectors) {
		object_collisions_[id].insert(space_obj.second->id);
		object_collisions_[space_obj.second->id].insert(id);
	}
}

void Space2D::moveObject(uint64_t id, double change_x, double change_y)
{
	//TODO: implement
	//handle changes in collisions
	////local copy of old collisions
	////find new collisions first (remove equal ones from local copy)
	////from collisions remaining in local copy: activate leaving trigger
	//////TODO: will need a buffer of leaving triggers that only goes once per game tick (after doing all object moves)
}
