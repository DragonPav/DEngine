#pragma once
#include <memory>
#include "Camera.h"
#include "Sound.h"
#include "Vector2.h"
/** @class AudioManager
 * @brief Audio manager based on `miniaudio.h` engine
 * 
 * Used by Sound
 **/
class AudioManager {
private:
	const ma_uint32 LISTENER_INDEX = 0;
	ma_engine engine;
	/** @brief Update sound environment of listener
	 * @param pos Position of listener
	 * @param dir Look direction of listener
	 * @param up Up vector of listener
	 **/
	void update(Vector3 pos, Vector3 dir, Vector3 up);
	friend class Camera;
public:
	AudioManager();
	~AudioManager();
	#ifdef __ANDROID__
		/** @brief Create new `Sound` in 3D mode (with specified position)
		 * @param file AAsset in `AASSET_MODE_BUFFER`
		 * @param pos Position of sound
		 * @param dst Range of sound
		 * @return Pointer to created `Sound`
		**/
		std::unique_ptr<Sound> createSound(AAsset* file, Vector3 pos, Vector2 dst);
		/** @brief Create new `Sound` in 2D mode (without position)
		 * @param file AAsset in `AASSET_MODE_BUFFER`
		 * @return Pointer to created `Sound`
		**/
		std::unique_ptr<Sound> createSound(AAsset* file);
	#else
		/** @brief Create new `Sound` in 3D mode (with specified position)
		 * @param file Sound file
		 * @param pos Position of sound
		 * @param dst Range of sound
		 * @return Pointer to created `Sound`
		**/
		std::unique_ptr<Sound> createSound(const char* file, Vector3 pos, Vector2 dst);
		/** @brief Create new `Sound` in 2D mode (without position)
		 * @param file Sound file
		 * @return Pointer to created `Sound`
		**/
		std::unique_ptr<Sound> createSound(const char* file);
	#endif
};