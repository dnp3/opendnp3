using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Automatak.DNP3.Simulator
{
    public partial class LogControl : UserControl
    {
        public LogControl()
        {
            InitializeComponent();
        }

        public void AddRows(IEnumerable<string> rs)
        {
            if(rs.Any())
            { 
                foreach (var s in rs) rows.Add(s);
                while(rows.Count > maxRows)
                {
                    rows.RemoveAt(0);
                }
                if (!paused)
                {
                    this.Refresh();
                }                
            }
        }

        public void SetViewport(double fraction)
        {
            if (fraction >= 0 && fraction <= 1.0)
            { 
                this.viewFraction = fraction;
                this.Refresh();
            }
        }

        int CalcBackRows(ICollection<string> rows)
        {            
            if (viewFraction >= 0 && viewFraction <= 1.0)
            {
                return (int)Math.Floor((1.0 - viewFraction) * rows.Count);
            }
            else
            {
                return 0;
            }            
        }        

        public void Pause()
        {
            if (!paused)
            {
                this.paused = true;
                var rows = GetViewportRows();
                this.snapshot.Clear();
                foreach (var s in rows)
                {
                    snapshot.Add(s);
                }
                this.Refresh();
            }
        }

        public void Resume()
        {
            if (paused)
            {
                this.snapshot.Clear();
                this.paused = false;
                this.Refresh();
            }
        }

        public int MaxRows
        {
            get
            {
                return maxRows;
            }
        }

        public int NumRows
        {
            get
            {
                return GetCurrentRows().Count;
            }
        }        

        public void Clear()
        {
            rows.Clear();
            this.Refresh();
        }

        protected override void OnPaint(PaintEventArgs pe)
        {           
            base.OnPaint(pe);            

            PaintRows(pe.Graphics, GetViewportRows());               
        }

        IList<string> GetCurrentRows()
        {            
            if (paused)
            {
                return snapshot;                
            }
            else
            {
                return rows;
            }
        }

        public IEnumerable<string> GetAllRows()
        {
            return GetCurrentRows();
        }

        public IEnumerable<string> GetViewportRows()
        {
            return TrimRows(GetCurrentRows());
        }

        public IEnumerable<string> TrimRows(IList<string> list)
        {
            var rowsVisible = this.NumRowsVisible();

            if (rowsVisible < list.Count)
            {
                var skipCount = list.Count - rowsVisible - CalcBackRows(list);

                return rows.Skip(skipCount);
            }
            else
            {
                return rows;
            } 
        }

        private void PaintRows(Graphics g, IEnumerable<string> rows)
        {
            // Declare and instantiate a new pen.             
            var brush = new System.Drawing.SolidBrush(this.ForeColor);

            var ri = 0;

            foreach (var s in rows)
            {
                g.DrawString(s, this.Font, brush, new PointF(0, RowPosition(ri)));
                ++ri;
            } 
        }

        [Description("Test text displayed in the textbox"), Category("Look and Feel")]
        public float SpacingFactor
        {
            get { return spacingFactor; }
            set {
                if (value >= 0 && value <= 1.0)
                {
                    spacingFactor = value;
                }
            }
        }

        IEnumerable<String> VisibleLines
        {
            get
            {
                return GetViewportRows();
            }
        }

        public bool IsPaused
        {
            get
            {
                return paused;
            }
        }

        private float RowPosition(int row)
        {
            return ((1.0f + spacingFactor) * Font.Height) * row + (spacingFactor * Font.Height);
        }

        private int NumRowsVisible()
        {           
            var num = (int) Math.Floor(this.Height / (Font.Height * (1.0f + spacingFactor)));
            if (num > 0)
            {
                return num - 1;
            }
            else
            {
                return 0;
            }            
        }

        private float spacingFactor = 0.1f;
        private int maxRows = 500;
        private double viewFraction = 1.0;
        private bool paused = false;
        private IList<string> rows = new List<string>();
        private IList<string> snapshot = new List<string>();
    }
}

   
