using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

using FableMod.ContentManagement;
using FableMod.BIG;

namespace FableMod.Content.Forms
{
	/// <summary>
	/// Summary description for ucBIGEntryControl.
	/// </summary>
	public class ControlBIGEntry : System.Windows.Forms.UserControl
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
        private System.Windows.Forms.Label lblEntryTypeLabel;
        protected internal Label lblEntryType;
		protected AssetEntry m_Entry;
		public ControlBIGEntry()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			//this.SetStyle(ControlStyles.DoubleBuffer, true);
			//this.SetStyle(ControlStyles.ResizeRedraw, true);
			//this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
			//this.SetStyle(ControlStyles.UserPaint, true);
			// TODO: Add any initialization after the InitializeComponent call
		}

        public virtual void OnActivate()
        {
        }

        public virtual void OnDeactivate()
        {
        }
        
        public string EntryType
		{
			get
			{
				return lblEntryType.Text;
			}
			set 
			{
				lblEntryType.Text = value;
			}
		}
		
        public virtual void ApplyChanges(){}

		public virtual AssetEntry BIGEntry
		{
			get{return m_Entry;}
			set{m_Entry = value;}
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
            this.lblEntryTypeLabel = new System.Windows.Forms.Label();
            this.lblEntryType = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lblEntryTypeLabel
            // 
            this.lblEntryTypeLabel.Location = new System.Drawing.Point(8, 8);
            this.lblEntryTypeLabel.Name = "lblEntryTypeLabel";
            this.lblEntryTypeLabel.Size = new System.Drawing.Size(72, 16);
            this.lblEntryTypeLabel.TabIndex = 0;
            this.lblEntryTypeLabel.Text = "Entry Type :";
            // 
            // lblEntryType
            // 
            this.lblEntryType.AutoSize = true;
            this.lblEntryType.Location = new System.Drawing.Point(72, 8);
            this.lblEntryType.Name = "lblEntryType";
            this.lblEntryType.Size = new System.Drawing.Size(53, 13);
            this.lblEntryType.TabIndex = 1;
            this.lblEntryType.Text = "Unknown";
            // 
            // ControlBIGEntry
            // 
            this.Controls.Add(this.lblEntryType);
            this.Controls.Add(this.lblEntryTypeLabel);
            this.Name = "ControlBIGEntry";
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion
	}
}
