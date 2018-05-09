#pragma once

#define sUIFactroy					(GetUIFactoryInst())
#define DECLARE_UI_ROOT_CLASS()		DECLARE_ROOT_CLASS(UIObject)
#define DECLARE_UI_CLASS(type)		DECLARE_CLASS(type, UIObject)
#define IMPLEMENT_UI_ROOT_CLASS()	IMPLEMENT_ROOT_CLASS(UIObject, sUIFactroy)
#define IMPLEMENT_UI_CLASS(type)	IMPLEMENT_CLASS(type, UIObject, sUIFactroy)

namespace Pharos
{
	namespace Desktop
	{
		class UIObject;

		enum InputType
		{
			EIT_MOUSE_MOVE,
			EIT_LEFT_DOWN,
			EIT_LEFT_UP,
		};

		struct tagInputMsg
		{
			InputType type;
			int32 p1;
			int32 p2;
			int32 p3;
			int32 p4;
		};

		enum AnchorType
		{
			EAT_TopLeft,
			EAT_Top,
			EAT_TopRight,
			EAT_Left,
			EAT_Center,
			EAT_Right,
			EAT_BottomLeft,
			EAT_Bottom,
			EAT_BottomRight,
			EAT_End,
		};

		struct tagAnchor
		{
			tagAnchor(){ point = EAT_End; relativePoint = EAT_End; }
			AnchorType					point;				//参考点
			std::weak_ptr<UIObject>		relativeTo;			//参考控件，默认为父控件
			AnchorType					relativePoint;		//参考控件的参考点，默认为跟参考点一致
			Size2Di						offset;				//偏移
		};

		inline AnchorType GetAnchorType(const char8* type)
		{
			uint32 dwValue = Utils::xor32(type);

			switch (dwValue)
			{
			case 0x4C040931: return EAT_TopLeft;
			case 0x00706F54: return EAT_Top;
			case 0x2618083D: return EAT_TopRight;
			case 0x7466654C: return EAT_Left;
			case 0x746E1726: return EAT_Center;
			case 0x68676926: return EAT_Right;
			case 0x1138764B: return EAT_BottomLeft;
			case 0x7474022D: return EAT_Bottom;
			case 0x1D526A4A: return EAT_BottomRight;
			}

			return EAT_End;
		}
// 
// 		inline const char8* GetAnchorTypeString(AnchorType type)
// 		{
// 			switch (type)
// 			{
// 			case EAT_TopLeft: return ("TopLeft");
// 			case EAT_Top: return ("Top");
// 			case EAT_TopRight: return ("TopRight");
// 			case EAT_Left: return ("Left");
// 			case EAT_Center: return ("Center");
// 			case EAT_Right: return ("Right");
// 			case EAT_BottomLeft: return ("BottomLeft");
// 			case EAT_Bottom: return ("Bottom");
// 			case EAT_BottomRight: return ("BottomRight");
// 			}
// 			return ("");
// 		}

		enum TextAlignType
		{
			ETA_LeftTop,
			ETA_CenterTop,
			ETA_RightTop,
			ETA_CenterLeft,
			ETA_Center,
			ETA_CenterRight,
			ETA_LeftBottom,
			ETA_CenterBottom,
			ETA_RightBottom,
			ETA_End,
		};
// 
// 		inline TextAlignType GetTextAlignType(const char8* szName)
// 		{
// 			uint32 dwValue = Utils::xor32(szName);
// 
// 			switch (dwValue)
// 			{
// 			case 0x74160A18: return ETA_LeftTop;
// 			case 0x1B3A1756: return ETA_CenterTop;
// 			case 0x18083D26: return ETA_RightTop;
// 			case 0x11226340: return ETA_CenterLeft;
// 			case 0x746E1726: return ETA_Center;
// 			case 0x1D487F41: return ETA_CenterRight;
// 			case 0x00126761: return ETA_LeftBottom;
// 			case 0x76436352: return ETA_CenterBottom;
// 			case 0x1C654452: return ETA_RightBottom;
// 			}
// 
// 			return ETA_End;
// 		}

		enum ButtonState	// 按钮状态
		{
			EBS_PopUp,			// 正常弹起状态
			EBS_PushDown,		// 按下状态
			EBS_Highlight,		// 高亮状态，一般表示鼠标正指向
			EBS_Disabled
		};

		enum PutType
		{
			EPT_Horizontal,
			EPT_Vertical,
			EPT_End,
		};

		inline PutType GetPutType(const char8* type)
		{
			uint32 dwValue = Utils::xor32(type);

			switch (dwValue)
			{
			case 0x1D1C6C53: return EPT_Horizontal;
			case 0x1813063F: return EPT_Vertical;
			}

			return EPT_End;
		}

		//UI系统内部，控件之间互相传递的事件类型
		// enum UIEventType
		// {
		// 	EUIET_Active,		//控件激活
		// 	EUIET_Deactive,		//控件反激活
		// 	EUIET_Enable,		//控件启用
		// 	EUIET_Disable,		//禁用该控件
		// 	EUIET_Show,			//控件显示
		// 	EUIET_Hide,			//控件隐藏
		// 	EUIET_SizeChange,	//控件大小改变
		// 	EUIET_FrameMutex,	//控件互斥事件，
		// 	EUIET_Drop,			//控件拖动释放
		// 	EUIET_End,
		// };
		// 
		// //UI系统内部，控件之间互相传递的事件
		// struct tagUIEvent
		// {
		// 	tagUIEvent()
		// 	{type = EUIET_End;	pFrame = nullptr;	pObj = nullptr;}
		// 	UIEventType type;		//事件类型
		// 	FramePtr pFrame;		//发送该事件的框架UI
		// 	FramePtr pObj;			//发送该事件的UI
		// };
	}
}