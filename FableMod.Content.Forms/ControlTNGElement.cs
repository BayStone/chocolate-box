using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using FableMod.TNG;

namespace FableMod.Content.Forms
{
    public delegate void ValueChangedHandler(TNG.Element element);
    public delegate void SelectUIDHandler(TNG.Variable variable);

    public partial class ControlTNGElement : UserControl
    {
        public ControlTNGElement()
        {
            InitializeComponent();
        }

        public virtual int TotalHeight
        {
            get { return 0; }
        }

        public virtual void ApplySelectUID(SelectUIDHandler handler)
        {
        }
    }
}
