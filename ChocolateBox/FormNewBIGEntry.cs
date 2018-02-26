using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using FableMod.BIG;

namespace ChocolateBox
{
    public partial class FormNewBIGEntry : Form
    {
        private BIGBank myBank;

        public FormNewBIGEntry(BIGBank bank)
        {
            InitializeComponent();

            myBank = bank;

            for (int i = 0; i < myBank.EntryCount; ++i)
            {
                comboBoxTemplates.Items.Add(
                    myBank.get_Entries(i).DevSymbolName);
            }

            comboBoxTemplates.SelectedIndex = 0;
        }

        public string SymbolName
        {
            get { return txtName.Text; }
        }

        public AssetEntry Template
        {
            get 
            {
                return myBank.FindEntryBySymbolName(
                    comboBoxTemplates.Text);
            }
        }

        private void buttonCreate_Click(object sender, EventArgs e)
        {
            if (SymbolName == "" || Template == null)
            {
                FormMain.Instance.ErrorMessage("Missing information.");
                return;
            }

            if (myBank.FindEntryBySymbolName(SymbolName) != null)
            {
                FormMain.Instance.ErrorMessage(
                    "Entry with the same name already exists.");
            }
            else
                DialogResult = DialogResult.OK;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }
    }
}