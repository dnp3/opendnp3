using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Drawing;

namespace Automatak.Simulator.API
{
    public interface ISimulatorPlugin
    {
        Image PluginImage
        {
            get;
        }

        ImageList NodeImageList
        {
            get;
        }

        String RootDisplayName
        {
            get;
        }

        String UniqueId
        {
            get;
        }

        ISimulatorNode Create(ISimulatorNodeCallbacks callbacks);
    }
}
