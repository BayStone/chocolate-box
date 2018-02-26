using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ChocolateBox
{
    public partial class FormFileController : Form
    {
        private FileController myController;

        public FormFileController()
        {
            InitializeComponent();
        }

        public FileController Controller
        {
            get { return myController; }
            set { myController = value; }
        }

        protected void SaveChanges()
        {
        }

        private void FormFileController_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason != CloseReason.UserClosing)
                return;

            if (Modified)
            {
                DialogResult Result = MessageBox.Show(
                   "Save Changes?",
                   Text,
                   MessageBoxButtons.YesNoCancel,
                   MessageBoxIcon.Exclamation);

                if (Result == DialogResult.Yes)
                {
                    SaveChanges();
                }
                else if (Result == DialogResult.Cancel)
                {
                    e.Cancel = true;
                }
            }

            Controller.CloseForm(this);
        }

        protected override void OnLoad(EventArgs e)
        {
            if (Controller != null)
                Text = Text+" - "+Controller.RelativeFileName;

            base.OnLoad(e);
        }

        protected virtual bool Modified
        {
            get { return false; }
        }
    }
}