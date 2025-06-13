
#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include "./objects.hpp"

namespace Physics {
    bool isOnFloor(Objects::Object *rect1, Objects::Object *rect2);
    bool isColliding(Objects::Object *rect1, Objects::Object *rect2);
}

#endif//PHYSICS_H
#ifdef PHYSICS_IMPLEMENTATION

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

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
	glm::vec2 distance = glm::vec2(rect1->position.x - rect2->position.x, rect1->position.y - rect2->position.y);
	// std::cout << "x: " << distance.x << " y: " << distance.y << std::endl;
	if ((distance.x > 6 || distance.x < -6) && (distance.y > 6 || distance.y < -6)) return false;
	
	bool is_colliding = true;
	
	is_colliding &= (rect1->position.x < rect2->position.x + rect2->scale.x && rect1->position.x + rect1->scale.x > rect2->position.x) || (rect1->position.x + rect1->scale.x > rect2->position.x && rect1->position.x < rect2->position.x + rect2->scale.x);
	is_colliding &= (rect1->position.y < rect2->position.y + rect2->scale.y && rect1->position.y + rect1->scale.y > rect2->position.y) || (rect1->position.y + rect1->scale.y > rect2->position.y && rect1->position.y < rect2->position.y + rect2->scale.y);
	
	return is_colliding;
    }
    
}

#endif//PHYSICS_IMPLEMENTATION
