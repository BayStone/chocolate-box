namespace ChocolateBox
{
    partial class FormNewGameBINEntry
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
            this.textBoxName = new System.Windows.Forms.TextBox();
            this.buttonCreate = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBoxTemplates = new System.Windows.Forms.ComboBox();
            this.comboBoxDefs = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // lblSymbolName
            // 
            this.lblSymbolName.AutoSize = true;
            this.lblSymbolName.Location = new System.Drawing.Point(12, 14);
            this.lblSymbolName.Name = "lblSymbolName";
            this.lblSymbolName.Size = new System.Drawing.Size(75, 13);
            this.lblSymbolName.TabIndex = 2;
            this.lblSymbolName.Text = "Symbol Name:";
            // 
            // lblDefinitionType
            // 
            this.lblDefinitionType.AutoSize = true;
            this.lblDefinitionType.Location = new System.Drawing.Point(12, 39);
            this.lblDefinitionType.Name = "lblDefinitionType";
            this.lblDefinitionType.Size = new System.Drawing.Size(81, 13);
            this.lblDefinitionType.TabIndex = 3;
            this.lblDefinitionType.Text = "Definition Type:";
            // 
            // textBoxName
            // 
            this.textBoxName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxName.Location = new System.Drawing.Point(99, 11);
            this.textBoxName.Name = "textBoxName";
            this.textBoxName.Size = new System.Drawing.Size(296, 20);
            this.textBoxName.TabIndex = 0;
            // 
            // buttonCreate
            // 
            this.buttonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCreate.Location = new System.Drawing.Point(239, 93);
            this.buttonCreate.Name = "buttonCreate";
            this.buttonCreate.Size = new System.Drawing.Size(75, 23);
            this.buttonCreate.TabIndex = 3;
            this.buttonCreate.Text = "Create";
            this.buttonCreate.UseVisualStyleBackColor = true;
            this.buttonCreate.Click += new System.EventHandler(this.buttonCreate_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(320, 93);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 4;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 64);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "Template:";
            // 
            // comboBoxTemplates
            // 
            this.comboBoxTemplates.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBoxTemplates.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBoxTemplates.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTemplates.Enabled = false;
            this.comboBoxTemplates.FormattingEnabled = true;
            this.comboBoxTemplates.Location = new System.Drawing.Point(99, 61);
            this.comboBoxTemplates.MaxDropDownItems = 16;
            this.comboBoxTemplates.Name = "comboBoxTemplates";
            this.comboBoxTemplates.Size = new System.Drawing.Size(296, 21);
            this.comboBoxTemplates.Sorted = true;
            this.comboBoxTemplates.TabIndex = 2;
            this.comboBoxTemplates.DropDown += new System.EventHandler(this.comboBoxTemplates_DropDown);
            // 
            // comboBoxDefs
            // 
            this.comboBoxDefs.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBoxDefs.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBoxDefs.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDefs.FormattingEnabled = true;
            this.comboBoxDefs.Location = new System.Drawing.Point(99, 36);
            this.comboBoxDefs.MaxDropDownItems = 16;
            this.comboBoxDefs.Name = "comboBoxDefs";
            this.comboBoxDefs.Size = new System.Drawing.Size(296, 21);
            this.comboBoxDefs.Sorted = true;
            this.comboBoxDefs.TabIndex = 1;
            this.comboBoxDefs.SelectedIndexChanged += new System.EventHandler(this.comboBoxDefs_SelectedIndexChanged);
            // 
            // FormNewGameBINEntry
            // 
            this.AcceptButton = this.buttonCreate;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonCancel;
            this.ClientSize = new System.Drawing.Size(407, 128);
            this.Controls.Add(this.comboBoxDefs);
            this.Controls.Add(this.comboBoxTemplates);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonCreate);
            this.Controls.Add(this.textBoxName);
            this.Controls.Add(this.lblDefinitionType);
            this.Controls.Add(this.lblSymbolName);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormNewGameBINEntry";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "BIN: New Entry";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblSymbolName;
        private System.Windows.Forms.Label lblDefinitionType;
        private System.Windows.Forms.TextBox textBoxName;
        private System.Windows.Forms.Button buttonCreate;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBoxTemplates;
        private System.Windows.Forms.ComboBox comboBoxDefs;
    }
}