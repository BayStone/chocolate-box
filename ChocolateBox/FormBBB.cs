using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using FableMod.BBB;

namespace ChocolateBox
{
    public partial class FormBBB : FormTreeFileController
    {
        BBBFile myBBB;

        public FormBBB()
        {
            InitializeComponent();
        }

        public void Build(BBBFile bbb, Progress progress)
        {
            myBBB = bbb;

            TreeNode root = new TreeNode();

            root.Text = "Files";

            treeView.Nodes.Add(root);
            
            progress.Begin(myBBB.EntryCount);

            for (int i = 0; i < myBBB.EntryCount; ++i)
            {
                BBBEntry entry = myBBB.get_Entries(i);

                string name = entry.FileName;

                name += string.Format(" ({0})", entry.FileSize);

                AddToTree(root, name, name);
                
                progress.Update();
            }

            progress.End();
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void extractToolStripMenuItem_Click(object sender, EventArgs e)
        {
            folderBrowserDialog.SelectedPath = Settings.FableDirectory;

            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                BBBExtractor extractor = new BBBExtractor(
                    myBBB, folderBrowserDialog.SelectedPath);

                FormProcess form = new FormProcess(extractor);

                form.ShowDialog();

                form = null;
            }
        }
    }
}