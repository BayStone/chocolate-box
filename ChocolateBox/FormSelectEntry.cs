using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;

using FableMod.ContentManagement;

namespace ChocolateBox
{
    public partial class FormSelectEntry : Form
    {
        object mySelected = null;

        public FormSelectEntry()
        {
            InitializeComponent();
        }

        public FormSelectEntry(string current)
        {
            InitializeComponent();

            if (current != null)
            {
                textBoxName.Text = current;
            }
            else
                textBoxName.Text = "";
        }

        protected void AddEntry(
            string name, string type, object entry)
        {
            ListViewItem item = new ListViewItem(name);

            if (!string.IsNullOrEmpty(type))
                item.SubItems.Add(type);
            
            item.Tag = entry;
            listViewEntries.Items.Add(item);

            if (name == textBoxName.Text)
                item.Selected = true;
        }

        protected virtual void AddEntries(
            Regex regex, string name, ProgressBar progressBar)
        {
        }

        protected void FindEntries()
        {
            buttonOK.Enabled = false;

            listViewEntries.Items.Clear();

            if (textBoxName.Text.Length > 0)
            {
                string name = textBoxName.Text;
                    
                Regex regex = null;
                
                if (name.IndexOf("[\\DEV\\") < 0)
                {
                    try
                    {   
                        regex = new Regex(name, RegexOptions.IgnoreCase);
                    }
                    catch (Exception ex)
                    {
                        FormMain.Instance.ErrorMessage(ex.ToString());
                        return;
                    }
                }

                try
                {
                    AddEntries(
                            regex, 
                            name, 
                            progressBar);
                }
                catch (Exception)
                {
                    AddEntries(null, name, progressBar);
                }

                if (listViewEntries.Items.Count > 0)
                {
                    listViewEntries.Items[0].Selected = true;
                    listViewEntries.Focus();
                }
            }
        }

        private void buttonFind_Click(object sender, EventArgs e)
        {
            FindEntries();
        }

        private void listViewEntries_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewEntries.SelectedItems.Count != 0)
            {
                mySelected = listViewEntries.SelectedItems[0].Tag;
                buttonOK.Enabled = true;
            }
        }

        protected virtual void ShowSelectedEntry()
        {
        }

        public object Selected
        {
            get { return mySelected; }
        }

        private void listViewEntries_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (listViewEntries.SelectedItems.Count != 0)
            {
                mySelected = listViewEntries.SelectedItems[0].Tag;
                buttonOK.Enabled = true;
            }

            if (mySelected != null)
            {
                ShowSelectedEntry();
            }
        }

        private void textBoxName_TextChanged(object sender, EventArgs e)
        {
            buttonFind.Enabled = textBoxName.Text.Length > 0;
        }

        private void textBoxName_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && buttonFind.Enabled)
            {
                e.Handled = true;
                FindEntries();
            }
        }

    }
}