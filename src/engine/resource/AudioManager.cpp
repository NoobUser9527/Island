#include "AudioManager.hpp"

#include <SDL3_mixer/SDL_mixer.h>
#include <spdlog/spdlog.h>

namespace engine::resource
{

AudioManager::AudioManager()
{
    if (!MIX_Init())
    {
        throw std::runtime_error("Failed to construct AudioManager: MIX_Init failed");
    }
    spdlog::trace("Initialized SDL_mixer");

    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec); // initialize all fields to 0

    desiredSpec.format = SDL_AUDIO_F32;
    desiredSpec.freq = 44100;
    desiredSpec.channels = 2;

    MIX_Mixer* mixer = MIX_CreateMixer(&desiredSpec);
    if (mixer == nullptr)
    {
        MIX_Quit();
        spdlog::error("Failed to create mixer: {}", SDL_GetError());
        throw std::runtime_error(std::string("Failed to construct AudioManager: MIX_CreateMixer failed. SDL error: ") + SDL_GetError());
    }
    mixer_ = mixer;

    spdlog::trace("AudioManager constructed");
}

AudioManager::~AudioManager()
{
    MIX_StopAllTracks(mixer_, -1);

    clear();
    if (mixer_)
    {
        MIX_DestroyMixer(mixer_);
        mixer_ = nullptr;
    }
    MIX_Quit();
    spdlog::trace("AudioManager destroyed");
}

MIX_Audio* AudioManager::load(std::string_view file_path)
{
    auto it = audio_map_.find(std::string(file_path));
    if (it != audio_map_.end())
    {
        spdlog::trace("Audio already loaded: {}", file_path);
        return it->second.get();
    }

    MIX_Audio* audio = MIX_LoadAudio(mixer_, file_path.data(), true);
    if (audio == nullptr)
    {
        spdlog::error("Failed to load audio: {}. SDL_mixer error: {}", file_path, SDL_GetError());
        return nullptr;
    }

    audio_map_.emplace(file_path, std::unique_ptr<MIX_Audio, SDLAudioDeleter>(audio));
    spdlog::info("Loaded audio: {}", file_path);
    return audio;
}

MIX_Audio* AudioManager::get(std::string_view file_path)
{
    auto it = audio_map_.find(file_path.data());
    if (it != audio_map_.end())
    {
        return it->second.get();
    }
    else
    {
        spdlog::warn("Audio not found: {}. Attempting to load...", file_path);
        return load(file_path);
    }
}

void AudioManager::unload(std::string_view file_path)
{
    auto it = audio_map_.find(std::string(file_path));
    if (it != audio_map_.end())
    {
        audio_map_.erase(it);
        spdlog::info("Unloaded audio: {}", file_path);
    }
    else
    {
        spdlog::warn("Audio not found, cannot unload: {}", file_path);
    }
}

void AudioManager::clear()
{
    audio_map_.clear();
    spdlog::info("All audio resources unloaded");
}

} // namespace engine::resource
