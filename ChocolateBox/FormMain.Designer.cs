namespace ChocolateBox
{
    partial class FormMain
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.newModPackageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadModPackageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.saveAndRunFableToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fileListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objectBuilderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateUIDDatabaseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.completeUIDCheckToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panelLeft = new System.Windows.Forms.Panel();
            this.listBoxFiles = new System.Windows.Forms.ListBox();
            this.contextMenuStripFile = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.backupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.rollBackToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitterMain = new System.Windows.Forms.Splitter();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.openFileDialogMod = new System.Windows.Forms.OpenFileDialog();
            this.timerTip = new System.Windows.Forms.Timer(this.components);
            this.menuStripMain.SuspendLayout();
            this.panelLeft.SuspendLayout();
            this.contextMenuStripFile.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.editorsToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.menuStripMain.Size = new System.Drawing.Size(560, 24);
            this.menuStripMain.TabIndex = 1;
            this.menuStripMain.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.toolStripMenuItem1,
            this.newModPackageToolStripMenuItem,
            this.loadModPackageToolStripMenuItem,
            this.toolStripMenuItem2,
            this.saveAndRunFableToolStripMenuItem,
            this.toolStripMenuItem4,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.loadToolStripMenuItem.Text = "&Load...";
            this.loadToolStripMenuItem.ToolTipText = "Load a Fable resource file";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(172, 6);
            // 
            // newModPackageToolStripMenuItem
            // 
            this.newModPackageToolStripMenuItem.Name = "newModPackageToolStripMenuItem";
            this.newModPackageToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.newModPackageToolStripMenuItem.Text = "New Mod Package...";
            this.newModPackageToolStripMenuItem.ToolTipText = "Create a new Fable mod package";
            this.newModPackageToolStripMenuItem.Click += new System.EventHandler(this.newModPackageToolStripMenuItem_Click);
            // 
            // loadModPackageToolStripMenuItem
            // 
            this.loadModPackageToolStripMenuItem.Name = "loadModPackageToolStripMenuItem";
            this.loadModPackageToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.loadModPackageToolStripMenuItem.Text = "Load Mod Package...";
            this.loadModPackageToolStripMenuItem.ToolTipText = "Load an existing Fable mod package";
            this.loadModPackageToolStripMenuItem.Click += new System.EventHandler(this.loadModPackageToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(172, 6);
            // 
            // saveAndRunFableToolStripMenuItem
            // 
            this.saveAndRunFableToolStripMenuItem.Name = "saveAndRunFableToolStripMenuItem";
            this.saveAndRunFableToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.saveAndRunFableToolStripMenuItem.Text = "Save and Run Fable";
            this.saveAndRunFableToolStripMenuItem.ToolTipText = "Save everything, close windows and run Fable";
            this.saveAndRunFableToolStripMenuItem.Click += new System.EventHandler(this.saveAllAndRunFableToolStripMenuItem_Click);
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(172, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
            this.exitToolStripMenuItem.Text = "&Exit";
            this.exitToolStripMenuItem.ToolTipText = "Exit application";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileListToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.viewToolStripMenuItem.Text = "&View";
            // 
            // fileListToolStripMenuItem
            // 
            this.fileListToolStripMenuItem.Checked = true;
            this.fileListToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.fileListToolStripMenuItem.Name = "fileListToolStripMenuItem";
            this.fileListToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.fileListToolStripMenuItem.Text = "File List";
            this.fileListToolStripMenuItem.Click += new System.EventHandler(this.fileListToolStripMenuItem_Click);
            // 
            // editorsToolStripMenuItem
            // 
            this.editorsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.editorToolStripMenuItem,
            this.objectBuilderToolStripMenuItem});
            this.editorsToolStripMenuItem.Name = "editorsToolStripMenuItem";
            this.editorsToolStripMenuItem.Size = new System.Drawing.Size(52, 20);
            this.editorsToolStripMenuItem.Text = "&Editors";
            // 
            // editorToolStripMenuItem
            // 
            this.editorToolStripMenuItem.Name = "editorToolStripMenuItem";
            this.editorToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.editorToolStripMenuItem.Text = "&Region Editor";
            this.editorToolStripMenuItem.ToolTipText = "3D Region Editor";
            this.editorToolStripMenuItem.Click += new System.EventHandler(this.editorToolStripMenuItem_Click);
            // 
            // objectBuilderToolStripMenuItem
            // 
            this.objectBuilderToolStripMenuItem.Name = "objectBuilderToolStripMenuItem";
            this.objectBuilderToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.objectBuilderToolStripMenuItem.Text = "Object Builder";
            this.objectBuilderToolStripMenuItem.ToolTipText = "Create new objects";
            this.objectBuilderToolStripMenuItem.Click += new System.EventHandler(this.itemBuilderToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generateUIDDatabaseToolStripMenuItem,
            this.completeUIDCheckToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.toolsToolStripMenuItem.Text = "Tools";
            // 
            // generateUIDDatabaseToolStripMenuItem
            // 
            this.generateUIDDatabaseToolStripMenuItem.Name = "generateUIDDatabaseToolStripMenuItem";
            this.generateUIDDatabaseToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.generateUIDDatabaseToolStripMenuItem.Text = "Generate UID Database";
            this.generateUIDDatabaseToolStripMenuItem.ToolTipText = "Generate an UID database";
            this.generateUIDDatabaseToolStripMenuItem.Click += new System.EventHandler(this.generateUIDDatabaseToolStripMenuItem_Click);
            // 
            // completeUIDCheckToolStripMenuItem
            // 
            this.completeUIDCheckToolStripMenuItem.Name = "completeUIDCheckToolStripMenuItem";
            this.completeUIDCheckToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
            this.completeUIDCheckToolStripMenuItem.Text = "UID Check";
            this.completeUIDCheckToolStripMenuItem.ToolTipText = "Check all TNG files for duplicate UIDs";
            this.completeUIDCheckToolStripMenuItem.Click += new System.EventHandler(this.completeUIDCheckToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.aboutToolStripMenuItem.Text = "About ChocolateBox";
            this.aboutToolStripMenuItem.ToolTipText = "About ChocolateBox";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // panelLeft
            // 
            this.panelLeft.Controls.Add(this.listBoxFiles);
            this.panelLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelLeft.Location = new System.Drawing.Point(0, 24);
            this.panelLeft.Name = "panelLeft";
            this.panelLeft.Padding = new System.Windows.Forms.Padding(3);
            this.panelLeft.Size = new System.Drawing.Size(174, 374);
            this.panelLeft.TabIndex = 2;
            // 
            // listBoxFiles
            // 
            this.listBoxFiles.ContextMenuStrip = this.contextMenuStripFile;
            this.listBoxFiles.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBoxFiles.FormattingEnabled = true;
            this.listBoxFiles.Location = new System.Drawing.Point(3, 3);
            this.listBoxFiles.Name = "listBoxFiles";
            this.listBoxFiles.Size = new System.Drawing.Size(168, 368);
            this.listBoxFiles.TabIndex = 0;
            this.listBoxFiles.DoubleClick += new System.EventHandler(this.listBoxFiles_DoubleClick);
            // 
            // contextMenuStripFile
            // 
            this.contextMenuStripFile.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.toolStripMenuItem3,
            this.backupToolStripMenuItem,
            this.rollBackToolStripMenuItem});
            this.contextMenuStripFile.Name = "contextMenuStripFile";
            this.contextMenuStripFile.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.contextMenuStripFile.Size = new System.Drawing.Size(114, 76);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(110, 6);
            // 
            // backupToolStripMenuItem
            // 
            this.backupToolStripMenuItem.Name = "backupToolStripMenuItem";
            this.backupToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.backupToolStripMenuItem.Text = "Backup";
            this.backupToolStripMenuItem.Click += new System.EventHandler(this.backupToolStripMenuItem_Click);
            // 
            // rollBackToolStripMenuItem
            // 
            this.rollBackToolStripMenuItem.Name = "rollBackToolStripMenuItem";
            this.rollBackToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.rollBackToolStripMenuItem.Text = "Rollback";
            this.rollBackToolStripMenuItem.Click += new System.EventHandler(this.rollBackToolStripMenuItem_Click);
            // 
            // splitterMain
            // 
            this.splitterMain.Location = new System.Drawing.Point(174, 24);
            this.splitterMain.Name = "splitterMain";
            this.splitterMain.Size = new System.Drawing.Size(6, 374);
            this.splitterMain.TabIndex = 3;
            this.splitterMain.TabStop = false;
            // 
            // openFileDialog
            // 
            this.openFileDialog.Filter = "Fable Files (*.big;*.bin;*.wad;*.wld; *.stb)|*.big;*.bin;*.wld;*.wad; *.tng;*.stb" +
    "||";
            this.openFileDialog.Multiselect = true;
            this.openFileDialog.Title = "Open Fable File...";
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel});
            this.statusStrip.Location = new System.Drawing.Point(0, 398);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(560, 22);
            this.statusStrip.TabIndex = 5;
            this.statusStrip.Text = "Feel the Blaze...!!";
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(94, 17);
            this.toolStripStatusLabel.Text = "Feel the Blaze...!!";
            // 
            // openFileDialogMod
            // 
            this.openFileDialogMod.DefaultExt = "fmp";
            this.openFileDialogMod.Filter = "Fable Mod Packages (*.fmp)|*.fmp|All Files (*.*)|*.*||";
            // 
            // timerTip
            // 
            this.timerTip.Tick += new System.EventHandler(this.timerTip_Tick);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(560, 420);
            this.Controls.Add(this.splitterMain);
            this.Controls.Add(this.panelLeft);
            this.Controls.Add(this.menuStripMain);
            this.Controls.Add(this.statusStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "FormMain";
            this.RegistryKey = "SOFTWARE\\FableMod\\ChocolateBox";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ChocolateBox";
            this.Title = "ChocolateBox";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.Shown += new System.EventHandler(this.FormMain_Shown);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMain_FormClosing);
            this.menuStripMain.ResumeLayout(false);
            this.menuStripMain.PerformLayout();
            this.panelLeft.ResumeLayout(false);
            this.contextMenuStripFile.ResumeLayout(false);
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStripMain;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.Panel panelLeft;
        private System.Windows.Forms.ListBox listBoxFiles;
        private System.Windows.Forms.Splitter splitterMain;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem editorsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileListToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAndRunFableToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newModPackageToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem loadModPackageToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog openFileDialogMod;
        private System.Windows.Forms.Timer timerTip;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generateUIDDatabaseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem completeUIDCheckToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripFile;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem backupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem rollBackToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objectBuilderToolStripMenuItem;
    }
}

