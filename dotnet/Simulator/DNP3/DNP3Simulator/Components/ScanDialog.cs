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

namespace Automatak.Simulator.DNP3.Components
{
    


    public partial class ScanDialog : Form
    {
        readonly IMaster master;
        readonly IList<ScanInfo> scans = new List<ScanInfo>();

        public ScanDialog(IMaster master)
        {
            InitializeComponent();

            this.master = master;
        }

        private void buttonOnce_Click(object sender, EventArgs e)
        {            
            master.Scan(GetClassHeaders(classFieldControlScan.ClassFieldValue), TaskConfig.Default);
        }

        private static IEnumerable<Header> GetClassHeaders(ClassField classes)
        { 
            IList<Header> headers = new List<Header>();            
            if (classes.Class1)
            {
                headers.Add(Header.AllObjects(60, 2));
            }
            if (classes.Class2)
            {
                headers.Add(Header.AllObjects(60, 3));
            }
            if (classes.Class3)
            {
                headers.Add(Header.AllObjects(60, 4));
            }
            if (classes.Class0)
            {
                headers.Add(Header.AllObjects(60, 1));
            }
            return headers;
        }

        private static string DetailedDescription(IEnumerable<Header> headers)
        {
            return String.Join(", ", headers);
        }

        private static string ClassDescription(ClassField classes)
        {
            ICollection<string> names = new List<string>();

            if (classes.Class1) names.Add("1");
            if (classes.Class2) names.Add("2");
            if (classes.Class3) names.Add("3");
            if (classes.Class0) names.Add("0");

            return names.Any() ? String.Format("Class {0}", String.Join("/", names.ToArray())) : "Empty";
        }

        private void buttonBind_Click(object sender, EventArgs e)
        {
            var period = TimeSpan.FromMilliseconds(Convert.ToDouble(this.numericUpDownPeriod.Value));
            var classes = classFieldControlScan.ClassFieldValue;
            var headers = GetClassHeaders(classes);            
            var info = new ScanInfo(master.AddScan(headers, period, TaskConfig.Default), ClassDescription(classes), period, DetailedDescription(headers));
            this.scans.Add(info);
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public IEnumerable<ScanInfo> ConfiguredScans
        {
            get
            {
                return scans;
            }
        }


    }
}
