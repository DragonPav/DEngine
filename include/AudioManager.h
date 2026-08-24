#pragma once
#include "Camera.h"
#include "Sound.h"
#include "Vector2.h"
class AudioManager {
private:
	const ma_uint32 LISTENER_INDEX = 0;
	ma_engine engine;
	void update(Vector3 pos, Vector3 dir, Vector3 up);
	friend class Camera;
public:
	AudioManager();
	~AudioManager();
	Sound* createSound(const char* file, Vector3 pos, Vector2 dst);
	Sound* createSound(const char* file);
};