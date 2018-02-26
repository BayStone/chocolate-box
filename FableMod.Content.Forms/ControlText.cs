using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

using FableMod.ContentManagement;
using FableMod.BIG;

namespace FableMod.Content.Forms
{
    public class ControlText : ControlBIGEntry
	{
		private System.Windows.Forms.TextBox txtContent;
		private System.ComponentModel.IContainer components = null;
		private System.Windows.Forms.TextBox txtIdentifier;
		private System.Windows.Forms.Label lblIdentifier;
		private System.Windows.Forms.TextBox txtSoundFile;
		private System.Windows.Forms.Label lblSoundFile;
		private System.Windows.Forms.TextBox txtSpeaker;
		private System.Windows.Forms.Label lblSpeaker;
		private System.Windows.Forms.TextBox txtModifiers;
		private System.Windows.Forms.Label lblModifiers;
		private System.Windows.Forms.Label lblContent;

		protected BIGText m_Text;

		public ControlText()
		{
			// This call is required by the Windows Form Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitializeComponent call
		}
		public override void ApplyChanges()
		{
			if (m_Text != null)
			{
				m_Text.Identifier = txtIdentifier.Text;
				m_Text.Content = txtContent.Text;
				m_Text.Modifiers = txtModifiers.Lines;
				m_Text.SoundFile = txtSoundFile.Text;
				m_Text.Speaker = txtSpeaker.Text;

				m_Text.ApplyToEntry(m_Entry);
			}
		}
		public override AssetEntry BIGEntry
		{
			get
			{
				return base.BIGEntry;
			}
			set
			{
				base.BIGEntry = value;
				m_Text = new BIGText(m_Entry);
				txtIdentifier.Text = m_Text.Identifier;
				txtModifiers.Lines = m_Text.Modifiers;
				txtSoundFile.Text = m_Text.SoundFile;
				txtSpeaker.Text = m_Text.Speaker;
				txtContent.Text = m_Text.Content;
			}
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.txtIdentifier = new System.Windows.Forms.TextBox();
            this.lblIdentifier = new System.Windows.Forms.Label();
            this.txtContent = new System.Windows.Forms.TextBox();
            this.lblContent = new System.Windows.Forms.Label();
            this.txtSoundFile = new System.Windows.Forms.TextBox();
            this.lblSoundFile = new System.Windows.Forms.Label();
            this.txtModifiers = new System.Windows.Forms.TextBox();
            this.lblModifiers = new System.Windows.Forms.Label();
            this.txtSpeaker = new System.Windows.Forms.TextBox();
            this.lblSpeaker = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // txtIdentifier
            // 
            this.txtIdentifier.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtIdentifier.Location = new System.Drawing.Point(74, 28);
            this.txtIdentifier.Name = "txtIdentifier";
            this.txtIdentifier.Size = new System.Drawing.Size(379, 20);
            this.txtIdentifier.TabIndex = 2;
            // 
            // lblIdentifier
            // 
            this.lblIdentifier.AutoSize = true;
            this.lblIdentifier.Location = new System.Drawing.Point(8, 31);
            this.lblIdentifier.Name = "lblIdentifier";
            this.lblIdentifier.Size = new System.Drawing.Size(50, 13);
            this.lblIdentifier.TabIndex = 3;
            this.lblIdentifier.Text = "Identifier:";
            // 
            // txtContent
            // 
            this.txtContent.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtContent.Location = new System.Drawing.Point(74, 124);
            this.txtContent.Multiline = true;
            this.txtContent.Name = "txtContent";
            this.txtContent.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtContent.Size = new System.Drawing.Size(379, 93);
            this.txtContent.TabIndex = 4;
            // 
            // lblContent
            // 
            this.lblContent.AutoSize = true;
            this.lblContent.Location = new System.Drawing.Point(8, 127);
            this.lblContent.Name = "lblContent";
            this.lblContent.Size = new System.Drawing.Size(47, 13);
            this.lblContent.TabIndex = 14;
            this.lblContent.Text = "Content:";
            // 
            // txtSoundFile
            // 
            this.txtSoundFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtSoundFile.Location = new System.Drawing.Point(74, 52);
            this.txtSoundFile.Name = "txtSoundFile";
            this.txtSoundFile.Size = new System.Drawing.Size(379, 20);
            this.txtSoundFile.TabIndex = 6;
            // 
            // lblSoundFile
            // 
            this.lblSoundFile.AutoSize = true;
            this.lblSoundFile.Location = new System.Drawing.Point(8, 56);
            this.lblSoundFile.Name = "lblSoundFile";
            this.lblSoundFile.Size = new System.Drawing.Size(60, 13);
            this.lblSoundFile.TabIndex = 7;
            this.lblSoundFile.Text = "Sound File:";
            // 
            // txtModifiers
            // 
            this.txtModifiers.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtModifiers.Location = new System.Drawing.Point(74, 100);
            this.txtModifiers.Name = "txtModifiers";
            this.txtModifiers.Size = new System.Drawing.Size(379, 20);
            this.txtModifiers.TabIndex = 12;
            // 
            // lblModifiers
            // 
            this.lblModifiers.AutoSize = true;
            this.lblModifiers.Location = new System.Drawing.Point(8, 103);
            this.lblModifiers.Name = "lblModifiers";
            this.lblModifiers.Size = new System.Drawing.Size(52, 13);
            this.lblModifiers.TabIndex = 13;
            this.lblModifiers.Text = "Modifiers:";
            // 
            // txtSpeaker
            // 
            this.txtSpeaker.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtSpeaker.Location = new System.Drawing.Point(74, 76);
            this.txtSpeaker.Name = "txtSpeaker";
            this.txtSpeaker.Size = new System.Drawing.Size(379, 20);
            this.txtSpeaker.TabIndex = 10;
            // 
            // lblSpeaker
            // 
            this.lblSpeaker.AutoSize = true;
            this.lblSpeaker.Location = new System.Drawing.Point(8, 79);
            this.lblSpeaker.Name = "lblSpeaker";
            this.lblSpeaker.Size = new System.Drawing.Size(50, 13);
            this.lblSpeaker.TabIndex = 11;
            this.lblSpeaker.Text = "Speaker:";
            // 
            // ControlText
            // 
            this.Controls.Add(this.txtSpeaker);
            this.Controls.Add(this.lblSpeaker);
            this.Controls.Add(this.txtModifiers);
            this.Controls.Add(this.lblModifiers);
            this.Controls.Add(this.txtSoundFile);
            this.Controls.Add(this.lblSoundFile);
            this.Controls.Add(this.txtContent);
            this.Controls.Add(this.lblContent);
            this.Controls.Add(this.txtIdentifier);
            this.Controls.Add(this.lblIdentifier);
            this.EntryType = "Text";
            this.Name = "ControlText";
            this.Size = new System.Drawing.Size(465, 229);
            this.Controls.SetChildIndex(this.lblIdentifier, 0);
            this.Controls.SetChildIndex(this.txtIdentifier, 0);
            this.Controls.SetChildIndex(this.lblContent, 0);
            this.Controls.SetChildIndex(this.txtContent, 0);
            this.Controls.SetChildIndex(this.lblSoundFile, 0);
            this.Controls.SetChildIndex(this.txtSoundFile, 0);
            this.Controls.SetChildIndex(this.lblModifiers, 0);
            this.Controls.SetChildIndex(this.txtModifiers, 0);
            this.Controls.SetChildIndex(this.lblSpeaker, 0);
            this.Controls.SetChildIndex(this.txtSpeaker, 0);
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion
	}
}

