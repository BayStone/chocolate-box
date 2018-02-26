using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;

using FableMod.BIG;
using FableMod.ContentManagement;

namespace ChocolateBox
{
    public partial class FormSelectBIGEntry : FormSelectEntry
    {
        private AssetBank myBank;
        private LinkDestination myLink;

        public FormSelectBIGEntry(
            AssetBank bank, LinkDestination link, string current)
            :
            base(current)
        {
            myBank = bank;
            myLink = link;

            FindEntries();

            listViewEntries.Columns.RemoveAt(1);
            
            listViewEntries.Columns[0].Width = 
                listViewEntries.ClientSize.Width;
        }

        public new AssetEntry Selected
        {
            get { return (AssetEntry)base.Selected; }
        }

        protected override void AddEntries(
            Regex regex, string name, ProgressBar progressBar)
        {
            progressBar.Maximum = myBank.EntryCount;

            for (int i = 0; i < myBank.EntryCount; ++i)
            {
                AssetEntry entry = myBank.get_Entries(i);

                try
                {
                    if (regex != null &&
                        (regex.IsMatch(entry.DevSymbolName) ||
                         regex.IsMatch(entry.ID.ToString())))
                    {
                        AddEntry(
                            entry.DevSymbolName,
                            "",
                            entry);
                    }
                    else if (name == entry.DevSymbolName ||
                        name == entry.ID.ToString())
                    {
                        AddEntry(
                           entry.DevSymbolName,
                           "",
                           entry);
                    }
                }
                catch (Exception)
                {
                }

                progressBar.Value = i;
                progressBar.Update();
            }
        }

        protected override void ShowSelectedEntry()
        {
            ContentManager mgr = ContentManager.Instance;

            ContentObject obj = null;

            if (myLink == LinkDestination.ModelName)
            {
                obj = mgr.FindEntry(
                    LinkDestination.ModelName,
                    Selected.DevSymbolName);
            }
            else if (myLink == LinkDestination.MainTextureID ||
                     myLink == LinkDestination.ModelID ||
                     myLink == LinkDestination.TextID ||
                     myLink == LinkDestination.TextGroupID)
            {
                obj = mgr.FindEntry(myLink, Selected.ID);
            }

            if (obj != null)
            {
                mgr.ShowEntry(obj.Object, true);
            }
            else
            {
                MessageBox.Show(this, "Object not found");
            }
        }
    }
}