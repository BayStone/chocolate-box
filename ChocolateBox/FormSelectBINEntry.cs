using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;

using FableMod.BIN;
using FableMod.ContentManagement;

namespace ChocolateBox
{
    public partial class FormSelectBINEntry : FormSelectEntry
    {
        BINFile myBIN;
        string myCDef;
        
        public FormSelectBINEntry(BINFile binfile, string current)
            :
            base(current)
        {
            myBIN = binfile;
            FindEntries();
        }

        public new BINEntry Selected
        {
            get { return (BINEntry)base.Selected; }
        }

        public string CDefType
        {
            get { return myCDef; }
            set { myCDef = value; }
        }
        
        protected override void AddEntries(
            Regex regex, string name, ProgressBar progressBar)
        {
            progressBar.Maximum = myBIN.EntryCount;

            List<string> types = null;

            if (!string.IsNullOrEmpty(myCDef))
            {
                types = new List<string>(myCDef.Split(';'));
            }

            for (int i = 0; i < myBIN.EntryCount; ++i)
            {
                BINEntry entry = myBIN.get_Entries(i);

                if ((regex != null && regex.IsMatch(entry.Name)) ||
                    name == entry.Name)
                {
                    if (types == null || types.IndexOf(entry.Definition) >= 0)
                    {
                        AddEntry(
                            entry.Name,
                            "",
                            entry);
                    }
                }
                
                progressBar.Value = i;
                progressBar.Update();
            }
        }

        protected override void ShowSelectedEntry()
        {
            ContentManager mgr = ContentManager.Instance;

            ContentObject obj = mgr.FindEntry(
                LinkDestination.GameBINEntryName,
                Selected.Name);

            if (obj != null)
            {
                // Open the model.

                DefinitionType defType = mgr.Definitions.GetDefinition(
                    Selected.Definition);

                if (defType != null)
                {
                    defType.ReadIn(Selected);

                    FableMod.ContentManagement.Control c = 
                        defType.FindControl(0xC8636B2E);

	                if (c != null && c.Members.Count == 5)
	                {
		                Member m = (Member)c.Members[1];

                        mgr.ShowEntry(
                            LinkDestination.ModelID, 
                            m.Value,
                            true);
                    }

                    c = null;
                    defType = null;
                }
            }
        }
    }
}