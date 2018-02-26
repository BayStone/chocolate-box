using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ChocolateBox
{
    public partial class FormRegionSelect : Form
    {
        public FormRegionSelect()
        {
            InitializeComponent();
        }

        private void listBoxRegions_SelectedIndexChanged(object sender, EventArgs e)
        {
            buttonOK.Enabled = listBoxRegions.SelectedIndex >= 0;
        }

        private void listBoxRegions_DoubleClick(object sender, EventArgs e)
        {
            if (listBoxRegions.SelectedIndex >= 0)
            {
                DialogResult = DialogResult.OK;
            }
        }
    }
}