using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace WpfApp1
{
    class MyViewModel
    {
        public string Name { get; } = "Host D3DWindow";

        public class RedrawCommand : ICommand
        {
            public event EventHandler CanExecuteChanged;

            public bool CanExecute(object parameter)
            {
                return true;
            }

            public void Execute(object parameter)
            {
                WindowHost.GetInstance().Redraw();
            }
        }
        public RedrawCommand MyRedraw { private set; get; } = new RedrawCommand();
    }
}
