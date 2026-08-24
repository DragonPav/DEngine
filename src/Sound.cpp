#include "Sound.h"
Sound::Sound() {
	is3d = false;
}
Sound::Sound(Vector3 pos) {
	position = pos;
}
Sound::~Sound() {
	ma_sound_uninit(&sound);
}
void Sound::setPosition(Vector3 pos) {
	if (is3d) position = pos;
}
void Sound::play() {
	ma_sound_start(&sound);
}
void Sound::stop() {
	ma_sound_stop(&sound);
}
void Sound::setVolume(float volume) {
	ma_sound_set_volume(&sound, volume);
}
void Sound::setPitch(float pitch) {
	ma_sound_set_pitch(&sound, pitch);
}
void Sound::setLooped(bool looped) {
	ma_sound_set_looping(&sound, looped ? MA_TRUE : MA_FALSE);
}
bool Sound::isPlaying() {
	switch (ma_sound_is_playing(&sound)) {
		case MA_TRUE:
			return true;
		default:
			return false;
	}
}
Vector3* Sound::getPosition() {
	if (is3d) {
		return &position;
	} else {
		return nullptr;
	}
}
float Sound::getVolume() {
	return ma_sound_get_volume(&sound);
}
float Sound::getPitch() {
	return ma_sound_get_pitch(&sound);
}
bool Sound::isLooped() {
	switch (ma_sound_is_looping(&sound)) {
		case MA_TRUE:
			return true;
		default:
			return false;
	}
}
bool Sound::is3D() {
	return is3d;
}