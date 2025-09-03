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

#include <borealis/core/video.hpp>
#include <windows.ui.core.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.Graphics.Display.h>
namespace brls
{
	class WinRTVideoContext: public VideoContext
	{
	public:
		WinRTVideoContext(winrt::Windows::UI::Core::CoreWindow coreWindow, std::string windowTitle, uint32_t windowWidth, uint32_t windowHeight, float windowXPos, float windowYPos);
		~WinRTVideoContext() override;

		void clear(NVGcolor color) override;
		void beginFrame() override;
		void endFrame() override;
		void setSwapInterval(int interval) override;
		void resetState() override;
		void fullScreen(bool fs) override;
		double getScaleFactor() override;
		NVGcontext* getNVGContext() override;

		ABI::Windows::UI::Core::ICoreWindow* getWindow();

	private:
		ABI::Windows::UI::Core::ICoreWindow* window=nullptr;
		NVGcontext* nvgContext=nullptr;
	};

} // namespace brls
