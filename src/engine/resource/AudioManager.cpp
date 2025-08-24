#include "AudioManager.hpp"

#include <SDL3_mixer/SDL_mixer.h>
#include <spdlog/spdlog.h>

namespace engine::resource {

AudioManager::AudioManager() {

    if (!MIX_Init()) {
        throw std::runtime_error("AudioManager 构造失败: 无法初始化 SDL_mixer");
    }
    spdlog::trace("SDL_mixer 初始化成功");

    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec); // 初始化所有字段为0

    desiredSpec.format = SDL_AUDIO_F32;
    desiredSpec.freq = 44100;
    desiredSpec.channels = 2;
    {
        // for (int i = 0; i < 1; ++i) {
        //     spdlog::info("设备[{}]: {}", i, SDL_GetAudioDeviceName(i, 0));
        // }

    }

    // MIX_Mixer *mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desiredSpec);
    MIX_Mixer *mixer = MIX_CreateMixer(&desiredSpec);
    if (mixer == nullptr) {
        MIX_Quit();
        spdlog::error("SDL Error: {}", SDL_GetError());

        throw std::runtime_error("AudioManager 构造失败: MIX_CreateMixerDevice 失败" + std::string(SDL_GetError()));
    }
    mixer_ = mixer;

    spdlog::trace("AudioManager 构造完成");
}

AudioManager::~AudioManager() {
    MIX_StopAllTracks(mixer_, -1);

    clear();
    if (mixer_) {
        MIX_DestroyMixer(mixer_);
        mixer_ = nullptr;
    }
    MIX_Quit();
    spdlog::trace("AudioManager 销毁完成");
}

MIX_Audio *AudioManager::load(const std::string &file_path) {
    if (audio_map_.find(file_path) != audio_map_.end()) {
        spdlog::warn("AudioManager: 音频已加载: {}", file_path);
        return audio_map_[file_path].get();
    }

    MIX_Audio *audio = MIX_LoadAudio(mixer_, file_path.c_str(), true);
    if (audio == nullptr) {
        spdlog::error("AudioManager: 无法加载音频: {}. SDL_mixer错误: {}", file_path, SDL_GetError());
        return nullptr;
    }

    audio_map_.emplace(file_path, std::unique_ptr<MIX_Audio, SDLAudioDeleter>(audio));
    spdlog::info("AudioManager: 音频加载成功: {}", file_path);
    return audio;
}

MIX_Audio *AudioManager::get(const std::string &file_path) {
    auto it = audio_map_.find(file_path);
    if (it != audio_map_.end()) {
        return it->second.get();
    } else {
        spdlog::warn("AudioManager: 音频未找到: {} 尝试加载", file_path);
        return load(file_path);
    }
}


void AudioManager::unload(const std::string &file_path) {
    auto it = audio_map_.find(file_path);
    if (it != audio_map_.end()) {
        audio_map_.erase(it);
        spdlog::info("AudioManager: 音频卸载成功: {}", file_path);
    } else {
        spdlog::warn("AudioManager: 音频未找到, 无法卸载: {}", file_path);
    }
}

void AudioManager::clear() {
    audio_map_.clear();
    spdlog::info("AudioManager: 所有音频已卸载");
}

} //  namespace engine::resouce
