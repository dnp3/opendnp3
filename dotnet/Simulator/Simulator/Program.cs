using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.Simulator.API;
using Automatak.Simulator.DNP3;

namespace Automatak.Simulator
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            var plugins = new List<ISimulatorPluginFactory>() { DNP3SimulatorPluginFactory.Instance };

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            var splashOnLoad = true;

            if (args.Length >= 1 && args[0] == "-nosplash")
            {
                splashOnLoad = false;
            }

            var form = new SimulatorForm(plugins, splashOnLoad);
            Application.Run(form);
        }
    }
}
