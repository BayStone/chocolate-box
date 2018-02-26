using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

using FableMod.ContentManagement;
using FableMod.BIG;
using FableMod.Gfx.Integration;

namespace FableMod.Content.Forms
{
	/// <summary>
	/// Summary description for ucBIGTexture.
	/// </summary>
	public class ControlTexture : ControlBIGEntry
	{
		private System.Windows.Forms.Label lblWidth;
		private System.Windows.Forms.TextBox txtWidth;
		private System.Windows.Forms.TextBox txtHeight;
		private System.Windows.Forms.Label lblHeight;
		private System.Windows.Forms.TextBox txtDepth;
		private System.Windows.Forms.Label lblDepth;
		private System.Windows.Forms.TextBox txtFrameWidth;
		private System.Windows.Forms.Label lblFrameWidth;
		private System.Windows.Forms.TextBox txtFrameHeight;
		private System.Windows.Forms.Label lblFrameHeight;
		private System.Windows.Forms.TextBox txtFrameCount;
		private System.Windows.Forms.Label lblFrameCount;
		private System.Windows.Forms.Label lblPreview;
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.Button btnSave;
		private System.Windows.Forms.Label lblCurrentFrame;
		private System.Windows.Forms.NumericUpDown nudCurrentFrame;
		private System.Windows.Forms.Button btnOpenNew;
		private System.Windows.Forms.Button btnAddFrame;
		private System.Windows.Forms.Label lblAlpha;
		private System.Windows.Forms.NumericUpDown nudAlpha;
        private OpenFileDialog openFileDialog;
        private SaveFileDialog saveFileDialog;
        private Panel panelTexture;

		private GfxTexture m_Texture;

        public ControlTexture()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			//this.SetStyle(ControlStyles.DoubleBuffer, true);
			this.SetStyle(ControlStyles.ResizeRedraw, true);
			this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
			this.SetStyle(ControlStyles.UserPaint, true);

            openFileDialog.Filter = GfxTexture.LOAD_FILE_FILTER;
            saveFileDialog.Filter = GfxTexture.SAVE_FILE_FILTER;
		}

		public override void ApplyChanges()
		{
			try
			{
				m_Texture.FrameWidth = int.Parse(txtFrameWidth.Text);
			}
			catch (Exception)
			{
				MessageBox.Show(this, 
                    "Frame Width entered was not a valid frame width.", 
                    "Invalid Frame Width");
			}

			try
			{
				m_Texture.FrameHeight = int.Parse(txtFrameHeight.Text);
			}
			catch (Exception)
			{
				MessageBox.Show(this, 
                    "Frame Height entered was not a valid frame height.", 
                    "Invalid Frame Height");
			}

			m_Texture.AlphaChannels = (byte)nudAlpha.Value;

			m_Texture.ApplyToEntry(m_Entry);
		}
		private void RefreshDisplay()
		{
			if (m_Texture != null)
			{
				txtWidth.Text = m_Texture.Width.ToString();
				txtHeight.Text = m_Texture.Height.ToString();
				txtDepth.Text = m_Texture.Depth.ToString();
				txtFrameCount.Text = m_Texture.FrameCount.ToString();
				txtFrameWidth.Text = m_Texture.FrameWidth.ToString();
				txtFrameHeight.Text = m_Texture.FrameHeight.ToString();
				nudAlpha.Value = (decimal)m_Texture.AlphaChannels;

				EntryType = "Texture - " + m_Texture.Format.ToString();

                /*
				if (m_Texture.Width + 12 > this.Width)
					this.Width = m_Texture.Width + 24;

				this.Height = m_Texture.Height + 120;
                */

				nudCurrentFrame.Maximum = m_Texture.FrameCount-1;
			}

            Invalidate();
		}

		public override AssetEntry BIGEntry
		{
			set
			{
				m_Entry = value;
				m_Texture = new GfxTexture(m_Entry);
				RefreshDisplay();
				this.Invalidate(true);
			}
		}

		protected override void OnPaintBackground(PaintEventArgs pevent)
		{
			base.OnPaintBackground (pevent);

            if (m_Texture != null)
            {
                m_Texture.Draw(
                    panelTexture,
                    panelTexture.ClientRectangle,
                   (int)nudCurrentFrame.Value);
            }
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

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.lblWidth = new System.Windows.Forms.Label();
            this.txtWidth = new System.Windows.Forms.TextBox();
            this.txtHeight = new System.Windows.Forms.TextBox();
            this.lblHeight = new System.Windows.Forms.Label();
            this.txtDepth = new System.Windows.Forms.TextBox();
            this.lblDepth = new System.Windows.Forms.Label();
            this.txtFrameWidth = new System.Windows.Forms.TextBox();
            this.lblFrameWidth = new System.Windows.Forms.Label();
            this.txtFrameHeight = new System.Windows.Forms.TextBox();
            this.lblFrameHeight = new System.Windows.Forms.Label();
            this.txtFrameCount = new System.Windows.Forms.TextBox();
            this.lblFrameCount = new System.Windows.Forms.Label();
            this.lblPreview = new System.Windows.Forms.Label();
            this.btnSave = new System.Windows.Forms.Button();
            this.nudCurrentFrame = new System.Windows.Forms.NumericUpDown();
            this.lblCurrentFrame = new System.Windows.Forms.Label();
            this.btnOpenNew = new System.Windows.Forms.Button();
            this.btnAddFrame = new System.Windows.Forms.Button();
            this.lblAlpha = new System.Windows.Forms.Label();
            this.nudAlpha = new System.Windows.Forms.NumericUpDown();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.panelTexture = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.nudCurrentFrame)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudAlpha)).BeginInit();
            this.SuspendLayout();
            // 
            // lblWidth
            // 
            this.lblWidth.Location = new System.Drawing.Point(12, 32);
            this.lblWidth.Name = "lblWidth";
            this.lblWidth.Size = new System.Drawing.Size(40, 16);
            this.lblWidth.TabIndex = 2;
            this.lblWidth.Text = "Width:";
            // 
            // txtWidth
            // 
            this.txtWidth.Location = new System.Drawing.Point(48, 28);
            this.txtWidth.Name = "txtWidth";
            this.txtWidth.ReadOnly = true;
            this.txtWidth.Size = new System.Drawing.Size(44, 20);
            this.txtWidth.TabIndex = 3;
            // 
            // txtHeight
            // 
            this.txtHeight.Location = new System.Drawing.Point(48, 52);
            this.txtHeight.Name = "txtHeight";
            this.txtHeight.ReadOnly = true;
            this.txtHeight.Size = new System.Drawing.Size(44, 20);
            this.txtHeight.TabIndex = 5;
            // 
            // lblHeight
            // 
            this.lblHeight.Location = new System.Drawing.Point(12, 56);
            this.lblHeight.Name = "lblHeight";
            this.lblHeight.Size = new System.Drawing.Size(40, 16);
            this.lblHeight.TabIndex = 4;
            this.lblHeight.Text = "Height:";
            // 
            // txtDepth
            // 
            this.txtDepth.Location = new System.Drawing.Point(48, 76);
            this.txtDepth.Name = "txtDepth";
            this.txtDepth.ReadOnly = true;
            this.txtDepth.Size = new System.Drawing.Size(44, 20);
            this.txtDepth.TabIndex = 7;
            // 
            // lblDepth
            // 
            this.lblDepth.Location = new System.Drawing.Point(12, 80);
            this.lblDepth.Name = "lblDepth";
            this.lblDepth.Size = new System.Drawing.Size(40, 16);
            this.lblDepth.TabIndex = 6;
            this.lblDepth.Text = "Depth:";
            // 
            // txtFrameWidth
            // 
            this.txtFrameWidth.Location = new System.Drawing.Point(168, 28);
            this.txtFrameWidth.Name = "txtFrameWidth";
            this.txtFrameWidth.Size = new System.Drawing.Size(44, 20);
            this.txtFrameWidth.TabIndex = 9;
            // 
            // lblFrameWidth
            // 
            this.lblFrameWidth.Location = new System.Drawing.Point(100, 32);
            this.lblFrameWidth.Name = "lblFrameWidth";
            this.lblFrameWidth.Size = new System.Drawing.Size(72, 16);
            this.lblFrameWidth.TabIndex = 8;
            this.lblFrameWidth.Text = "Frame Width:";
            // 
            // txtFrameHeight
            // 
            this.txtFrameHeight.Location = new System.Drawing.Point(168, 52);
            this.txtFrameHeight.Name = "txtFrameHeight";
            this.txtFrameHeight.Size = new System.Drawing.Size(44, 20);
            this.txtFrameHeight.TabIndex = 11;
            // 
            // lblFrameHeight
            // 
            this.lblFrameHeight.Location = new System.Drawing.Point(96, 56);
            this.lblFrameHeight.Name = "lblFrameHeight";
            this.lblFrameHeight.Size = new System.Drawing.Size(76, 16);
            this.lblFrameHeight.TabIndex = 10;
            this.lblFrameHeight.Text = "Frame Height:";
            // 
            // txtFrameCount
            // 
            this.txtFrameCount.Location = new System.Drawing.Point(168, 76);
            this.txtFrameCount.Name = "txtFrameCount";
            this.txtFrameCount.ReadOnly = true;
            this.txtFrameCount.Size = new System.Drawing.Size(44, 20);
            this.txtFrameCount.TabIndex = 13;
            // 
            // lblFrameCount
            // 
            this.lblFrameCount.Location = new System.Drawing.Point(100, 80);
            this.lblFrameCount.Name = "lblFrameCount";
            this.lblFrameCount.Size = new System.Drawing.Size(76, 16);
            this.lblFrameCount.TabIndex = 14;
            this.lblFrameCount.Text = "Frame Count:";
            // 
            // lblPreview
            // 
            this.lblPreview.Location = new System.Drawing.Point(12, 104);
            this.lblPreview.Name = "lblPreview";
            this.lblPreview.Size = new System.Drawing.Size(48, 16);
            this.lblPreview.TabIndex = 15;
            this.lblPreview.Text = "Preview:";
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(312, 52);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(92, 26);
            this.btnSave.TabIndex = 16;
            this.btnSave.Text = "Save As...";
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // nudCurrentFrame
            // 
            this.nudCurrentFrame.Location = new System.Drawing.Point(128, 100);
            this.nudCurrentFrame.Maximum = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.nudCurrentFrame.Name = "nudCurrentFrame";
            this.nudCurrentFrame.Size = new System.Drawing.Size(36, 20);
            this.nudCurrentFrame.TabIndex = 19;
            this.nudCurrentFrame.ValueChanged += new System.EventHandler(this.nudCurrentFrame_ValueChanged);
            // 
            // lblCurrentFrame
            // 
            this.lblCurrentFrame.Location = new System.Drawing.Point(88, 104);
            this.lblCurrentFrame.Name = "lblCurrentFrame";
            this.lblCurrentFrame.Size = new System.Drawing.Size(44, 16);
            this.lblCurrentFrame.TabIndex = 18;
            this.lblCurrentFrame.Text = "Frame:";
            // 
            // btnOpenNew
            // 
            this.btnOpenNew.Location = new System.Drawing.Point(312, 20);
            this.btnOpenNew.Name = "btnOpenNew";
            this.btnOpenNew.Size = new System.Drawing.Size(92, 26);
            this.btnOpenNew.TabIndex = 20;
            this.btnOpenNew.Text = "Open...";
            this.btnOpenNew.Click += new System.EventHandler(this.btnOpenNew_Click);
            // 
            // btnAddFrame
            // 
            this.btnAddFrame.Location = new System.Drawing.Point(312, 84);
            this.btnAddFrame.Name = "btnAddFrame";
            this.btnAddFrame.Size = new System.Drawing.Size(92, 26);
            this.btnAddFrame.TabIndex = 21;
            this.btnAddFrame.Text = "Add Frame...";
            this.btnAddFrame.Click += new System.EventHandler(this.btnAddFrame_Click);
            // 
            // lblAlpha
            // 
            this.lblAlpha.Location = new System.Drawing.Point(220, 32);
            this.lblAlpha.Name = "lblAlpha";
            this.lblAlpha.Size = new System.Drawing.Size(40, 16);
            this.lblAlpha.TabIndex = 22;
            this.lblAlpha.Text = "Alpha:";
            // 
            // nudAlpha
            // 
            this.nudAlpha.Location = new System.Drawing.Point(264, 28);
            this.nudAlpha.Maximum = new decimal(new int[] {
            4,
            0,
            0,
            0});
            this.nudAlpha.Name = "nudAlpha";
            this.nudAlpha.Size = new System.Drawing.Size(36, 20);
            this.nudAlpha.TabIndex = 23;
            this.nudAlpha.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // panelTexture
            // 
            this.panelTexture.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelTexture.BackColor = System.Drawing.SystemColors.Control;
            this.panelTexture.Location = new System.Drawing.Point(11, 126);
            this.panelTexture.Name = "panelTexture";
            this.panelTexture.Size = new System.Drawing.Size(393, 201);
            this.panelTexture.TabIndex = 24;
            // 
            // ControlTexture
            // 
            this.Controls.Add(this.panelTexture);
            this.Controls.Add(this.nudAlpha);
            this.Controls.Add(this.lblAlpha);
            this.Controls.Add(this.btnAddFrame);
            this.Controls.Add(this.btnOpenNew);
            this.Controls.Add(this.nudCurrentFrame);
            this.Controls.Add(this.lblCurrentFrame);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.lblPreview);
            this.Controls.Add(this.txtFrameCount);
            this.Controls.Add(this.lblFrameCount);
            this.Controls.Add(this.txtFrameHeight);
            this.Controls.Add(this.lblFrameHeight);
            this.Controls.Add(this.txtFrameWidth);
            this.Controls.Add(this.lblFrameWidth);
            this.Controls.Add(this.txtDepth);
            this.Controls.Add(this.lblDepth);
            this.Controls.Add(this.txtHeight);
            this.Controls.Add(this.lblHeight);
            this.Controls.Add(this.txtWidth);
            this.Controls.Add(this.lblWidth);
            this.EntryType = "Texture";
            this.Name = "ControlTexture";
            this.Size = new System.Drawing.Size(416, 330);
            this.Controls.SetChildIndex(this.lblWidth, 0);
            this.Controls.SetChildIndex(this.txtWidth, 0);
            this.Controls.SetChildIndex(this.lblHeight, 0);
            this.Controls.SetChildIndex(this.txtHeight, 0);
            this.Controls.SetChildIndex(this.lblDepth, 0);
            this.Controls.SetChildIndex(this.txtDepth, 0);
            this.Controls.SetChildIndex(this.lblFrameWidth, 0);
            this.Controls.SetChildIndex(this.txtFrameWidth, 0);
            this.Controls.SetChildIndex(this.lblFrameHeight, 0);
            this.Controls.SetChildIndex(this.txtFrameHeight, 0);
            this.Controls.SetChildIndex(this.lblFrameCount, 0);
            this.Controls.SetChildIndex(this.txtFrameCount, 0);
            this.Controls.SetChildIndex(this.lblPreview, 0);
            this.Controls.SetChildIndex(this.btnSave, 0);
            this.Controls.SetChildIndex(this.lblCurrentFrame, 0);
            this.Controls.SetChildIndex(this.nudCurrentFrame, 0);
            this.Controls.SetChildIndex(this.btnOpenNew, 0);
            this.Controls.SetChildIndex(this.btnAddFrame, 0);
            this.Controls.SetChildIndex(this.lblAlpha, 0);
            this.Controls.SetChildIndex(this.nudAlpha, 0);
            this.Controls.SetChildIndex(this.panelTexture, 0);
            ((System.ComponentModel.ISupportInitialize)(this.nudCurrentFrame)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudAlpha)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private void btnSave_Click(object sender, System.EventArgs e)
		{
			if (m_Texture != null)
			{
				if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
				{
					m_Texture.Save(
                        saveFileDialog.FileName,
                        (int)nudCurrentFrame.Value);
				}
			}
		}

		private void nudCurrentFrame_ValueChanged(object sender, System.EventArgs e)
		{
			this.Invalidate();
		}

		private void btnOpenNew_Click(object sender, System.EventArgs e)
		{
            FormOpenTexture form = new FormOpenTexture();

            if (form.ShowDialog(this) == DialogResult.OK)
			{
				GfxTexture t;

				try 
				{
                    t = new GfxTexture(form.FileName, form.Format);
				}
				catch (Exception ex)
				{
                    MessageBox.Show(this, ex.Message);
                    return;
				}
				
                m_Texture = t;

				RefreshDisplay();
			}

            form.Dispose();
		}

		private void btnAddFrame_Click(object sender, System.EventArgs e)
		{
            openFileDialog.InitialDirectory =
                ContentManager.Instance.DataDirectory;

            if (openFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                try
				{
					m_Texture.AddFrame(openFileDialog.FileName);
				}
				catch (Exception ex)
				{
					MessageBox.Show(this, ex.Message);
				}

				RefreshDisplay();
            }
		}
	}
}
