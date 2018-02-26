using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using FableMod.ContentManagement;

namespace ChocolateBox
{
    public partial class FormModPackage : Form
    {
        private ModPackage myPackage;

        public FormModPackage()
        {
            InitializeComponent();

            myPackage = new ModPackage();

            Text = "New Mod Package";

            buttonApply.Visible = false;
        }

        private void FillList()
        {
            listViewEntries.Items.Clear();

            Collection<ContentObject> items = myPackage.Items;

            foreach (ContentObject o in items)
            {
                ListViewItem item = new ListViewItem(o.Name);

                item.SubItems.Add(o.Type.ToString());

                item.Tag = o;
                listViewEntries.Items.Add(item);
            }

            items = null;
        }

        public FormModPackage(string fileName)
        {
            InitializeComponent();

            myPackage = new ModPackage();

            myPackage.Load(fileName, null);

            FillList();

            Text = "Mod Package - "+fileName;
            textBoxDesc.Text = myPackage.Description;

            buttonApply.Visible = true;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void AddObject(ContentObject o)
        {
            try
            {
                if (!myPackage.Add(o.Type, o.Object))
                {
                    MessageBox.Show(
                        this,
                        "Object "+o.Name+" not supported.");
                    return;
                }

                FillList();
            }
            catch (Exception)
            {
            }
        }

        private void listViewEntries_DragDrop(object sender, DragEventArgs e)
        {
            AddObject(
                (ContentObject)e.Data.GetData(typeof(ContentObject)));
        }

        private void listViewEntries_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.None;

            if (e.Data.GetDataPresent(typeof(ContentObject)))
            {
                ContentObject o = 
                    (ContentObject)e.Data.GetData(typeof(ContentObject));

                if (o.Type != ContentType.Unknown)
                {
                    e.Effect = DragDropEffects.Copy;
                    return;
                }
            }

            e.Effect = DragDropEffects.None;
        }

        private void buttonSaveAs_Click(object sender, EventArgs e)
        {
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                myPackage.Description = textBoxDesc.Text;

                try
                {
                    myPackage.Save(saveFileDialog.FileName, null);
                }
                catch (Exception ex)
                {
                    FormMain.Instance.ErrorMessage(ex.Message);
                }
            }
        }

        private void buttonApply_Click(object sender, EventArgs e)
        {
            try
            {
                if (MessageBox.Show(
                    "Other editor forms need to be closed before the mod can be applied.\r\n"+
                    "Are you sure you want to continue?",
                   FormMain.Instance.Title,
                   MessageBoxButtons.YesNoCancel,
                   MessageBoxIcon.Exclamation) == DialogResult.Yes)
                {
                    FormMain.Instance.CloseChildren(typeof(FormModPackage));

                    myPackage.Apply();

                    FormMain.Instance.InfoMessage(
                        "Package applied successfully.");
                }
            }
            catch (Exception ex)
            {
                FormMain.Instance.ErrorMessage(ex.Message);
            }
        }

        private void buttonModified_Click(object sender, EventArgs e)
        {
            GetModifiedProcessor proc = new GetModifiedProcessor();

            FormProcess form = new FormProcess(proc);
            form.ShowDialog();
            form.Dispose();

            foreach (ContentObject o in proc.Objects)
                AddObject(o);

            proc = null;
        }

        private void listViewEntries_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                foreach (ListViewItem item in listViewEntries.SelectedItems)
                {
                    ContentObject o = (ContentObject)item.Tag;

                    if (!myPackage.Remove(o.Type, o.Name))
                    {
                        MessageBox.Show("Failed to remove "+o.Name+".");
                    }
                }

                FillList();
            }
        }

        private void buttonMerge_Click(object sender, EventArgs e)
        {
            if (openFileDialogFMP.ShowDialog() == DialogResult.OK)            
            {
                ModPackage package = new ModPackage();

                package.Load(openFileDialogFMP.FileName, null);

                myPackage.Merge(package);

                FillList();

                package.Dispose();

                textBoxDesc.Text = myPackage.Description;
            }
        }

        private void FormModPackage_FormClosing(object sender, FormClosingEventArgs e)
        {
            myPackage.Dispose();
        }
    }
}