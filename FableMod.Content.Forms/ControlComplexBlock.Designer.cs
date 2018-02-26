namespace FableMod.Content.Forms
{
    partial class ControlComplexBlock
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
            this.panelTop = new System.Windows.Forms.Panel();
            this.labelName = new System.Windows.Forms.Label();
            this.buttonExpand = new System.Windows.Forms.Button();
            this.buttonDefault = new System.Windows.Forms.Button();
            this.panelTab = new System.Windows.Forms.Panel();
            this.panelControls = new System.Windows.Forms.Panel();
            this.panelExpansion = new System.Windows.Forms.Panel();
            this.panelControlsBase = new System.Windows.Forms.Panel();
            this.panelTop.SuspendLayout();
            this.panelExpansion.SuspendLayout();
            this.panelControlsBase.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelTop
            // 
            this.panelTop.BackColor = System.Drawing.SystemColors.Control;
            this.panelTop.Controls.Add(this.labelName);
            this.panelTop.Controls.Add(this.buttonExpand);
            this.panelTop.Controls.Add(this.buttonDefault);
            this.panelTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelTop.Location = new System.Drawing.Point(0, 0);
            this.panelTop.Name = "panelTop";
            this.panelTop.Padding = new System.Windows.Forms.Padding(0, 0, 11, 0);
            this.panelTop.Size = new System.Drawing.Size(301, 22);
            this.panelTop.TabIndex = 0;
            // 
            // labelName
            // 
            this.labelName.AutoSize = true;
            this.labelName.Font = new System.Drawing.Font("Microsoft Sans Serif", 7F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelName.Location = new System.Drawing.Point(24, 4);
            this.labelName.Name = "labelName";
            this.labelName.Size = new System.Drawing.Size(34, 13);
            this.labelName.TabIndex = 3;
            this.labelName.Text = "Name";
            // 
            // buttonExpand
            // 
            this.buttonExpand.Location = new System.Drawing.Point(0, 0);
            this.buttonExpand.Name = "buttonExpand";
            this.buttonExpand.Size = new System.Drawing.Size(21, 21);
            this.buttonExpand.TabIndex = 2;
            this.buttonExpand.TabStop = false;
            this.buttonExpand.Text = "+";
            this.buttonExpand.UseVisualStyleBackColor = true;
            this.buttonExpand.Click += new System.EventHandler(this.buttonExpand_Click);
            // 
            // buttonDefault
            // 
            this.buttonDefault.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonDefault.Location = new System.Drawing.Point(266, 0);
            this.buttonDefault.Name = "buttonDefault";
            this.buttonDefault.Size = new System.Drawing.Size(35, 21);
            this.buttonDefault.TabIndex = 2;
            this.buttonDefault.TabStop = false;
            this.buttonDefault.Text = "Def";
            this.buttonDefault.UseVisualStyleBackColor = true;
            this.buttonDefault.Click += new System.EventHandler(this.buttonDefault_Click);
            // 
            // panelTab
            // 
            this.panelTab.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelTab.Location = new System.Drawing.Point(0, 0);
            this.panelTab.Name = "panelTab";
            this.panelTab.Size = new System.Drawing.Size(24, 226);
            this.panelTab.TabIndex = 1;
            // 
            // panelControls
            // 
            this.panelControls.AutoScroll = true;
            this.panelControls.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panelControls.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelControls.Location = new System.Drawing.Point(0, 0);
            this.panelControls.Name = "panelControls";
            this.panelControls.Size = new System.Drawing.Size(277, 226);
            this.panelControls.TabIndex = 2;
            // 
            // panelExpansion
            // 
            this.panelExpansion.Controls.Add(this.panelControlsBase);
            this.panelExpansion.Controls.Add(this.panelTab);
            this.panelExpansion.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelExpansion.Location = new System.Drawing.Point(0, 22);
            this.panelExpansion.Name = "panelExpansion";
            this.panelExpansion.Size = new System.Drawing.Size(301, 226);
            this.panelExpansion.TabIndex = 3;
            this.panelExpansion.Visible = false;
            // 
            // panelControlsBase
            // 
            this.panelControlsBase.Controls.Add(this.panelControls);
            this.panelControlsBase.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelControlsBase.Location = new System.Drawing.Point(24, 0);
            this.panelControlsBase.Name = "panelControlsBase";
            this.panelControlsBase.Size = new System.Drawing.Size(277, 226);
            this.panelControlsBase.TabIndex = 3;
            // 
            // ControlComplexBlock
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.Controls.Add(this.panelExpansion);
            this.Controls.Add(this.panelTop);
            this.Name = "ControlComplexBlock";
            this.Size = new System.Drawing.Size(301, 248);
            this.panelTop.ResumeLayout(false);
            this.panelTop.PerformLayout();
            this.panelExpansion.ResumeLayout(false);
            this.panelControlsBase.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelTab;
        protected internal System.Windows.Forms.Panel panelControls;
        protected internal System.Windows.Forms.Label labelName;
        protected internal System.Windows.Forms.Panel panelControlsBase;
        protected internal System.Windows.Forms.Panel panelTop;
        protected internal System.Windows.Forms.Button buttonDefault;
        protected internal System.Windows.Forms.Panel panelExpansion;
        protected internal System.Windows.Forms.Button buttonExpand;
    }
}
