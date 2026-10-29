#include "AudioManager.h"
#include "Logger.h"
AudioManager::AudioManager() {
	ma_result result = ma_engine_init(nullptr, &engine);
	if (result != MA_SUCCESS) {
		Logger::print(Logger::Error, "[AudioManager]: Failed to initialize miniaudio engine: " + std::to_string(result));
	} else {
		Logger::print(Logger::Info, "[AudioManager]: miniaudio engine initialized");
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
#ifdef __ANDROID__
	std::unique_ptr<Sound> AudioManager::createSound(AAsset* file, Vector3 pos, Vector2 dst) {
		if (file == nullptr) {
			Logger::print(Logger::Error, "[AudioManager]: Audio file is null");
		}
		size_t size = AAsset_getLength(file);
		const void* buf = AAsset_getBuffer(file);
		std::unique_ptr<Sound> ret = std::make_unique<Sound>(pos);
		ret->asset = file;
		ma_decoder* decoder = new ma_decoder;
		ma_result result = ma_decoder_init_memory(buf, size, nullptr, decoder);
		if (result != MA_SUCCESS) {
			delete decoder;
			Logger::print(Logger::Error, "[AudioManager]: Failed to init decoder: " + std::to_string(result));
			return nullptr;
		} else {
			result = ma_sound_init_from_data_source(&engine, decoder, MA_SOUND_FLAG_DECODE, nullptr, &ret->sound);
			if (result != MA_SUCCESS) {
				Logger::print(Logger::Error, "[AudioManager]: Failed to create sound: " + std::to_string(result));
				return nullptr;
			}
		}
		ma_sound_set_position(&ret->sound, pos.getX(), pos.getY(), pos.getZ());
		ma_sound_set_spatialization_enabled(&ret->sound, MA_TRUE);
		ma_sound_set_attenuation_model(&ret->sound, ma_attenuation_model_inverse);
		ma_sound_set_min_distance(&ret->sound, dst.getX());
		ma_sound_set_max_distance(&ret->sound, dst.getY());
		return ret;
	}
	std::unique_ptr<Sound> AudioManager::createSound(AAsset* file) {
		if (file == nullptr) {
			Logger::print(Logger::Error, "[AudioManager]: Audio file is null");
		}
		size_t size = AAsset_getLength(file);
		const void* buf = AAsset_getBuffer(file);
		std::unique_ptr<Sound> ret = std::make_unique<Sound>();
		ret->asset = file;
		ma_decoder* decoder = new ma_decoder;
		ma_result result = ma_decoder_init_memory(buf, size, nullptr, decoder);
		if (result != MA_SUCCESS) {
			delete decoder;
			Logger::print(Logger::Error, "[AudioManager]: Failed to init decoder: " + std::to_string(result));
			return nullptr;
		} else {
			result = ma_sound_init_from_data_source(&engine, decoder, MA_SOUND_FLAG_DECODE, nullptr, &ret->sound);
			if (result != MA_SUCCESS) {
				Logger::print(Logger::Error, "[AudioManager]: Failed to create sound: " + std::to_string(result));
				return nullptr;
			}
		}
		ma_sound_set_spatialization_enabled(&ret->sound, MA_FALSE);
		return ret;
	}
#else
	std::unique_ptr<Sound> AudioManager::createSound(const char* file, Vector3 pos, Vector2 dst) {
		std::unique_ptr<Sound> ret = std::make_unique<Sound>(pos);
		ma_result result = ma_sound_init_from_file(&engine, file, MA_SOUND_FLAG_DECODE, nullptr, nullptr, &ret->sound);
		if (result != MA_SUCCESS) {
			Logger::print(Logger::Error, "[AudioManager]: Failed to create sound: " + std::to_string(result));
			return ret;
		}
		ma_sound_set_position(&ret->sound, pos.getX(), pos.getY(), pos.getZ());
		ma_sound_set_spatialization_enabled(&ret->sound, MA_TRUE);
		ma_sound_set_attenuation_model(&ret->sound, ma_attenuation_model_inverse);
		ma_sound_set_min_distance(&ret->sound, dst.getX());
		ma_sound_set_max_distance(&ret->sound, dst.getY());
		return ret;
	}
	std::unique_ptr<Sound> AudioManager::createSound(const char* file) {
		std::unique_ptr<Sound> ret = std::make_unique<Sound>();
		ma_result result = ma_sound_init_from_file(&engine, file, MA_SOUND_FLAG_DECODE, nullptr, nullptr, &ret->sound);
		if (result != MA_SUCCESS) {
			Logger::print(Logger::Error, "[AudioManager]: Failed to create sound: " + std::to_string(result));
			return ret;
		}
		ma_sound_set_spatialization_enabled(&ret->sound, MA_FALSE);
		return ret;
	}
#endif