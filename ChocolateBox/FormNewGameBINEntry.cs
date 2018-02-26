using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using FableMod.BIN;
using FableMod.ContentManagement;

namespace ChocolateBox
{
    public partial class FormNewGameBINEntry : Form
    {
        private BINFile myBIN;
        private bool myRenewTemplates = true;

        public FormNewGameBINEntry(BINFile bin)
        {
            InitializeComponent();

            myBIN = bin;

            DefinitionType[] defTypes =
                FileDatabase.Instance.Definitions.GetDefinitions();

            foreach (DefinitionType defType in defTypes)
            {
                comboBoxDefs.Items.Add(defType.Name);
            }
        }

        public string Template
        {
            get { return comboBoxTemplates.Text; }
        }
        
        public string Definition
        {
            get { return comboBoxDefs.Text; }
        }

        public string SymbolName
        {
            get { return textBoxName.Text; }
        }

        private void buttonCreate_Click(object sender, EventArgs e)
        {
            if (SymbolName == "" || Definition == "" || Template == "")
            {
                FormMain.Instance.ErrorMessage("Missing information.");
                return;
            }

            if (myBIN.GetEntryByName(SymbolName) != null)
            {
                FormMain.Instance.ErrorMessage("Entry already exists.");
            }
            else
                DialogResult = DialogResult.OK;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void comboBoxTemplates_DropDown(object sender, EventArgs e)
        {
            if (myRenewTemplates)
            {
                myRenewTemplates = false;

                BINEntry[] entries = 
                    myBIN.GetEntriesByDefinition(Definition);

                comboBoxTemplates.Items.Clear();

                foreach (BINEntry entry in entries)
                {
                    if (entry.Name != "")
                        comboBoxTemplates.Items.Add(entry.Name);
                }

                entries = null;
            }
        }

        private void comboBoxDefs_SelectedIndexChanged(object sender, EventArgs e)
        {
            comboBoxTemplates.Enabled = comboBoxDefs.SelectedIndex >= 0;
            myRenewTemplates = true;
        }
    }
}