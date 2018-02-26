namespace ChocolateBox
{
    partial class FormBIG
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
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findByIDToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findByNameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findModifiedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.addEntryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.menuStripMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // treeView
            // 
            this.treeView.LineColor = System.Drawing.Color.Black;
            this.treeView.Location = new System.Drawing.Point(0, 24);
            this.treeView.Size = new System.Drawing.Size(514, 370);
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.menuStripMain.Size = new System.Drawing.Size(514, 24);
            this.menuStripMain.TabIndex = 1;
            this.menuStripMain.Text = "Main Menu";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveAsToolStripMenuItem,
            this.toolStripMenuItem1,
            this.closeToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveAsToolStripMenuItem.Text = "Save &As...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(149, 6);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.closeToolStripMenuItem.Text = "&Close";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.findByIDToolStripMenuItem,
            this.findByNameToolStripMenuItem,
            this.findModifiedToolStripMenuItem,
            this.toolStripMenuItem2,
            this.addEntryToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.editToolStripMenuItem.Text = "&Edit";
            this.editToolStripMenuItem.Paint += new System.Windows.Forms.PaintEventHandler(this.editToolStripMenuItem_Paint);
            // 
            // findByIDToolStripMenuItem
            // 
            this.findByIDToolStripMenuItem.Name = "findByIDToolStripMenuItem";
            this.findByIDToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.F)));
            this.findByIDToolStripMenuItem.Size = new System.Drawing.Size(191, 22);
            this.findByIDToolStripMenuItem.Text = "Find By &ID";
            this.findByIDToolStripMenuItem.Click += new System.EventHandler(this.findByIDToolStripMenuItem_Click);
            // 
            // findByNameToolStripMenuItem
            // 
            this.findByNameToolStripMenuItem.Name = "findByNameToolStripMenuItem";
            this.findByNameToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F)));
            this.findByNameToolStripMenuItem.Size = new System.Drawing.Size(191, 22);
            this.findByNameToolStripMenuItem.Text = "Find By &Name";
            this.findByNameToolStripMenuItem.Click += new System.EventHandler(this.findByNameToolStripMenuItem_Click);
            // 
            // findModifiedToolStripMenuItem
            // 
            this.findModifiedToolStripMenuItem.Name = "findModifiedToolStripMenuItem";
            this.findModifiedToolStripMenuItem.Size = new System.Drawing.Size(191, 22);
            this.findModifiedToolStripMenuItem.Text = "Find &Modified";
            this.findModifiedToolStripMenuItem.Click += new System.EventHandler(this.findModifiedToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(188, 6);
            // 
            // addEntryToolStripMenuItem
            // 
            this.addEntryToolStripMenuItem.Name = "addEntryToolStripMenuItem";
            this.addEntryToolStripMenuItem.Size = new System.Drawing.Size(191, 22);
            this.addEntryToolStripMenuItem.Text = "&Add Entry";
            this.addEntryToolStripMenuItem.Click += new System.EventHandler(this.addEntryToolStripMenuItem_Click);
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.DefaultExt = "big";
            this.saveFileDialog.Filter = "BIG Files (*.big)|*.big||";
            // 
            // FormBIG
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(514, 394);
            this.Controls.Add(this.menuStripMain);
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "FormBIG";
            this.Text = "BIG";
            this.Controls.SetChildIndex(this.menuStripMain, 0);
            this.Controls.SetChildIndex(this.treeView, 0);
            this.menuStripMain.ResumeLayout(false);
            this.menuStripMain.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.ToolStripMenuItem findByIDToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findByNameToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem findModifiedToolStripMenuItem;
        protected internal System.Windows.Forms.MenuStrip menuStripMain;
        protected System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        protected System.Windows.Forms.ToolStripMenuItem addEntryToolStripMenuItem;

    }
}