using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using FableMod;

namespace FableMod.Content.Forms
{
    public partial class ControlThingVariable : ControlTNGElement
    {
        private TNG.Variable myVariable;
        private Control myValueControl;
        private static int SPACE_LC = 5;
        private static int SPACE_LB = 5;

        public SelectUIDHandler SelectUID;

        private static Font fontRegular;
        private static Font fontBold;

        public ControlThingVariable()
        {
            InitializeComponent();
        }

        public ControlThingVariable(TNG.Variable variable)
        {
            InitializeComponent();

            myVariable = variable;

            labelName.Text = myVariable.Name+":";

            buttonDefault.Enabled = variable.HasDefault;

            SuspendLayout();

            int x1 = labelName.Location.X+labelName.Width+SPACE_LC;
            int x2 = buttonDefault.Location.X;

            switch (myVariable.Type)
            {
            case TNG.VariableType.Unknown:
            case TNG.VariableType.String:
            case TNG.VariableType.QuoteString:
            case TNG.VariableType.Integer:
            case TNG.VariableType.Float:
                {
                    buttonBrowse.Visible = false;
                    checkBox.Visible = false;
                    textBox.Left = x1;
                    textBox.Width = (x2-x1)-SPACE_LB;
                    textBox.Text = myVariable.StringValue;
                    myValueControl = textBox;
                }
                break;

            case TNG.VariableType.GameEnum:
                {
                    checkBox.Visible = false;
                    buttonBrowse.Left = x2 - SPACE_LB - buttonBrowse.Width;
                    textBox.Left = x1;
                    textBox.Text = myVariable.StringValue;
                    textBox.Width = (buttonBrowse.Left-x1)-SPACE_LB;
                    myValueControl = textBox;
                }
                break;

            case TNG.VariableType.UID:
                {
                    checkBox.Visible = false;
                    buttonBrowse.Left = x2 - SPACE_LB - buttonBrowse.Width;
                    textBox.Left = x1;
                    textBox.Text = myVariable.StringValue;
                    textBox.Width = (buttonBrowse.Left - x1) - SPACE_LB;
                    myValueControl = textBox;
                }
                break;

            case TNG.VariableType.Boolean:
                {
                    textBox.Visible = false;
                    buttonBrowse.Visible = false;
                    checkBox.Left = x1;
                    checkBox.Width = (x2-x1)-SPACE_LB;
                    checkBox.Checked = (bool)myVariable.Value;
                    myValueControl = checkBox;
                }
                break;
            }

            ResumeLayout(false);

            UpdateFont();

            myVariable.Changed += 
                new FableMod.TNG.ElementChangedHandler(variable_Changed);
        }

        public override void ApplySelectUID(SelectUIDHandler handler)
        {
            SelectUID += handler;

            base.ApplySelectUID(handler);
        }

        public override int TotalHeight
        {
            get { return Height; }
        }

        private void UpdateFont()
        {
            if (myVariable.HasDefault)
            {
                if (fontRegular == null)
                {
                    fontRegular = new Font(labelName.Font, FontStyle.Regular);
                }

                if (fontBold == null)
                {
                    fontBold = new Font(labelName.Font, FontStyle.Bold);
                }
                
                if (myVariable.IsDefault() && labelName.Font.Bold)
                    labelName.Font = fontRegular; 
                else if (!myVariable.IsDefault() && !labelName.Font.Bold)
                    labelName.Font = fontBold; 
            }
        }

        protected void variable_Changed(FableMod.TNG.Element element)
        {
            // Get the changed message from the TNG variable.
            UpdateFont();

            if (myVariable.Type == FableMod.TNG.VariableType.Boolean)
            {
                CheckBox checkBox = (CheckBox)myValueControl;
                checkBox.Checked = (bool)myVariable.Value;
            }
            else
            {
                TextBox textBox = (TextBox)myValueControl;
                textBox.Text = myVariable.StringValue;
            }
        }

        private void textBox_Leave(object sender, EventArgs e)
        {
            TextBox textBox = (TextBox)sender;

            try
            {
                myVariable.Value = textBox.Text;
            }
            catch (Exception)
            {
                // Restore value.
                textBox.Text = myVariable.StringValue;
            }
        }

        private void checkBox_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox checkBox = (CheckBox)sender;
            myVariable.Value = checkBox.Checked ? "TRUE" : "FALSE";
        }

        private void button_Click(object sender, EventArgs e)
        {
            if (myVariable.Type == FableMod.TNG.VariableType.GameEnum)
            {
                ContentManagement.ContentManager mgr = 
                    ContentManagement.ContentManager.Instance;

                Object o = mgr.SelectEntry(
                    new ContentManagement.Link(
                        ContentManagement.LinkDestination.GameBINEntryName,
                        myVariable.Restriction),
                    myVariable.Value);

                if (o != null)
                    myVariable.Value = o.ToString();
            }
            else
            {
                if (SelectUID != null)
                {
                    SelectUID(myVariable);
                }
            }
        }

        public int LabelWidth
        {
            get
            {
                return labelName.Width;
            }

            set
            {
                SuspendLayout();

                labelName.AutoSize = false;
                labelName.Width = value;
                int x = labelName.Location.X+labelName.Width+SPACE_LC;
                myValueControl.Left = x;

                if (myVariable.Type == FableMod.TNG.VariableType.GameEnum ||
                    myVariable.Type == FableMod.TNG.VariableType.UID)
                {
                    myValueControl.Width = 
                        (buttonBrowse.Location.X-x)-SPACE_LB;
                }
                else
                {
                    myValueControl.Width =
                        (buttonBrowse.Location.X - x) - SPACE_LB;
                }

                ResumeLayout(true);
            }
        }

        private void buttonDefault_Click(object sender, EventArgs e)
        {
            myVariable.ToDefault();
        }
    }
}
