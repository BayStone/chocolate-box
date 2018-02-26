using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using FableMod.BIG;
using FableMod.ContentManagement;
using FableMod.Gfx.Integration;

namespace FableMod.Content.Forms
{
    public partial class ControlPhysicsModel : ControlBIGEntry
    {
        private GfxController myController;
        private GfxView myView;
        private GfxTagModel myModel = null;

        public ControlPhysicsModel()
        {
            InitializeComponent();

            myController = new GfxController();
            myView  = new GfxView();
            myController.AddView(myView);
            panelView.Controls.Add(myView);
            myView.Dock = DockStyle.Fill;
            myView.BringToFront();
        }

        public override void ApplyChanges()
        {
            myModel.CompileToEntry(BIGEntry);
        }

        public override AssetEntry BIGEntry
        {
            get
            {
                return base.BIGEntry;
            }
            set
            {
                base.BIGEntry = value;

                if (myModel != null)
                    myModel.Dispose();

                buttonExport.Enabled = false;

                myModel = new GfxTagModel(value);

                myController.ResetObjects();
                myController.AddModel(myModel);
                myView.FrontCamera();
                myView.Activate(true);

                buttonExport.Enabled = true;
            }
        }

        private void buttonExport_Click(object sender, EventArgs e)
        {
            if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                myModel.ExportX(saveFileDialog.FileName);
            }
        }

        private void buttonImport_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                try
                {
                    myModel.ImportX(openFileDialog.FileName);

                    myController.ResetObjects();
                    myController.AddModel(myModel);
                    myView.FrontCamera();
                    myView.Activate(true);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(this, ex.Message);
                }
            }
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            myModel.Clear();

            myController.ResetObjects();
            myView.FrontCamera();
            myView.Activate(true);
        }
    }
}
