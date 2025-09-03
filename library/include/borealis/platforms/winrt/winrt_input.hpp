#pragma once

#include <borealis/core/input.hpp>

#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/windows.devices.input.h>

namespace brls
{

	class WinRTInputManager: public InputManager
	{
	public:
		explicit WinRTInputManager(winrt::Windows::UI::Core::CoreWindow coreWindow);

		~WinRTInputManager();

		short getControllersConnectedCount() override;

		void updateUnifiedControllerState(ControllerState* state) override;

		void updateControllerState(ControllerState* state, int controller);

		bool getKeyboardKeyState(BrlsKeyboardScancode state) override;

		void updateTouchStates(std::vector<RawTouchState>* states) override;

		void updateMouseStates(RawMouseState* state) override;

		void sendRumble(unsigned short controller, unsigned short lowFreqMotor, unsigned short highFreqMotor) override;

		void runloopStart() override;

		void setPointerLock(bool lock) override;

	private:
		winrt::Windows::UI::Core::CoreWindow coreWindow=nullptr;
		Point scrollOffset;
		Point pointerOffset;
		Point pointerOffsetBuffer;
		bool pointerLocked=false;
		void OnPointerPressed(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::PointerEventArgs const& args);
		void OnPointerReleased(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::PointerEventArgs const& args);
		void OnPointerMoved(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::PointerEventArgs const& args);
		void OnKeyDownOrUp(winrt::Windows::UI::Core::KeyEventArgs const& args, bool kenDown);

	};
};
