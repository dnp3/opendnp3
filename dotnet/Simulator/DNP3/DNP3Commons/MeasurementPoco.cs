using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ComponentModel;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Commons
{    
    class MeasurementPoco : INotifyPropertyChanged
    {        
        public event PropertyChangedEventHandler PropertyChanged;

        private void NotifyPropertyChanged(string name)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(name));
            }
        }

        public MeasurementPoco(int row, string sValue, MeasurementBase meas, UInt16 index, MeasType type)
        {
            this.sValue = sValue;
            this.index = index;
            this.flags = meas.Quality.ToString("X2");
            this.timeStamp = meas.Timestamp;
            this.row = row;  
            this.type = type;
        }        

        public void Update(string sValue, MeasurementBase meas)
        {
            Value = sValue;
            Flags = meas.Quality.ToString("X2");
            TimeStamp = meas.Timestamp;
        }

        private UInt16 index;
        private string sValue;
        private string flags;
        private DateTime timeStamp;
        private int row;
        private MeasType type;

        public void NextRow()
        {
            ++row;
        }
     
        public UInt16 Index
        {
            get { return index; }
            set { 
                index = value;
                NotifyPropertyChanged("Index");
            }
        }

        public string Value
        {
            get { return sValue; }
            set { 
                sValue = value;
                NotifyPropertyChanged("Value");
            }
        }

        public string Flags
        {
            get { return flags; }
            set { 
                flags = value;
                NotifyPropertyChanged("Flags");
            }
        }

        public DateTime TimeStamp
        {
            get { return timeStamp; }
            set { 
                timeStamp = value;
                NotifyPropertyChanged("TimeStamp");
            }
        }

        [Browsable(false)]
        public int Row
        {
            get { return row; }
            set { row = value; }
        }       

        [Browsable(false)]
        public MeasType Type
        {
            get { return type; }
            set { type = value; }
        }  
    }
}
