
#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include "./objects.hpp"

namespace Physics {
    bool isOnFloor(Objects::Object *rect1, Objects::Object *rect2);
    bool isColliding(Objects::Object *rect1, Objects::Object *rect2);
}

#endif//PYYSICS_Y
#ifdef PHYSICS_IMPLEMENTATION

#include <iostream>
#include <math.h>
#include "./objects.hpp"

namespace Physics {
    
    bool isOnFloor(Objects::Object *rect1, Objects::Object *rect2) {
	if (isColliding(rect1, rect2)) {
	    return (rect1->position.y <= rect2->position.y + rect2->scale.y);
	}
	return false;
    }
    
    bool isColliding(Objects::Object *rect1, Objects::Object *rect2) {
	bool is_colliding = true;
	
	is_colliding &= (rect1->position.x < rect2->position.x + rect2->scale.x && rect1->position.x + rect1->scale.x > rect2->position.x) || (rect1->position.x + rect1->scale.x > rect2->position.x && rect1->position.x < rect2->position.x + rect2->scale.x);
	is_colliding &= (rect1->position.y < rect2->position.y + rect2->scale.y && rect1->position.y + rect1->scale.y > rect2->position.y) || (rect1->position.y + rect1->scale.y > rect2->position.y && rect1->position.y < rect2->position.y + rect2->scale.y);
	
	return is_colliding;
    }
    
}

#endif//PYYSICS_Y
