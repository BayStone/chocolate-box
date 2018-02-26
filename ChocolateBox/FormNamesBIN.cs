using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using FableMod.BIN;
using FableMod.Forms;

namespace ChocolateBox
{
    public partial class FormNamesBIN : FormFileController
    {
        NamesBINFile myNames;

        public FormNamesBIN()
        {
            InitializeComponent();
        }

        public void Build(NamesBINFile bin, Progress progress)
        {
            myNames = bin;

            progress.Begin(bin.EntryCount);

            for (int i = 0; i < bin.EntryCount; ++i)
            {
                NamesBINEntry entry = bin.get_Entries(i);

                ListViewItem item = new ListViewItem();
                item.Text = entry.Name;
                
                item.SubItems.Add(entry.Enum.ToString());
                item.SubItems.Add(entry.Offset.ToString());

                listViewEntries.Items.Add(item);

                progress.Update();
            }

            progress.End();
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormTextBox form = new FormTextBox();
            form.labelInput.Text = "Input name:";
            form.Text = "Names.BIN: New Entry";

            if (form.ShowDialog() == DialogResult.OK)
            {
                NamesBINEntry entry = 
                    myNames.AddEntry(form.textBoxInput.Text);

                ListViewItem item = new ListViewItem();
                item.Text = entry.Name;

                item.SubItems.Add(entry.Enum.ToString());
                item.SubItems.Add(entry.Offset.ToString());

                listViewEntries.Items.Add(item);

                listViewEntries.Sort();
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFileDialog.InitialDirectory = Controller.Directory;
            saveFileDialog.FileName = myNames.OriginalFileName;
            
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                myNames.Save(saveFileDialog.FileName);
                FormMain.Instance.InfoMessage("OK.");
            }
        }

    }
}