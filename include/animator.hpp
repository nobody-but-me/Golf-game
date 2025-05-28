
#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <vector>

namespace Animator {
    
    class Animation {
	private:
	    std::vector<int> frames;
	    int *sprite_index;
	    bool loop = false;
	    int delay_length = 0;
	    int delay = 0;
	public:
	    bool is_playing = false;
	    
	    Animation(std::vector<int> *p_frames, bool p_loop, int p_delay_length, int *p_sprite_index);
	    ~Animation();
	    
	    void stop();
	    void play();
    };
}

#endif//ANIMATION_H
#ifdef ANIMATOR_IMPLEMENTATION

#include <iostream>
#include <vector>

namespace Animator {
    
    Animation::Animation(std::vector<int> *p_frames, bool p_loop, int p_delay_length, int *p_sprite_index) {
	delay_length = p_delay_length;
	sprite_index = p_sprite_index;
	frames = *p_frames;
	loop = p_loop;
	return;
    }
    Animation::~Animation() {
	is_playing = false;
	return;
    }
    
    void Animation::stop() {
	is_playing = false;
	return;
    }
    void Animation::play() {
	is_playing = true;
	
	size_t length = frames.size();
	// I am almost sure this is the uglier code I've ever written.
	if (loop == true) {
	    if (is_playing) {
		if (delay < delay_length) {
		    delay++;
		} else {
		    if (*sprite_index < int(length)) {
			*sprite_index += 1;
		    } else {
			*sprite_index = frames[0];
		    }
		    delay = 0;
		}
	    }
	} else {
	    if (delay < delay_length) {
		delay++;
	    } else {
		if (*sprite_index < int(length)) {
		    *sprite_index += 1;
		} else {
		    is_playing = false;
		}
		delay = 0;
	    }
	}
	return;
    }
}

#endif//ANIMATOR_IMPLEMENTATION
