#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace engine::resource
{

class AudioManager final
{
    friend class ResourceManager;

private:
    struct SDLAudioDeleter
    {
        void operator()(MIX_Audio* audio) const
        {
            if (audio)
            {
                MIX_DestroyAudio(audio);
            }
        }
    };

    MIX_Mixer* mixer_ = nullptr;
    std::unordered_map<std::string, std::unique_ptr<MIX_Audio, SDLAudioDeleter>> audio_map_;

public:
    AudioManager();
    ~AudioManager();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;

private:
    MIX_Audio* load(std::string_view file_path);
    MIX_Audio* get(std::string_view file_path);
    void unload(std::string_view file_path);
    void clear();

    // void stop();
};

} // namespace engine::resource