#include "AudioManager.h"
AudioManager::AudioManager() {
	ma_result result = ma_engine_init(nullptr, &engine);
	if (result != MA_SUCCESS) {
		std::cerr << "Failed to initialize miniaudio engine: " << result << std::endl;
	} else {
		std::cout << "miniaudio engine initialized" << std::endl;
	}
}
AudioManager::~AudioManager() {
	ma_engine_uninit(&engine);
}
void AudioManager::update(Vector3 pos, Vector3 dir, Vector3 up) {
	ma_engine_listener_set_position(&engine, LISTENER_INDEX, pos.getX(), pos.getY(), pos.getZ());
	ma_engine_listener_set_direction(&engine, LISTENER_INDEX, dir.getX(), dir.getY(), dir.getZ());
	ma_engine_listener_set_world_up(&engine, LISTENER_INDEX, up.getX(), up.getY(), up.getZ());
}
Sound* AudioManager::createSound(const char* file, Vector3 pos, Vector2 dst) {
	Sound* ret = new Sound(pos);
	ma_result result = ma_sound_init_from_file(&engine, file, MA_SOUND_FLAG_DECODE, nullptr, nullptr, &ret->sound);
	if (result != MA_SUCCESS) {
		std::cerr << "Failed to create sound: " << result << std::endl;
		return ret;
	}
	ma_sound_set_position(&ret->sound, pos.getX(), pos.getY(), pos.getZ());
	ma_sound_set_spatialization_enabled(&ret->sound, MA_TRUE);
	ma_sound_set_attenuation_model(&ret->sound, ma_attenuation_model_inverse);
	ma_sound_set_min_distance(&ret->sound, dst.getX());
	ma_sound_set_max_distance(&ret->sound, dst.getY());
	return ret;
}
Sound* AudioManager::createSound(const char* file) {
	Sound* ret = new Sound();
	ma_result result = ma_sound_init_from_file(&engine, file, MA_SOUND_FLAG_DECODE, nullptr, nullptr, &ret->sound);
	if (result != MA_SUCCESS) {
		std::cerr << "Failed to create sound: " << result << std::endl;
		return ret;
	}
	ma_sound_set_spatialization_enabled(&ret->sound, MA_FALSE);
	return ret;
}