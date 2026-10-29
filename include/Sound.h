#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "miniaudio.h"
#include "Vector3.h"
#ifdef __ANDROID__
#include <android/asset_manager.h>
#endif
class Sound {
private:
	Vector3 position;
	ma_sound sound;
	bool is3d = true;
	#ifdef __ANDROID__
	AAsset* asset;
	#endif
	friend class AudioManager;
public:
	Sound();
	Sound(Vector3 pos);
	~Sound();
	void setPosition(Vector3 pos);
	void play();
	void stop();
	void setVolume(float volume);
	void setPitch(float pitch);
	void setLooped(bool looped);
	bool isPlaying();
	/** @return If sound is not in 3D returns `nullptr` */
	Vector3* getPosition();
	float getVolume();
	float getPitch();
	bool isLooped();
	/** @brief If sound position was set returns `true`, `false` otherwise */
	bool is3D();
};