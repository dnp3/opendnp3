using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Drawing;

namespace Automatak.Simulator.UI
{
    public interface IColorScheme
    {
        Color BackColor
        {
            get;
        }

        Color Info
        {
            get;
        }

        Color Warn
        {
            get;
        }

        Color Error
        {
            get;
        }
    }
}
