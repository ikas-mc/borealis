
/*
		Copyright 2023 xfangfang
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
#include <ppltasks.h>
#include <strings.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/windows.system.userprofile.h>

#include <borealis/core/application.hpp>
#include <borealis/core/i18n.hpp>
#include <borealis/core/logger.hpp>
#include <borealis/platforms/winrt/winrt_platform.hpp>
#include <unordered_map>

namespace brls
{
	WinRTPlatform::WinRTPlatform()
	{
		// override local
		if (Platform::APP_LOCALE_DEFAULT == LOCALE_AUTO)
		{
			auto lang=winrt::to_string(winrt::Windows::System::UserProfile::GlobalizationPreferences::Languages().GetAt(0));
			brls::Logger::info("Windows GlobalizationPreferences lang: {}", lang);

			if (lang.starts_with("zh-Hans"))
			{
				this->locale=LOCALE_ZH_HANS;
			}
			else if (lang.starts_with("zh-Hant"))
			{
				this->locale=LOCALE_ZH_HANT;
			}
			brls::Logger::info("Set app locale: {}", this->locale);
		}
	}

	void WinRTPlatform::createWindow(std::string windowTitle, uint32_t windowWidth, uint32_t windowHeight, float windowXPos, float windowYPos)
	{
		brls::Logger::info("winrt createWindow: {}", windowTitle);
		coreWindow=winrt::Windows::UI::Core::CoreWindow::GetForCurrentThread();
		displayRequest=winrt::Windows::System::Display::DisplayRequest();
		this->videoContext=new WinRTVideoContext(coreWindow, windowTitle, windowWidth, windowHeight, windowXPos, windowYPos);
		this->inputManager=new WinRTInputManager(coreWindow);
		this->imeManager=new WinRTImeManager(coreWindow);
		this->audioPlayer=new WinRTAudioPlayer();
	}

	void WinRTPlatform::restoreWindow()
	{
	}

	void WinRTPlatform::setWindowAlwaysOnTop(bool enable)
	{
	}

	void WinRTPlatform::setWindowSize(uint32_t windowWidth, uint32_t windowHeight)
	{
	}

	void WinRTPlatform::setWindowSizeLimits(uint32_t windowMinWidth, uint32_t windowMinHeight, uint32_t windowMaxWidth, uint32_t windowMaxHeight)
	{
	}

	void WinRTPlatform::setWindowPosition(int windowXPos, int windowYPos)
	{
	}

	void WinRTPlatform::setWindowState(uint32_t windowWidth, uint32_t windowHeight, int windowXPos, int windowYPos)
	{
	}

	void WinRTPlatform::disableScreenDimming(bool disable, const std::string& reason, const std::string& app)
	{
		if (this->screenDimmingDisabled == disable)
		{
			return;
		}

		try
		{
			displayRequest.RequestActive();
			this->screenDimmingDisabled=disable;
		}
		catch (const std::exception& e)
		{
			Logger::warning("Winrt DisplayRequest release failed {}", e.what());
		}
	}

	bool WinRTPlatform::isScreenDimmingDisabled()
	{
		return this->screenDimmingDisabled;
	}

	void WinRTPlatform::pasteToClipboard(const std::string& text)
	{
		using namespace winrt::Windows::ApplicationModel::DataTransfer;

		auto dataPackage=DataPackage();
		dataPackage.SetText(winrt::to_hstring(text));
		if (!Clipboard::SetContentWithOptions(dataPackage, nullptr))
		{
			brls::Logger::error("Failed to set clipboard content, text={}", text);
		}
	}

	std::string WinRTPlatform::pasteFromClipboard()
	{
		using namespace winrt::Windows::ApplicationModel::DataTransfer;

		DataPackageView dataPackageView=Clipboard::GetContent();
		if (dataPackageView.Contains(StandardDataFormats::Text()))
		{
			winrt::hstring text=concurrency::create_task([dataPackageView] { return dataPackageView.GetTextAsync().get(); })
				.get();
			return winrt::to_string(text);
		}
		return "";
	}

	std::string WinRTPlatform::getName()
	{
		return "WinRT";
	}

	bool WinRTPlatform::processEvent()
	{
		winrt::Windows::UI::Core::CoreDispatcher dispatcher=coreWindow.Dispatcher();
		dispatcher.ProcessEvents(winrt::Windows::UI::Core::CoreProcessEventsOption::ProcessAllIfPresent);

		return true;
	}

	bool WinRTPlatform::mainLoopIteration()
	{
		return processEvent();
	}

	AudioPlayer* WinRTPlatform::getAudioPlayer()
	{
		return this->audioPlayer;
	}

	VideoContext* WinRTPlatform::getVideoContext()
	{
		return this->videoContext;
	}

	InputManager* WinRTPlatform::getInputManager()
	{
		return this->inputManager;
	}

	ImeManager* WinRTPlatform::getImeManager()
	{
		return this->imeManager;
	}

	WinRTPlatform::~WinRTPlatform()
	{
		delete this->audioPlayer;
		delete this->inputManager;
		delete this->imeManager;
		delete this->videoContext;
	}

} // namespace brls
