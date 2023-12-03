using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace IbfControlPanel
{
    public partial class Panel : Form
    {
        private PictureBox[] leftDiodes;
        private PictureBox[] rightDiodes;

        public Panel()
        {
            InitializeComponent();
            Cursor.Hide();
            serialPort.Open();
            // serialPortSupport.Open();

            leftDiodes = new PictureBox[]{
                pictureBox8, pictureBox7, pictureBox6,
                pictureBox5, pictureBox4, pictureBox3,
                pictureBox2, pictureBox1
            };

            rightDiodes = new PictureBox[]{
                pictureBox16, pictureBox15, pictureBox14,
                pictureBox13, pictureBox12, pictureBox11,
                pictureBox10, pictureBox9
            };
        }

        private void trackBarLeft_ValueChanged(object sender, EventArgs e)
        {

            for (int i = 0; i < 8; i++)
            {
                leftDiodes[i].BackColor = Color.Silver;
            }

            for (int i = 0; i < trackBarLeft.Value; i++)
            {
                leftDiodes[i].BackColor = Color.YellowGreen;

                if (i >= 6)
                {
                    leftDiodes[i].BackColor = Color.Gold;
                }
            }

            serialPort.Write("L" + trackBarLeft.Value);
            Thread.Sleep(900);
        }

        private void trackBarRight_ValueChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < 8; i++)
            {
                rightDiodes[i].BackColor = Color.Silver;
            }

            for (int i = 0; i < trackBarRight.Value; i++)
            {
                rightDiodes[i].BackColor = Color.YellowGreen;

                if (i >= 6)
                {
                    rightDiodes[i].BackColor = Color.Gold;
                }
            }

            serialPort.Write("R" + trackBarRight.Value);
            Thread.Sleep(900);
        }

        private void failOneButton_Click(object sender, EventArgs e)
        {
            serialPort.Write("F1");
            Thread.Sleep(1100);
            if (failOneButton.BackColor == Color.Silver)
            {
                failOneButton.BackColor = Color.YellowGreen;
                failOneButton.ForeColor = Color.Black;

            }
            else {
                failOneButton.BackColor = Color.Silver;
                failOneButton.ForeColor = SystemColors.ControlText;
            }
        }

        private void failTwoButton_Click(object sender, EventArgs e)
        {
            serialPort.Write("F2");
            Thread.Sleep(1100);
            if (failTwoButton.BackColor == Color.Silver)
            {
                failTwoButton.BackColor = Color.YellowGreen;
                failTwoButton.ForeColor = Color.Black;
            }
            else
            {
                failTwoButton.BackColor = Color.Silver;
                failTwoButton.ForeColor = SystemColors.ControlText;
            }
        }

        private void resetButton_Click(object sender, EventArgs e)
        {
            serialPort.Write("C1");
            // serialPortSupport.Write("C1");
            Thread.Sleep(750);
            failOneButton.BackColor = Color.Silver;
            failTwoButton.BackColor = Color.Silver;
            backlight_button.BackColor = Color.Silver;
            for (int i = 0; i < 8; i++)
            {
                rightDiodes[i].BackColor = Color.Silver;
                leftDiodes[i].BackColor = Color.Silver;
            }
            trackBarLeft.Value = 0;
            trackBarRight.Value = 0;
        }

        private void backlight_Click(object sender, EventArgs e)
        {
            //serialPortSupport.Write("B1");
            Thread.Sleep(750);
            if (backlight_button.BackColor == Color.Silver)
            {
                backlight_button.BackColor = Color.YellowGreen;
                backlight_button.ForeColor = Color.Black;
            }
            else
            {
                backlight_button.BackColor = Color.Silver;
                backlight_button.ForeColor = SystemColors.ControlText;
            }
        }
    }
}
