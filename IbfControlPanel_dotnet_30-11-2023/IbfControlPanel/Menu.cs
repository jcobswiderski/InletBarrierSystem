using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace IbfControlPanel
{
    public partial class Menu : Form
    {
        public Panel panel;
        bool active = false;

        public Menu()
        {
            InitializeComponent();

            int x = 115;
            int y = 35;

            this.MinimumSize = new Size(x+2, y+1); 
            this.MaximumSize = new Size(x+2, y+1); 
            this.AutoSize = true; 
            this.AutoSizeMode = AutoSizeMode.GrowAndShrink;

            ShowPanelBtn.MinimumSize = new Size(x, y); 
            ShowPanelBtn.MaximumSize = new Size(x, y); 
            ShowPanelBtn.AutoSize = true; 
            ShowPanelBtn.AutoSizeMode = AutoSizeMode.GrowAndShrink;
        }

        private void ShowPanelBtn_Click(object sender, EventArgs e)
        {
            if (panel == null)
            {
                panel = new Panel();
                panel.MdiParent = null;
                panel.Show();
            }
            else {
                panel.Activate();
            }

            if (active) {
                panel.Hide();
                ShowPanelBtn.BackColor = System.Drawing.Color.AntiqueWhite;
            } else {
                panel.Show();

                ShowPanelBtn.BackColor = System.Drawing.Color.LightGreen;
            }
            active = !active;

        }
    }
}
