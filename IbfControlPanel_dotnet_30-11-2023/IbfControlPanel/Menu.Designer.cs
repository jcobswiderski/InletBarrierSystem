namespace IbfControlPanel
{
    partial class Menu
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ShowPanelBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ShowPanelBtn
            // 
            this.ShowPanelBtn.BackColor = System.Drawing.Color.AntiqueWhite;
            this.ShowPanelBtn.Location = new System.Drawing.Point(1, 2);
            this.ShowPanelBtn.Name = "ShowPanelBtn";
            this.ShowPanelBtn.Size = new System.Drawing.Size(125, 50);
            this.ShowPanelBtn.TabIndex = 0;
            this.ShowPanelBtn.Text = "IBF CONTROL";
            this.ShowPanelBtn.UseVisualStyleBackColor = false;
            this.ShowPanelBtn.Click += new System.EventHandler(this.ShowPanelBtn_Click);
            // 
            // Menu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(650, 75);
            this.ControlBox = false;
            this.Controls.Add(this.ShowPanelBtn);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.IsMdiContainer = true;
            this.Location = new System.Drawing.Point(950, 1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Menu";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "IBF MENU";
            this.TopMost = true;
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button ShowPanelBtn;
    }
}