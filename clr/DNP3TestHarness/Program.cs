using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using DNP3.Interface;
using DNP3.Adapter;

namespace Automatak.DNP3.Simulator
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            var manager = DNP3ManagerFactory.CreateManager();
            var masterPlugins = new List<IMasterPluginFactory> { GUIMasterPluginFactory.Instance };

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            var form = new SimulatorForm(manager, masterPlugins);
            Application.Run(form);
        }
    }
}
