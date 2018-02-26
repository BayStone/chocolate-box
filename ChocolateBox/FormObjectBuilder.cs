using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using FableMod.ContentManagement;
using FableMod.BIN;
using FableMod.BIG;

namespace ChocolateBox
{
    public partial class FormObjectBuilder : Form
    {
        ObjectTemplate myTemplate = new ObjectTemplate();
        Dictionary<int, ComboBox> myComboBoxes = 
            new Dictionary<int, ComboBox>();
        Dictionary<string, ComboBox> myComboBoxPool = 
            new Dictionary<string, ComboBox>();
        Dictionary<int, CheckBox> myRefCheckBoxes = 
            new Dictionary<int, CheckBox>();
        BINFile myGameBin = null;

        private class AssetComboBoxItem : IComparable
        {
            public AssetEntry Entry;

            public AssetComboBoxItem(AssetEntry entry)
            {
                Entry = entry;
            }

            public override string ToString()
            {
                return Entry.DevSymbolName;
            }

            public override bool Equals(object obj)
            {
                if (obj.GetType() == typeof(string))
                    return Entry.DevSymbolName.Equals(obj);

                return base.Equals(obj);
            }

            public int CompareTo(object o)
            {
                AssetComboBoxItem item = o as AssetComboBoxItem;
                
                return Entry.DevSymbolName.CompareTo(
                    item.Entry.DevSymbolName);
            }
        }
    
        public FormObjectBuilder()
        {
            InitializeComponent();

            myGameBin = ContentManager.Instance.Objects;
        }

        private ComboBox NewComboBox(string name)
        {
            // Note! The pool concept is quite loose here...
            // the objects in the pool are being used by
            // the current template. This due to event handlers.

            ComboBox box;

            if (myComboBoxPool.TryGetValue(name, out box))
            {
                myComboBoxPool.Remove(name);
                return box;
            }

            return new ComboBox();
        }

        private void AddToPool(string name, ComboBox box)
        {
            if (myComboBoxPool.ContainsKey(name))
                return;

            myComboBoxPool.Add(name, box);
        }

        private void FormItemBuilder_Load(object sender, EventArgs e)
        {
            DefinitionType[] defTypes =
                ContentManager.Instance.Definitions.GetDefinitions();

            foreach (DefinitionType defType in defTypes)
            {
                comboBoxDefs.Items.Add(defType.Name);
            }
        }

        private void AssetFill(ComboBox box, AssetTemplate item)
        {
            BIGBank bank;

            if (item.Type == ContentType.Text)
            {
                bank = ContentManager.Instance.TextBank;
                
                for (int i = 0; i < bank.EntryCount; ++i)
                {
                    AssetEntry entry = bank.get_Entries(i);

                    if (entry.Type == 0)
                        box.Items.Add(new AssetComboBoxItem(entry));
                }
            }
            else if (item.Type == ContentType.Graphics)
            {
                bank = ContentManager.Instance.GraphicsBank;

                for (int i = 0; i < bank.EntryCount; ++i)
                {
                    AssetEntry entry = bank.get_Entries(i);

                    uint type = entry.Type;

                    if (type == 0x1 || 
                        type == 0x2 || 
                        type == 0x4 || 
                        type == 0x5)
                    {
                        box.Items.Add(new AssetComboBoxItem(entry));
                    }
                }
            }
            else if (item.Type == ContentType.MainTextures)
            {
                bank = ContentManager.Instance.MainTextureBank;

                for (int i = 0; i < bank.EntryCount; ++i)
                {
                    box.Items.Add(new AssetComboBoxItem(bank.get_Entries(i)));
                }
            }
        }

        private void UpdateLinks(ComboBox comboBox)
        {
            BaseTemplate item =
                myTemplate.get_Items(int.Parse(comboBox.Tag.ToString()));

            if (item.GetType() == typeof(DefTypeTemplate))
            {
                DefTypeTemplate defItem = item as DefTypeTemplate;

                foreach (KeyValuePair<int, ComboBox> kvp in myComboBoxes)
                {
                    if (kvp.Key != defItem.ID)
                    {
                        BaseTemplate tmpItem = myTemplate.get_Items(
                            int.Parse(kvp.Value.Tag.ToString()));

                        if (tmpItem.LinkTo == item)
                        {
                            if (tmpItem.GetType() == typeof(DefTypeTemplate))
                            {
                                DefTypeTemplate defTmpItem = 
                                    tmpItem as DefTypeTemplate;

                                string id = GetCDefDataID(
                                    comboBox.SelectedItem.ToString(),
                                    defTmpItem.Type);

                                if (id != null)
                                {
                                    // Notice that this causes another event.

                                    kvp.Value.Enabled = true;

                                    kvp.Value.SelectedIndex = 
                                        kvp.Value.Items.IndexOf(id);
                                }
                            }
                            else
                            {
                                AssetTemplate assetItem = 
                                    tmpItem as AssetTemplate;

                                string id = GetAssetLink(
                                    comboBox.SelectedItem.ToString(),
                                    assetItem.ControlID,
                                    assetItem.Element);

                                if (id != null)
                                {
                                    kvp.Value.Enabled = true;
                                    
                                    kvp.Value.SelectedIndex = 
                                        kvp.Value.Items.IndexOf(id);
                                }
                            }
                        }
                    }
                }
            }
        }

        private void itemComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateLinks(sender as ComboBox);
        }

        private void DefTypeLink(
            BINEntry entryDst, BINEntry entrySrc, string cdef)
        {
            DefinitionType def =
                FileDatabase.Instance.Definitions.GetDefinition(
                    entryDst.Definition);

            def.ReadIn(entryDst);

            if (def.CDefs == null)
                return;

            CDefLink link = def.get_CDefLinks(cdef);

            if (link != null)
            {
                link.DataID = entrySrc.ID.ToString();

                def.Write(entryDst);

                return;
            }
            
            def = null;
        }

        private string GetCDefDataID(
            string defName, string cdef)
        {
            BINEntry entry = myGameBin.GetEntryByName(defName);

            if (entry != null)
            {
                DefinitionType def =
                    FileDatabase.Instance.Definitions.GetDefinition(
                        entry.Definition);

                def.ReadIn(entry);

                if (def.CDefs != null)
                {
                    CDefLink link = def.get_CDefLinks(cdef);

                    if (link != null)
                        return link.DataID.ToString();
                }
            }

            return null;
        }

        private string GetAssetLink(
            string defName, UInt32 control, int element)
        {
            BINEntry entry = GetBINEntry(defName);

            if (entry != null)
            {
                DefinitionType def = 
                    FileDatabase.Instance.Definitions.GetDefinition(
                        entry.Definition);

                def.ReadIn(entry);

                FableMod.ContentManagement.Control c = 
                    def.FindControl(control);

                if (c != null)
                {
                    Member mbr = (Member)c.Members[element];

                    ContentObject o = ContentManager.Instance.FindEntry(
                        mbr.Link.To, mbr.Value);

                    if (o != null)
                        return o.Name;
                }
            }

            return null;
        }

        private void AssetLink(
            BINEntry entryDst, 
            AssetTemplate assetItem, UInt32 assetId)
        {
            DefinitionType def = 
                ContentManager.Instance.Definitions.GetDefinition(
                    entryDst.Definition);

            def.ReadIn(entryDst);

            FableMod.ContentManagement.Control c =
                def.FindControl(assetItem.ControlID);

            if (c != null)
            {
                ((Member)c.Members[assetItem.Element]).Value = 
                    assetId.ToString();

                def.Write(entryDst);
            }

            def = null;
        }

        private string GetItemName(string item)
        {
            string tmp = item.ToUpper();
            tmp = tmp.Replace(" ", "_");
            tmp = tmp.Replace("-", "_");
            tmp = tmp.Replace(":", "_");
            return tmp;
        }

        /// <summary>
        /// Convert the combobox item string into BINEntry.
        /// </summary>
        /// <param name="comboItem"></param>
        /// <returns></returns>
        private BINEntry GetBINEntry(string comboItem)
        {
            BINEntry entry = myGameBin.GetEntryByName(comboItem);

            if (entry == null)
            {
                // CDef might just be the ID.

                try
                {
                    entry = myGameBin.get_Entries(
                        int.Parse(comboItem.ToString()));
                }
                catch (Exception)
                {
                }
            }

            return entry;
        }

        private string GetAssetName(AssetBank bank, string name)
        {
            if (bank.FindEntryBySymbolName(name) == null)
                return name;

            int id = 1;

            while (id <= 99)
            {
                string tmpName = string.Format("{0}_{1:D2}", name, id);

                if (bank.FindEntryBySymbolName(tmpName) == null)
                    return tmpName;

                ++id;
            }

            return null;
        }

        /// <summary>
        /// Fix the links to self.
        /// </summary>
        /// <param name="entry"></param>
        /// <param name="originalId"></param>
        private void FixMainEntry(BINEntry entry, int originalId)
        {
            DefinitionType def = 
                ContentManager.Instance.Definitions.GetDefinition(
                entry.Definition);

            def.ReadIn(entry);

            def.FixLinks(
                LinkDestination.GameBINEntryID, originalId, entry.ID);

            def.Write(entry);
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            if (textBoxName.Text.Length == 0)
            {
                FormMain.Instance.ErrorMessage(
                    "Invalid base name. It can not be empty.");
                return;
            }

            string name = textBoxName.Text;

            Dictionary<int, BINEntry> binEntries = 
                new Dictionary<int, BINEntry>();
            
            Dictionary<int, AssetEntry> assetEntries = 
                new Dictionary<int, AssetEntry>();

            // Build the objects...

            StringBuilder sb = new StringBuilder();
            sb.AppendLine("Created objects:");

            for (int i = 0; i < myTemplate.ItemCount; ++i)
            {
                BaseTemplate item = myTemplate.get_Items(i);

                bool isRef = myRefCheckBoxes[item.ID].Checked;

                object selItem = myComboBoxes[item.ID].SelectedItem;

                if (selItem == null)
                    continue;

                if (item.GetType() == typeof(DefTypeTemplate))
                {
                    DefTypeTemplate defItem = item as DefTypeTemplate;

                    BINEntry defEntry = GetBINEntry(selItem.ToString());

                    if (defEntry != null)
                    {
                        string objName = defItem.Type+"_"+name;

                        if (!checkBoxNamed.Checked)
                        {
                            // Not on, so check the item flag.
                            if (!item.Named)
                                objName = "";
                        }

                        BINEntry entry = null;

                        if (isRef)
                            entry = defEntry;
                        else
                        {
                            entry = myGameBin.AddEntry(
                                objName,
                                defItem.Type,
                                defEntry.Data);

                            entry.Modified = true;
                            
                            if (!string.IsNullOrEmpty(entry.Name))
                            {
                                sb.AppendFormat("  {0}", entry.Name);
                            }
                            else
                            {
                                sb.AppendFormat("  {0}:{1}", 
                                    entry.Definition, entry.ID);
                            }
                           
                            sb.AppendLine("");
                        }

                        binEntries.Add(item.ID, entry);
                    }
                    else
                    {
                        FormMain.Instance.ErrorMessage("Entry not found");
                    }
                }
                else
                {
                    AssetEntry defEntry = 
                        ((AssetComboBoxItem)selItem).Entry;

                    if (defEntry != null)
                    {
                        AssetEntry entry;

                        if (isRef)
                            entry = defEntry;
                        else
                        {
                            AssetTemplate assetItem = item as AssetTemplate;

                            entry = defEntry.Bank.NewEntry(
                                GetAssetName(
                                    defEntry.Bank,
                                    assetItem.Prefix+"_"+name),
                                defEntry.Type);

                            entry.SubHeader = defEntry.SubHeader;
                            entry.Data = defEntry.Data;

                            sb.AppendFormat("  {0}", entry.DevSymbolName);
                            sb.AppendLine("");
                        }

                        assetEntries.Add(item.ID, entry);
                    }
                }
            }

            // Link objects!

            for (int i = 0; i < myTemplate.ItemCount; ++i)
            {
                BaseTemplate item = myTemplate.get_Items(i);

                if (item.LinkTo == null)
                    continue;

                DefTypeTemplate defLink = item.LinkTo;
                 
                if (item.GetType() == typeof(DefTypeTemplate))
                {
                    DefTypeLink(
                       binEntries[defLink.ID],
                       binEntries[item.ID],
                       item.Name);
                }
                else
                {
                    AssetEntry assetEntry = null;

                    if (assetEntries.TryGetValue(item.ID, out assetEntry))
                    {
                        AssetLink(
                            binEntries[defLink.ID],
                            (AssetTemplate)item,
                            assetEntry.ID);
                    }
                }
            }

            DefTypeTemplate baseItem = 
                (DefTypeTemplate)myTemplate.get_Items(0);
            
            FixMainEntry(binEntries[baseItem.ID], baseItem.OriginalID);

            binEntries = null;
            assetEntries = null;

            FormMain.Instance.InfoMessage(sb.ToString());
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void comboBoxTemplate_DropDownClosed(object sender, EventArgs e)
        {
            if (comboBoxTemplate.SelectedIndex < 0)
            {
                panelTemplate.Visible = false;
                buttonCreate.Enabled = false;
                return;
            }

            BINEntry entry = myGameBin.GetEntryByName(
                comboBoxTemplate.SelectedItem.ToString());

            myTemplate.Build(entry);

            panelProgress.Visible = true;
            panelProgress.Update();

            panelTemplate.Visible = false;
            panelTemplate.Controls.Clear();
            panelTemplate.SuspendLayout();

            myComboBoxes.Clear();
            myRefCheckBoxes.Clear();

            int x = labelTemplate.Left;
            int y = 5;
            int width = 0;
            int height = 0;
            int yspace = 10;

            CheckBox objCheckBox = new CheckBox();

            // This should be the main object type.
            myComboBoxes.Add(
                myTemplate.get_Items(0).ID,
                comboBoxTemplate);

            myRefCheckBoxes.Add(
                myTemplate.get_Items(0).ID,
                objCheckBox);

            comboBoxTemplate.Tag = 0;

            // Start from the child items...
            for (int i = 1; i < myTemplate.ItemCount; ++i)
            {
                Label lbl = new Label();
                lbl.AutoSize = true;

                lbl.Text = myTemplate.get_Items(i).Name+":";

                panelTemplate.Controls.Add(lbl);

                lbl.Left = x;
                lbl.Top = y;

                y += lbl.Height+yspace;

                if (lbl.Width > width)
                    width = lbl.Width;

                if (lbl.Height > height)
                    height = lbl.Height;
            }

            y = 5;

            int right = panelTop.Width-comboBoxDefs.Right;

            progressBar.Value = 0;
            progressBar.Maximum = myTemplate.ItemCount;
            progressBar.Update();

            for (int i = 1; i < myTemplate.ItemCount; ++i)
            {
                BaseTemplate item = myTemplate.get_Items(i);

                ComboBox comboBox = NewComboBox(item.Name);

                panelTemplate.Controls.Add(comboBox);

                comboBox.Left = width+x+5;
                comboBox.Top = y;
                comboBox.Width = comboBoxDefs.Right-comboBox.Left-80;
                comboBox.Anchor |= AnchorStyles.Right;
                comboBox.DropDownStyle = ComboBoxStyle.DropDownList;
                comboBox.AutoCompleteSource = 
                    AutoCompleteSource.ListItems;
                comboBox.AutoCompleteMode = 
                    AutoCompleteMode.SuggestAppend;
                comboBox.Tag = i;
                comboBox.Sorted = true;
                //comboBox.Enabled = string.IsNullOrEmpty(item.LinkTo);

                if (item.GetType() == typeof(DefTypeTemplate))
                {
                    DefTypeTemplate def = item as DefTypeTemplate;

                    if (comboBox.Items.Count == 0)
                    {
                        // Fill it up.

                        BINEntry[] entries = 
                            myGameBin.GetEntriesByDefinition(def.Type);

                        foreach (BINEntry ent in entries)
                        {
                            comboBox.Items.Add((ent.Name.Length > 0) 
                                ? ent.Name : ent.ID.ToString());
                        }
                    }
                }
                else
                {
                    if (comboBox.Items.Count == 0)
                    {
                        // New combobox.
                        AssetFill(comboBox, (AssetTemplate)item);
                    }
                }

                if (comboBox.Items.Count > 0)
                    comboBox.SelectedIndex = 0;

                comboBox.SelectedIndexChanged += 
                    new EventHandler(itemComboBox_SelectedIndexChanged);

                y += height+yspace;

                // We need to access this later.
                myComboBoxes.Add(item.ID, comboBox);

                progressBar.Value++;
                progressBar.Update();   
            }

            // Add ref checkboxes...

            for (int i = 1; i < myTemplate.ItemCount; ++i)
            {
                BaseTemplate item = myTemplate.get_Items(i);

                CheckBox checkBox = new CheckBox();

                panelTemplate.Controls.Add(checkBox);

                checkBox.Left = myComboBoxes[item.ID].Right+5;
                checkBox.Top = myComboBoxes[item.ID].Top;
                checkBox.Text = "Ref";
                checkBox.Checked = item.Ref;
                checkBox.Anchor = AnchorStyles.Top | AnchorStyles.Right;
                toolTip.SetToolTip(
                    checkBox, "Use this object as a reference");

                // We need to access this later.
                myRefCheckBoxes.Add(item.ID, checkBox);
            }

            // Add the boxes to the pool, so we can use them later.

            for (int i = 1; i < myTemplate.ItemCount; ++i)
            {
                BaseTemplate item = myTemplate.get_Items(i);
                AddToPool(item.Name, myComboBoxes[item.ID]);
            }

            UpdateLinks(comboBoxTemplate);

            panelTemplate.ResumeLayout();
            panelTemplate.Visible = true;

            panelTemplate.Visible = true;
            buttonCreate.Enabled = true;

            panelProgress.Visible = false;
            panelProgress.Update();
        }

        private void comboBoxDefs_DropDownClosed(object sender, EventArgs e)
        {
            panelTemplate.Visible = false;

            comboBoxTemplate.Enabled = comboBoxDefs.SelectedIndex >= 0;

            comboBoxTemplate.Items.Clear();

            if (comboBoxDefs.SelectedIndex < 0)
                return;

            BINEntry[] entries = myGameBin.GetEntriesByDefinition(
                comboBoxDefs.SelectedItem.ToString());

            foreach (BINEntry entry in entries)
            {
                if (!string.IsNullOrEmpty(entry.Name))
                    comboBoxTemplate.Items.Add(entry.Name);
            }
        }
    }
}