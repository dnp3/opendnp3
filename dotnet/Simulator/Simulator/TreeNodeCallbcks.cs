using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.Simulator.API;

namespace Automatak.Simulator
{
    class TreeNodeCallbcks : ISimulatorNodeCallbacks
    {
        public readonly TreeNode node = new TreeNode("temp");

        void ISimulatorNodeCallbacks.ChangeImage(int index)
        {
            
        }
    }
}
