using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;

using FableMod.BIN;

namespace ChocolateBox
{
    public partial class FormSelectNamesEntry : FormSelectEntry
    {
        private NamesBINFile myNames;
        private Regex myRestriction;

        public FormSelectNamesEntry(
            NamesBINFile names, string current, string restriction) :
            base(current)
        {
            InitializeComponent();

            myNames = names;
            myRestriction = new Regex(restriction);
        }

        public new NamesBINEntry Selected
        {
            get { return (NamesBINEntry)base.Selected; }
        }

        protected override void AddEntries(
            Regex regex, string name, ProgressBar progressBar)
        {
            progressBar.Maximum = myNames.EntryCount;

            for (int i = 0; i < myNames.EntryCount; ++i)
            {
                NamesBINEntry entry = myNames.get_Entries(i);

                if (myRestriction.IsMatch(entry.Name))
                {
                    if (regex != null && regex.IsMatch(entry.Name))
                    {
                        AddEntry(
                            entry.Name,
                            "",
                            entry);
                    }
                    else if (name == entry.Name)
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
    }
}