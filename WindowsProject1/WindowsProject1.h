#pragma once

#include "resource.h"

using namespace System;

namespace ClassLibrary1 {

	public ref class Class1
	{
		// TODO: このクラスの、ユーザーのメソッドをここに追加してください。
	public:
		IntPtr CreateHostedWindow(IntPtr parentHwnd, int width, int height);
		void DestroyHostedWindow(IntPtr hWnd);

		void Effect();
	};
}
