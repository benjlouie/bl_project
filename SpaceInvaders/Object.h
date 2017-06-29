#pragma once
#include <vector>
#include "../Quadtree/Quadtree.h"

class Object
{
public:
	//constructors
	Object(const Object& o); //copy constructor
	Object(Object&& o); //move constructor
	Object(Region<double>& bounding_box);
	Object(Region<double>&& bounding_box);
	Object(Region<double>& bounding_box, std::vector<Region<double>>& hit_boxes);
	Object(Region<double>& bounding_box, std::vector<Region<double>>&& hit_boxes);
	Object(Region<double>&& bounding_box, std::vector<Region<double>>&& hit_boxes);
	Object() = default;
	~Object() = default;
	//operators
	Object& operator=(const Object& rhs); //copy assignment operator
	Object& operator=(Object&& rhs); //move assignment operator

	//void setSprite(sprite file);
	void setSpritePosition(Region<double> &sprite_position);
	void setHitBoxes(std::vector<Region<double>>& hit_boxes);
	void setHitBoxes(std::vector<Region<double>>&& hit_boxes);
	void addHitBox(Region<double> &hit_box);
	void removeHitBoxes(void);
	unsigned checkCollision(Region<double> collider);

private:
	Region<double> box_bounding_; //specifies size of bounding box
								  //TODO: change so only length,width are used
	std::vector<Region<double>> box_hits_; //% (0-1) based hitbox inside bounding box
										   //TODO: put in vector of actions (funcs) when hitbox is hit
	//sprite_ here
	Region<double> box_sprite_; //% (0-1) based on bounding box
};

Object::Object(const Object& o)
{
	box_bounding_ = o.box_bounding_;
	box_hits_ = o.box_hits_;
}
Object::Object(Object && o)
{
	box_bounding_ = o.box_bounding_;
	box_hits_ = std::move(o.box_hits_);
}
Object::Object(Region<double>& bounding_box) 
{
	box_bounding_ = bounding_box;
}
Object::Object(Region<double>&& bounding_box)
	: box_bounding_(std::forward<Region<double>>(bounding_box))
{
}
Object::Object(Region<double>& bounding_box, std::vector<Region<double>>& hit_boxes)
{
	box_bounding_ = bounding_box;
	box_hits_ = hit_boxes;
}
Object::Object(Region<double>& bounding_box, std::vector<Region<double>>&& hit_boxes)
	: box_hits_(std::forward<std::vector<Region<double>>>(hit_boxes))
{
	box_bounding_ = bounding_box;
}
Object::Object(Region<double>&& bounding_box, std::vector<Region<double>>&& hit_boxes)
	: box_bounding_(std::forward<Region<double>>(bounding_box))
	, box_hits_(std::forward<std::vector<Region<double>>>(hit_boxes))
{
}
Object& Object::operator=(const Object& rhs)
{
	box_bounding_ = rhs.box_bounding_;
	box_hits_ = rhs.box_hits_;
	return *this;
}
Object& Object::operator=(Object && rhs)
{
	box_bounding_ = rhs.box_bounding_;
	box_hits_ = std::move(rhs.box_hits_);
	return *this;
}

// set sprite position relative to bounding box
// x,y
// 0,0 | 1,0
// ----|----
// 0,1 | 1,1
void Object::setSpritePosition(Region<double>& sprite_position)
{
	box_sprite_ = sprite_position;
}

void Object::setHitBoxes(std::vector<Region<double>>& hit_boxes)
{
	box_hits_ = hit_boxes;
}
void Object::setHitBoxes(std::vector<Region<double>>&& hit_boxes)
{
	box_hits_ = std::forward<std::vector<Region<double>>>(hit_boxes);
}
void Object::addHitBox(Region<double>& hit_box)
{
	box_hits_.push_back(hit_box);
}
void Object::removeHitBoxes(void)
{
	box_hits_.clear();
}

unsigned Object::checkCollision(Region<double> collider)
{
	unsigned hit_count = 0;
	if (box_bounding_.intersect(collider)) {
		//inside bounding box, check for hitbox collisions
		double xLen = box_bounding_.xHigh_ - box_bounding_.xLow_;
		double yLen = box_bounding_.yHigh_ - box_bounding_.yLow_;

		for (auto hitbox : box_hits_) {
			//convert hitbox %s to actual coordinates
			Region<double> hitbox_actual{ xLen * hitbox.xLow_ + box_bounding_.xLow_
				, xLen * hitbox.xHigh_ + box_bounding_.xLow_
				, yLen * hitbox.yLow_ + box_bounding_.yLow_
				, yLen * hitbox.yHigh_ + box_bounding_.yLow_ };
			if (hitbox_actual.intersect(collider)) {
				hit_count++;
				//TODO: activate hitbox functions
			}
		}
	}
	return hit_count;
}

