namespace ChocolateBox
{
    partial class FormObjectBuilder
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
            this.comboBoxDefs = new System.Windows.Forms.ComboBox();
            this.panelTop = new System.Windows.Forms.Panel();
            this.comboBoxTemplate = new System.Windows.Forms.ComboBox();
            this.labelTemplate = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.panelBottom = new System.Windows.Forms.Panel();
            this.checkBoxNamed = new System.Windows.Forms.CheckBox();
            this.textBoxName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonCreate = new System.Windows.Forms.Button();
            this.buttonClose = new System.Windows.Forms.Button();
            this.panelTemplate = new System.Windows.Forms.Panel();
            this.panelProgress = new System.Windows.Forms.Panel();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.panelTop.SuspendLayout();
            this.panelBottom.SuspendLayout();
            this.panelProgress.SuspendLayout();
            this.SuspendLayout();
            // 
            // comboBoxDefs
            // 
            this.comboBoxDefs.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxDefs.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBoxDefs.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBoxDefs.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDefs.FormattingEnabled = true;
            this.comboBoxDefs.Location = new System.Drawing.Point(96, 6);
            this.comboBoxDefs.Name = "comboBoxDefs";
            this.comboBoxDefs.Size = new System.Drawing.Size(301, 21);
            this.comboBoxDefs.Sorted = true;
            this.comboBoxDefs.TabIndex = 0;
            this.toolTip.SetToolTip(this.comboBoxDefs, "Select object base type");
            this.comboBoxDefs.DropDownClosed += new System.EventHandler(this.comboBoxDefs_DropDownClosed);
            // 
            // panelTop
            // 
            this.panelTop.Controls.Add(this.comboBoxTemplate);
            this.panelTop.Controls.Add(this.labelTemplate);
            this.panelTop.Controls.Add(this.label1);
            this.panelTop.Controls.Add(this.comboBoxDefs);
            this.panelTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelTop.Location = new System.Drawing.Point(0, 0);
            this.panelTop.Name = "panelTop";
            this.panelTop.Size = new System.Drawing.Size(409, 64);
            this.panelTop.TabIndex = 1;
            // 
            // comboBoxTemplate
            // 
            this.comboBoxTemplate.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxTemplate.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBoxTemplate.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBoxTemplate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTemplate.Enabled = false;
            this.comboBoxTemplate.FormattingEnabled = true;
            this.comboBoxTemplate.Location = new System.Drawing.Point(96, 36);
            this.comboBoxTemplate.Name = "comboBoxTemplate";
            this.comboBoxTemplate.Size = new System.Drawing.Size(301, 21);
            this.comboBoxTemplate.Sorted = true;
            this.comboBoxTemplate.TabIndex = 3;
            this.toolTip.SetToolTip(this.comboBoxTemplate, "Select object template");
            this.comboBoxTemplate.DropDownClosed += new System.EventHandler(this.comboBoxTemplate_DropDownClosed);
            // 
            // labelTemplate
            // 
            this.labelTemplate.AutoSize = true;
            this.labelTemplate.Location = new System.Drawing.Point(12, 39);
            this.labelTemplate.Name = "labelTemplate";
            this.labelTemplate.Size = new System.Drawing.Size(54, 13);
            this.labelTemplate.TabIndex = 2;
            this.labelTemplate.Text = "Template:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "DefinitionType:";
            // 
            // panelBottom
            // 
            this.panelBottom.Controls.Add(this.checkBoxNamed);
            this.panelBottom.Controls.Add(this.textBoxName);
            this.panelBottom.Controls.Add(this.label2);
            this.panelBottom.Controls.Add(this.buttonCreate);
            this.panelBottom.Controls.Add(this.buttonClose);
            this.panelBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelBottom.Location = new System.Drawing.Point(0, 237);
            this.panelBottom.Name = "panelBottom";
            this.panelBottom.Size = new System.Drawing.Size(409, 91);
            this.panelBottom.TabIndex = 2;
            // 
            // checkBoxNamed
            // 
            this.checkBoxNamed.AutoSize = true;
            this.checkBoxNamed.Location = new System.Drawing.Point(15, 6);
            this.checkBoxNamed.Name = "checkBoxNamed";
            this.checkBoxNamed.Size = new System.Drawing.Size(124, 17);
            this.checkBoxNamed.TabIndex = 4;
            this.checkBoxNamed.Text = "CDef Name Override";
            this.checkBoxNamed.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this.toolTip.SetToolTip(this.checkBoxNamed, "Give names to all created CDef objects");
            this.checkBoxNamed.UseVisualStyleBackColor = true;
            // 
            // textBoxName
            // 
            this.textBoxName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxName.Location = new System.Drawing.Point(90, 29);
            this.textBoxName.MaxLength = 48;
            this.textBoxName.Name = "textBoxName";
            this.textBoxName.Size = new System.Drawing.Size(307, 20);
            this.textBoxName.TabIndex = 3;
            this.toolTip.SetToolTip(this.textBoxName, "Base identifier for the created object(s). Names are not checked!");
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 32);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(65, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Base Name:";
            // 
            // buttonCreate
            // 
            this.buttonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCreate.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonCreate.Enabled = false;
            this.buttonCreate.Location = new System.Drawing.Point(241, 55);
            this.buttonCreate.Name = "buttonCreate";
            this.buttonCreate.Size = new System.Drawing.Size(75, 23);
            this.buttonCreate.TabIndex = 1;
            this.buttonCreate.Text = "Create";
            this.toolTip.SetToolTip(this.buttonCreate, "Create the object(s)");
            this.buttonCreate.UseVisualStyleBackColor = true;
            this.buttonCreate.Click += new System.EventHandler(this.buttonOk_Click);
            // 
            // buttonClose
            // 
            this.buttonClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonClose.Location = new System.Drawing.Point(322, 55);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(75, 23);
            this.buttonClose.TabIndex = 0;
            this.buttonClose.Text = "Close";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // panelTemplate
            // 
            this.panelTemplate.AutoScroll = true;
            this.panelTemplate.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panelTemplate.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelTemplate.Location = new System.Drawing.Point(0, 99);
            this.panelTemplate.Name = "panelTemplate";
            this.panelTemplate.Size = new System.Drawing.Size(409, 138);
            this.panelTemplate.TabIndex = 3;
            this.panelTemplate.Visible = false;
            // 
            // panelProgress
            // 
            this.panelProgress.Controls.Add(this.progressBar);
            this.panelProgress.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelProgress.Location = new System.Drawing.Point(0, 64);
            this.panelProgress.Name = "panelProgress";
            this.panelProgress.Size = new System.Drawing.Size(409, 35);
            this.panelProgress.TabIndex = 4;
            this.panelProgress.Visible = false;
            // 
            // progressBar
            // 
            this.progressBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBar.Location = new System.Drawing.Point(15, 6);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(382, 23);
            this.progressBar.TabIndex = 0;
            // 
            // toolTip
            // 
            this.toolTip.IsBalloon = true;
            // 
            // FormObjectBuilder
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(409, 328);
            this.Controls.Add(this.panelTemplate);
            this.Controls.Add(this.panelProgress);
            this.Controls.Add(this.panelTop);
            this.Controls.Add(this.panelBottom);
            this.Name = "FormObjectBuilder";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Object Builder";
            this.Load += new System.EventHandler(this.FormItemBuilder_Load);
            this.panelTop.ResumeLayout(false);
            this.panelTop.PerformLayout();
            this.panelBottom.ResumeLayout(false);
            this.panelBottom.PerformLayout();
            this.panelProgress.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBoxDefs;
        private System.Windows.Forms.Panel panelTop;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panelBottom;
        private System.Windows.Forms.Button buttonCreate;
        private System.Windows.Forms.Button buttonClose;
        private System.Windows.Forms.Panel panelTemplate;
        private System.Windows.Forms.TextBox textBoxName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox checkBoxNamed;
        private System.Windows.Forms.ComboBox comboBoxTemplate;
        private System.Windows.Forms.Label labelTemplate;
        private System.Windows.Forms.Panel panelProgress;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.ToolTip toolTip;
    }
}