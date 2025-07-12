/*
    Copyright 2021 natinusala
    Copyright 2020-2021 p-sam
    Copyright 2025 ikas-mc

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <borealis/core/logger.hpp>
#include <borealis/platforms/winrt/winrt_audio.hpp>

namespace brls
{

#define QLAUNCH_PID 0x0100000000001000
#define QLAUNCH_MOUNT_POINT "qlaunch"
#define ROMFS_MOUNT_POINT "romfs"
#define BFSAR_PATH "/sound/qlaunch.bfsar"

const std::wstring SOUNDS_MAP[_SOUND_MAX] = {
    L"", // SOUND_NONE
    L"resources\\sound\\winrt\\SOUND_FOCUS_CHANGE.wav", // SOUND_FOCUS_CHANGE
    L"resources\\sound\\winrt\\SOUND_FOCUS_ERROR.wav", // SOUND_FOCUS_ERROR
    L"resources\\sound\\winrt\\SOUND_CLICK.wav", // SOUND_CLICK
    L"resources\\sound\\winrt\\SOUND_BACK.wav", // SOUND_BACK
    L"resources\\sound\\winrt\\SOUND_FOCUS_SIDEBAR.wav", // SOUND_FOCUS_SIDEBAR
    L"resources\\sound\\winrt\\SOUND_CLICK_ERROR.wav", // SOUND_CLICK_ERROR
    L"resources\\sound\\winrt\\SOUND_HONK.wav", // SOUND_HONK
    L"resources\\sound\\winrt\\SOUND_CLICK_SIDEBAR.wav", // SOUND_CLICK_SIDEBAR
    L"resources\\sound\\winrt\\SOUND_TOUCH_UNFOCUS.wav", // SOUND_TOUCH_UNFOCUS
    L"resources\\sound\\winrt\\SOUND_TOUCH.wav", // SOUND_TOUCH
    L"resources\\sound\\winrt\\SOUND_SLIDER_TICK.wav", // SOUND_SLIDER_TICK
    L"resources\\sound\\winrt\\SOUND_SLIDER_RELEASE.wav" // SOUND_SLIDER_RELEASE
};

WinRTAudioPlayer::WinRTAudioPlayer()
{

    // Create DirectXTK for Audio objects
    DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
    eflags |= DirectX::AudioEngine_Debug;
#endif
    m_audEngine = std::make_unique<DirectX::AudioEngine>(eflags);
    // m_soundEffect = std::make_unique<DirectX::SoundEffect>(m_audEngine.get(), L"resources\\sound\\winrt\\1.wav");
}

bool WinRTAudioPlayer::load(enum Sound sound)
{
    if (sound == SOUND_NONE)
    {
        return true;
    }

    bool loaded= soundsState[sound];

    if (!loaded)
    {
        soundsState[sound] = true;

        std::wstring soundName = SOUNDS_MAP[sound];

        if (soundName.empty())
        {
            return false;
        }

        try
        {
            sounds[sound] = new DirectX::SoundEffect(m_audEngine.get(), soundName.c_str());
        }
        catch (std::exception)
        {
        }
    }

    return sounds[sound] != nullptr;
}

bool WinRTAudioPlayer::play(enum Sound sound, float pitch)
{
    if (sound == SOUND_NONE){
        return true;
    }

    if (!soundsState[sound])
    {
        if (!load(sound))
        {
            return false;
        }
    }

    DirectX::SoundEffect* effcet = sounds[sound];
    if (effcet)
    {
        effcet->Play();
    }

    return true;
}

WinRTAudioPlayer::~WinRTAudioPlayer()
{
    for (int i = 0; i++; i < _SOUND_MAX)
    {
        if (sounds[i])
        {
            delete sounds[i];
            sounds[i] = nullptr;
        }
    }
}

} // namespace brls
