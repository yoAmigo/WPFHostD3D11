#pragma once

#include "resource.h"

using namespace System;

namespace ClassLibrary2 {

	public ref class Class2
	{
		// TODO: このクラスの、ユーザーのメソッドをここに追加してください。
	public:
		IntPtr CreateHostedWindow(IntPtr parentHwnd, int width, int height);
		void DestroyHostedWindow(IntPtr hWnd);

		void Effect();
	};
}