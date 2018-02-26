using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.IO;
using System.Windows.Forms;

using FableMod.BIN;
using FableMod.ContentManagement;

namespace ChocolateBox
{
    public partial class FormBIN : FormTreeFileController
    {
        private BINFile myBIN;
        
        public FormBIN()
        {
            InitializeComponent();

            string iconsDir = Settings.DataDirectory+"icons\\";

            treeView.ImageList.Images.Add(
                Image.FromFile(iconsDir+"definitiontype.bmp"));

            dumpToFolderToolStripMenuItem.Visible =
                Settings.GetBool("BIN", "Dump", false);
        }

        protected override Int32 GetObjectID(object o)
        {
            return ((BINEntry)o).ID;
        }

        protected override bool ObjectIsModified(object o)
        {
            return o.GetType() == typeof(BINEntry) &&
                ((BINEntry)o).Modified;
        }

        protected override void ShowObject(object o)
        {
            BINEntry entry = (BINEntry)o;

            Form form = null;

            if (entry.Definition == "CCutsceneDef")
            {
                form = new FableMod.Content.Forms.FormCutscene(entry);
                // TODO: Ask whether or not user wants this mode?
            }
            else
                form = new FableMod.Content.Forms.FormBINEntry(entry);
            
            FormMain.Instance.AddMDI(form);
        }

        protected override bool DeleteObject(object o)
        {
            return myBIN.RemoveEntry((BINEntry)o);
        }

        protected TreeNode AddEntry(BINEntry entry)
        {
            TreeNode parent = null;

            for (int i = 0; i < treeView.Nodes.Count; ++i)
            {
                if (treeView.Nodes[i].Text == entry.Definition)
                {
                    parent = treeView.Nodes[i];
                    break;
                }
            }

            if (parent == null)
            {
                parent = new TreeNode();

                parent.Text = entry.Definition;
                parent.ImageIndex = 2;
                parent.SelectedImageIndex = 2;

                treeView.Nodes.Add(parent);
            }

            TreeNode node = AddToTree(parent, entry.Name, entry);

            return node;
        }

        public void Build(BINFile bin, Progress progress)
        {
            myBIN = bin;

            progress.Begin(bin.EntryCount);

            for (int i = 0; i < bin.EntryCount; ++i)
            {
                AddEntry(bin.get_Entries(i));

                progress.Update();
            }

            progress.End();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFileDialog.InitialDirectory = Controller.Directory;
            saveFileDialog.FileName = myBIN.OriginalFileName;

            if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                FormProcess form = new FormProcess(new FileProcessor(
                    Controller, saveFileDialog.FileName));

                form.ShowDialog();

                form.Dispose();
            }
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void findToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FindByName();
        }

        private void addEntryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormNewGameBINEntry form = new FormNewGameBINEntry(myBIN);
                   
            if (form.ShowDialog(this) == DialogResult.OK)
            {                
                byte[] data = null;
                
                BINEntry defEntry = myBIN.GetEntryByName(
                    form.Template);

                if (defEntry == null)
                {
                    form.Dispose();
                    form = null;
                    MessageBox.Show(this, "Default entry not found.");
                    return;
                }
                
                data = defEntry.Data;
        
                BINEntry entry = myBIN.AddEntry(
                    form.SymbolName, form.Definition, data);

                DefinitionType defType = 
                    ContentManager.Instance.Definitions.GetDefinition(
                        form.Definition);
                
                defType.ReadIn(entry);

                defType.FixLinks(
                    LinkDestination.GameBINEntryID,
                    defEntry.ID,
                    entry.ID);
                
                defType.Write(entry);
                
                treeView.SelectedNode = AddEntry(entry);

                data = null;
            }

            form.Dispose();
            form = null;
        }

        private void editToolStripMenuItem_Paint(object sender, PaintEventArgs e)
        {
        }

        private void findModifiedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FindModified();
        }

        private void findByIDToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FindByID();
        }

        private void dumpToFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                BINDumber dumber = new BINDumber(
                    myBIN, folderBrowserDialog.SelectedPath);

                FormProcess form = new FormProcess(dumber);

                form.Text = "Dumping BIN: "+myBIN.OriginalFileName+"...";

                form.ShowDialog();

                form = null;
            }
        }
    }
}