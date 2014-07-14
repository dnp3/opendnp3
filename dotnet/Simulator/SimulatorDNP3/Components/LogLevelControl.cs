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
    public partial class LogLevelControl : UserControl
    {
        public LogLevelControl()
        {
            InitializeComponent();
        }

        public LogFilter Filters
        {
            get
            {
                uint flags = LogFilters.EVENT;

                if (checkBoxError.Checked)
                {
                    flags |= LogFilters.ERROR;
                }

                if (checkBoxWarn.Checked)
                {
                    flags |= LogFilters.WARNING;
                }

                if (checkBoxInfo.Checked)
                {
                    flags |= LogFilters.INFO;
                }

                if (checkBoxDebug.Checked)
                {
                    flags |= LogFilters.DEBUG;
                }

                if (checkBoxLink.Checked)
                {
                    flags |= (LogFilters.LINK_RX | LogFilters.LINK_TX);
                }

                if (checkBoxLinkRaw.Checked)
                {
                    flags |= (LogFilters.LINK_RX_HEX | LogFilters.LINK_TX_HEX);
                }

                if (checkBoxTransport.Checked)
                {
                    flags |= (LogFilters.TRANSPORT_RX | LogFilters.TRANSPORT_TX);
                }

                if (checkBoxAppHeader.Checked)
                {
                    flags |= (LogFilters.APP_HEADER_RX | LogFilters.APP_HEADER_TX);
                    flags |= (LogFilters.APP_OBJECT_RX | LogFilters.APP_OBJECT_TX);
                }
                
                return new LogFilter(flags);
            }
        }
    }
}
