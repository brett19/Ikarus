#pragma once
#include "Vector3.hpp"
#include "SmartPtr.hpp"
#include "InputConstants.hpp"

namespace Halia
{
	namespace EventType
	{
		enum EventType
		{
			KEYDOWN,		// [KeyEvent] Overlay
			KEYUP,			// [KeyEvent] Overlay
			MOUSEDOWN,		// [PosEvent] Overlay
			MOUSEMOVE,		// [PosEvent] Overlay
			MOUSEUP,		// [PosEvent] Overlay
			CLICK,			// [PosEvent] Overlay, 3DObject
			GETFOCUS,		// [Event] Overlay
			LOSTFOCUS,		// [Event] Overlay
			SELECTED,		// [Event] OverlayComboItem, OverlayListItem
			DESELECTED,		// [Event] OverlayComboItem, OverlayListItem
			VALUECHANGED,	// [Event] OverlayTextBox, OverlayComboBox, OverlayListItem
			GOTDATA,		// [Event] Networking
			CONNECTED,		// [Event] Networking
			DISCONNECTED,	// [Event] Networking
			ERR,			// [ErrorEvent] *

			CUSTOM = 0x100	// [??] *
		};
	};

	struct Event : public SmartPtr
	{
		EventType::EventType mType;
		class EventDispatcher *mTarget;
	};

	struct KeyEvent
	{
		KeyCode::KeyCode mKey;
	};

	struct PosEvent
	{
		Vector3 mPosition;
	};

	struct ErrorEvent
	{
		int mCode;
		std::string mText;
	};
};