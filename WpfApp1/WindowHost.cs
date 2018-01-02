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
    using ClassLibrary1;
    class WindowHost : HwndHost
    {
        static WindowHost instance;
        public static WindowHost GetInstance() { return instance; }

        Class1 wrapper = new Class1();

        public  WindowHost()
        {
            wrapper = new Class1();
            if (instance == null) instance = this;
        }

        ~WindowHost() { instance = null; }

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

        public void Effect()
        {
            if (wrapper == null) return;
            wrapper.Effect();
        }
    }
}
