#define WINVER 0x0601
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<windowsx.h>
#include<winuser.h>
#include<commctrl.h>
#define WM_POINTERUPDATE 0x0245
#define WM_POINTERDOWN 0x0246
#define WM_POINTERUP 0x0247
#define WM_TRAY (WM_USER + 1)
#define menu1 0x1111
#define menu2 0x2222
#define menu3 0x3333
#define menu4 0x4444
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
u64 startthread(void*, void*);
void stopthread();
//
void windowwrite();
void eventwrite(u64,u64,u64,u64);
void say(char* fmt,...);




//
struct windata
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;
	u64 thread;
};
//console
static HWND console;		//console window
//tray
static HWND dummy;
static NOTIFYICONDATA nid;	//托盘属性 
static HMENU hMenu;		//托盘菜单
//window
static HWND window;				//my window
static HDC realdc;
static BITMAPINFO info;
static TOUCHINPUT touchpoint[10];
static int pointercount=0;
static int pointerid[10];
//
static char dragpath[MAX_PATH];
static int width=512;
static int height=512;
static int leftdown=0,rightdown=0;
static POINT pt, pe;
static RECT rt, re;








void bitmapinfo(int w, int h)
{
	info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth=w;
	info.bmiHeader.biHeight=-h;
	info.bmiHeader.biPlanes=1;
	info.bmiHeader.biBitCount=32;
	info.bmiHeader.biCompression=0;
	info.bmiHeader.biSizeImage=w*h*4;
	info.bmiHeader.biXPelsPerMeter=0;
	info.bmiHeader.biYPelsPerMeter=0;
	info.bmiHeader.biClrUsed=0;
	info.bmiHeader.biClrImportant=0;
	info.bmiColors[0].rgbBlue=255;
	info.bmiColors[0].rgbGreen=255;
	info.bmiColors[0].rgbRed=255;
	info.bmiColors[0].rgbReserved=255;
}
LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		//拖拽文件
		case WM_DROPFILES:
		{
			HDROP hDrop = (HDROP)wparam;
			UINT nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); // 拖拽文件个数
			int i;
			for (i = 0; i < nFileNum; i++)  
			{
				DragQueryFile(hDrop, i, dragpath, MAX_PATH);//获得拖曳的文件名
			}
			DragFinish(hDrop);      //释放hDrop

			eventwrite(
			(u64)dragpath,
			0x656c6966, 0, 0
			);
			return 0;
		}

		//按键
		case WM_KEYDOWN:
		{
			switch(wparam)
			{
				case VK_UP:				//up
				case VK_LEFT:			//left
				case VK_RIGHT:			//right
				case VK_DOWN:			//down
				case 0x70:				//f1
				case 0x71:				//f2
				case 0x72:				//f3
				case 0x73:				//f4
				{
					eventwrite(wparam, 0x64626b, 0, 0);
					break;
				}
			}
			//say("key:%x\n",wparam);
			return 0;
		}

		//文字
		case WM_CHAR:
		{
			if(wparam==0x1b)eventwrite(0x1b, 0x64626b, 0, 0);
			else eventwrite(wparam, 0x72616863, 0, 0);
			return 0;
		}
/*
		//触摸
		case WM_TOUCH:
		{
			int i=0;
			int count=wparam;
			GetTouchInputInfo(
				(HTOUCHINPUT)lparam,
				count,
				touchpoint,
				sizeof(TOUCHINPUT)
			);

			for(i=0;i<count;i++)
			{
				type[i]=0x4070;

				key[i]=i;
				key[i]=(value<<16)+0;
				key[i]=(value<<16)+touchpoint[i].y;
				key[i]=(value<<16)+touchpoint[i].x;

				say("id=%x:x=%x,y=%x\n",
					touchpoint[i].dwID,
					touchpoint[i].x,
					touchpoint[i].y
				);

			}
			this=0;
			that=count-1;
			return 0;
		}
*/
		case WM_POINTERDOWN:
		{
			int x;
			for(x=0;x<10;x++)
			{
				if( pointerid[x] == -1 )
				{
					pointerid[x]=(u8)(wparam);
					break;
				}
			}
			if(x>=10)return 0;

			pt.y=GET_Y_LPARAM(lparam);
			pt.x=GET_X_LPARAM(lparam);
			ScreenToClient(window, &pt);

			eventwrite(
			pt.x + (pt.y<<16) + ((u64)x<<48),
			0x2b70, 0, 0
			);
			return 0;
		}
		case WM_POINTERUP:
		{
			int x;
			for(x=0;x<10;x++)
			{
				if( pointerid[x] == (u8)(wparam) )
				{
					pointerid[x]=-1;
					break;
				}
			}
			if(x>=10)return 0;

			pt.y=GET_Y_LPARAM(lparam);
			pt.x=GET_X_LPARAM(lparam);
			ScreenToClient(window, &pt);

			eventwrite(
			pt.x + (pt.y<<16) + ((u64)x<<48),
			0x2d70, 0, 0
			);
			return 0;
		}
		case WM_POINTERUPDATE:
		{
			int x;
			for(x=0;x<10;x++)
			{
				if( pointerid[x] == (u8)(wparam) )break;
			}
			if(x>=10)return 0;

			pt.y=GET_Y_LPARAM(lparam);
			pt.x=GET_X_LPARAM(lparam);
			ScreenToClient(window, &pt);

			eventwrite(
			pt.x + (pt.y<<16) + ((u64)x<<48),
			0x4070, 0, 0
			);
			return 0;
		}

		//滚轮
		case WM_MOUSEWHEEL:
		{
			GetCursorPos(&pt);
			ScreenToClient(window, &pt);

			if( ((wparam>>16) & 0xffff ) < 0xf000 )
			{
				eventwrite(
				pt.x + (pt.y<<16) + ((u64)4<<48),
				0x2b6d, 0, 0
				);
			}
			else
			{
				eventwrite(
				pt.x + (pt.y<<16) + ((u64)5<<48),
				0x2b6d, 0, 0
				);
			}

			return 0;
		}

		//鼠标移动
		case WM_MOUSEMOVE:
		{
			//say("moving\n");
			if(leftdown>0)
			{
				//say("moving\n");
				GetCursorPos(&pe);		// 获取光标指针的新位置

				if(rightdown==0xff)		//左右一起按下
				{
					re.left=rt.left+(pe.x - pt.x);		// 窗口新的水平位置
					re.top =rt.top+(pe.y - pt.y);		// 窗口新的垂直位置
					MoveWindow(window,re.left,re.top,re.right,re.bottom,1);// 移动窗口
				}

				else		//只是左键在拖动
				{
					//'xyz ','move'
					eventwrite(
					(pe.x-pt.x) + ((pe.y-pt.y)<<16) + ((u64)1<<48),
					0x406d, 0, 0
					);

					//say("%d,%d\n",pe.x,pe.y);
					GetCursorPos(&pt);	// 获取鼠标当前位置
				}
			}
			return 0;
		}

		//鼠标左键弹起
		case WM_LBUTTONUP:
		{
			if(leftdown==1)eventwrite(lparam + ((u64)1<<48), 0x2d6d, 0, 0);
			leftdown=0;
			return 0;
		}

		//鼠标右键弹起
		case WM_RBUTTONUP:
		{
			if(rightdown==1)
			{
				eventwrite(lparam + ((u64)1<<48), 0x2d6d, 0, 0);
			}

			rightdown=0;
			return 0;
		}

		//鼠标左键按下
		case WM_LBUTTONDOWN:		//鼠标左键点下
		{
			leftdown=1;
			GetCursorPos(&pt);		// 获取鼠标光标指针当前位置

			if(rightdown>0)
			{
				leftdown=rightdown=0xff;
				GetWindowRect(window,&rt);   // 获取窗口位置与大小
				re.right=rt.right-rt.left;               // 保存窗口宽度
				re.bottom=rt.bottom-rt.top; // 保存窗口高度
			}
			return 0;
		}

		//鼠标右键按下
		case WM_RBUTTONDOWN:
		{
			rightdown=1;
			GetCursorPos(&pt);		// 获取鼠标光标指针当前位置

			if(leftdown>0)
			{
				leftdown=rightdown=0xff;
				GetWindowRect(window,&rt);   // 获取窗口位置与大小
				re.right=rt.right-rt.left;               // 保存窗口宽度
				re.bottom=rt.bottom-rt.top; // 保存窗口高度
			}
			return 0;
		}

		//窗口尺寸改变
		case WM_SIZE:
		{
			width=lparam&0xffff;
			height=(lparam>>16)&0xffff;
			printf("wm_size:%d,%d\n",width,height);

			bitmapinfo(width, height);
			eventwrite(lparam, 0x657a6973, 0, 0);
			return 0;
		}

		//显示
		case WM_PAINT:
		{
			say("WM_PAINT\n");
			//windowwrite(p);

			//这里必须调这个函数，不然cpu占用满
			return DefWindowProc(window, msg, wparam, lparam);
		}

		//摧毁
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		//。。。。。。。
		default:
		{
			return DefWindowProc(window, msg, wparam, lparam);
		}
	}
}
int createmywindow()
{
	//Registering Class
	char *AppTitle="haha";
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;				//hInst;
	wc.hIcon = LoadIcon(NULL,IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = AppTitle;
	if(!RegisterClass(&wc))return 0;

	//创建窗口
	window = CreateWindow(
		AppTitle, AppTitle, WS_OVERLAPPEDWINDOW,		//WS_POPUP | WS_MINIMIZEBOX=无边框
		100, 100, width+16, height+39,
		NULL, NULL, 0, NULL);
	if(!window)return 0;

	//拿dc
	realdc=GetDC(window);
	if(!realdc)return 0;

	//透明
	LONG t = GetWindowLong(window, GWL_EXSTYLE);
	SetWindowLong(window, GWL_EXSTYLE, t | WS_EX_LAYERED);
	SetLayeredWindowAttributes(window, 0, 0xf8, LWA_ALPHA);

	//显示窗口
	ShowWindow(window,SW_SHOW);
	UpdateWindow(window);

	//窗口标题
	//SetWindowText(window,"hahahaha");
}
void enabledrag()
{
	typedef BOOL (WINAPI *ChangeWindowMessageFilterProc)(UINT,u32);

	DragAcceptFiles(window, TRUE);

	//1
	HMODULE hUser = LoadLibraryA("user32.dll");
	if (!hUser){say("failed to load\n");exit(-1);}

	//2
	ChangeWindowMessageFilterProc proc;
	proc=(ChangeWindowMessageFilterProc)GetProcAddress(hUser, "ChangeWindowMessageFilter");
	if(!proc){say("can't drag\n");exit(-1);}

	//3
	proc(WM_COPYDATA,1);
	proc(WM_DROPFILES,1);
	proc(0x0049, 1);
}
DWORD WINAPI uievent(struct windata* p)
{
	MSG msg;

	//图形窗口
	createmywindow();

	//打开拖拽
	enabledrag();

	//打开触摸
	RegisterTouchWindow(window, 0);

	//bmp
	bitmapinfo(p->w, p->h);

	//一个一个处理
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//收不到就返回失败消息
	eventwrite(0, 0, 0, 0);
	return 0;
}








LRESULT CALLBACK trayproc(HWND hd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_TRAY:
	{
	switch(lparam) 
	{
		case WM_LBUTTONDOWN:
		{
			if( IsWindowVisible(console) )ShowWindow(console, SW_HIDE);
			else ShowWindow(console, SW_SHOW);
			break;
		}
		case WM_RBUTTONDOWN: 
		{ 
			//获取鼠标坐标 
			POINT pt;
			GetCursorPos(&pt); 
 
			//解决在菜单外单击左键菜单不消失的问题 
			SetForegroundWindow(hd); 

			//显示并获取选中的菜单 
			int cmd=TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, 0, hd,0); 
			if(cmd == menu1)
			{
				if( IsWindowVisible(console) )ShowWindow(console, SW_HIDE);
				else ShowWindow(console, SW_SHOW);
			}
			else if(cmd == menu2)
			{
				if( IsWindowVisible(window) )ShowWindow(window, SW_HIDE);
				else ShowWindow(window, SW_SHOW);
			}
			else if(cmd == menu3)
			{
				//lstrcpy(nid.szInfoTitle, "message");
				lstrcpy(nid.szInfo, TEXT("i am groot"));
				nid.uTimeout = 0;
				Shell_NotifyIcon(NIM_MODIFY, &nid);
			}
			else if(cmd == menu4)
			{
				PostMessage(window, WM_DESTROY, 0, 0);
			}
			break;
		}//case WM_RBUTTONDOWN
	}//switch(lparam) 
	}//case WM_TRAY
	}//switch(msg)

	return DefWindowProc(hd, msg, wparam, lparam);
}
DWORD WINAPI createtray()
{
	MSG msg;
	WNDCLASS wc;
	char* str = "MessageOnly";

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = trayproc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;				//hInst;
	wc.hIcon = LoadIcon(NULL,IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = str;
	if(!RegisterClass(&wc))return 0;

	//the dummy
	dummy = CreateWindow(
		str, str, 0,
		0, 0, 0, 0,
		HWND_MESSAGE, 0, 0, 0
	);

	//the tray
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = dummy;
	nid.uID = 0xabef;		//ID_TRAY_APP_ICON;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_TRAY;
	nid.hIcon = LoadIcon(NULL,IDI_WINLOGO);
	lstrcpy(nid.szTip, "i am groot!");
	Shell_NotifyIcon(NIM_ADD, &nid);
	hMenu = CreatePopupMenu();    //生成托盘菜单 
	AppendMenu(hMenu, MF_STRING, menu1, TEXT("console")); 
	AppendMenu(hMenu, MF_STRING, menu2, TEXT("window")); 
	AppendMenu(hMenu, MF_STRING, menu3, TEXT("about")); 
	AppendMenu(hMenu, MF_STRING, menu4, TEXT("exit"));

	//forever
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void deletetray()
{
	Shell_NotifyIcon(NIM_DELETE, &nid);
}








void windowread()
{
}
void windowwrite(struct windata* p)
{
	//写屏
	SetDIBitsToDevice(
		realdc,
		0, 0,			//目标位置x,y
		p->w, p->h,		//dib宽,高
		0, 0,			//来源起始x,y
		0, p->h,		//起始扫描线,数组中扫描线数量,
		(void*)p->buf,	//rbg颜色数组
		&info,			//bitmapinfo
		DIB_RGB_COLORS	//颜色格式
	);
	//printf("result:%x\n",result);
}
void windowlist()
{
}
void windowchange()
{
	//RECT rc;
	//GetWindowRect(window,&rc);
	//MoveWindow(window , rc.left , rc.top , width+16 , height+40 , 0);
}
void windowstart(struct windata* p)
{
	//构造info
	p->buf = (u64)malloc(2048*1024*4);
	p->w = 512;
	p->h = 512;
/*
	SetWindowPos(
		window, 0, 0, 0,
		width+16, height+39,
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER
	);
*/
	//
	p->thread = startthread(uievent, p);
}
void windowstop()
{
	//ShowWindow(window,SW_SHOW);

	//关闭触摸
	UnregisterTouchWindow(window);

	//释放dc
	ReleaseDC(window,realdc);
}
void windowcreate()
{
	int x;
	for(x=0;x<10;x++)pointerid[x] = -1;

	//console
	console = GetConsoleWindow();
	//ShowWindow(console, SW_SHOW);

	//tray
	startthread(createtray, 0);
}
void windowdelete()
{
	//关闭托盘
	deletetray();
}
