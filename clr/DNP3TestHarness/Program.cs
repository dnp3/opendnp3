using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using DNP3.Interface;
using DNP3.Adapter;

namespace DNP3TestHarness
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            var suppressSplash = args.Contains("-sf");

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            var manager = DNP3ManagerFactory.CreateManager();
            var form = new SimulatorForm(manager, suppressSplash);
            Application.Run(form);
        }
    }
}
