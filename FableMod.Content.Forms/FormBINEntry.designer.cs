namespace FableMod.Content.Forms
{
    partial class FormBINEntry
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
            this.lblSymbolName = new System.Windows.Forms.Label();
            this.lblDefinitionType = new System.Windows.Forms.Label();
            this.txtSymbolName = new System.Windows.Forms.TextBox();
            this.txtDefinitionType = new System.Windows.Forms.TextBox();
            this.btnExport = new System.Windows.Forms.Button();
            this.btnViewData = new System.Windows.Forms.Button();
            this.btnImport = new System.Windows.Forms.Button();
            this.btnApplyChanges = new System.Windows.Forms.Button();
            this.spltContainerMain = new System.Windows.Forms.SplitContainer();
            this.textBoxId = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.spltContainerMain.Panel1.SuspendLayout();
            this.spltContainerMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblSymbolName
            // 
            this.lblSymbolName.AutoSize = true;
            this.lblSymbolName.Location = new System.Drawing.Point(9, 12);
            this.lblSymbolName.Name = "lblSymbolName";
            this.lblSymbolName.Size = new System.Drawing.Size(75, 13);
            this.lblSymbolName.TabIndex = 0;
            this.lblSymbolName.Text = "Symbol Name:";
            // 
            // lblDefinitionType
            // 
            this.lblDefinitionType.AutoSize = true;
            this.lblDefinitionType.Location = new System.Drawing.Point(9, 36);
            this.lblDefinitionType.Name = "lblDefinitionType";
            this.lblDefinitionType.Size = new System.Drawing.Size(81, 13);
            this.lblDefinitionType.TabIndex = 1;
            this.lblDefinitionType.Text = "Definition Type:";
            // 
            // txtSymbolName
            // 
            this.txtSymbolName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtSymbolName.Location = new System.Drawing.Point(97, 9);
            this.txtSymbolName.Name = "txtSymbolName";
            this.txtSymbolName.Size = new System.Drawing.Size(300, 20);
            this.txtSymbolName.TabIndex = 2;
            // 
            // txtDefinitionType
            // 
            this.txtDefinitionType.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtDefinitionType.Location = new System.Drawing.Point(97, 33);
            this.txtDefinitionType.Name = "txtDefinitionType";
            this.txtDefinitionType.Size = new System.Drawing.Size(300, 20);
            this.txtDefinitionType.TabIndex = 3;
            // 
            // btnExport
            // 
            this.btnExport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnExport.Location = new System.Drawing.Point(109, 85);
            this.btnExport.Name = "btnExport";
            this.btnExport.Size = new System.Drawing.Size(92, 24);
            this.btnExport.TabIndex = 4;
            this.btnExport.Text = "Export";
            this.btnExport.UseVisualStyleBackColor = true;
            this.btnExport.Click += new System.EventHandler(this.btnExport_Click);
            // 
            // btnViewData
            // 
            this.btnViewData.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnViewData.Location = new System.Drawing.Point(11, 85);
            this.btnViewData.Name = "btnViewData";
            this.btnViewData.Size = new System.Drawing.Size(92, 24);
            this.btnViewData.TabIndex = 5;
            this.btnViewData.Text = "View Data";
            this.btnViewData.UseVisualStyleBackColor = true;
            this.btnViewData.Click += new System.EventHandler(this.btnViewData_Click);
            // 
            // btnImport
            // 
            this.btnImport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnImport.Location = new System.Drawing.Point(207, 85);
            this.btnImport.Name = "btnImport";
            this.btnImport.Size = new System.Drawing.Size(92, 24);
            this.btnImport.TabIndex = 6;
            this.btnImport.Text = "Import";
            this.btnImport.UseVisualStyleBackColor = true;
            this.btnImport.Click += new System.EventHandler(this.btnImport_Click);
            // 
            // btnApplyChanges
            // 
            this.btnApplyChanges.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnApplyChanges.Location = new System.Drawing.Point(305, 85);
            this.btnApplyChanges.Name = "btnApplyChanges";
            this.btnApplyChanges.Size = new System.Drawing.Size(92, 24);
            this.btnApplyChanges.TabIndex = 7;
            this.btnApplyChanges.Text = "Apply Changes";
            this.btnApplyChanges.UseVisualStyleBackColor = true;
            this.btnApplyChanges.Click += new System.EventHandler(this.btnApplyChanges_Click);
            // 
            // spltContainerMain
            // 
            this.spltContainerMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.spltContainerMain.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.spltContainerMain.IsSplitterFixed = true;
            this.spltContainerMain.Location = new System.Drawing.Point(0, 0);
            this.spltContainerMain.Name = "spltContainerMain";
            this.spltContainerMain.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // spltContainerMain.Panel1
            // 
            this.spltContainerMain.Panel1.Controls.Add(this.textBoxId);
            this.spltContainerMain.Panel1.Controls.Add(this.label1);
            this.spltContainerMain.Panel1.Controls.Add(this.txtSymbolName);
            this.spltContainerMain.Panel1.Controls.Add(this.btnApplyChanges);
            this.spltContainerMain.Panel1.Controls.Add(this.lblSymbolName);
            this.spltContainerMain.Panel1.Controls.Add(this.btnImport);
            this.spltContainerMain.Panel1.Controls.Add(this.lblDefinitionType);
            this.spltContainerMain.Panel1.Controls.Add(this.btnViewData);
            this.spltContainerMain.Panel1.Controls.Add(this.txtDefinitionType);
            this.spltContainerMain.Panel1.Controls.Add(this.btnExport);
            this.spltContainerMain.Panel1MinSize = 85;
            this.spltContainerMain.Size = new System.Drawing.Size(409, 174);
            this.spltContainerMain.SplitterDistance = 120;
            this.spltContainerMain.TabIndex = 8;
            // 
            // textBoxId
            // 
            this.textBoxId.Location = new System.Drawing.Point(97, 59);
            this.textBoxId.Name = "textBoxId";
            this.textBoxId.ReadOnly = true;
            this.textBoxId.Size = new System.Drawing.Size(68, 20);
            this.textBoxId.TabIndex = 9;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 62);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(21, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "ID:";
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.DefaultExt = "fbf";
            this.saveFileDialog.Filter = "Fable BIN Files (*.fbf)|*.fbf|All Files (*.*)|*.*||";
            // 
            // openFileDialog
            // 
            this.openFileDialog.DefaultExt = "fbf";
            this.openFileDialog.Filter = "Fable BIN Files (*.fbf)|*.fbf|All Files (*.*)|*.*||";
            this.openFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog_FileOk);
            // 
            // FormBINEntry
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.CausesValidation = false;
            this.ClientSize = new System.Drawing.Size(409, 174);
            this.Controls.Add(this.spltContainerMain);
            this.MinimumSize = new System.Drawing.Size(417, 34);
            this.Name = "FormBINEntry";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "BIN Entry";
            this.spltContainerMain.Panel1.ResumeLayout(false);
            this.spltContainerMain.Panel1.PerformLayout();
            this.spltContainerMain.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label lblSymbolName;
        private System.Windows.Forms.Label lblDefinitionType;
        private System.Windows.Forms.TextBox txtSymbolName;
        private System.Windows.Forms.TextBox txtDefinitionType;
        private System.Windows.Forms.Button btnExport;
        private System.Windows.Forms.Button btnViewData;
        private System.Windows.Forms.Button btnImport;
        private System.Windows.Forms.Button btnApplyChanges;
        private System.Windows.Forms.SplitContainer spltContainerMain;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.TextBox textBoxId;
        private System.Windows.Forms.Label label1;
    }
}