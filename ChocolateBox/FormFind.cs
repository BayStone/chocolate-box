using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ChocolateBox
{
    public partial class FormFind : Form
    {
        public FormFind()
        {
            InitializeComponent();
        }

        private void textBoxName_TextChanged(object sender, EventArgs e)
        {
            buttonOK.Enabled = textBoxName.Text.Length > 0;
        }
    }
}