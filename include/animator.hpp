
#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <vector>

namespace Animator {
    
    class Animation {
	private:
	    std::vector<int> frames;
	    int delay_length = 0;
	    int *sprite_index;
	    bool loop = false;
	    int delay = 0;
	    int index = 0;
	public:
	    bool is_playing = false;
	    bool played = false;
	    
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
	played = true;
	return;
    }
    void Animation::play() {
	is_playing = true;
	
	// I am almost sure this is the uglier code I've ever written.
	if (is_playing) {
	    if (loop) {
		if (delay < delay_length) {
		    delay++;
		} else {
		    size_t length = frames.size();
		    if (index < int(length)) {
			*sprite_index = frames[index];
			index++;
		    } else {
			*sprite_index = frames[0];
			index = 0;
		    }
		    delay = 0;
		}
	    } else {
		if (delay < delay_length) {
		    delay++;
		} else {
		    size_t length = frames.size();
		    if (index < int(length)) {
			*sprite_index = frames[index];
			index++;
		    } else {
			is_playing = false;
			played = true;
			index = 0;
		    }
		    delay = 0;
		}
	    }
	}
	// if (loop == true) {
	//     if (is_playing) {
	//     }
	// } else {
	// }
	return;
    }
}

#endif//ANIMATOR_IMPLEMENTATION
