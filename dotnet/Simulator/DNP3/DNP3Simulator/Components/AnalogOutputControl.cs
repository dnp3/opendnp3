using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.Simulator.Commons;
using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class AnalogOutputControl : UserControl
    {
        enum AOType
        {
            Integer16,
            Integer32,
            Float32,
            Double64            
        };

        public AnalogOutputControl()
        {
            InitializeComponent();

            comboBoxType.DataSource = Enum.GetValues(typeof(AOType));
        }

        public Func<ICommandProcessor, Task<CommandResponse>> DirectOperateAction
        {
            get
            {
                
                var index = Convert.ToUInt16(this.numericUpDownIndex.Value);
                var value = numericUpDownValue.Value;
                var type = (AOType) comboBoxType.SelectedValue;
                switch (type)
                { 
                    case(AOType.Integer16):
                        return (ICommandProcessor cp) => cp.DirectOperate(new AnalogOutputInt16(SafeConvert.ToInt16(value)) , index, TaskConfig.Default);
                    case (AOType.Integer32):
                        return (ICommandProcessor cp) => cp.DirectOperate(new AnalogOutputInt32(SafeConvert.ToInt32(value)), index, TaskConfig.Default);
                    case(AOType.Float32):
                        return (ICommandProcessor cp) => cp.DirectOperate(new AnalogOutputFloat32(SafeConvert.ToSingle(value)), index, TaskConfig.Default);
                    default:
                        return (ICommandProcessor cp) => cp.DirectOperate(new AnalogOutputDouble64(SafeConvert.ToDouble(value)), index, TaskConfig.Default);
                }                
            }
        }

        public Func<ICommandProcessor, Task<CommandResponse>> SelectAndOperateAction
        {
            get
            {

                var index = Convert.ToUInt16(this.numericUpDownIndex.Value);
                var value = numericUpDownValue.Value;
                var type = (AOType)comboBoxType.SelectedValue;
                switch (type)
                {
                    case (AOType.Integer16):
                        return (ICommandProcessor cp) => cp.SelectAndOperate(new AnalogOutputInt16(Convert.ToInt16(value)), index, TaskConfig.Default);
                    case (AOType.Integer32):
                        return (ICommandProcessor cp) => cp.SelectAndOperate(new AnalogOutputInt32(Convert.ToInt32(value)), index, TaskConfig.Default);
                    case (AOType.Float32):
                        return (ICommandProcessor cp) => cp.SelectAndOperate(new AnalogOutputFloat32(Convert.ToSingle(value)), index, TaskConfig.Default);
                    default:
                        return (ICommandProcessor cp) => cp.SelectAndOperate(new AnalogOutputDouble64(Convert.ToDouble(value)), index, TaskConfig.Default);
                }
            }
        }
    }
}
