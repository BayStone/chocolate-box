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
    public delegate void ThingChangedHandler(FableMod.TNG.Thing thing);

    public partial class ControlThing : ControlComplexBlock
    {
        private FableMod.TNG.Thing myThing = null;

        public event ThingChangedHandler ThingChanged;

        public ControlThing()
        {
            InitializeComponent();
        }

        public ControlThing(FableMod.TNG.Thing thing)
        {
            InitializeComponent();
            Thing = thing;
        }

        public FableMod.TNG.Thing Thing
        {
            get { return myThing; }
            set
            {
                if (myThing != value)
                {
                    Collapse();

                    myThing = value;

                    Setup(myThing);

                    labelName.Text = 
                    myThing.Name+" ["+myThing.DefinitionType+"]";

                    textBoxDefType.Text = myThing.DefinitionType;
                    textBoxUID.Text = myThing.UID;
                    numericUpDownPlayer.Value = myThing.Player;

                    Expand();
                }
            }
        }

        protected override void OnBlockChanged()
        {
            if (ThingChanged != null)
                ThingChanged(myThing);

            base.OnBlockChanged();
        }

        /// <summary>
        /// Add the base info panel to the height.
        /// </summary>
        /// <returns></returns>
        protected override int GetControlsHeight()
        {
            int height = base.GetControlsHeight();

            if (Collapsed)
                return height;

            return height+panelBase.Height;
        }

        private void buttonOpen_Click(object sender, EventArgs e)
        {
            ContentManagement.ContentManager mgr = 
                ContentManagement.ContentManager.Instance;

            ContentManagement.ContentObject obj = mgr.FindEntry(
                ContentManagement.LinkDestination.GameBINEntryName,
                myThing.DefinitionType);

            if (obj != null)
            {
                mgr.ShowEntry(obj.Object, false);
            }
        }

        private void buttonGenerate_Click(object sender, EventArgs e)
        {
            myThing.UID = FableMod.TNG.UIDManager.Generate();
            textBoxUID.Text = myThing.UID;
        }
    }
}
