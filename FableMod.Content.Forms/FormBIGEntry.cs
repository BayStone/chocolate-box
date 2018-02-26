using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;

using FableMod.BIG;
using FableMod.ContentManagement;

namespace FableMod.Content.Forms
{
	/// <summary>
	/// Summary description for frmBIGEntryView.
	/// </summary>
	public class FormBIGEntry : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Panel pnlStandardView;
		private System.Windows.Forms.Label lblSymbolName;
		private System.Windows.Forms.TextBox txtSymbolName;
		private System.Windows.Forms.Label lblID;
		private System.Windows.Forms.TextBox txtID;
		private System.Windows.Forms.Label lblType;
		private System.Windows.Forms.TextBox txtType;
		private System.Windows.Forms.Label lblDevSources;
		private System.Windows.Forms.TextBox txtDevSources;
		private AssetEntry m_Entry;
		private System.Windows.Forms.Button btnViewSubHeader;
		private System.Windows.Forms.Button btnExport;
		private ControlBIGEntry	becAdvanced;
		private System.Windows.Forms.Button btnApply;
		private System.Windows.Forms.Button btnViewData;
        private Button btnImport;
        private GroupBox gbEntryData;
        private GroupBox gbSubHeader;
        private Button btnImportSubHeader;
        private Button btnExportSubHeader;
        private Splitter splitter;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public FormBIGEntry(AssetEntry entry)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

            m_Entry = entry;
            Text = m_Entry.DevSymbolName;
            txtSymbolName.Text = m_Entry.DevSymbolName;
            txtID.Text = m_Entry.ID.ToString();
            txtType.Text = m_Entry.Type.ToString();
            txtDevSources.Lines = m_Entry.DevSources;

            try
            {
                AssetArchive arc = m_Entry.Archive;

                if (arc.GetType() == typeof(BIGFile))
                {
                    uint type = m_Entry.Type;
                    int content = arc.ContentType;

                    if ((content == 0x47 || 
                         content == 0x2B || 
                         content == 0x4B) && 
                        (type == 0x0 || 
                         type == 0x1 || 
                         type == 0x2))
                    {
                        becAdvanced = new ControlTexture();
                        
                    }
                    else if ((content == 0x49) && 
                             (type == 0x1 || 
                              type == 0x2 || 
                              type == 0x4 || 
                              type == 0x5))
                    {
                        becAdvanced = new ControlModel();
                    }
                    else if (content == 0x49 && type == 0x3)
                    {
                        becAdvanced = new ControlPhysicsModel();
                    }
                    else if ((content == 0x2A) && (type == 0x0))
                    {
                        becAdvanced = new ControlText();
                    }
                    else if ((content == 0x2A) && (type == 0x1))
                    {
                        becAdvanced = new ControlTextGroup();
                    }

                    if (becAdvanced != null)
                    {
                        // Adjust window appropriately.

                        becAdvanced.BIGEntry = m_Entry;

                        if (ClientSize.Width < becAdvanced.Width)
                        {
                            ClientSize = new Size(
                                becAdvanced.Width,
                                pnlStandardView.Height+
                                becAdvanced.Height);
                        }
                        else
                        {
                            ClientSize = new Size(
                                ClientSize.Width, 
                                pnlStandardView.Height+
                                becAdvanced.Height);
                        }

                        becAdvanced.Dock = DockStyle.Fill;

                        this.Controls.Add(becAdvanced);

                        becAdvanced.BringToFront();

                        /*
                        becAdvanced.Anchor = 
                            AnchorStyles.Top |
                            AnchorStyles.Left |
                            AnchorStyles.Right |
                            AnchorStyles.Bottom;
                         */
                    }
                    else
                    {
                        throw new Exception(string.Format(
                            "Unknown content type: {0}:{1:X}:{2:X}",
                            arc.OriginalFileName,
                            content, 
                            type));
                    }

                        /*
                    
                    else if ((ent.Archive.ContentType == 0x041b))
                    {
                        becAdvanced = new ucBIGShader();
                        becAdvanced.Location = new Point(0, pnlStandardView.Bottom);
                        becAdvanced.BIGEntry = ent;
                        this.Controls.Add(becAdvanced);
                        if (this.ClientSize.Width < becAdvanced.Width)
                            this.ClientSize = new Size(becAdvanced.Width, becAdvanced.Bottom + 10);
                        else
                            this.ClientSize = new Size(this.ClientSize.Width, becAdvanced.Bottom + 10);
                    }
                    else
                    {
                        this.ClientSize = pnlStandardView.Size;
                    }
                    */
                }
            }
            catch (Exception ex)
            {
                becAdvanced = null;
                
                FableMod.Forms.Messages.Error(ex);
                
                this.ClientSize = pnlStandardView.Size;
            }

			Invalidate(true);
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
            this.pnlStandardView = new System.Windows.Forms.Panel();
            this.gbEntryData = new System.Windows.Forms.GroupBox();
            this.btnExport = new System.Windows.Forms.Button();
            this.btnViewData = new System.Windows.Forms.Button();
            this.btnImport = new System.Windows.Forms.Button();
            this.gbSubHeader = new System.Windows.Forms.GroupBox();
            this.btnImportSubHeader = new System.Windows.Forms.Button();
            this.btnExportSubHeader = new System.Windows.Forms.Button();
            this.btnViewSubHeader = new System.Windows.Forms.Button();
            this.btnApply = new System.Windows.Forms.Button();
            this.txtDevSources = new System.Windows.Forms.TextBox();
            this.lblDevSources = new System.Windows.Forms.Label();
            this.txtType = new System.Windows.Forms.TextBox();
            this.lblType = new System.Windows.Forms.Label();
            this.txtID = new System.Windows.Forms.TextBox();
            this.lblID = new System.Windows.Forms.Label();
            this.txtSymbolName = new System.Windows.Forms.TextBox();
            this.lblSymbolName = new System.Windows.Forms.Label();
            this.splitter = new System.Windows.Forms.Splitter();
            this.pnlStandardView.SuspendLayout();
            this.gbEntryData.SuspendLayout();
            this.gbSubHeader.SuspendLayout();
            this.SuspendLayout();
            // 
            // pnlStandardView
            // 
            this.pnlStandardView.Controls.Add(this.gbEntryData);
            this.pnlStandardView.Controls.Add(this.gbSubHeader);
            this.pnlStandardView.Controls.Add(this.btnApply);
            this.pnlStandardView.Controls.Add(this.txtDevSources);
            this.pnlStandardView.Controls.Add(this.lblDevSources);
            this.pnlStandardView.Controls.Add(this.txtType);
            this.pnlStandardView.Controls.Add(this.lblType);
            this.pnlStandardView.Controls.Add(this.txtID);
            this.pnlStandardView.Controls.Add(this.lblID);
            this.pnlStandardView.Controls.Add(this.txtSymbolName);
            this.pnlStandardView.Controls.Add(this.lblSymbolName);
            this.pnlStandardView.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlStandardView.Location = new System.Drawing.Point(0, 0);
            this.pnlStandardView.Name = "pnlStandardView";
            this.pnlStandardView.Size = new System.Drawing.Size(350, 219);
            this.pnlStandardView.TabIndex = 0;
            // 
            // gbEntryData
            // 
            this.gbEntryData.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gbEntryData.Controls.Add(this.btnExport);
            this.gbEntryData.Controls.Add(this.btnViewData);
            this.gbEntryData.Controls.Add(this.btnImport);
            this.gbEntryData.Location = new System.Drawing.Point(12, 138);
            this.gbEntryData.Name = "gbEntryData";
            this.gbEntryData.Size = new System.Drawing.Size(326, 44);
            this.gbEntryData.TabIndex = 15;
            this.gbEntryData.TabStop = false;
            this.gbEntryData.Text = "Entry Data";
            // 
            // btnExport
            // 
            this.btnExport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnExport.Location = new System.Drawing.Point(114, 14);
            this.btnExport.Name = "btnExport";
            this.btnExport.Size = new System.Drawing.Size(100, 24);
            this.btnExport.TabIndex = 9;
            this.btnExport.Text = "Export";
            this.btnExport.Click += new System.EventHandler(this.btnExport_Click);
            // 
            // btnViewData
            // 
            this.btnViewData.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnViewData.Location = new System.Drawing.Point(8, 14);
            this.btnViewData.Name = "btnViewData";
            this.btnViewData.Size = new System.Drawing.Size(100, 24);
            this.btnViewData.TabIndex = 11;
            this.btnViewData.Text = "View Data";
            this.btnViewData.Click += new System.EventHandler(this.btnViewData_Click);
            // 
            // btnImport
            // 
            this.btnImport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnImport.Location = new System.Drawing.Point(220, 14);
            this.btnImport.Name = "btnImport";
            this.btnImport.Size = new System.Drawing.Size(100, 24);
            this.btnImport.TabIndex = 12;
            this.btnImport.Text = "Import";
            this.btnImport.Click += new System.EventHandler(this.btnImport_Click);
            // 
            // gbSubHeader
            // 
            this.gbSubHeader.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gbSubHeader.Controls.Add(this.btnImportSubHeader);
            this.gbSubHeader.Controls.Add(this.btnExportSubHeader);
            this.gbSubHeader.Controls.Add(this.btnViewSubHeader);
            this.gbSubHeader.Location = new System.Drawing.Point(12, 88);
            this.gbSubHeader.Name = "gbSubHeader";
            this.gbSubHeader.Size = new System.Drawing.Size(326, 44);
            this.gbSubHeader.TabIndex = 13;
            this.gbSubHeader.TabStop = false;
            this.gbSubHeader.Text = "Sub Header";
            // 
            // btnImportSubHeader
            // 
            this.btnImportSubHeader.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnImportSubHeader.Location = new System.Drawing.Point(220, 14);
            this.btnImportSubHeader.Name = "btnImportSubHeader";
            this.btnImportSubHeader.Size = new System.Drawing.Size(100, 24);
            this.btnImportSubHeader.TabIndex = 14;
            this.btnImportSubHeader.Text = "Import";
            this.btnImportSubHeader.Click += new System.EventHandler(this.btnImportSubHeader_Click);
            // 
            // btnExportSubHeader
            // 
            this.btnExportSubHeader.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnExportSubHeader.Location = new System.Drawing.Point(114, 14);
            this.btnExportSubHeader.Name = "btnExportSubHeader";
            this.btnExportSubHeader.Size = new System.Drawing.Size(100, 24);
            this.btnExportSubHeader.TabIndex = 14;
            this.btnExportSubHeader.Text = "Export";
            this.btnExportSubHeader.Click += new System.EventHandler(this.btnExportSubHeader_Click);
            // 
            // btnViewSubHeader
            // 
            this.btnViewSubHeader.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnViewSubHeader.Location = new System.Drawing.Point(8, 14);
            this.btnViewSubHeader.Name = "btnViewSubHeader";
            this.btnViewSubHeader.Size = new System.Drawing.Size(100, 24);
            this.btnViewSubHeader.TabIndex = 8;
            this.btnViewSubHeader.Text = "View Sub Header";
            this.btnViewSubHeader.Click += new System.EventHandler(this.btnViewSubHeader_Click);
            // 
            // btnApply
            // 
            this.btnApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnApply.Location = new System.Drawing.Point(232, 188);
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(100, 24);
            this.btnApply.TabIndex = 10;
            this.btnApply.Text = "Apply Changes";
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // txtDevSources
            // 
            this.txtDevSources.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtDevSources.Location = new System.Drawing.Point(226, 36);
            this.txtDevSources.Multiline = true;
            this.txtDevSources.Name = "txtDevSources";
            this.txtDevSources.Size = new System.Drawing.Size(112, 46);
            this.txtDevSources.TabIndex = 7;
            this.txtDevSources.WordWrap = false;
            // 
            // lblDevSources
            // 
            this.lblDevSources.AutoSize = true;
            this.lblDevSources.Location = new System.Drawing.Point(148, 36);
            this.lblDevSources.Name = "lblDevSources";
            this.lblDevSources.Size = new System.Drawing.Size(72, 13);
            this.lblDevSources.TabIndex = 6;
            this.lblDevSources.Text = "Dev Sources:";
            // 
            // txtType
            // 
            this.txtType.Location = new System.Drawing.Point(90, 62);
            this.txtType.Name = "txtType";
            this.txtType.Size = new System.Drawing.Size(52, 20);
            this.txtType.TabIndex = 5;
            // 
            // lblType
            // 
            this.lblType.AutoSize = true;
            this.lblType.Location = new System.Drawing.Point(50, 65);
            this.lblType.Name = "lblType";
            this.lblType.Size = new System.Drawing.Size(34, 13);
            this.lblType.TabIndex = 4;
            this.lblType.Text = "Type:";
            // 
            // txtID
            // 
            this.txtID.Location = new System.Drawing.Point(90, 36);
            this.txtID.Name = "txtID";
            this.txtID.Size = new System.Drawing.Size(52, 20);
            this.txtID.TabIndex = 3;
            // 
            // lblID
            // 
            this.lblID.AutoSize = true;
            this.lblID.Location = new System.Drawing.Point(63, 38);
            this.lblID.Name = "lblID";
            this.lblID.Size = new System.Drawing.Size(21, 13);
            this.lblID.TabIndex = 2;
            this.lblID.Text = "ID:";
            // 
            // txtSymbolName
            // 
            this.txtSymbolName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtSymbolName.Location = new System.Drawing.Point(90, 8);
            this.txtSymbolName.Name = "txtSymbolName";
            this.txtSymbolName.Size = new System.Drawing.Size(248, 20);
            this.txtSymbolName.TabIndex = 1;
            // 
            // lblSymbolName
            // 
            this.lblSymbolName.AutoSize = true;
            this.lblSymbolName.Location = new System.Drawing.Point(9, 11);
            this.lblSymbolName.Name = "lblSymbolName";
            this.lblSymbolName.Size = new System.Drawing.Size(75, 13);
            this.lblSymbolName.TabIndex = 0;
            this.lblSymbolName.Text = "Symbol Name:";
            // 
            // splitter
            // 
            this.splitter.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitter.Dock = System.Windows.Forms.DockStyle.Top;
            this.splitter.Location = new System.Drawing.Point(0, 219);
            this.splitter.Name = "splitter";
            this.splitter.Size = new System.Drawing.Size(350, 8);
            this.splitter.TabIndex = 1;
            this.splitter.TabStop = false;
            // 
            // FormBIGEntry
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(350, 280);
            this.Controls.Add(this.splitter);
            this.Controls.Add(this.pnlStandardView);
            this.MinimumSize = new System.Drawing.Size(358, 0);
            this.Name = "FormBIGEntry";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "BIG Entry";
            this.Activated += new System.EventHandler(this.FormBIGEntry_Activated);
            this.Deactivate += new System.EventHandler(this.FormBIGEntry_Deactivate);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormBIGEntry_FormClosing);
            this.pnlStandardView.ResumeLayout(false);
            this.pnlStandardView.PerformLayout();
            this.gbEntryData.ResumeLayout(false);
            this.gbSubHeader.ResumeLayout(false);
            this.ResumeLayout(false);

		}
		#endregion

		private void btnViewSubHeader_Click(object sender, System.EventArgs e)
		{
            uint type = m_Entry.Type;
            int content = m_Entry.Archive.ContentType;

            if (content == 0x49 && type == 0x3)
                return;
            
            FormDataViewer form = new FormDataViewer(m_Entry.SubHeader);
            form.MdiParent = this.MdiParent;
            form.Show();
		}

        private void btnExport_Click(object sender, System.EventArgs e)
        {
            SaveFileDialog sf = new SaveFileDialog();

            if (sf.ShowDialog() == DialogResult.OK)
            {
                FileStream f = File.Create(sf.FileName);
                
                byte[] data = m_Entry.Data;
                
                if (data[data.Length-3] == 0x11 && 
                    data[data.Length-2] == 0x00 && 
                    data[data.Length-1] == 0x00)
                {
                    byte[] decomp;

                    try
                    {
                        decomp = FableMod.Data.LZO.DecompressRaw(
                            data, 4, data.Length-4);
                        
                        if (MessageBox.Show(this, 
                            "This file seems to be entirely lzo compressed."+
                            "Would you like to decompress it before saving to file?", 
                            "LZO Compression Found", 
                            MessageBoxButtons.YesNo) == DialogResult.Yes)
                        {
                            f.Write(decomp, 0, decomp.Length);
                            f.Close();
                            return;
                        }

                    }
                    catch (Exception)
                    {
                    }
                }

                f.Write(data, 0, data.Length);
                f.Close();
            }
        }

		private void btnApply_Click(object sender, System.EventArgs e)
		{
			m_Entry.DevSymbolName = txtSymbolName.Text;
			m_Entry.DevSources = txtDevSources.Lines;

			try
			{
				m_Entry.Type = uint.Parse(txtType.Text);
			}
			catch (Exception)
			{
				MessageBox.Show(
                    this, 
                    "Type entered was not a valid type number.", 
                    "Invalid Type");
			}

			try
			{

				m_Entry.ID = uint.Parse(txtID.Text);
			}
			catch (Exception)
			{
				MessageBox.Show(this, 
                    "ID entered was not a valid ID number.", 
                    "Invalid ID");
			}

			if (becAdvanced != null)
				becAdvanced.ApplyChanges();
		}

		private void btnViewData_Click(object sender, System.EventArgs e)
		{
            FormDataViewer form = new FormDataViewer(m_Entry.Data);	
			form.MdiParent = this.MdiParent;
			form.Show();
		}

        private void btnImport_Click(object sender, EventArgs e)
        {
			OpenFileDialog of = new OpenFileDialog();

            of.InitialDirectory = ContentManager.Instance.DataDirectory;

            if (of.ShowDialog() == DialogResult.OK)
            {
                FileStream f = File.OpenRead(of.FileName);
                byte[] data = new byte[f.Length];
                f.Read(data, 0, (int)f.Length);
                m_Entry.Data = data;
            }
        }

        private void btnExportSubHeader_Click(object sender, EventArgs e)
        {
            SaveFileDialog sf = new SaveFileDialog();

            sf.InitialDirectory = ContentManager.Instance.DataDirectory;

            if (sf.ShowDialog() == DialogResult.OK)
            {
                FileStream f = File.Create(sf.FileName);
                f.Write(m_Entry.SubHeader, 0, m_Entry.SubHeader.Length);
                f.Close();
            }
        }

        private void btnImportSubHeader_Click(object sender, EventArgs e)
        {
            OpenFileDialog of = new OpenFileDialog();

            of.InitialDirectory = ContentManager.Instance.DataDirectory;

            if (of.ShowDialog() == DialogResult.OK)
            {
                FileStream f = File.OpenRead(of.FileName);
                byte[] data = new byte[f.Length];
                f.Read(data, 0, (int)f.Length);
                m_Entry.SubHeader = data;
            }
        }

        private void FormBIGEntry_Deactivate(object sender, EventArgs e)
        {
            if (becAdvanced != null)
                becAdvanced.OnDeactivate();
        }

        private void FormBIGEntry_Activated(object sender, EventArgs e)
        {
            if (becAdvanced != null)
                becAdvanced.OnActivate();
        }

        private void FormBIGEntry_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (false)
            {
                DialogResult Result = MessageBox.Show(
                    Text,
                    "Object has been modified. Apply changes?",
                    MessageBoxButtons.YesNoCancel,
                    MessageBoxIcon.Exclamation);

                if (Result == DialogResult.Yes)
                {
                    btnApply_Click(null, null);
                }
                else
                {
                    e.Cancel = true;
                }
            }
        }
	}
}
