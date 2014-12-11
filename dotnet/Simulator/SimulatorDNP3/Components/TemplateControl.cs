using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class TemplateControl : UserControl
    {
        public TemplateControl()
        {
            InitializeComponent();

            this.CheckState();
        }
        
        public void SetRecords(IEnumerable<EventRecord> records)
        {
            
                this.SuspendLayout();                
                this.listViewMeas.Items.Clear();
                UInt16 index = 0;
                foreach (var row in records)
                {                                         
                    listViewMeas.Items.Add(CreateItem(index, row.clazz));
                    ++index;
                }
                this.numericUpDownCount.Value = index;
                this.ResumeLayout();
        }

        public IEnumerable<EventRecord> GetRecords()
        {
            var list = new List<EventRecord>();
            ushort index = 0;
            foreach (ListViewItem item in listViewMeas.Items)
            {
                var pc = (PointClass) Enum.Parse(typeof(PointClass), item.SubItems[1].Text);
                list.Add(new EventRecord(index, pc));
                ++index;
            }
            return list;            
        }

        void CheckState()
        {
            if (listViewMeas.SelectedIndices.Count > 0)
            {
                this.buttonEdit.Enabled = true;
            }
            else
            {
                this.buttonEdit.Enabled = false;
            }
        }

        private void listViewMeas_SelectedIndexChanged(object sender, EventArgs e)
        {
            CheckState();
        }

        private void buttonEdit_Click(object sender, EventArgs e)
        {
            var items = listViewMeas.SelectedItems;
            if (items.Count > 0)
            {
                using (var dialog = new TemplateItemDialog())
                {
                    dialog.ShowDialog();
                    if (dialog.DialogResult == DialogResult.OK)
                    {
                        var pointClass = dialog.SelectedPointClass.ToString();
                        listViewMeas.SuspendLayout();
                        foreach (ListViewItem item in items)
                        {
                            item.SubItems[1].Text = pointClass;
                        }
                        listViewMeas.ResumeLayout();
                    }
                }
            }            
        }

        static ListViewItem CreateItem(int index, PointClass pc)
        {
            var strings = new String[] { index.ToString(), pc.ToString() };
            return new ListViewItem(strings);
        }

        private void numericUpDownCount_ValueChanged(object sender, EventArgs e)
        {
            if (this.numericUpDownCount.Value > this.listViewMeas.Items.Count)
            {
                int number = Decimal.ToInt32(this.numericUpDownCount.Value) - this.listViewMeas.Items.Count;
                int start = this.listViewMeas.Items.Count;                
                listViewMeas.SuspendLayout();
                for (int i = 0; i < number; ++i)
                {
                    listViewMeas.Items.Add(CreateItem(start, PointClass.Class1));
                    ++start;
                }
                listViewMeas.ResumeLayout();
            }
            else                 
            {
                listViewMeas.SuspendLayout();
                while (this.numericUpDownCount.Value < this.listViewMeas.Items.Count)
                {
                    this.listViewMeas.Items.RemoveAt(listViewMeas.Items.Count - 1);
                }
                listViewMeas.ResumeLayout();
            }
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            this.numericUpDownCount.Value = 0;
        }
      
    }
}
