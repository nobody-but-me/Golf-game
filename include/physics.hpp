
#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include "./objects.hpp"

namespace Physics {
    bool isOnFloor(Objects::Object *rect1, Objects::Object *rect2);
    
    bool isHorizontallyAligned(Objects::Object *rect1, Objects::Object *rect2);
    bool isVerticallyAligned(Objects::Object *rect1, Objects::Object *rect2);
    
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
	return false;
    }
    
    bool isHorizontallyAligned(Objects::Object *rect1, Objects::Object *rect2) {
	return (rect1->position.x < rect2->position.x + rect2->scale.x &&
                rect1->position.x + rect1->scale.x > rect2->position.x);
    }
    bool isVerticallyAligned(Objects::Object *rect1, Objects::Object *rect2) {
	return (rect1->position.y < rect2->position.y + rect2->scale.y &&
                rect1->position.y + rect1->scale.y > rect2->position.y);
    }
    
    bool isColliding(Objects::Object *rect1, Objects::Object *rect2) {
	const float MARGIN = 6.0f;
	
	glm::vec2 center1 = {rect1->position.x + rect1->scale.x / 2.0f, rect1->position.y + rect1->scale.y / 2.0f};
	glm::vec2 center2 = {rect2->position.x + rect2->scale.x / 2.0f, rect2->position.y + rect2->scale.y / 2.0f};
	
	glm::vec2 distance = {std::abs(center1.x - center2.x), std::abs(center1.y - center2.y)};
	glm::vec2 max = {(rect1->scale.x + rect2->scale.x) / 2.0f + MARGIN, (rect1->scale.y + rect2->scale.y) / 2.0f + MARGIN};
	if (distance.x > max.x || distance.y > max.y) return false;
	
	return (rect1->position.x < rect2->position.x + rect2->scale.x &&
                rect1->position.x + rect1->scale.x > rect2->position.x &&
		rect1->position.y < rect2->position.y + rect2->scale.y &&
                rect1->position.y + rect1->scale.y > rect2->position.y);
    }
}

#endif//PHYSICS_IMPLEMENTATION
