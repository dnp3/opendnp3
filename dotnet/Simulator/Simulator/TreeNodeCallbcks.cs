using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Drawing;
using System.Windows.Forms;

using Automatak.Simulator.API;

namespace Automatak.Simulator
{
    class TreeNodeCallbacks : ISimulatorNodeCallbacks
    {
        readonly Control control;

        public TreeNodeCallbacks(Control control)
        {
            this.control = control;
            this.defaultColor = node.ForeColor;
        }

        public readonly TreeNode node = new TreeNode("temp");
        public readonly Color defaultColor;

        void ISimulatorNodeCallbacks.ChangeState(NodeState state)
        {
            if (control.InvokeRequired)
            {
                ISimulatorNodeCallbacks temp = this;
                control.BeginInvoke(new Action(() => { temp.ChangeState(state); }));
            }
            else
            {
                node.ForeColor = GetForeColor(state);
                node.BackColor = GetBackColor(state);
            }
        }
       
        void ISimulatorNodeCallbacks.ChangeImage(int index)
        {
            if (control.InvokeRequired)
            {
                ISimulatorNodeCallbacks temp = this;
                control.BeginInvoke(new Action(() => { temp.ChangeImage(index); }));
            }
            else
            {
                node.ImageIndex = index;
                node.SelectedImageIndex = index;        
            }            
        }

        Color GetForeColor(NodeState state)
        {
            switch (state)
            {
                case (NodeState.Active):
                    return Color.Green;
                case (NodeState.Failed):
                    return Color.Red;
                case (NodeState.Waiting):
                    return Color.White;
                default:
                    return defaultColor;
            }
        }

        Color GetBackColor(NodeState state)
        {
            switch (state)
            {                
                case (NodeState.Waiting):
                    return Color.Black;
                default:
                    return Color.White;
            }
        }

    }
}
