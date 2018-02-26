using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace FableMod.Content.Forms
{
    public partial class ControlElementArray : ControlComplexBlock
    {
        public ControlElementArray()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Override the default action. We're using it for clear.
        /// </summary>
        protected override void ToDefault()
        {
            panelControls.Controls.Clear();
            ((FableMod.TNG.ElementArray)myBlock).Clear();
            buttonDefault.Enabled = false;
            buttonExpand.Enabled = false;
            buttonExpand.Text = "+";
            myCollapsed = true;
            UpdateHeight();
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            FableMod.TNG.Element element = 
                ((FableMod.TNG.ElementArray)myBlock).Add();

            AddElement(element);

            panelControls.SuspendLayout();

            panelControls.Controls[
                panelControls.Controls.Count-1].BringToFront();

            panelControls.ResumeLayout(true);
            
            bool enabled = panelControls.Controls.Count > 0;

            buttonExpand.Enabled = enabled;
            buttonDefault.Enabled = enabled;

            array_ControlsUpdated();

            element = null;
        }
    }
}
