using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Interop;
using System.Windows.Controls;

namespace WpfApp1
{
    using ClassLibrary2;
    class WindowHost2 : HwndHost
    {
        static WindowHost2 instance = null;
        public static WindowHost2 GetInstance() { return instance; }

        Class2 wrapper = new Class2();

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            var border = Parent as Border;
            var child = wrapper.CreateHostedWindow(hwndParent.Handle, (int)border.Width, (int)border.Height);
            return new HandleRef(this, child);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            if (wrapper == null) return;
            wrapper.DestroyHostedWindow(hwnd.Handle);
        }
    }
}
