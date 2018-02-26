namespace ChocolateBox
{
    partial class FormModPackage
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
            this.listViewEntries = new System.Windows.Forms.ListView();
            this.columnHeader0 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.buttonSaveAs = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxDesc = new System.Windows.Forms.TextBox();
            this.buttonApply = new System.Windows.Forms.Button();
            this.panelButtons = new System.Windows.Forms.Panel();
            this.buttonMerge = new System.Windows.Forms.Button();
            this.buttonModified = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.splitter = new System.Windows.Forms.Splitter();
            this.openFileDialogFMP = new System.Windows.Forms.OpenFileDialog();
            this.panelButtons.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewEntries
            // 
            this.listViewEntries.AllowDrop = true;
            this.listViewEntries.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader0,
            this.columnHeader1});
            this.listViewEntries.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewEntries.Location = new System.Drawing.Point(8, 107);
            this.listViewEntries.Name = "listViewEntries";
            this.listViewEntries.Size = new System.Drawing.Size(460, 172);
            this.listViewEntries.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.listViewEntries.TabIndex = 0;
            this.listViewEntries.UseCompatibleStateImageBehavior = false;
            this.listViewEntries.View = System.Windows.Forms.View.Details;
            this.listViewEntries.DragDrop += new System.Windows.Forms.DragEventHandler(this.listViewEntries_DragDrop);
            this.listViewEntries.DragEnter += new System.Windows.Forms.DragEventHandler(this.listViewEntries_DragEnter);
            this.listViewEntries.KeyDown += new System.Windows.Forms.KeyEventHandler(this.listViewEntries_KeyDown);
            // 
            // columnHeader0
            // 
            this.columnHeader0.Text = "Name";
            this.columnHeader0.Width = 220;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Type";
            this.columnHeader1.Width = 150;
            // 
            // buttonSaveAs
            // 
            this.buttonSaveAs.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSaveAs.Location = new System.Drawing.Point(304, 9);
            this.buttonSaveAs.Name = "buttonSaveAs";
            this.buttonSaveAs.Size = new System.Drawing.Size(75, 23);
            this.buttonSaveAs.TabIndex = 2;
            this.buttonSaveAs.Text = "Save As...";
            this.buttonSaveAs.UseVisualStyleBackColor = true;
            this.buttonSaveAs.Click += new System.EventHandler(this.buttonSaveAs_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCancel.Location = new System.Drawing.Point(385, 9);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 3;
            this.buttonCancel.Text = "Close";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.DefaultExt = "fmp";
            this.saveFileDialog.Filter = "Fable Mod Packages (*.fmp)|*.fmp|All Files (*.*)|*.*||";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(-3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Description:";
            // 
            // textBoxDesc
            // 
            this.textBoxDesc.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxDesc.Location = new System.Drawing.Point(0, 16);
            this.textBoxDesc.Multiline = true;
            this.textBoxDesc.Name = "textBoxDesc";
            this.textBoxDesc.Size = new System.Drawing.Size(460, 77);
            this.textBoxDesc.TabIndex = 5;
            // 
            // buttonApply
            // 
            this.buttonApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonApply.Location = new System.Drawing.Point(41, 9);
            this.buttonApply.Name = "buttonApply";
            this.buttonApply.Size = new System.Drawing.Size(75, 23);
            this.buttonApply.TabIndex = 6;
            this.buttonApply.Text = "Apply";
            this.buttonApply.UseVisualStyleBackColor = true;
            this.buttonApply.Visible = false;
            this.buttonApply.Click += new System.EventHandler(this.buttonApply_Click);
            // 
            // panelButtons
            // 
            this.panelButtons.Controls.Add(this.buttonMerge);
            this.panelButtons.Controls.Add(this.buttonModified);
            this.panelButtons.Controls.Add(this.buttonCancel);
            this.panelButtons.Controls.Add(this.buttonApply);
            this.panelButtons.Controls.Add(this.buttonSaveAs);
            this.panelButtons.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelButtons.Location = new System.Drawing.Point(8, 279);
            this.panelButtons.Name = "panelButtons";
            this.panelButtons.Size = new System.Drawing.Size(460, 32);
            this.panelButtons.TabIndex = 7;
            // 
            // buttonMerge
            // 
            this.buttonMerge.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonMerge.Location = new System.Drawing.Point(223, 9);
            this.buttonMerge.Name = "buttonMerge";
            this.buttonMerge.Size = new System.Drawing.Size(75, 23);
            this.buttonMerge.TabIndex = 8;
            this.buttonMerge.Text = "Merge...";
            this.buttonMerge.UseVisualStyleBackColor = true;
            this.buttonMerge.Click += new System.EventHandler(this.buttonMerge_Click);
            // 
            // buttonModified
            // 
            this.buttonModified.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonModified.Location = new System.Drawing.Point(122, 9);
            this.buttonModified.Name = "buttonModified";
            this.buttonModified.Size = new System.Drawing.Size(95, 23);
            this.buttonModified.TabIndex = 7;
            this.buttonModified.Text = "Get Modified";
            this.buttonModified.UseVisualStyleBackColor = true;
            this.buttonModified.Click += new System.EventHandler(this.buttonModified_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.textBoxDesc);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(8, 8);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(460, 93);
            this.panel1.TabIndex = 8;
            // 
            // splitter
            // 
            this.splitter.Cursor = System.Windows.Forms.Cursors.HSplit;
            this.splitter.Dock = System.Windows.Forms.DockStyle.Top;
            this.splitter.Location = new System.Drawing.Point(8, 101);
            this.splitter.Name = "splitter";
            this.splitter.Size = new System.Drawing.Size(460, 6);
            this.splitter.TabIndex = 9;
            this.splitter.TabStop = false;
            // 
            // openFileDialogFMP
            // 
            this.openFileDialogFMP.DefaultExt = "fmp";
            this.openFileDialogFMP.Filter = "Fable Mod Packages (*.fmp)|*.fmp||";
            // 
            // FormModPackage
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(476, 319);
            this.Controls.Add(this.listViewEntries);
            this.Controls.Add(this.splitter);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.panelButtons);
            this.Name = "FormModPackage";
            this.Padding = new System.Windows.Forms.Padding(8);
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Mod Package";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormModPackage_FormClosing);
            this.panelButtons.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewEntries;
        private System.Windows.Forms.Button buttonSaveAs;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.ColumnHeader columnHeader0;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxDesc;
        private System.Windows.Forms.Button buttonApply;
        private System.Windows.Forms.Panel panelButtons;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Splitter splitter;
        private System.Windows.Forms.Button buttonModified;
        private System.Windows.Forms.Button buttonMerge;
        private System.Windows.Forms.OpenFileDialog openFileDialogFMP;
    }
}