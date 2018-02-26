using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using FableMod.BIG;
using FableMod.ContentManagement;

namespace ChocolateBox
{
    public partial class FormBIG : FormTreeFileController
    {
        private BIGFile myBIG;
        
        public FormBIG()
        {
            InitializeComponent();

            try
            {
                treeView.ImageList.Images.Add(Image.FromFile(
                    Settings.DataDirectory+"icons\\bank.bmp"));
            }
            catch (Exception)
            {
            }
        }

        protected override Int32 GetObjectID(object o)
        {
            if (o.GetType() == typeof(AssetEntry))
            {
                return (Int32)((AssetEntry)o).ID;
            }
            else
                return -1;
        }

        protected override void ShowObject(object o)
        {
            if (o.GetType() == typeof(AssetEntry))
            {
                FormMain.Instance.AddMDI(
                    new FableMod.Content.Forms.FormBIGEntry(
                    (AssetEntry)o));
            }
        }

        protected override bool DeleteObject(object o)
        {
            if (o.GetType() == typeof(AssetEntry))
            {
                ((AssetEntry)o).Detach();
                return true;
            }
            
            MessageBox.Show("not yet implemented");
            
            return false;
        }

        public virtual void Build(BIGFile big, Progress progress)
        {
            myBIG = big;

            int numEntries = 0;

            for (int i = 0; i < myBIG.BankCount; ++i)
                numEntries += myBIG.get_Banks(i).EntryCount;

            progress.Begin(numEntries);

            for (int i = 0; i < myBIG.BankCount; ++i)
            {
                TreeNode bankNode = new TreeNode();

                bankNode.Text = myBIG.get_Banks(i).Name;
                bankNode.Tag = myBIG.get_Banks(i);
                bankNode.ImageIndex = 2;
                bankNode.SelectedImageIndex = 2;

                AddNode(null, bankNode);
                
                for (int j = 0; j < big.get_Banks(i).EntryCount; ++j)
                {
                    AssetEntry entry = myBIG.get_Banks(i).get_Entries(j);

                    AddToTree(
                        bankNode,
                        entry.DevSymbolName,
                        entry);

                    progress.Update();
                }
            }

            progress.End();

            if (myBIG == FileDatabase.Instance.Textures)
                addEntryToolStripMenuItem.Text = "Add Texture";
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFileDialog.InitialDirectory = Controller.Directory;
            saveFileDialog.FileName = myBIG.OriginalFileName;

            if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string fileName = saveFileDialog.FileName;
                
                FormProcess form = new FormProcess(new FileProcessor(
                    Controller, fileName));

                form.ShowDialog();
               
                form.Dispose();

                if (!Controller.FileLoaded)
                {
                    // We need to close this window.
                    Close();
                }
            }
        }

        private void findByIDToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FindByID();
        }

        private void findByNameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FindByName();
        }

        protected virtual AssetEntry CreateEntry(BIGBank bank)
        {
            FormNewBIGEntry form = new FormNewBIGEntry(bank);

            AssetEntry entry = null;

            if (form.ShowDialog() == DialogResult.OK)
            {
                entry = new AssetEntry(
                    form.SymbolName,
                    bank.GetNewID(),
                    form.Template.Type,
                    bank);

                entry.SubHeader = form.Template.SubHeader;
                entry.Data = form.Template.Data;
            }

            form.Dispose();
            form = null;

            return entry;
        }

        private void addEntryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            BIGBank bank = (BIGBank)treeView.SelectedNode.Tag;

            AssetEntry entry = CreateEntry(bank);

            if (entry != null)
            {
                bank.AddEntry(entry);

                treeView.SelectedNode = AddToTree(
                    treeView.SelectedNode, entry.DevSymbolName, entry);
            }
        }

        private void editToolStripMenuItem_Paint(object sender, PaintEventArgs e)
        {
            object selected = treeView.SelectedNode != null ? 
                treeView.SelectedNode.Tag : null;

            addEntryToolStripMenuItem.Enabled =
                selected != null &&
                selected.GetType() == typeof(BIGBank);
        }

        protected override bool ObjectIsModified(object o)
        {
            return o.GetType() == typeof(AssetEntry) &&
                ((AssetEntry)o).Modified;
        }

        private void findModifiedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FindModified();
        }
    }
}