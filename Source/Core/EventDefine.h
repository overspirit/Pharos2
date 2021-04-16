#pragma once

namespace Pharos
{
#if defined(_MACOS_PLATFORM_)
    enum KEY_CODE
    {
        KEY_A                    = 0x00,
        KEY_S                    = 0x01,
        KEY_D                    = 0x02,
        KEY_F                    = 0x03,
        KEY_H                    = 0x04,
        KEY_G                    = 0x05,
        KEY_Z                    = 0x06,
        KEY_X                    = 0x07,
        KEY_C                    = 0x08,
        KEY_V                    = 0x09,
        KEY_B                    = 0x0B,
        KEY_Q                    = 0x0C,
        KEY_W                    = 0x0D,
        KEY_E                    = 0x0E,
        KEY_R                    = 0x0F,
        KEY_Y                    = 0x10,
        KEY_T                    = 0x11,
        KEY_1                    = 0x12,
        KEY_2                    = 0x13,
        KEY_3                    = 0x14,
        KEY_4                    = 0x15,
        KEY_6                    = 0x16,
        KEY_5                    = 0x17,
        KEY_Equal                = 0x18,
        KEY_9                    = 0x19,
        KEY_7                    = 0x1A,
        KEY_Minus                = 0x1B,
        KEY_8                    = 0x1C,
        KEY_0                    = 0x1D,
        KEY_RightBracket         = 0x1E,
        KEY_O                    = 0x1F,
        KEY_U                    = 0x20,
        KEY_LeftBracket          = 0x21,
        KEY_I                    = 0x22,
        KEY_P                    = 0x23,
        KEY_L                    = 0x25,
        KEY_J                    = 0x26,
        KEY_Quote                = 0x27,
        KEY_K                    = 0x28,
        KEY_Semicolon            = 0x29,
        KEY_Backslash            = 0x2A,
        KEY_Comma                = 0x2B,
        KEY_Slash                = 0x2C,
        KEY_N                    = 0x2D,
        KEY_M                    = 0x2E,
        KEY_Period               = 0x2F,
        KEY_Grave                = 0x32,
        KEY_KeypadDecimal        = 0x41,
        KEY_KeypadMultiply       = 0x43,
        KEY_KeypadPlus           = 0x45,
        KEY_KeypadClear          = 0x47,
        KEY_KeypadDivide         = 0x4B,
        KEY_KeypadEnter          = 0x4C,
        KEY_KeypadMinus          = 0x4E,
        KEY_KeypadEquals         = 0x51,
        KEY_Keypad0              = 0x52,
        KEY_Keypad1              = 0x53,
        KEY_Keypad2              = 0x54,
        KEY_Keypad3              = 0x55,
        KEY_Keypad4              = 0x56,
        KEY_Keypad5              = 0x57,
        KEY_Keypad6              = 0x58,
        KEY_Keypad7              = 0x59,
        KEY_Keypad8              = 0x5B,
        KEY_Keypad9              = 0x5C,
        KEY_Return                    = 0x24,
        KEY_Tab                       = 0x30,
        KEY_Space                     = 0x31,
        KEY_Delete                    = 0x33,
        KEY_Escape                    = 0x35,
        KEY_Command                   = 0x37,
        KEY_Shift                     = 0x38,
        KEY_CapsLock                  = 0x39,
        KEY_Option                    = 0x3A,
        KEY_Control                   = 0x3B,
        KEY_RightCommand              = 0x36,
        KEY_RightShift                = 0x3C,
        KEY_RightOption               = 0x3D,
        KEY_RightControl              = 0x3E,
        KEY_Function                  = 0x3F,
        KEY_F17                       = 0x40,
        KEY_VolumeUp                  = 0x48,
        KEY_VolumeDown                = 0x49,
        KEY_Mute                      = 0x4A,
        KEY_F18                       = 0x4F,
        KEY_F19                       = 0x50,
        KEY_F20                       = 0x5A,
        KEY_F5                        = 0x60,
        KEY_F6                        = 0x61,
        KEY_F7                        = 0x62,
        KEY_F3                        = 0x63,
        KEY_F8                        = 0x64,
        KEY_F9                        = 0x65,
        KEY_F11                       = 0x67,
        KEY_F13                       = 0x69,
        KEY_F16                       = 0x6A,
        KEY_F14                       = 0x6B,
        KEY_F10                       = 0x6D,
        KEY_F12                       = 0x6F,
        KEY_F15                       = 0x71,
        KEY_Help                      = 0x72,
        KEY_Home                      = 0x73,
        KEY_PageUp                    = 0x74,
        KEY_ForwardDelete             = 0x75,
        KEY_F4                        = 0x76,
        KEY_End                       = 0x77,
        KEY_F2                        = 0x78,
        KEY_PageDown                  = 0x79,
        KEY_F1                        = 0x7A,
        KEY_LeftArrow                 = 0x7B,
        KEY_RightArrow                = 0x7C,
        KEY_DownArrow                 = 0x7D,
        KEY_UpArrow                   = 0x7E
    };

#elif defined(_LINUX_PLATFORM_)
    enum KEY_CODE
    {
		KEY_SPACE              = 32,
		KEY_APOSTROPHE         = 39,  /* ' */
		KEY_COMMA              = 44,  /* , */
		KEY_MINUS              = 45,  /* - */
		KEY_PERIOD             = 46,  /* . */
		KEY_SLASH              = 47,  /* / */
		KEY_0                  = 48,
		KEY_1                  = 49,
		KEY_2                  = 50,
		KEY_3                  = 51,
		KEY_4                  = 52,
		KEY_5                  = 53,
		KEY_6                  = 54,
		KEY_7                  = 55,
		KEY_8                  = 56,
		KEY_9                  = 57,
		KEY_SEMICOLON          = 59, /* ; */
		KEY_EQUAL              = 61, /* = */
		KEY_A                  = 65,
		KEY_B                  = 66,
		KEY_C                  = 67,
		KEY_D                  = 68,
		KEY_E                  = 69,
		KEY_F                  = 70,
		KEY_G                  = 71,
		KEY_H                  = 72,
		KEY_I                  = 73,
		KEY_J                  = 74,
		KEY_K                  = 75,
		KEY_L                  = 76,
		KEY_M                  = 77,
		KEY_N                  = 78,
		KEY_O                  = 79,
		KEY_P                  = 80,
		KEY_Q                  = 81,
		KEY_R                  = 82,
		KEY_S                  = 83,
		KEY_T                  = 84,
		KEY_U                  = 85,
		KEY_V                  = 86,
		KEY_W                  = 87,
		KEY_X                  = 88,
		KEY_Y                  = 89,
		KEY_Z                  = 90,
		KEY_LEFT_BRACKET       = 91,  /* [ */
		KEY_BACKSLASH          = 92,  /* \ */
		KEY_RIGHT_BRACKET      = 93,  /* ] */
		KEY_GRAVE_ACCENT       = 96,  /* ` */
		KEY_WORLD_1            = 161, /* non-US #1 */
		KEY_WORLD_2            = 162, /* non-US #2 */

		/* Function keys */
		KEY_ESCAPE             = 256,
		KEY_ENTER              = 257,
		KEY_TAB                = 258,
		KEY_BACKSPACE          = 259,
		KEY_INSERT             = 260,
		KEY_DELETE             = 261,
		KEY_RIGHT              = 262,
		KEY_LEFT               = 263,
		KEY_DOWN               = 264,
		KEY_UP                 = 265,
		KEY_PAGE_UP            = 266,
		KEY_PAGE_DOWN          = 267,
		KEY_HOME               = 268,
		KEY_END                = 269,
		KEY_CAPS_LOCK          = 280,
		KEY_SCROLL_LOCK        = 281,
		KEY_NUM_LOCK           = 282,
		KEY_PRINT_SCREEN       = 283,
		KEY_PAUSE              = 284,
		KEY_F1                 = 290,
		KEY_F2                 = 291,
		KEY_F3                 = 292,
		KEY_F4                 = 293,
		KEY_F5                 = 294,
		KEY_F6                 = 295,
		KEY_F7                 = 296,
		KEY_F8                 = 297,
		KEY_F9                 = 298,
		KEY_F10                = 299,
		KEY_F11                = 300,
		KEY_F12                = 301,
		KEY_F13                = 302,
		KEY_F14                = 303,
		KEY_F15                = 304,
		KEY_F16                = 305,
		KEY_F17                = 306,
		KEY_F18                = 307,
		KEY_F19                = 308,
		KEY_F20                = 309,
		KEY_F21                = 310,
		KEY_F22                = 311,
		KEY_F23                = 312,
		KEY_F24                = 313,
		KEY_F25                = 314,
		KEY_KP_0               = 320,
		KEY_KP_1               = 321,
		KEY_KP_2               = 322,
		KEY_KP_3               = 323,
		KEY_KP_4               = 324,
		KEY_KP_5               = 325,
		KEY_KP_6               = 326,
		KEY_KP_7               = 327,
		KEY_KP_8               = 328,
		KEY_KP_9               = 329,
		KEY_KP_DECIMAL         = 330,
		KEY_KP_DIVIDE          = 331,
		KEY_KP_MULTIPLY        = 332,
		KEY_KP_SUBTRACT        = 333,
		KEY_KP_ADD             = 334,
		KEY_KP_ENTER           = 335,
		KEY_KP_EQUAL           = 336,
		KEY_LEFT_SHIFT         = 340,
		KEY_LEFT_CONTROL       = 341,
		KEY_LEFT_ALT           = 342,
		KEY_LEFT_SUPER         = 343,
		KEY_RIGHT_SHIFT        = 344,
		KEY_RIGHT_CONTROL      = 345,
		KEY_RIGHT_ALT          = 346,
		KEY_RIGHT_SUPER        = 347,
		KEY_MENU               = 348,
	};
	
#else
	enum KEY_CODE
	{
		KEY_NULL = 0x01,
		KEY_ESCAPE = 0x01,
		KEY_1 = 0x02,
		KEY_2 = 0x03,
		KEY_3 = 0x04,
		KEY_4 = 0x05,
		KEY_5 = 0x06,
		KEY_6 = 0x07,
		KEY_7 = 0x08,
		KEY_8 = 0x09,
		KEY_9 = 0x0A,
		KEY_0 = 0x0B,
		KEY_MINUS = 0x0C,   /* - on main keyboard */
		KEY_EQUALS = 0x0D,
		KEY_BACK = 0x0E,   /* backspace */
		KEY_TAB = 0x0F,
		KEY_Q = 0x10,
		KEY_W = 0x11,
		KEY_E = 0x12,
		KEY_R = 0x13,
		KEY_T = 0x14,
		KEY_Y = 0x15,
		KEY_U = 0x16,
		KEY_I = 0x17,
		KEY_O = 0x18,
		KEY_P = 0x19,
		KEY_LBRACKET = 0x1A,
		KEY_RBRACKET = 0x1B,
		KEY_RETURN = 0x1C,   /* Enter on main keyboard */
		KEY_LCONTROL = 0x1D,
		KEY_A = 0x1E,
		KEY_S = 0x1F,
		KEY_D = 0x20,
		KEY_F = 0x21,
		KEY_G = 0x22,
		KEY_H = 0x23,
		KEY_J = 0x24,
		KEY_K = 0x25,
		KEY_L = 0x26,
		KEY_SEMICOLON = 0x27,
		KEY_APOSTROPHE = 0x28,
		KEY_GRAVE = 0x29,   /* accent grave */
		KEY_LSHIFT = 0x2A,
		KEY_BACKSLASH = 0x2B,
		KEY_Z = 0x2C,
		KEY_X = 0x2D,
		KEY_C = 0x2E,
		KEY_V = 0x2F,
		KEY_B = 0x30,
		KEY_N = 0x31,
		KEY_M = 0x32,
		KEY_COMMA = 0x33,
		KEY_PERIOD = 0x34,   /* . on main keyboard */
		KEY_SLASH = 0x35,   /* / on main keyboard */
		KEY_RSHIFT = 0x36,
		KEY_MULTIPLY = 0x37,    /* * on numeric keypad */
		KEY_LMENU = 0x38,    /* left Alt */
		KEY_SPACE = 0x39,
		KEY_CAPITAL = 0x3A,
		KEY_F1 = 0x3B,
		KEY_F2 = 0x3C,
		KEY_F3 = 0x3D,
		KEY_F4 = 0x3E,
		KEY_F5 = 0x3F,
		KEY_F6 = 0x40,
		KEY_F7 = 0x41,
		KEY_F8 = 0x42,
		KEY_F9 = 0x43,
		KEY_F10 = 0x44,
		KEY_NUMLOCK = 0x45,
		KEY_SCROLL = 0x46,    /* Scroll Lock */
		KEY_NUMPAD7 = 0x47,
		KEY_NUMPAD8 = 0x48,
		KEY_NUMPAD9 = 0x49,
		KEY_SUBTRACT = 0x4A,    /* - on numeric keypad */
		KEY_NUMPAD4 = 0x4B,
		KEY_NUMPAD5 = 0x4C,
		KEY_NUMPAD6 = 0x4D,
		KEY_ADD = 0x4E,    /* + on numeric keypad */
		KEY_NUMPAD1 = 0x4F,
		KEY_NUMPAD2 = 0x50,
		KEY_NUMPAD3 = 0x51,
		KEY_NUMPAD0 = 0x52,
		KEY_DECIMAL = 0x53,    /* . on numeric keypad */
		KEY_OEM_102 = 0x56,    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
		KEY_F11 = 0x57,
		KEY_F12 = 0x58,
		KEY_F13 = 0x64,    /*                     (NEC PC98) */
		KEY_F14 = 0x65,    /*                     (NEC PC98) */
		KEY_F15 = 0x66,    /*                     (NEC PC98) */
		KEY_KANA = 0x70,    /* (Japanese keyboard)            */
		KEY_ABNT_C1 = 0x73,    /* /? on Brazilian keyboard */
		KEY_CONVERT = 0x79,    /* (Japanese keyboard)            */
		KEY_NOCONVERT = 0x7B,    /* (Japanese keyboard)            */
		KEY_YEN = 0x7D,    /* (Japanese keyboard)            */
		KEY_ABNT_C2 = 0x7E,    /* Numpad . on Brazilian keyboard */
		KEY_NUMPADEQUALS = 0x8D,    /* = on numeric keypad (NEC PC98) */
		KEY_PREVTRACK = 0x90,    /* Previous Track (KEY_CIRCUMFLEX on Japanese keyboard) */
		KEY_AT = 0x91,    /*                     (NEC PC98) */
		KEY_COLON = 0x92,    /*                     (NEC PC98) */
		KEY_UNDERLINE = 0x93,    /*                     (NEC PC98) */
		KEY_KANJI = 0x94,    /* (Japanese keyboard)            */
		KEY_STOP = 0x95,    /*                     (NEC PC98) */
		KEY_AX = 0x96,    /*                     (Japan AX) */
		KEY_UNLABELED = 0x97,    /*                        (J3100) */
		KEY_NEXTTRACK = 0x99,    /* Next Track */
		KEY_NUMPADENTER = 0x9C,    /* Enter on numeric keypad */
		KEY_RCONTROL = 0x9D,
		KEY_MUTE = 0xA0,    /* Mute */
		KEY_CALCULATOR = 0xA1,    /* Calculator */
		KEY_PLAYPAUSE = 0xA2,    /* Play / Pause */
		KEY_MEDIASTOP = 0xA4,    /* Media Stop */
		KEY_VOLUMEDOWN = 0xAE,    /* Volume - */
		KEY_VOLUMEUP = 0xB0,    /* Volume + */
		KEY_WEBHOME = 0xB2,    /* Web home */
		KEY_NUMPADCOMMA = 0xB3,    /* , on numeric keypad (NEC PC98) */
		KEY_DIVIDE = 0xB5,    /* / on numeric keypad */
		KEY_SYSRQ = 0xB7,
		KEY_RMENU = 0xB8,    /* right Alt */
		KEY_PAUSE = 0xC5,    /* Pause */
		KEY_HOME = 0xC7,    /* Home on arrow keypad */
		KEY_UP = 0xC8,    /* UpArrow on arrow keypad */
		KEY_PRIOR = 0xC9,    /* PgUp on arrow keypad */
		KEY_LEFT = 0xCB,    /* LeftArrow on arrow keypad */
		KEY_RIGHT = 0xCD,    /* RightArrow on arrow keypad */
		KEY_END = 0xCF,    /* End on arrow keypad */
		KEY_DOWN = 0xD0,    /* DownArrow on arrow keypad */
		KEY_NEXT = 0xD1,    /* PgDn on arrow keypad */
		KEY_INSERT = 0xD2,    /* Insert on arrow keypad */
		KEY_DELETE = 0xD3,    /* Delete on arrow keypad */
		KEY_LWIN = 0xDB,    /* Left Windows key */
		KEY_RWIN = 0xDC,    /* Right Windows key */
		KEY_APPS = 0xDD,    /* AppMenu key */
		KEY_POWER = 0xDE,    /* System Power */
		KEY_SLEEP = 0xDF,    /* System Sleep */
		KEY_WAKE = 0xE3,    /* System Wake */
		KEY_WEBSEARCH = 0xE5,    /* Web Search */
		KEY_WEBFAVORITES = 0xE6,    /* Web Favorites */
		KEY_WEBREFRESH = 0xE7,    /* Web Refresh */
		KEY_WEBSTOP = 0xE8,    /* Web Stop */
		KEY_WEBFORWARD = 0xE9,    /* Web Forward */
		KEY_WEBBACK = 0xEA,    /* Web Back */
		KEY_MYCOMPUTER = 0xEB,    /* My Computer */
		KEY_MAIL = 0xEC,    /* Mail */
		KEY_MEDIASELECT = 0xED,    /* Media Select */
		KEY_NUM = 0xEE,
	};
#endif

	enum MOUSE_CODE
	{
		MOUSE_LEFT = 0x00,
		MOUSE_RIGHT = 0x01,
		MOUSE_MID = 0x02,
		MOUSE_BTN4 = 0x03,
		MOUSE_BTN5 = 0x04,
		MOUSE_BTN6 = 0x05,
		MOUSE_BTN7 = 0x06,
		MOUSE_BTN8 = 0x07,
		MOUSE_NUM = 0x08,
	};

	enum CODE_STATE
	{
		STATE_KEEP = 0,
		STATE_DOWN = 1,
		STATE_UP = 2,
	};

	struct MouseEvent
	{
		MOUSE_CODE button;
		CODE_STATE state;
		int32 x;
		int32 y;
		int32 ox;
		int32 oy;
		int32 wheel;
		bool shift;
		bool ctrl;
		bool alt;
	};

	struct KeyEvent
	{
		KEY_CODE key;
		CODE_STATE state;
		bool shift;
		bool ctrl;
		bool alt;
	};

	struct JoyStickEvent
	{

	};
}
