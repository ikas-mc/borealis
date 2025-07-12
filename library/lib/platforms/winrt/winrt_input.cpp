/*
	Copyright 2021 natinusala
	Copyright 2021 XITRIX
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

#include <borealis/core/application.hpp>
#include <borealis/core/logger.hpp>
#include <borealis/platforms/winrt/winrt_input.hpp>
#include <mutex>
#include <unordered_map>

namespace brls
{

#define BTN_RELEASED 0
#define BTN_PRESSED 1
#define BTN_STICKY 2
#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_MIDDLE 1
#define MOUSE_BUTTON_RIGHT 2

#include <winrt/Windows.System.h>

#include <borealis/core/input.hpp>

	BrlsKeyboardScancode virtualKeyToBrlsKeyboardScancode (winrt::Windows::System::VirtualKey virtualKey)
	{
		switch (virtualKey)
		{
			// 数字键 0-9
		case winrt::Windows::System::VirtualKey::Number0:
			return BRLS_KBD_KEY_0;
		case winrt::Windows::System::VirtualKey::Number1:
			return BRLS_KBD_KEY_1;
		case winrt::Windows::System::VirtualKey::Number2:
			return BRLS_KBD_KEY_2;
		case winrt::Windows::System::VirtualKey::Number3:
			return BRLS_KBD_KEY_3;
		case winrt::Windows::System::VirtualKey::Number4:
			return BRLS_KBD_KEY_4;
		case winrt::Windows::System::VirtualKey::Number5:
			return BRLS_KBD_KEY_5;
		case winrt::Windows::System::VirtualKey::Number6:
			return BRLS_KBD_KEY_6;
		case winrt::Windows::System::VirtualKey::Number7:
			return BRLS_KBD_KEY_7;
		case winrt::Windows::System::VirtualKey::Number8:
			return BRLS_KBD_KEY_8;
		case winrt::Windows::System::VirtualKey::Number9:
			return BRLS_KBD_KEY_9;

			// 字母键 A-Z
		case winrt::Windows::System::VirtualKey::A:
			return BRLS_KBD_KEY_A;
		case winrt::Windows::System::VirtualKey::B:
			return BRLS_KBD_KEY_B;
		case winrt::Windows::System::VirtualKey::C:
			return BRLS_KBD_KEY_C;
		case winrt::Windows::System::VirtualKey::D:
			return BRLS_KBD_KEY_D;
		case winrt::Windows::System::VirtualKey::E:
			return BRLS_KBD_KEY_E;
		case winrt::Windows::System::VirtualKey::F:
			return BRLS_KBD_KEY_F;
		case winrt::Windows::System::VirtualKey::G:
			return BRLS_KBD_KEY_G;
		case winrt::Windows::System::VirtualKey::H:
			return BRLS_KBD_KEY_H;
		case winrt::Windows::System::VirtualKey::I:
			return BRLS_KBD_KEY_I;
		case winrt::Windows::System::VirtualKey::J:
			return BRLS_KBD_KEY_J;
		case winrt::Windows::System::VirtualKey::K:
			return BRLS_KBD_KEY_K;
		case winrt::Windows::System::VirtualKey::L:
			return BRLS_KBD_KEY_L;
		case winrt::Windows::System::VirtualKey::M:
			return BRLS_KBD_KEY_M;
		case winrt::Windows::System::VirtualKey::N:
			return BRLS_KBD_KEY_N;
		case winrt::Windows::System::VirtualKey::O:
			return BRLS_KBD_KEY_O;
		case winrt::Windows::System::VirtualKey::P:
			return BRLS_KBD_KEY_P;
		case winrt::Windows::System::VirtualKey::Q:
			return BRLS_KBD_KEY_Q;
		case winrt::Windows::System::VirtualKey::R:
			return BRLS_KBD_KEY_R;
		case winrt::Windows::System::VirtualKey::S:
			return BRLS_KBD_KEY_S;
		case winrt::Windows::System::VirtualKey::T:
			return BRLS_KBD_KEY_T;
		case winrt::Windows::System::VirtualKey::U:
			return BRLS_KBD_KEY_U;
		case winrt::Windows::System::VirtualKey::V:
			return BRLS_KBD_KEY_V;
		case winrt::Windows::System::VirtualKey::W:
			return BRLS_KBD_KEY_W;
		case winrt::Windows::System::VirtualKey::X:
			return BRLS_KBD_KEY_X;
		case winrt::Windows::System::VirtualKey::Y:
			return BRLS_KBD_KEY_Y;
		case winrt::Windows::System::VirtualKey::Z:
			return BRLS_KBD_KEY_Z;

			// 功能键
		case winrt::Windows::System::VirtualKey::Escape:
			return BRLS_KBD_KEY_ESCAPE;
		case winrt::Windows::System::VirtualKey::Enter:
			return BRLS_KBD_KEY_ENTER;
		case winrt::Windows::System::VirtualKey::Tab:
			return BRLS_KBD_KEY_TAB;
		case winrt::Windows::System::VirtualKey::Back:
			return BRLS_KBD_KEY_BACKSPACE;
		case winrt::Windows::System::VirtualKey::Space:
			return BRLS_KBD_KEY_SPACE;
		case winrt::Windows::System::VirtualKey::Left:
			return BRLS_KBD_KEY_LEFT;
		case winrt::Windows::System::VirtualKey::Right:
			return BRLS_KBD_KEY_RIGHT;
		case winrt::Windows::System::VirtualKey::Up:
			return BRLS_KBD_KEY_UP;
		case winrt::Windows::System::VirtualKey::Down:
			return BRLS_KBD_KEY_DOWN;

			// 功能键 F1-F12
		case winrt::Windows::System::VirtualKey::F1:
			return BRLS_KBD_KEY_F1;
		case winrt::Windows::System::VirtualKey::F2:
			return BRLS_KBD_KEY_F2;
		case winrt::Windows::System::VirtualKey::F3:
			return BRLS_KBD_KEY_F3;
		case winrt::Windows::System::VirtualKey::F4:
			return BRLS_KBD_KEY_F4;
		case winrt::Windows::System::VirtualKey::F5:
			return BRLS_KBD_KEY_F5;
		case winrt::Windows::System::VirtualKey::F6:
			return BRLS_KBD_KEY_F6;
		case winrt::Windows::System::VirtualKey::F7:
			return BRLS_KBD_KEY_F7;
		case winrt::Windows::System::VirtualKey::F8:
			return BRLS_KBD_KEY_F8;
		case winrt::Windows::System::VirtualKey::F9:
			return BRLS_KBD_KEY_F9;
		case winrt::Windows::System::VirtualKey::F10:
			return BRLS_KBD_KEY_F10;
		case winrt::Windows::System::VirtualKey::F11:
			return BRLS_KBD_KEY_F11;
		case winrt::Windows::System::VirtualKey::F12:
			return BRLS_KBD_KEY_F12;

			// 其他键
		case winrt::Windows::System::VirtualKey::Delete:
			return BRLS_KBD_KEY_DELETE;
		case winrt::Windows::System::VirtualKey::Insert:
			return BRLS_KBD_KEY_INSERT;
		case winrt::Windows::System::VirtualKey::Home:
			return BRLS_KBD_KEY_HOME;
		case winrt::Windows::System::VirtualKey::End:
			return BRLS_KBD_KEY_END;
		case winrt::Windows::System::VirtualKey::PageUp:
			return BRLS_KBD_KEY_PAGE_UP;
		case winrt::Windows::System::VirtualKey::PageDown:
			return BRLS_KBD_KEY_PAGE_DOWN;

		default:
			return BRLS_KBD_KEY_UNKNOWN; // 未知键
		}
	}

	static std::unordered_map<BrlsKeyboardScancode, int> keyboardKeys{};

	auto controllers{ winrt::single_threaded_vector<winrt::Windows::Gaming::Input::UINavigationController> () };

	static int mouseButtons[3] = { 0 };

	static inline int getMouseButtonState (int buttonIndex)
	{
		if (mouseButtons[buttonIndex] == BTN_STICKY)
		{
			mouseButtons[buttonIndex] = BTN_RELEASED;
			return BTN_PRESSED;
		} else
		{
			return mouseButtons[buttonIndex];
		}
	}

	static inline int getKeyboardKeys (BrlsKeyboardScancode code)
	{
		if (keyboardKeys.find (code) == keyboardKeys.end ())
			return BTN_RELEASED;
		// Using BTN_STICKY to ensure that no input is lost
		// It's very helpful for devices with frame drops such as PSV
		if (keyboardKeys[code] == BTN_STICKY)
		{
			keyboardKeys[code] = BTN_RELEASED;
			return BTN_PRESSED;
		} else
		{
			return keyboardKeys[code];
		}
	}

	//TODO touch
	struct WinRTTouchPoint {
		bool pressed = false;
		winrt::Windows::Foundation::Point position;
	};

	static std::mutex touchPointsMutex;
	static std::unordered_map<uint32_t, WinRTTouchPoint> activeTouchPoints;

	WinRTInputManager::WinRTInputManager (winrt::Windows::UI::Core::CoreWindow coreWindow) :coreWindow (coreWindow)
	{
		coreWindow.PointerPressed ({ this, &WinRTInputManager::OnPointerPressed });
		coreWindow.PointerReleased ({ this, &WinRTInputManager::PointerReleased });
		coreWindow.PointerMoved ({ this, &WinRTInputManager::OnPointerMoved }); // <-- add this line
		coreWindow.KeyDown ([&](winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::KeyEventArgs const& args) {
			winrt::Windows::System::VirtualKey key = args.VirtualKey ();
			auto v = virtualKeyToBrlsKeyboardScancode (key);
			keyboardKeys[v] = BTN_PRESSED;
			updateKeyboardState (args); });
		coreWindow.KeyUp ([&](winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::KeyEventArgs const& args) {
			winrt::Windows::System::VirtualKey key = args.VirtualKey ();
			auto v = virtualKeyToBrlsKeyboardScancode (key);
			keyboardKeys[v] = BTN_STICKY;
			updateKeyboardState (args); });

		auto list = winrt::Windows::Gaming::Input::UINavigationController::UINavigationControllers ();

		for (const auto& controller : list)
		{
			controllers.Append (controller);
		}

		winrt::Windows::Gaming::Input::UINavigationController::UINavigationControllerAdded (
			[](winrt::Windows::Foundation::IInspectable const&, winrt::Windows::Gaming::Input::UINavigationController const& controller) {
				auto list = winrt::Windows::Gaming::Input::UINavigationController::UINavigationControllers ();
				controllers.Clear ();
				for (const auto& controller : list)
				{
					controllers.Append (controller);
				}
			});

		winrt::Windows::Gaming::Input::UINavigationController::UINavigationControllerRemoved (
			[](winrt::Windows::Foundation::IInspectable const&, winrt::Windows::Gaming::Input::UINavigationController const& controller) {
				auto list = winrt::Windows::Gaming::Input::UINavigationController::UINavigationControllers ();
				controllers.Clear ();
				for (const auto& controller : list)
				{
					controllers.Append (controller);
				}
			});

		coreWindow.PointerWheelChanged ([&](winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::PointerEventArgs const& args) {
			auto point = args.CurrentPoint ();
			auto delta = point.Properties ().MouseWheelDelta ();
			//scrollOffset.x = point.Properties().MouseWheelDelta().
			scrollOffset.y = delta;
			pointerOffset.x = point.Position ().X;
			pointerOffset.y = point.Position ().Y; });


		Application::getRunLoopEvent ()->subscribe ([this]() {
			if (fabs (scrollOffset.y) < 1) scrollOffset.y = 0;
			else scrollOffset.y *= 0.8;
			if (fabs (scrollOffset.x) < 1) scrollOffset.x = 0;
			else scrollOffset.x *= 0.8;

			pointerOffset.x = 0;
			pointerOffset.y = 0; });
	}

	void WinRTInputManager::OnPointerPressed (winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::PointerEventArgs const& args)
	{
		auto point = args.CurrentPoint ();
		if (point.PointerDevice ().PointerDeviceType () == winrt::Windows::Devices::Input::PointerDeviceType::Touch)
		{
			std::lock_guard<std::mutex> lock (touchPointsMutex);
			WinRTTouchPoint tp;
			tp.pressed = true;
			tp.position = point.Position ();
			activeTouchPoints[point.PointerId ()] = tp;
		}

		auto kind = point.Properties ().PointerUpdateKind ();
		if (kind == winrt::Windows::UI::Input::PointerUpdateKind::LeftButtonPressed)
		{
			mouseButtons[MOUSE_BUTTON_LEFT] = BTN_PRESSED;
		} else if (kind == winrt::Windows::UI::Input::PointerUpdateKind::MiddleButtonPressed)
		{
			mouseButtons[MOUSE_BUTTON_MIDDLE] = BTN_PRESSED;
		} else if (kind == winrt::Windows::UI::Input::PointerUpdateKind::RightButtonPressed)
		{
			mouseButtons[MOUSE_BUTTON_RIGHT] = BTN_PRESSED;
		}
	}

	void WinRTInputManager::PointerReleased (winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::PointerEventArgs const& args)
	{
		auto point = args.CurrentPoint ();
		if (point.PointerDevice ().PointerDeviceType () == winrt::Windows::Devices::Input::PointerDeviceType::Touch)
		{
			std::lock_guard<std::mutex> lock (touchPointsMutex);
			activeTouchPoints.erase (point.PointerId ());
		}

		auto kind = point.Properties ().PointerUpdateKind ();
		if (kind == winrt::Windows::UI::Input::PointerUpdateKind::LeftButtonReleased)
		{
			mouseButtons[MOUSE_BUTTON_LEFT] = BTN_STICKY;
		} else if (kind == winrt::Windows::UI::Input::PointerUpdateKind::MiddleButtonReleased)
		{
			mouseButtons[MOUSE_BUTTON_MIDDLE] = BTN_STICKY;
		} else if (kind == winrt::Windows::UI::Input::PointerUpdateKind::RightButtonReleased)
		{
			mouseButtons[MOUSE_BUTTON_RIGHT] = BTN_STICKY;
		}
	}

	void WinRTInputManager::OnPointerMoved (winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::PointerEventArgs const& args)
	{
		auto point = args.CurrentPoint ();
		if (point.PointerDevice ().PointerDeviceType () == winrt::Windows::Devices::Input::PointerDeviceType::Touch)
		{
			std::lock_guard<std::mutex> lock (touchPointsMutex);
			auto id = point.PointerId ();
			auto pos = point.Position ();
			auto it = activeTouchPoints.find (id);
			if (it != activeTouchPoints.end ())
			{
				it->second.position = pos;
			}
		}
	}

	short WinRTInputManager::getControllersConnectedCount ()
	{
		return controllers.Size ();
	}

	void WinRTInputManager::updateUnifiedControllerState (ControllerState* state)
	{
		for (bool& button : state->buttons)
		{
			button = false;
		}

		int controllerIndex = 0;
		for (auto c : controllers)
		{
			ControllerState localState{};
			updateControllerState (&localState, controllerIndex++);

			for (size_t i = 0; i < _BUTTON_MAX; i++)
			{
				state->buttons[i] |= localState.buttons[i];
			}
		}

		state->buttons[BUTTON_X] |= getKeyboardKeys (BRLS_KBD_KEY_X);
		state->buttons[BUTTON_Y] |= getKeyboardKeys (BRLS_KBD_KEY_Y);
		state->buttons[BUTTON_BACK] |= getKeyboardKeys (BRLS_KBD_KEY_F1);
		// state->buttons[BUTTON_GUIDE] |= getKeyboardKeys(BRLS_KBD_KEY_UNKNOWN);
		state->buttons[BUTTON_START] |= getKeyboardKeys (BRLS_KBD_KEY_F2);
		state->buttons[BUTTON_LSB] |= getKeyboardKeys (BRLS_KBD_KEY_Q);
		state->buttons[BUTTON_RSB] |= getKeyboardKeys (BRLS_KBD_KEY_P);
		state->buttons[BUTTON_LB] |= getKeyboardKeys (BRLS_KBD_KEY_L);
		state->buttons[BUTTON_RB] |= getKeyboardKeys (BRLS_KBD_KEY_R);
		state->buttons[BUTTON_X] |= getKeyboardKeys (BRLS_KBD_KEY_R);
		state->buttons[BUTTON_UP] |= getKeyboardKeys (BRLS_KBD_KEY_UP);
		state->buttons[BUTTON_DOWN] |= getKeyboardKeys (BRLS_KBD_KEY_DOWN);
		state->buttons[BUTTON_LEFT] |= getKeyboardKeys (BRLS_KBD_KEY_LEFT);
		state->buttons[BUTTON_RIGHT] |= getKeyboardKeys (BRLS_KBD_KEY_RIGHT);

		if (Application::isSwapInputKeys ())
		{
			state->buttons[BUTTON_B] |= getKeyboardKeys (BRLS_KBD_KEY_KP_ENTER);
			state->buttons[BUTTON_B] |= getKeyboardKeys (BRLS_KBD_KEY_ENTER);
			state->buttons[BUTTON_A] |= getKeyboardKeys (BRLS_KBD_KEY_ESCAPE);
			// state->buttons[BUTTON_A] |= getKeyboardKeys(SDL_SCANCODE_AC_BACK);
		} else
		{
			state->buttons[BUTTON_A] |= getKeyboardKeys (BRLS_KBD_KEY_KP_ENTER);
			state->buttons[BUTTON_A] |= getKeyboardKeys (BRLS_KBD_KEY_ENTER);
			state->buttons[BUTTON_B] |= getKeyboardKeys (BRLS_KBD_KEY_ESCAPE);
			// state->buttons[BUTTON_B] |= getKeyboardKeys(SDL_SCANCODE_AC_BACK);
		}

		state->buttons[BUTTON_NAV_UP] |= state->buttons[BUTTON_UP];
		state->buttons[BUTTON_NAV_RIGHT] |= state->buttons[BUTTON_RIGHT];
		state->buttons[BUTTON_NAV_DOWN] |= state->buttons[BUTTON_DOWN];
		state->buttons[BUTTON_NAV_LEFT] |= state->buttons[BUTTON_LEFT];
	}

	void WinRTInputManager::updateControllerState (ControllerState* state, int controllerIndex)
	{
		auto controller = controllerIndex >= controllers.Size () ? controllers.GetAt (controllers.Size () - 1) : controllers.GetAt (controllerIndex);

		for (bool& button : state->buttons)
		{
			button = false;
		}

		// https://learn.microsoft.com/en-us/windows/uwp/gaming/ui-navigation-controller
		auto reading = controller.GetCurrentReading ();

		using namespace winrt::Windows::Gaming::Input;

		state->buttons[BUTTON_A] = (reading.RequiredButtons & RequiredUINavigationButtons::Accept) == RequiredUINavigationButtons::Accept;
		state->buttons[BUTTON_B] = (reading.RequiredButtons & RequiredUINavigationButtons::Cancel) == RequiredUINavigationButtons::Cancel;
		state->buttons[BUTTON_START] = (reading.RequiredButtons & RequiredUINavigationButtons::Menu) == RequiredUINavigationButtons::Menu;
		state->buttons[BUTTON_BACK] = (reading.RequiredButtons & RequiredUINavigationButtons::View) == RequiredUINavigationButtons::View;

		state->buttons[BUTTON_UP] = (reading.RequiredButtons & RequiredUINavigationButtons::Up) == RequiredUINavigationButtons::Up || (reading.OptionalButtons & OptionalUINavigationButtons::ScrollUp) == OptionalUINavigationButtons::ScrollUp;
		state->buttons[BUTTON_DOWN] = (reading.RequiredButtons & RequiredUINavigationButtons::Down) == RequiredUINavigationButtons::Down || (reading.OptionalButtons & OptionalUINavigationButtons::ScrollDown) == OptionalUINavigationButtons::ScrollDown;
		state->buttons[BUTTON_LEFT] = (reading.RequiredButtons & RequiredUINavigationButtons::Left) == RequiredUINavigationButtons::Left || (reading.OptionalButtons & OptionalUINavigationButtons::ScrollLeft) == OptionalUINavigationButtons::ScrollLeft;
		state->buttons[BUTTON_RIGHT] = (reading.RequiredButtons & RequiredUINavigationButtons::Right) == RequiredUINavigationButtons::Right || (reading.OptionalButtons & OptionalUINavigationButtons::ScrollRight) == OptionalUINavigationButtons::ScrollRight;

		state->buttons[BUTTON_LB] = (reading.OptionalButtons & OptionalUINavigationButtons::PageLeft) == OptionalUINavigationButtons::PageLeft;
		state->buttons[BUTTON_RB] = (reading.OptionalButtons & OptionalUINavigationButtons::PageRight) == OptionalUINavigationButtons::PageRight;

		state->buttons[BUTTON_LT] = (reading.OptionalButtons & OptionalUINavigationButtons::PageUp) == OptionalUINavigationButtons::PageUp;
		state->buttons[BUTTON_RT] = (reading.OptionalButtons & OptionalUINavigationButtons::PageDown) == OptionalUINavigationButtons::PageDown;

		state->buttons[BUTTON_X] = (reading.OptionalButtons & OptionalUINavigationButtons::Context1) == OptionalUINavigationButtons::Context1;
		state->buttons[BUTTON_Y] = (reading.OptionalButtons & OptionalUINavigationButtons::Context2) == OptionalUINavigationButtons::Context2;

		state->buttons[BUTTON_RSB] = (reading.OptionalButtons & OptionalUINavigationButtons::Context4) == OptionalUINavigationButtons::Context4;
		state->buttons[BUTTON_LSB] = (reading.OptionalButtons & OptionalUINavigationButtons::Context3) == OptionalUINavigationButtons::Context3;
	}

	bool WinRTInputManager::getKeyboardKeyState (BrlsKeyboardScancode key)
	{
		return getKeyboardKeys (key);
	}

	void WinRTInputManager::updateTouchStates (std::vector<RawTouchState>* states)
	{
		std::lock_guard<std::mutex> lock (touchPointsMutex);
		for (const auto& kv : activeTouchPoints)
		{
			RawTouchState state;
			state.pressed = kv.second.pressed;
			state.fingerId = static_cast<int>(kv.first);
			state.position.x = kv.second.position.X;
			state.position.y = kv.second.position.Y;
			states->push_back (state);
		}
	}

	void WinRTInputManager::updateMouseStates (RawMouseState* state)
	{
		winrt::Windows::Foundation::Point p = coreWindow.PointerPosition ();
		winrt::Windows::Foundation::Rect windowBounds = coreWindow.Bounds ();

		double scaleFactor = brls::Application::getPlatform ()->getVideoContext ()->getScaleFactor ();
		int x = p.X - windowBounds.X;
		int y = p.Y - windowBounds.Y;

		state->position.x = x * scaleFactor / Application::windowScale;
		state->position.y = y * scaleFactor / Application::windowScale;

		state->offset = pointerOffset;
		state->scroll = scrollOffset;

		state->leftButton = getMouseButtonState (MOUSE_BUTTON_LEFT);
		state->middleButton = getMouseButtonState (MOUSE_BUTTON_MIDDLE);
		state->rightButton = getMouseButtonState (MOUSE_BUTTON_RIGHT);
	}

	void WinRTInputManager::setPointerLock (bool lock)
	{
		pointerLocked = lock;
	}

	void WinRTInputManager::runloopStart ()
	{
		pointerOffset = pointerOffsetBuffer;
		pointerOffsetBuffer.x = 0;
		pointerOffsetBuffer.y = 0;
	}

	void WinRTInputManager::sendRumble (unsigned short controller, unsigned short lowFreqMotor, unsigned short highFreqMotor)
	{}

	void WinRTInputManager::updateMouseMotion ()
	{}

	void WinRTInputManager::updateMouseWheel ()
	{
		// this->getMouseScrollOffsetChanged()->fire(Point(event.x, event.y));
	}

	void WinRTInputManager::updateControllerSensorsUpdate ()
	{}

	void WinRTInputManager::updateKeyboardState (winrt::Windows::UI::Core::KeyEventArgs const& event)
	{
		auto* self = (WinRTInputManager*)Application::getPlatform ()->getInputManager ();
		KeyState state{};
		state.key = virtualKeyToBrlsKeyboardScancode (event.VirtualKey ());
		state.pressed = event.KeyStatus ().WasKeyDown;

		auto ctrlState = coreWindow.GetKeyState (winrt::Windows::System::VirtualKey::Control);
		auto altState = coreWindow.GetKeyState (winrt::Windows::System::VirtualKey::Menu);
		auto shiftlState = coreWindow.GetKeyState (winrt::Windows::System::VirtualKey::Shift);
		auto winState = coreWindow.GetKeyState (winrt::Windows::System::VirtualKey::Application);

		if ((ctrlState & winrt::Windows::UI::Core::CoreVirtualKeyStates::Down) == winrt::Windows::UI::Core::CoreVirtualKeyStates::Down)
			state.mods |= BRLS_KBD_MODIFIER_CTRL;
		if ((altState & winrt::Windows::UI::Core::CoreVirtualKeyStates::Down) == winrt::Windows::UI::Core::CoreVirtualKeyStates::Down)
			state.mods |= BRLS_KBD_MODIFIER_ALT;
		if ((shiftlState & winrt::Windows::UI::Core::CoreVirtualKeyStates::Down) == winrt::Windows::UI::Core::CoreVirtualKeyStates::Down)
			state.mods |= BRLS_KBD_MODIFIER_SHIFT;
		if ((winState & winrt::Windows::UI::Core::CoreVirtualKeyStates::Down) == winrt::Windows::UI::Core::CoreVirtualKeyStates::Down)
			state.mods |= BRLS_KBD_MODIFIER_META;

		self->getKeyboardKeyStateChanged ()->fire (state);
		Application::setActiveEvent (true);
	}

	WinRTInputManager::~WinRTInputManager ()
	{
		controllers.Clear ();
		std::lock_guard<std::mutex> lock (touchPointsMutex);
		activeTouchPoints.clear ();
	}

};