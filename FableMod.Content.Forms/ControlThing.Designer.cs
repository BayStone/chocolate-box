namespace FableMod.Content.Forms
{
    partial class ControlThing
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBoxBase = new System.Windows.Forms.GroupBox();
            this.buttonGenerate = new System.Windows.Forms.Button();
            this.numericUpDownPlayer = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonOpen = new System.Windows.Forms.Button();
            this.textBoxUID = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxDefType = new System.Windows.Forms.TextBox();
            this.panelBase = new System.Windows.Forms.Panel();
            this.panelControlsBase.SuspendLayout();
            this.panelTop.SuspendLayout();
            this.panelExpansion.SuspendLayout();
            this.groupBoxBase.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayer)).BeginInit();
            this.panelBase.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelControls
            // 
            this.panelControls.Location = new System.Drawing.Point(0, 128);
            this.panelControls.Size = new System.Drawing.Size(442, 244);
            // 
            // panelControlsBase
            // 
            this.panelControlsBase.Controls.Add(this.panelBase);
            this.panelControlsBase.Size = new System.Drawing.Size(442, 372);
            this.panelControlsBase.Controls.SetChildIndex(this.panelBase, 0);
            this.panelControlsBase.Controls.SetChildIndex(this.panelControls, 0);
            // 
            // panelTop
            // 
            this.panelTop.Size = new System.Drawing.Size(466, 22);
            // 
            // buttonDefault
            // 
            this.buttonDefault.Location = new System.Drawing.Point(1238, 0);
            // 
            // panelExpansion
            // 
            this.panelExpansion.Size = new System.Drawing.Size(466, 372);
            // 
            // groupBoxBase
            // 
            this.groupBoxBase.Controls.Add(this.buttonGenerate);
            this.groupBoxBase.Controls.Add(this.numericUpDownPlayer);
            this.groupBoxBase.Controls.Add(this.label3);
            this.groupBoxBase.Controls.Add(this.buttonOpen);
            this.groupBoxBase.Controls.Add(this.textBoxUID);
            this.groupBoxBase.Controls.Add(this.label2);
            this.groupBoxBase.Controls.Add(this.label1);
            this.groupBoxBase.Controls.Add(this.textBoxDefType);
            this.groupBoxBase.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBoxBase.Location = new System.Drawing.Point(10, 10);
            this.groupBoxBase.Name = "groupBoxBase";
            this.groupBoxBase.Size = new System.Drawing.Size(422, 108);
            this.groupBoxBase.TabIndex = 1;
            this.groupBoxBase.TabStop = false;
            this.groupBoxBase.Text = "Base Information";
            // 
            // buttonGenerate
            // 
            this.buttonGenerate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonGenerate.Location = new System.Drawing.Point(354, 47);
            this.buttonGenerate.Name = "buttonGenerate";
            this.buttonGenerate.Size = new System.Drawing.Size(62, 21);
            this.buttonGenerate.TabIndex = 8;
            this.buttonGenerate.Text = "Generate";
            this.buttonGenerate.UseVisualStyleBackColor = true;
            this.buttonGenerate.Click += new System.EventHandler(this.buttonGenerate_Click);
            // 
            // numericUpDownPlayer
            // 
            this.numericUpDownPlayer.Location = new System.Drawing.Point(91, 76);
            this.numericUpDownPlayer.Maximum = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.numericUpDownPlayer.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.numericUpDownPlayer.Name = "numericUpDownPlayer";
            this.numericUpDownPlayer.Size = new System.Drawing.Size(38, 20);
            this.numericUpDownPlayer.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 78);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(39, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Player:";
            // 
            // buttonOpen
            // 
            this.buttonOpen.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonOpen.Location = new System.Drawing.Point(354, 21);
            this.buttonOpen.Name = "buttonOpen";
            this.buttonOpen.Size = new System.Drawing.Size(62, 21);
            this.buttonOpen.TabIndex = 4;
            this.buttonOpen.Text = "Open";
            this.buttonOpen.UseVisualStyleBackColor = true;
            this.buttonOpen.Click += new System.EventHandler(this.buttonOpen_Click);
            // 
            // textBoxUID
            // 
            this.textBoxUID.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxUID.Location = new System.Drawing.Point(91, 48);
            this.textBoxUID.Name = "textBoxUID";
            this.textBoxUID.Size = new System.Drawing.Size(257, 20);
            this.textBoxUID.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 51);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "UID:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Definition Type:";
            // 
            // textBoxDefType
            // 
            this.textBoxDefType.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxDefType.Location = new System.Drawing.Point(91, 22);
            this.textBoxDefType.Name = "textBoxDefType";
            this.textBoxDefType.ReadOnly = true;
            this.textBoxDefType.Size = new System.Drawing.Size(257, 20);
            this.textBoxDefType.TabIndex = 0;
            // 
            // panelBase
            // 
            this.panelBase.Controls.Add(this.groupBoxBase);
            this.panelBase.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelBase.Location = new System.Drawing.Point(0, 0);
            this.panelBase.Name = "panelBase";
            this.panelBase.Padding = new System.Windows.Forms.Padding(10);
            this.panelBase.Size = new System.Drawing.Size(442, 128);
            this.panelBase.TabIndex = 3;
            // 
            // ControlThing
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "ControlThing";
            this.Size = new System.Drawing.Size(466, 394);
            this.panelControlsBase.ResumeLayout(false);
            this.panelTop.ResumeLayout(false);
            this.panelTop.PerformLayout();
            this.panelExpansion.ResumeLayout(false);
            this.groupBoxBase.ResumeLayout(false);
            this.groupBoxBase.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPlayer)).EndInit();
            this.panelBase.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxBase;
        private System.Windows.Forms.Button buttonGenerate;
        private System.Windows.Forms.NumericUpDown numericUpDownPlayer;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonOpen;
        private System.Windows.Forms.TextBox textBoxUID;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxDefType;
        private System.Windows.Forms.Panel panelBase;


    }
}
