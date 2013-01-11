using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace TestSetControlLibrary
{
    public class FilterLevelComboBox : System.Windows.Forms.ComboBox
    {
        public FilterLevelComboBox() : base()
        { 
            this.Items.Add("Event");
            this.Items.Add("Error");
            this.Items.Add("Warning");
            this.Items.Add("Info");
            this.Items.Add("Interpret");
            this.Items.Add("Comms");
            this.Items.Add("Debug");
            this.Text = "Warning";
        }

        public FilterLevel SelectedLevel
        {
            get
            {
                switch (this.SelectedText)
                {                    
                    case("Event"): 
                        return FilterLevel.LEV_EVENT;
                    case ("Error"): 
                        return FilterLevel.LEV_EVENT;
                    case ("Warning"): 
                        return FilterLevel.LEV_EVENT;
                    case ("Info"): 
                        return FilterLevel.LEV_EVENT;
                    case ("Interpret"): 
                        return FilterLevel.LEV_EVENT;
                    case ("Comms"): 
                        return FilterLevel.LEV_EVENT;
                    case ("Debug"): 
                        return FilterLevel.LEV_EVENT;
                    default:
                        return FilterLevel.LEV_INFO;
                }
            }
        }
    }
}
