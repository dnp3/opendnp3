using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;

namespace Automatak.Simulator
{
    partial class AboutBox : Form
    {
        public AboutBox()
        {
            InitializeComponent();

            string[] lines = {
                                                  
                 "This software is based on open source components with the following licenses:",
                 "",
                 "opendnp3",                 
                 "http://www.automatak.com/opendnp3",
                 "http://www.apache.org/licenses/LICENSE-2.0.html",
                 "",
                 "asio",
                 "http://think-async.com/",
                 "http://www.boost.org/users/license.html",
                 "",
                 "This software is distributed on an \"AS IS\" BASIS",
                 "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.",
            };


            this.richTextBox1.Text = String.Join(Environment.NewLine, lines);            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void richTextBox1_LinkClicked(object sender, LinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start(e.LinkText);
        }        
    }
}
