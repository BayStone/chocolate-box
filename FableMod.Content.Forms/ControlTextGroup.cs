using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

using FableMod.ContentManagement;
using FableMod.BIG;

namespace FableMod.Content.Forms
{
	public class ControlTextGroup : ControlBIGEntry
	{
		private System.Windows.Forms.Label lblEntries;
		private System.Windows.Forms.Button btnAdd;
		private System.Windows.Forms.Button btnRemove;
		private System.Windows.Forms.Label lblID;
		private System.Windows.Forms.TextBox txtID;
		private System.ComponentModel.IContainer components = null;
		private System.Windows.Forms.ListView lvEntries;
		private System.Windows.Forms.ColumnHeader chID;
		private System.Windows.Forms.ColumnHeader chContent;
		private System.Windows.Forms.Button btnMoveUp;
		private System.Windows.Forms.Button btnMoveDown;
		private System.Windows.Forms.Button btnRefresh;
		private BIGTextGroup m_TextGroup;
		
		public ControlTextGroup()
		{
			// This call is required by the Windows Form Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitializeComponent call
		}

		public override void ApplyChanges()
		{
			if (m_TextGroup != null)
			{
				SaveListViewToTextGroup();
				m_TextGroup.ApplyToEntry(m_Entry);
			}
		}
		private void RefreshDisplay()
		{
			lvEntries.Items.Clear();
			UInt32[] entries = m_TextGroup.Entries;
			for(int i=0;i<entries.Length;i++)
			{
				AssetEntry content = m_Entry.Bank.FindEntryByID(entries[i]);
				string[] items = new string[2];
				items[0] = entries[i].ToString();
				if (content != null)
				{
					items[1] = (new BIGText(content)).Content;
				}
				lvEntries.Items.Add(new ListViewItem(items));
			}
		}
		private void SaveListViewToTextGroup()
		{
			UInt32[] entries = new UInt32[lvEntries.Items.Count];
			for(int i=0;i<lvEntries.Items.Count;i++)
			{
				try
				{
					entries[i] = UInt32.Parse(lvEntries.Items[i].SubItems[0].Text);
				}
				catch(Exception exc)
				{

				}
			}
			m_TextGroup.Entries = entries;
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
				m_TextGroup = new BIGTextGroup(m_Entry);
				RefreshDisplay();
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
            this.lblEntries = new System.Windows.Forms.Label();
            this.lvEntries = new System.Windows.Forms.ListView();
            this.chID = new System.Windows.Forms.ColumnHeader();
            this.chContent = new System.Windows.Forms.ColumnHeader();
            this.btnAdd = new System.Windows.Forms.Button();
            this.btnRemove = new System.Windows.Forms.Button();
            this.lblID = new System.Windows.Forms.Label();
            this.txtID = new System.Windows.Forms.TextBox();
            this.btnMoveUp = new System.Windows.Forms.Button();
            this.btnMoveDown = new System.Windows.Forms.Button();
            this.btnRefresh = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lblEntries
            // 
            this.lblEntries.AutoSize = true;
            this.lblEntries.Location = new System.Drawing.Point(1, 40);
            this.lblEntries.Name = "lblEntries";
            this.lblEntries.Size = new System.Drawing.Size(42, 13);
            this.lblEntries.TabIndex = 2;
            this.lblEntries.Text = "Entries:";
            // 
            // lvEntries
            // 
            this.lvEntries.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lvEntries.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.chID,
            this.chContent});
            this.lvEntries.GridLines = true;
            this.lvEntries.HideSelection = false;
            this.lvEntries.Location = new System.Drawing.Point(58, 40);
            this.lvEntries.Name = "lvEntries";
            this.lvEntries.Size = new System.Drawing.Size(495, 208);
            this.lvEntries.TabIndex = 3;
            this.lvEntries.UseCompatibleStateImageBehavior = false;
            this.lvEntries.View = System.Windows.Forms.View.Details;
            this.lvEntries.DoubleClick += new System.EventHandler(this.lvEntries_DoubleClick);
            // 
            // chID
            // 
            this.chID.Text = "ID";
            // 
            // chContent
            // 
            this.chContent.Text = "Content";
            this.chContent.Width = 384;
            // 
            // btnAdd
            // 
            this.btnAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnAdd.Location = new System.Drawing.Point(140, 253);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(56, 21);
            this.btnAdd.TabIndex = 4;
            this.btnAdd.Text = "Add";
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnRemove
            // 
            this.btnRemove.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRemove.Location = new System.Drawing.Point(432, 254);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(121, 20);
            this.btnRemove.TabIndex = 5;
            this.btnRemove.Text = "Remove Selected";
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // lblID
            // 
            this.lblID.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblID.AutoSize = true;
            this.lblID.Location = new System.Drawing.Point(59, 259);
            this.lblID.Name = "lblID";
            this.lblID.Size = new System.Drawing.Size(21, 13);
            this.lblID.TabIndex = 6;
            this.lblID.Text = "ID:";
            // 
            // txtID
            // 
            this.txtID.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.txtID.Location = new System.Drawing.Point(86, 254);
            this.txtID.MaxLength = 5;
            this.txtID.Name = "txtID";
            this.txtID.Size = new System.Drawing.Size(48, 20);
            this.txtID.TabIndex = 7;
            this.txtID.Text = "0";
            // 
            // btnMoveUp
            // 
            this.btnMoveUp.Location = new System.Drawing.Point(4, 116);
            this.btnMoveUp.Name = "btnMoveUp";
            this.btnMoveUp.Size = new System.Drawing.Size(48, 23);
            this.btnMoveUp.TabIndex = 8;
            this.btnMoveUp.Text = "Up";
            this.btnMoveUp.Click += new System.EventHandler(this.btnMoveUp_Click);
            // 
            // btnMoveDown
            // 
            this.btnMoveDown.Location = new System.Drawing.Point(4, 145);
            this.btnMoveDown.Name = "btnMoveDown";
            this.btnMoveDown.Size = new System.Drawing.Size(48, 23);
            this.btnMoveDown.TabIndex = 9;
            this.btnMoveDown.Text = "Down";
            this.btnMoveDown.Click += new System.EventHandler(this.btnMoveDown_Click);
            // 
            // btnRefresh
            // 
            this.btnRefresh.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRefresh.Location = new System.Drawing.Point(305, 254);
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(121, 20);
            this.btnRefresh.TabIndex = 10;
            this.btnRefresh.Text = "Refresh Display";
            this.btnRefresh.Click += new System.EventHandler(this.btnRefresh_Click);
            // 
            // ControlConversation
            // 
            this.Controls.Add(this.btnRefresh);
            this.Controls.Add(this.btnMoveDown);
            this.Controls.Add(this.btnMoveUp);
            this.Controls.Add(this.txtID);
            this.Controls.Add(this.lblID);
            this.Controls.Add(this.btnRemove);
            this.Controls.Add(this.btnAdd);
            this.Controls.Add(this.lvEntries);
            this.Controls.Add(this.lblEntries);
            this.EntryType = "Text Group";
            this.Name = "ControlConversation";
            this.Size = new System.Drawing.Size(565, 284);
            this.Controls.SetChildIndex(this.lblEntries, 0);
            this.Controls.SetChildIndex(this.lvEntries, 0);
            this.Controls.SetChildIndex(this.btnAdd, 0);
            this.Controls.SetChildIndex(this.btnRemove, 0);
            this.Controls.SetChildIndex(this.lblID, 0);
            this.Controls.SetChildIndex(this.txtID, 0);
            this.Controls.SetChildIndex(this.btnMoveUp, 0);
            this.Controls.SetChildIndex(this.btnMoveDown, 0);
            this.Controls.SetChildIndex(this.btnRefresh, 0);
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private void btnAdd_Click(object sender, System.EventArgs e)
		{
			try
			{
				UInt32 id = UInt32.Parse(txtID.Text);
				SaveListViewToTextGroup();
				UInt32[] entries = m_TextGroup.Entries;
				UInt32[] newents = new UInt32[entries.Length+1];
				entries.CopyTo(newents, 0);
				newents[entries.Length] = id;
				m_TextGroup.Entries = newents;
				RefreshDisplay();
			}
			catch(Exception exc)
			{

			}
		}

		private void btnRemove_Click(object sender, System.EventArgs e)
		{
			for(int i=0;i<lvEntries.SelectedItems.Count;i++)
			{
				lvEntries.Items.Remove(lvEntries.SelectedItems[i]);
			}
		}

		private void btnMoveUp_Click(object sender, System.EventArgs e)
		{
			if (lvEntries.SelectedItems.Count != 0)
			{
				ListViewItem sel = lvEntries.SelectedItems[0];
				int index = lvEntries.Items.IndexOf(sel);

				if (index == 0)
					return;

				lvEntries.Items.Remove(sel);
				lvEntries.Items.Insert(index-1, sel);
				sel.Selected = true;
			}
		}

		private void btnMoveDown_Click(object sender, System.EventArgs e)
		{
			if (lvEntries.SelectedItems.Count != 0)
			{
				ListViewItem sel = lvEntries.SelectedItems[0];
				int index = lvEntries.Items.IndexOf(sel);

				if (index == lvEntries.Items.Count-1)
					return;

				lvEntries.Items.Remove(sel);
				lvEntries.Items.Insert(index+1, sel);
				sel.Selected = true;
			}
		}

		private void lvEntries_DoubleClick(object sender, System.EventArgs e)
		{
			if (lvEntries.SelectedItems.Count != 0)
			{
                try
				{
                    UInt32 id = UInt32.Parse(
                        lvEntries.SelectedItems[0].SubItems[0].Text);

                    ContentManager.Instance.ShowEntry(
                        LinkDestination.TextID, id, false);
				}
				catch (Exception)
				{
				}
			}
		}

		private void btnRefresh_Click(object sender, System.EventArgs e)
		{
			SaveListViewToTextGroup();
			RefreshDisplay();
		}

	}
}

