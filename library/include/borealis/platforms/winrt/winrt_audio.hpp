/*
	Copyright 2021 natinusala

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

#pragma once

#include <borealis/core/audio.hpp>
#include <Audio.h>

#pragma comment(lib, "xaudio2.lib")

namespace brls
{
	class WinRTAudioPlayer: public AudioPlayer
	{
	public:
		WinRTAudioPlayer();
		~WinRTAudioPlayer();

		bool load(enum Sound sound) override;
		bool play(enum Sound sound, float pitch) override;

	private:
		DirectX::SoundEffect* sounds[_SOUND_MAX]{ nullptr };
		bool soundsState[_SOUND_MAX]{ false };
		std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	};

} // namespace brls