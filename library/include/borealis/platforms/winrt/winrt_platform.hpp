#pragma once
class winrt_platform
{
};


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

#include <borealis/platforms/desktop/desktop_platform.hpp>
#include <borealis/platforms/winrt/winrt_video.hpp>
#include <borealis/platforms/winrt/winrt_ime.hpp>
#include <borealis/platforms/winrt/winrt_input.hpp>
#include <borealis/platforms/winrt/winrt_audio.hpp>
#include <winrt/windows.system.display.h>

namespace brls
{

	class WinRTPlatform: public DesktopPlatform
	{
	public:
		WinRTPlatform();
		~WinRTPlatform() override;

		std::string getName() override;
		void createWindow(std::string windowTitle, uint32_t windowWidth, uint32_t windowHeight, float windowXPos, float windowYPos) override;

		void restoreWindow() override;
		void setWindowSize(uint32_t windowWidth, uint32_t windowHeight) override;
		void setWindowSizeLimits(uint32_t windowMinWidth, uint32_t windowMinHeight, uint32_t windowMaxWidth, uint32_t windowMaxHeight) override;
		void setWindowPosition(int windowXPos, int windowYPos) override;
		void setWindowState(uint32_t windowWidth, uint32_t windowHeight, int windowXPos, int windowYPos) override;
		void disableScreenDimming(bool disable, const std::string& reason, const std::string& app) override;
		bool isScreenDimmingDisabled() override;

		void setWindowAlwaysOnTop(bool enable) override;
		void pasteToClipboard(const std::string& text) override;
		std::string pasteFromClipboard() override;
		bool mainLoopIteration() override;

		AudioPlayer* getAudioPlayer() override;
		VideoContext* getVideoContext() override;
		InputManager* getInputManager() override;
		ImeManager* getImeManager() override;
		bool processEvent();
	protected:
		winrt::Windows::System::Display::DisplayRequest displayRequest=nullptr;
		winrt::Windows::UI::Core::CoreWindow coreWindow=nullptr;
		WinRTVideoContext* videoContext=nullptr;
		WinRTAudioPlayer* audioPlayer=nullptr;
		WinRTInputManager* inputManager=nullptr;
		WinRTImeManager* imeManager=nullptr;
	};

} // namespace brls
