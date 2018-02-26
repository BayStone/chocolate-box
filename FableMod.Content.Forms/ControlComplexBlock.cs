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
    /// <summary>
    /// Handler for the expand/collapse.
    /// </summary>
    public delegate void ControlsUpdatedHandler();

    public partial class ControlComplexBlock : ControlTNGElement
    {
        protected bool myCollapsed = true;
        protected TNG.ComplexBlock myBlock = null;
        protected bool myBuilt = false;

        public event ControlsUpdatedHandler ControlsUpdated;
        
        public ControlComplexBlock()
        {
            InitializeComponent();
        }

        public override void ApplySelectUID(SelectUIDHandler handler)
        {
            ControlCollection coll = panelControls.Controls;

            for (int i = 0; i < coll.Count; ++i)
            {
                ((ControlTNGElement)coll[i]).ApplySelectUID(handler);
            }
        }

        protected void AddElement(TNG.Element element)
        {
            ControlTNGElement c = null;

            if (element.GetType() == typeof(TNG.Variable))
            {
                c = 
                    new ControlThingVariable((TNG.Variable)element);
            }
            else if (element.GetType() == typeof(TNG.ElementArray))
            {
                ControlElementArray c1 = new ControlElementArray();

                c1.Setup((TNG.ElementArray)element);

                // Add the expand/collapse/add/remove listener.
                c1.ControlsUpdated += 
                    new ControlsUpdatedHandler(array_ControlsUpdated);

                c = c1;
            }
            else
            {
                ControlComplexBlock c1 = new ControlComplexBlock();

                c1.Setup((TNG.ComplexBlock)element);

                // Add the expand/collapse/add/remove listener.
                c1.ControlsUpdated += 
                    new ControlsUpdatedHandler(array_ControlsUpdated);

                c = c1;
            }

            panelControls.Controls.Add(c);

            c.Dock = DockStyle.Top;
            c.BringToFront();

            c = null;
        }

        protected void UpdateLabelWidths()
        {
            ControlCollection coll = panelControls.Controls;

            int maxWidth = 0;

            int count = coll.Count;

            List<ControlThingVariable> vars = new List<ControlThingVariable>();

            for (int i = 0; i < count; ++i)
            {
                ControlThingVariable c = coll[i] as ControlThingVariable;

                if (c != null)
                {
                    vars.Add(c);

                    if (maxWidth < c.LabelWidth)
                        maxWidth = c.LabelWidth;
                }
            }

            // Apply max width.

            foreach (ControlThingVariable var in vars)
            {
                var.LabelWidth = maxWidth;
            }
        }

        public void Setup(TNG.ComplexBlock block)
        {
            // Add the variables to the panel.

            labelName.Text = block.Name;

            myBuilt = false;

            myBlock = block;

            panelControls.Controls.Clear();

            // Collapsed state.
            Height = panelTop.Height;
            panelExpansion.Visible = false;

            buttonExpand.Enabled = myBlock.ElementCount > 0;
            buttonDefault.Enabled = block.HasDefault;

            UpdateFont();

            myBlock.Changed += new FableMod.TNG.ElementChangedHandler(
                element_Changed);
        }

        protected virtual int GetControlsHeight()
        {
            int height = 0;

            if (myCollapsed)
                return height;
        
            ControlCollection coll = panelControls.Controls;

            int count = coll.Count;

            for (int i = 0; i < count; ++i)
            {
                height += ((ControlTNGElement)coll[i]).TotalHeight;
            }

            return height;
        }

        private void UpdateFont()
        {
            if (myBlock.HasDefault)
            {
                if (myBlock.IsDefault() && labelName.Font.Bold)
                    labelName.Font = 
                        new Font(labelName.Font, FontStyle.Regular);
                else if (!myBlock.IsDefault() && !labelName.Font.Bold)
                    labelName.Font = 
                        new Font(labelName.Font, FontStyle.Bold);
            }
        }

        /// <summary>
        /// Total height of the control
        /// </summary>
        public override int TotalHeight
        {
            get { return GetControlsHeight()+panelTop.Height; }
        }

        /// <summary>
        /// Is the control collapsed.
        /// </summary>
        public bool Collapsed
        {
            get { return myCollapsed; }
        }

        protected virtual void OnBlockChanged()
        {
        }

        protected void element_Changed(FableMod.TNG.Element element)
        {
            // Got the message from the block. 
            UpdateFont();
            
            OnBlockChanged();
        }

        protected void OnControlsUpdated()
        {
            if (ControlsUpdated != null)
                ControlsUpdated();
        }

        protected void array_ControlsUpdated()
        {
            // And... relay to the parent.
            UpdateHeight();

            OnControlsUpdated();
        }

        protected void UpdateHeight()
        {
            if (myCollapsed)
            {
                panelExpansion.Visible = false;
                panelExpansion.Height = 0;
            }
            else
            {
                panelExpansion.Visible = true;
                panelExpansion.Height = GetControlsHeight();
            }

            Height = TotalHeight;
        }

        public void Expand()
        {
            if (Collapsed)
                OnExpand();
        }

        public void Collapse()
        {
            if (!Collapsed)
                OnCollapse();
        }
        
        protected virtual void OnExpand()
        {
            SuspendLayout();

            myCollapsed = false;

            if (!myBuilt)
            {
                myBuilt = true;

                SuspendLayout();

                for (int i = 0; i < myBlock.ElementCount; ++i)
                {
                    AddElement(myBlock.get_Elements(i));
                }

                UpdateLabelWidths();

                ResumeLayout(false);
            }

            buttonExpand.Text = "-";

            array_ControlsUpdated();

            ResumeLayout(true);
        }

        protected virtual void OnCollapse()
        {
            SuspendLayout();
            
            myCollapsed = true;

            buttonExpand.Text = "+";
           
            array_ControlsUpdated();

            ResumeLayout(true);
        }

        private void buttonExpand_Click(object sender, EventArgs e)
        {
            if (buttonExpand.Text == "+")
            {
                OnExpand();
            }
            else
            {
                OnCollapse();
            }
        }

        protected virtual void ToDefault()
        {
            myBlock.ToDefault();
        }

        private void buttonDefault_Click(object sender, EventArgs e)
        {
            ToDefault();
        }
    }
}
