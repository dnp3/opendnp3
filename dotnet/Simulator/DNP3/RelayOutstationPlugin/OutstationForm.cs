using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.DNP3.Interface;
using Automatak.Simulator.DNP3.Commons;

namespace Automatak.Simulator.DNP3.RelayOutstationPlugin
{

    public partial class OutstationForm : Form, ICommandHandler
    {
        readonly IOutstation outstation;
        readonly ProxyCommandHandler commandHandler;
        
        static readonly double[] currentValues = new double[] { 1243, 1237, 1241, 1242 };
        static readonly double[] voltageValues = new double[] { 8031, 8022, 8017, 8034 };

        static readonly Rectangle tripBounds = new Rectangle(467, 174, 32, 32);
        static readonly Rectangle closeBounds = new Rectangle(467, 133, 32, 32);

        readonly IEnumerator<double> currentA = InfiniteEnumerable.Create(currentValues).GetEnumerator();
        readonly IEnumerator<double> currentB = InfiniteEnumerable.Create(currentValues).Skip(1).GetEnumerator();
        readonly IEnumerator<double> currentC = InfiniteEnumerable.Create(currentValues).Skip(2).GetEnumerator();

        readonly IEnumerator<double> voltageA = InfiniteEnumerable.Create(voltageValues).GetEnumerator();
        readonly IEnumerator<double> voltageB = InfiniteEnumerable.Create(voltageValues).Skip(1).GetEnumerator();
        readonly IEnumerator<double> voltageC = InfiniteEnumerable.Create(voltageValues).Skip(2).GetEnumerator();

        // state of the breaker
        bool closed = true;        

        public OutstationForm(IOutstation outstation, ProxyCommandHandler commandHandler)
        {
            InitializeComponent();

            this.outstation = outstation;
            this.commandHandler = commandHandler;

            commandHandler.CommandProxy = this; // proxy commands to the form                     
        }

        private void transparentButtonTrip_Click(object sender, EventArgs e)
        {
            this.closed = false;
            this.UpdateValues();
        }

        private void transparentButtonClose_Click(object sender, EventArgs e)
        {
            this.closed = true;
            this.UpdateValues();
        }

        private void UpdateValues()
        {
            var changes = new ChangeSet();
            var now = DateTime.Now;

            changes.Update(new Binary(closed, 1, now), 0);

            if (closed)
            {                
                changes.Update(new Analog(currentA.Current, 1, now), 0);
                changes.Update(new Analog(currentB.Current, 1, now), 1);
                changes.Update(new Analog(currentC.Current, 1, now), 2);

                this.labelValueIA.Text = String.Format("IA: {0}", currentA.Current);
                this.labelValueIB.Text = String.Format("IB: {0}", currentB.Current);
                this.labelValueIC.Text = String.Format("IC: {0}", currentC.Current);

                currentA.MoveNext();
                currentB.MoveNext();
                currentC.MoveNext();
            }
            else
            {
                changes.Update(new Analog(0, 1, now), 0);
                changes.Update(new Analog(0, 1, now), 1);
                changes.Update(new Analog(0, 1, now), 2);

                this.labelValueIA.Text = String.Format("IA: {0}", 0);
                this.labelValueIB.Text = String.Format("IB: {0}", 0);
                this.labelValueIC.Text = String.Format("IC: {0}", 0);
            }

            changes.Update(new Analog(voltageA.Current, 1, now), 3);
            changes.Update(new Analog(voltageB.Current, 1, now), 4);
            changes.Update(new Analog(voltageC.Current, 1, now), 5);

            this.labelValueVA.Text = String.Format("VA: {0}", voltageA.Current);
            this.labelValueVB.Text = String.Format("VB: {0}", voltageB.Current);
            this.labelValueVC.Text = String.Format("VC: {0}", voltageC.Current);

            voltageA.MoveNext();
            voltageB.MoveNext();
            voltageC.MoveNext();

            outstation.Load(changes);
        }

        private void OutstationForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }

        private void ProcessOperation(bool value)
        { 
            this.closed = value;
            this.UpdateValues();         
        }

        private void QueueOperation(bool value)
        {            
            this.BeginInvoke(new Action(() => this.ProcessOperation(value)));
        }

        CommandStatus OnControl(ControlRelayOutputBlock command, ushort index, bool execute)
        {
            if (index == 0)
            {
                switch (command.code)
                {
                    case (ControlCode.LATCH_ON):
                        if(execute) this.QueueOperation(true);
                        return CommandStatus.SUCCESS;

                    case (ControlCode.LATCH_OFF):
                        if (execute) this.QueueOperation(false);
                        return CommandStatus.SUCCESS;

                    default:
                        return CommandStatus.NOT_SUPPORTED;
                }
            }
            else
            {
                return CommandStatus.NOT_SUPPORTED;
            }
        }

        CommandStatus ICommandHandler.Select(ControlRelayOutputBlock command, ushort index)
        {
            return OnControl(command, index, false);
        }

        CommandStatus ICommandHandler.Select(AnalogOutputInt32 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Select(AnalogOutputInt16 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Select(AnalogOutputFloat32 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Select(AnalogOutputDouble64 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(ControlRelayOutputBlock command, ushort index)
        {
            return OnControl(command, index, true);
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputInt32 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputInt16 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputFloat32 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputDouble64 command, ushort index)
        {
            return CommandStatus.NOT_SUPPORTED;
        }

        private void timerUpdate_Tick(object sender, EventArgs e)
        {
            this.UpdateValues();
        }

        private void pictureBoxRelay_MouseMove(object sender, MouseEventArgs e)
        {
            if (tripBounds.Contains(e.Location))
            {
                this.Cursor = Cursors.Hand;
            }
            else if (closeBounds.Contains(e.Location))
            {
                this.Cursor = Cursors.Hand;
            }
            else
            {
                this.Cursor = Cursors.Default;
            }
        }       

        private void pictureBoxRelay_MouseClick(object sender, MouseEventArgs e)
        {
            if (tripBounds.Contains(e.Location))
            {
                this.closed = false;
                this.UpdateValues();
            }
            else if (closeBounds.Contains(e.Location))
            {
                this.closed = true;
                this.UpdateValues();
            }
        }

        private void OutstationForm_Load(object sender, EventArgs e)
        {
            this.UpdateValues();
        }


        void ICommandHandler.Start()
        {
            
        }

        void ICommandHandler.End()
        {
            
        }
    }
}
