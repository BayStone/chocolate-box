using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace ChocolateBox
{
    /// <summary>
    /// Progress interface for files. 
    /// Thread safe.
    /// </summary>
    public class Progress : FableMod.CLRCore.ProgressInterface
    {
        private string myInfo;
        private float myValue;

        protected override void SetValue(float value)
        {
            myValue = value;
        }

        public string Info 
        { 
            get { return myInfo; } 
            set { myInfo = value; }
        }

        public float Value 
        { 
            get { return myValue; }
            set { SetValue(value); }
        }
    }
}
