using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

using FableMod.Gfx.Integration;

namespace FableMod.Content.Forms
{
	/// <summary>
	/// Summary description for frmOpenTexture.
	/// </summary>
	public class FormOpenTexture : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ComboBox cbFormat;
		private System.Windows.Forms.Label lblFormat;
        private System.Windows.Forms.Label lblFile;
		private System.Windows.Forms.Button btnBrowse;
		private System.Windows.Forms.Button btnOpen;
		private System.Windows.Forms.Button btnCancel;
		public string FileName = null;
		public TextureFormat Format;
        private OpenFileDialog openFileDialog;
        private TextBox textBoxFileName;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public FormOpenTexture()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			cbFormat.Items.Clear();
			cbFormat.Items.AddRange(Enum.GetNames(typeof(TextureFormat)));
			cbFormat.SelectedIndex = 0;
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
            openFileDialog.Filter = GfxTexture.LOAD_FILE_FILTER;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.lblFormat = new System.Windows.Forms.Label();
            this.cbFormat = new System.Windows.Forms.ComboBox();
            this.lblFile = new System.Windows.Forms.Label();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.btnOpen = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.textBoxFileName = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // lblFormat
            // 
            this.lblFormat.AutoSize = true;
            this.lblFormat.Location = new System.Drawing.Point(16, 16);
            this.lblFormat.Name = "lblFormat";
            this.lblFormat.Size = new System.Drawing.Size(74, 13);
            this.lblFormat.TabIndex = 0;
            this.lblFormat.Text = "Import Format:";
            // 
            // cbFormat
            // 
            this.cbFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbFormat.Location = new System.Drawing.Point(96, 12);
            this.cbFormat.Name = "cbFormat";
            this.cbFormat.Size = new System.Drawing.Size(66, 21);
            this.cbFormat.TabIndex = 1;
            // 
            // lblFile
            // 
            this.lblFile.AutoSize = true;
            this.lblFile.Location = new System.Drawing.Point(16, 44);
            this.lblFile.Name = "lblFile";
            this.lblFile.Size = new System.Drawing.Size(52, 13);
            this.lblFile.TabIndex = 2;
            this.lblFile.Text = "Filename:";
            // 
            // btnBrowse
            // 
            this.btnBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnBrowse.Location = new System.Drawing.Point(476, 40);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(32, 21);
            this.btnBrowse.TabIndex = 4;
            this.btnBrowse.Text = "...";
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // btnOpen
            // 
            this.btnOpen.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOpen.Enabled = false;
            this.btnOpen.Location = new System.Drawing.Point(350, 73);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(76, 24);
            this.btnOpen.TabIndex = 5;
            this.btnOpen.Text = "Open";
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.Location = new System.Drawing.Point(432, 73);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(76, 24);
            this.btnCancel.TabIndex = 6;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // textBoxFileName
            // 
            this.textBoxFileName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxFileName.Location = new System.Drawing.Point(96, 41);
            this.textBoxFileName.Name = "textBoxFileName";
            this.textBoxFileName.ReadOnly = true;
            this.textBoxFileName.Size = new System.Drawing.Size(374, 20);
            this.textBoxFileName.TabIndex = 7;
            // 
            // FormOpenTexture
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(520, 109);
            this.Controls.Add(this.textBoxFileName);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOpen);
            this.Controls.Add(this.btnBrowse);
            this.Controls.Add(this.lblFile);
            this.Controls.Add(this.cbFormat);
            this.Controls.Add(this.lblFormat);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormOpenTexture";
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Open Texture";
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private void btnOpen_Click(object sender, System.EventArgs e)
		{
			Format = (TextureFormat)Enum.Parse(
                typeof(TextureFormat), 
                cbFormat.SelectedItem.ToString(), 
                true);
			
            DialogResult = DialogResult.OK;
			
            Close();
		}

		private void btnBrowse_Click(object sender, System.EventArgs e)
		{
			if (openFileDialog.ShowDialog(this) == DialogResult.OK)
			{
                FileName = openFileDialog.FileName;
				textBoxFileName.Text = FileName;
				btnOpen.Enabled = true;
			}
		}

		private void btnCancel_Click(object sender, System.EventArgs e)
		{
			this.DialogResult = DialogResult.Cancel;
			this.Close();
		}
	}
}
