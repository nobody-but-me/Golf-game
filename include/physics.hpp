
#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include "./objects.hpp"

namespace Physics {
    bool isRectOnFloor(Objects::Rectangle *rect1, Objects::Rectangle *rect2);
    bool isColliding(Objects::Rectangle *rect1, Objects::Rectangle *rect2);
}

#endif//PYYSICS_Y
#ifdef PHYSICS_IMPLEMENTATION

#include <iostream>
#include <math.h>
#include "./objects.hpp"

namespace Physics {
    
    bool isRectOnFloor(Objects::Rectangle *rect1, Objects::Rectangle *rect2) {
	if (isColliding(rect1, rect2)) {
	    return (rect1->self.position.y <= rect2->self.position.y + rect2->self.scale.y);
	}
	return false;
    }
    
    bool isColliding(Objects::Rectangle *rect1, Objects::Rectangle *rect2) {
	bool is_colliding = true;
	
	is_colliding &= (rect1->self.position.x < rect2->self.position.x + rect2->self.scale.x && rect1->self.position.x + rect1->self.scale.x > rect2->self.position.x) || (rect1->self.position.x + rect1->self.scale.x > rect2->self.position.x && rect1->self.position.x < rect2->self.position.x + rect2->self.scale.x);
	is_colliding &= (rect1->self.position.y < rect2->self.position.y + rect2->self.scale.y && rect1->self.position.y + rect1->self.scale.y > rect2->self.position.y) || (rect1->self.position.y + rect1->self.scale.y > rect2->self.position.y && rect1->self.position.y < rect2->self.position.y + rect2->self.scale.y);
	
	return is_colliding;
    }
    
}

#endif//PYYSICS_Y
