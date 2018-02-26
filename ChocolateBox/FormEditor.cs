using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections.ObjectModel;

using FableMod.Gfx.Integration;
using FableMod.ContentManagement;
using FableMod.BIN;
using FableMod;
using FableMod.TNG;

namespace ChocolateBox
{
    public partial class FormEditor : Form
    {
        private GfxThingController myController;
        private GfxThingView myView;
        private Collection<ThingMap> myMaps;
        private FableMod.TNG.Thing myCreatedThing;
        private List<FableMod.TNG.Thing> myImports = 
            new List<FableMod.TNG.Thing>();
        private bool myUpdateBlock = false;
        private FableMod.TNG.Thing[] mySelectedThings;
        private string myLastNewEntry = "";
        private FableMod.TNG.Thing myTeleporter = null;
        private FableMod.Gfx.Integration.ThingMap myTeleporterMap = null;
        private string myRegion = "";
        private FableMod.TNG.Variable myUIDVariable;

        private enum CreateMode
        {
            None,
            CreateNew,
            Import,
            SelectTeleporterSrc,
            SelectTeleporterDst,
            SelectObject,
        };

        private CreateMode myCreateMode = CreateMode.None;

        private class MapComboBoxItem
        {
            public ThingMap myMap;

            public MapComboBoxItem(ThingMap map)
            {
                myMap = map;
            }

            public override string ToString()
            {
                return myMap.Name;
            }
        }

        private class SectionComboBoxItem
        {
            public FableMod.TNG.Section mySection;

            public SectionComboBoxItem(FableMod.TNG.Section section)
            {
                mySection = section;
            }

            public override string ToString()
            {
                return mySection.Name;
            }
        }

        private void Init()
        {
            myController = new GfxThingController();

            myView = new GfxThingView();

            myController.AddView(myView);

            panelView.Controls.Add(myView);
            myView.Dock = DockStyle.Fill;
                        
            toolStripButtonNav.Visible =
                Settings.GetBool("Editor", "NavModeOn", false);

            myView.ThingSelected += 
                new ThingSelectedHandler(myView_ThingSelected);

            myView.ThingCreated += 
                new ThingCreateHandler(myView_ThingCreated);
        
            myView.ThingPicked += 
                new ThingPickHandler(myView_ThingPicked);
        }

        private void Thing_SelectUID(FableMod.TNG.Variable variable)
        {
            myView.Mode = EditorMode.Pick;
            myCreateMode = CreateMode.SelectObject;
            myUIDVariable = variable;
            controlThing.Enabled = false;
            //MessageBox.Show("Select an object, please.");
        }

        public FormEditor()
        {
            InitializeComponent();

            Init();
        }

        public FormEditor(string regionName)
        {
            InitializeComponent();

            Init();

            if (string.IsNullOrEmpty(regionName))
                return;

            WLDFileController wldc = FileDatabase.Instance.GetWLD();
            FableMod.WLD.WLDFile wld = wldc.WLD;

            FableMod.WLD.Region region = wld.GetRegion(regionName);

            if (region != null)
            {
                LoadRegion(region);
            }
            else
            {
                FormMain.Instance.ErrorMessage(
                    string.Format("Region {0} not found.", regionName));
            }
        }

        public FormEditor(
            FableMod.WLD.Region region,
            FableMod.TNG.Thing thing,
            FableMod.TNG.Thing teleporter,
            FableMod.Gfx.Integration.ThingMap map)
        {
            InitializeComponent();

            Init();

            SelectTeleporterDestination(thing, teleporter, map);

            LoadRegion(region);
        }

        private void SelectTeleporterDestination(
            FableMod.TNG.Thing thing,
            FableMod.TNG.Thing teleporter,
            FableMod.Gfx.Integration.ThingMap map)
        {
            myTeleporter = teleporter;
            myTeleporterMap = map;
            myCreatedThing = thing;

            myView.Mode = EditorMode.Create;
            myCreateMode = CreateMode.SelectTeleporterDst;
        }

        private void Reset()
        {
            myView.Reset();

            if (myMaps != null)
            {
                for (int i = 0; i < myMaps.Count; ++i)
                    myMaps[i].Destroy();
                
                myMaps.Clear();
            }

            myController.ResetObjects();

            comboBoxEditMap.Items.Clear();
            comboBoxEditSection.Items.Clear();
            comboBoxMap.Items.Clear();

            mySelectedThings = null;
        }

        private void LoadRegion(FableMod.WLD.Region region)
        {
            myView.Activate(false);

            WLDFileController wldc = FileDatabase.Instance.GetWLD();
            FableMod.WLD.WLDFile wld = wldc.WLD;

            string directory = Path.GetDirectoryName(wldc.FileName)+"\\";

            RegionLoader loader = new RegionLoader(
                directory, 
                myController, 
                region, 
                null,
                Settings.GetBool("Editor", "SeesMaps", false));

            FormProcess formProcess = new FormProcess(loader);

            formProcess.ShowDialog();

            formProcess.Dispose();

            myMaps = loader.Maps;

            myView.Name = region.RegionName;

            formProcess.Dispose();

            myView.IsometricCamera();

            comboBoxMap.Items.Clear();

            if (myMaps != null)
            {
                FillMapComboBox(comboBoxMap);
                FillMapComboBox(comboBoxEditMap);
            }

            Text = "Region Editor - "+region.RegionName;

            myView.Maps = myMaps;
            myView.EditSection = "NULL";
            myView.EditMap = myMaps[0];

            panelGeneral.Visible = true;

            if (trackBarDrawDistance.Maximum == 0)
            {
                trackBarDrawDistance.Minimum = 1;
                trackBarDrawDistance.Maximum = (int)myView.DrawDistance;
            }

            trackBarDrawDistance.Value = (int)myView.DrawDistance;

            toolStripMain.Enabled = true;
            
            thingsToolStripMenuItem.Enabled = true;
            editToolStripMenuItem.Enabled = true;
            viewToolStripMenuItem.Enabled = true;
            saveScreenshotAsToolStripMenuItem.Enabled = true;

            // Set the active region name.
            myRegion = region.RegionName;

            myController.ShowOfType(
                "Marker", markersToolStripMenuItem.Checked);
            myController.ShowOfType(
                "AICreature", creaturesToolStripMenuItem.Checked);
            myController.ShowOfType(
                "Object", objectsToolStripMenuItem.Checked);
            
            myView.Activate(true);

            myView.STBDebugFlora = sTBDebugFloraToolStripMenuItem.Checked;
            myView.STBDebugBlocks = sTBDebugBlocksToolStripMenuItem.Checked;
        }

        private void loadRegionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!CheckChanges())
                return;

            WLDFileController wldc = FileDatabase.Instance.GetWLD();
            FableMod.WLD.WLDFile wld = wldc.WLD;

            FormRegionSelect form = new FormRegionSelect();

            for (int i = 0; i < wld.RegionCount; ++i)
            {
                form.listBoxRegions.Items.Add(
                    wld.GetRegion(i).RegionName);
            }

            if (form.ShowDialog() != DialogResult.OK)
            {
                // Cancel!
                return;
            }

            form.Dispose();

            Reset();

            FableMod.WLD.Region region = wld.GetRegion(
                form.listBoxRegions.Items[
                    form.listBoxRegions.SelectedIndex].ToString());

            LoadRegion(region);
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void FillMapComboBox(ComboBox comboBox)
        {
            for (int i = 0; i < myMaps.Count; ++i)
            {
                MapComboBoxItem item = new MapComboBoxItem(myMaps[i]);
                comboBox.Items.Add(item);
            }
        }

        private void UpdateComboBoxes(FableMod.TNG.Thing[] things)
        {
            comboBoxSection.Items.Clear();

            if (things == null)
            {
                comboBoxMap.Enabled = false;
                comboBoxSection.Enabled = false;
                buttonChangeMap.Enabled = false;
                buttonChangeSection.Enabled = false;
            }
            else
            {
                comboBoxMap.Enabled = true;
                comboBoxSection.Enabled = true;
                buttonChangeMap.Enabled = myMaps.Count > 1;
                buttonChangeSection.Enabled = true;
            
                FableMod.TNG.TNGFile tngFile = null;

                FableMod.TNG.TNGFile tng = things[0].Section.TNGFile;
                
                for (int i = 1; i < things.Length; ++i)
                {
                    if (tng != things[i].Section.TNGFile)
                        tng = null;
                }

                comboBoxMap.SelectedIndex = -1;

                for (int i = 0; i < comboBoxMap.Items.Count; ++i)
                {
                    MapComboBoxItem item = 
                        (MapComboBoxItem)comboBoxMap.Items[i];

                    if (item.myMap.TNG == tng)
                    {
                        tngFile = tng;
                        comboBoxMap.SelectedIndex = i;
                        break;
                    }
                }

                comboBoxSection.Items.Clear();

                if (tng != null)
                {
                    FableMod.TNG.Section section = things[0].Section;

                    for (int i = 1; i < things.Length; ++i)
                    {
                        if (section != things[i].Section)
                            section = null;
                    }
        
                    for (int i = 0; i < tng.SectionCount; ++i)
                    {
                        SectionComboBoxItem item = new SectionComboBoxItem(
                            tng.get_Sections(i));

                        int index = comboBoxSection.Items.Add(item);

                        if (item.mySection == section)
                            comboBoxSection.SelectedIndex = index;
                    }
                }
            }
        }

        private void myView_ThingSelected(FableMod.TNG.Thing[] things)
        {
            if (myView.Mode != EditorMode.Normal)
                return;

            mySelectedThings = things;

            if (!panelLeft.Visible)
            {
                // The whole editing panel is not visible.
                return;
            }

            panelNormalMode.Visible = true;

            if (things != null && things.Length == 1)
            {
                controlThing.Thing = things[0];
                controlThing.ApplySelectUID(Thing_SelectUID);
                controlThing.Visible = true;
            }
            else
            {
                controlThing.Visible = false;
            }

            UpdateComboBoxes(things);
        }

        private void myView_ThingPicked(FableMod.TNG.Thing thing)
        {
            if (!string.IsNullOrEmpty(myUIDVariable.Restriction))
            {
                if (thing.Name != myUIDVariable.Restriction)
                {
                    // Invalid.
                    MessageBox.Show("Invalid object. Select \""+
                        myUIDVariable.Restriction+"\", please.");
                    return;
                }
            }

            MessageBox.Show("Selected "+
                thing.Name+":"+thing.DefinitionType+":"+thing.UID+".");

            myUIDVariable.Value = thing.UID;
            myUIDVariable = null;
            myCreateMode = CreateMode.None;
            myView.Mode = EditorMode.Normal;

            controlThing.Enabled = true;
        }

        private void myView_ThingCreated(
            float x, float y, float z,
            float nx, float ny, float nz)
        {
            ThingMap selectedMap = null;

            for (int i = 0; i < myMaps.Count; ++i)
            {
                if (myMaps[i].IsOver(x, y, z))
                {
                    selectedMap = myMaps[i];
                    break;
                }
            }

            if (selectedMap == null)
            {
                FormMain.Instance.ErrorMessage("Click over a map, please.");
                return;
            }

            if (myCreateMode == CreateMode.SelectTeleporterDst)
            {
                float dstx = x-myTeleporterMap.X;
                float dsty = y-myTeleporterMap.Y;

                CTCBlock block = 
                    myTeleporter.get_CTCs("CTCPhysicsStandard");

                if (MessageBox.Show(
                    "Create a two-way teleporter?",
                    "ChocolateBox",
                    MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    FableMod.TNG.Thing myOriginal = myCreatedThing;

                    if (SetupForTeleporter())
                    {
                        selectedMap.AddThing(
                            myController,
                            myCreatedThing,
                            "NULL",
                            x, y, z,
                            nx, ny, nz);

                        CTCBlock blockSrc = 
                            myOriginal.get_CTCs("CTCPhysicsStandard");
                       
                        CTCBlock blockDst = 
                            myTeleporter.get_CTCs("CTCPhysicsStandard");

                        blockDst.get_Variables("PositionX").Value = 
                            (myTeleporterMap.X+
                            (float)blockSrc.get_Variables("PositionX").Value)-
                            selectedMap.X;

                        blockDst.get_Variables("PositionY").Value = 
                            (myTeleporterMap.Y+
                            (float)blockSrc.get_Variables("PositionY").Value)-
                            selectedMap.Y;
                        
                        blockDst.get_Variables("PositionZ").Value = 
                            ((float)blockSrc.get_Variables(
                                "PositionZ").Value)+0.5f;

                        selectedMap.AddThing(
                            myController,
                            myTeleporter,
                            "NULL");

                        // So we spawn over the thing.
                        z += 0.5f;
                    }
                }
                else if (MessageBox.Show(
                    "Create a teleportation platform?\r\n"+
                    "This object is a visual que only.",
                    "ChocolateBox",
                    MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    FableMod.TNG.Thing platform = 
                         new FableMod.TNG.Thing("Object");

                    platform.Create(
                        FileDatabase.Instance.TNGDefinitions,
                        "OBJECT_GUILD_PEDESTAL_TELEPORT_01");

                    platform.UID = FableMod.TNG.UIDManager.Generate();
                    platform.Player = 4;
                    
                    selectedMap.AddThing(
                        myController,
                        platform,
                        "NULL",
                        x, y, z,
                        nx, ny, nz);
                
                    // So we spawn over the thing.
                    z += 0.5f;    
                }
                
                // Set the jump coordinates.
                block.get_Variables("PositionX").Value = dstx;
                block.get_Variables("PositionY").Value = dsty;
                block.get_Variables("PositionZ").Value = z;

                FormMain.Instance.InfoMessage(string.Format(
                    "Teleporter created from map \"{0}\" to map \"{1}\".",
                    myTeleporterMap.Name,
                    selectedMap.Name));

                myCreateMode = CreateMode.None;
                myView.Mode = EditorMode.Normal;

                //Close();

                return;
            }
            else if (myCreateMode == CreateMode.SelectTeleporterSrc)
            {
                try
                {
                    selectedMap.AddThing(
                        myController,
                        myCreatedThing,
                        "NULL",
                        x, y, z,
                        nx, ny, nz);

                    selectedMap.AddThing(
                        myController,
                        myTeleporter,
                        "NULL");
                }
                catch (Exception ex)
                {
                    FormMain.Instance.ErrorMessage(ex.Message);
                    return;
                }

                WLDFileController wldc = 
                    FileDatabase.Instance.GetWLD();
                FableMod.WLD.WLDFile wld = wldc.WLD;

                FormRegionSelect form = new FormRegionSelect();

                form.Text = "Select Destination Region";

                for (int i = 0; i < wld.RegionCount; ++i)
                {
                    form.listBoxRegions.Items.Add(
                        wld.GetRegion(i).RegionName);
                }

                if (form.ShowDialog() == DialogResult.OK)
                {
                    FableMod.WLD.Region region = wld.GetRegion(
                        form.listBoxRegions.Items[
                            form.listBoxRegions.SelectedIndex].
                            ToString());

                    if (region.RegionName != myRegion)
                    {
                        FormEditor formEditor = new FormEditor(
                            region,
                            myCreatedThing,
                            myTeleporter,
                            selectedMap);

                        FormMain.Instance.AddMDI(formEditor);
                    }
                    else
                    {
                        // Use this window.
                        SelectTeleporterDestination(
                            myCreatedThing, myTeleporter, selectedMap);
                        
                        return;
                    }
                }
                else
                {
                    myController.RemoveThing(myCreatedThing);
                    myController.RemoveThing(myTeleporter);
                }

                form.Dispose();

                myCreatedThing = null;

                // Reset variable.
                myTeleporter = null;
            }
            else if (myCreateMode == CreateMode.CreateNew)
            {
                try
                {
                    selectedMap.AddThing(
                        myController,
                        myCreatedThing,
                        "NULL",
                        x, y, z,
                        nx, ny, nz);
                }
                catch (Exception ex)
                {
                    FormMain.Instance.ErrorMessage(ex.Message);
                }

                myCreatedThing = null;
            }
            else if (myCreateMode == CreateMode.Import)
            {
                for (int i = 0; i < myImports.Count; ++i)
                {
                    FableMod.TNG.Thing thing = myImports[i];

                    selectedMap.AddThing(
                        myController, thing, "NULL");
                }
            }

            myImports.Clear();
            myCreateMode = CreateMode.None;
            myView.Mode = EditorMode.Normal;
        }

        private void thing_Changed(FableMod.TNG.Thing thing)
        {
            if (!myView.IsDoing)
                myView.UpdateAll();
        }

        private void topToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myView.TopCamera();
        }

        private void frontToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myView.FrontCamera();
        }

        private void rightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myView.RightCamera();
        }

        private bool CheckChanges()
        {
            if (myMaps == null)
                return true;

            toolStripButtonWorld_Click(null, null);

            List<Processor> processors = new List<Processor>();

            for (int i = 0; i < myMaps.Count; ++i)
            {
                ThingMap map = myMaps[i];

                if (map.Modified)
                {
                    DialogResult result = MessageBox.Show(
                        "Save changes to map "+map.Name+"?",
                        FormMain.Instance.Title,
                        MessageBoxButtons.YesNoCancel,
                        MessageBoxIcon.Exclamation);

                    if (result == DialogResult.Yes)
                    {
                        processors.Add(
                            new MapSaveProcessor(map.TNG, map.LEV));
                    }
                    else if (result == DialogResult.Cancel)
                    {
                        return false;
                    }
                }
            }

            if (processors.Count > 0)
            {
                FormProcess form = new FormProcess(processors);

                form.ShowDialog();

                form.Dispose();
            }

            processors = null;

            return true;
        }

        private void FormEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!CheckChanges())
            {
                e.Cancel = true;
                return;
            }

            Reset();

            // Release all resources.
            FableMod.Gfx.Integration.GfxManager.GetModelManager().Clear();
            FableMod.Gfx.Integration.GfxManager.GetTextureManager().Clear();
            FableMod.Gfx.Integration.GfxManager.GetThemeManager().Clear();

            myView.Destroy();
            myController.Destroy();
            
            myController = null;
            myMaps = null;
            myView = null;
        }

        private void showDirectionAxesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myView.DirectionAxes = !myView.DirectionAxes;
        }

        private void viewToolStripMenuItem_Paint(object sender, PaintEventArgs e)
        {
            showDirectionAxesToolStripMenuItem.Checked =
                myView.DirectionAxes;
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            object obj = 
                ContentManager.Instance.SelectEntry(
                new Link(
                    LinkDestination.GameBINEntryName,
                    null),
                myLastNewEntry);

            if (obj != null)
            {
                ContentObject o = ContentManager.Instance.FindEntry(
                    LinkDestination.GameBINEntryName, obj);

                if (o == null)
                {
                    FormMain.Instance.ErrorMessage(
                        "Object could not be retrieved");
                    return;
                }

                BINEntry entry = (BINEntry)o.Object;

                string thingName = "Object";

                if (entry.Definition == "BUILDING")
                    thingName = "Building";
                else if (entry.Definition == "CREATURE")
                    thingName = "AICreature";
                else if (entry.Definition == "MARKER")
                    thingName = "Marker";
                else if (entry.Definition == "HOLY_SITE")
                    thingName = "Holy Site";
                else if (entry.Definition == "VILLAGE")
                    thingName = "Village";
                
                myCreatedThing = new FableMod.TNG.Thing(thingName);

                myCreatedThing.Create(
                    FileDatabase.Instance.TNGDefinitions,
                    entry.Name);

                myLastNewEntry = entry.Name;

                myCreatedThing.UID = FableMod.TNG.UIDManager.Generate();
                myCreatedThing.Player = 4;

                entry = null;
                o = null;

                // Change to creation mode.
                myView.Mode = EditorMode.Create;
                myCreateMode = CreateMode.CreateNew;
            }
        }

        private void ImportTNG(string fileName)
        {
            FableMod.TNG.TNGFile tng = new FableMod.TNG.TNGFile(
                FileDatabase.Instance.TNGDefinitions);

            tng.Load(fileName);

            for (int i = 0; i < tng.SectionCount; ++i)
            {
                for (int j = 0; j < tng.get_Sections(i).Things.Count; ++j)
                {
                    myImports.Add(tng.get_Sections(i).Things[j]);
                }
            }

            // Do some initial setup.

            for (int i = 0; i < myImports.Count; ++i)
            {
                FableMod.TNG.Thing thing = myImports[i];
                thing.UID = FableMod.TNG.UIDManager.Generate();
                thing.Detach();
            }

            tng.Destroy();

            if (myImports.Count == 0)
            {
                FormMain.Instance.ErrorMessage("No things");
                return;
            }

            myView.Mode = EditorMode.Create;
            myCreateMode = CreateMode.Import;
        }

        private void importToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();

            fd.InitialDirectory = Settings.FableDirectory;
            fd.Filter = "TNG Files (*.tng)|*.tng|All Files (*.*)|*.*||";
            fd.DefaultExt = "tng";

            if (fd.ShowDialog() == DialogResult.OK)
            {
                ImportTNG(fd.FileName);
            }

            fd.Dispose();
        }

        public void SaveChanges(bool quiet)
        {
            StringBuilder sb = new StringBuilder();

            if (myMaps != null)
            {
                myView.SaveChanges();

                sb.AppendFormat("Saved files:\r\n");

                for (int i = 0; i < myMaps.Count; ++i)
                {
                    if (myMaps[i].Modified)
                    {
                        sb.AppendLine("\t"+myMaps[i].TNG.FileName);

                        myMaps[i].TNG.Save(myMaps[i].TNG.FileName);
                        myMaps[i].TNG.Modified = false;
                    }
                }

                if (!quiet)
                    FormMain.Instance.InfoMessage(sb.ToString());
            }

            sb = null;
        }

        private void saveRegionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveChanges(false);   
        }

        private void fileToolStripMenuItem_Paint(object sender, PaintEventArgs e)
        {
            importHeightfieldToolStripMenuItem.Enabled = myMaps.Count > 0;

            if (myMaps != null)
            {
                for (int i = 0; i < myMaps.Count; ++i)
                {
                    if (myMaps[i].Modified)
                    {
                        saveRegionToolStripMenuItem.Enabled = true;
                        return;
                    }
                }
            }

            saveRegionToolStripMenuItem.Enabled = false;
        }

        private void toolStripButtonWalk_Click(object sender, EventArgs e)
        {
            comboBoxEditMap.SelectedIndex = 
                comboBoxEditMap.FindStringExact(myView.EditMap.Name);
            
            toolStripButtonWalk.Checked = true;
            toolStripButtonWorld.Checked = false;
            toolStripButtonNav.Checked = false;

            thingsToolStripMenuItem.Enabled = false;

            panelNormalMode.Visible = false;
            panelNavMode.Visible = false;
            panelEditNav.Visible = false;
            panelWalkNav.Visible = true;

            myView.Mode = EditorMode.Walk;
        }

        private void toolStripButtonWorld_Click(object sender, EventArgs e)
        {
            thingsToolStripMenuItem.Enabled = true;
            
            myView.Mode = EditorMode.Normal;

            toolStripButtonWorld.Checked = true;
            toolStripButtonWalk.Checked = false;
            toolStripButtonNav.Checked = false;

            panelWalkNav.Visible = false;
            panelNavMode.Visible = false;
            panelEditNav.Visible = false;
        }

        private void toolStripButtonNav_Click(object sender, EventArgs e)
        {
            myView.Mode = EditorMode.Navigation;

            toolStripButtonNav.Checked = true;
            toolStripButtonWalk.Checked = false;
            toolStripButtonWorld.Checked = false;
            
            thingsToolStripMenuItem.Enabled = false;

            panelNormalMode.Visible = false;
            
            // Ordering.
            panelEditNav.Visible = true;
            panelWalkNav.Visible = true;
            panelNavMode.Visible = true;
            
            comboBoxEditMap.SelectedIndex = 
                comboBoxEditMap.FindStringExact(myView.EditMap.Name);

            myUpdateBlock = true;

            comboBoxEditSection.SelectedIndex =
                comboBoxEditSection.FindStringExact(myView.EditSection);

            numericUpDownSubset.Value = myView.EditSubset;
            
            myUpdateBlock = false;
        }

        private void numericUpDownBrush_ValueChanged(object sender, EventArgs e)
        {
            myView.BrushSize = (int)numericUpDownBrush.Value;
        }

        private void panelBrush_Paint(object sender, PaintEventArgs e)
        {
            numericUpDownBrush.Value = myView.BrushSize;
        }

        private void FillEditSectionComboBox(ThingMap map)
        {
            comboBoxEditSection.Items.Clear();

            FableMod.LEV.LEVFile file = map.LEV;

            for (int i = 0; i < file.SectionCount; ++i)
            {
                comboBoxEditSection.Items.Add(file.get_Sections(i).Name);
            }
        }

        private void comboBoxEditMap_SelectedIndexChanged(object sender, EventArgs e)
        {
            MapComboBoxItem item = 
                (MapComboBoxItem)comboBoxEditMap.SelectedItem;
            
            myView.EditMap = item.myMap;

            FillEditSectionComboBox(item.myMap);

            myView.Focus();
        }

        private void FormEditor_Load(object sender, EventArgs e)
        {
        }

        private void comboBoxEditSection_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (myUpdateBlock)
                return;

            myView.EditSection = comboBoxEditSection.Items[
                comboBoxEditSection.SelectedIndex].ToString();
        }

        private void numericUpDownSubset_ValueChanged(object sender, EventArgs e)
        {
            if (myUpdateBlock)
                return;

            myView.EditSubset = (int)numericUpDownSubset.Value;
        }

        private void ShowEditorTools(bool show)
        {
            editorToolsToolStripMenuItem.Checked = show;
            panelLeft.Visible = show;
            splitterMain.Visible = show;
        }

        private void editorToolsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowEditorTools(!editorToolsToolStripMenuItem.Checked);
        }

        private void buttonChangeSection_Click(object sender, EventArgs e)
        {
            SectionComboBoxItem item = (SectionComboBoxItem)
                comboBoxSection.Items[comboBoxSection.SelectedIndex];

            if (item != null)
            {
                buttonChangeSection.Enabled = false;
                buttonChangeSection.Update();

                foreach (FableMod.TNG.Thing thing in mySelectedThings)
                {
                    if (thing.Section != item.mySection)
                    {
                        thing.Detach();
                        item.mySection.AddThing(thing);
                    }
                }

                buttonChangeSection.Enabled = true;
            }
        }

        private void radioButtonNav_CheckedChanged(object sender, EventArgs e)
        {
            myView.NavEditMode = NavigationEditMode.Navigation;
        }

        private void radioButtonDynamic_CheckedChanged(object sender, EventArgs e)
        {
            myView.NavEditMode = NavigationEditMode.Dynamic;
        }

        private void trackBarDrawDistance_ValueChanged(object sender, EventArgs e)
        {
            myView.DrawDistance = trackBarDrawDistance.Value;
        }

        private void buttonResetNav_Click(object sender, EventArgs e)
        {
            myView.ResetNavSections();

            FillEditSectionComboBox(myView.EditMap);
        }

        private void buttonNewSection_Click(object sender, EventArgs e)
        {
            MessageBox.Show(this, "not yet implemented");
        }

        private void buttonAddSubset_Click(object sender, EventArgs e)
        {
            MessageBox.Show(this, "not yet implemented");
        }

        private void buttonChangeMap_Click(object sender, EventArgs e)
        {
            MapComboBoxItem item = (MapComboBoxItem)
                comboBoxMap.Items[comboBoxMap.SelectedIndex];

            if (item != null)
            {
                FableMod.TNG.Section section =
                    item.myMap.TNG.get_Sections("NULL");

                if (section == null)
                {
                    section = new Section("NULL");
                    item.myMap.TNG.AddSection(section);
                }

                buttonChangeMap.Enabled = false;
                buttonChangeMap.Update();

                foreach (FableMod.TNG.Thing thing in mySelectedThings)
                {
                    thing.Detach();
                    section.AddThing(thing);
                }

                buttonChangeMap.Enabled = true;
            }
        }

        private void fullScreenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            fullScreenToolStripMenuItem.Checked = 
                !fullScreenToolStripMenuItem.Checked;

            if (fullScreenToolStripMenuItem.Checked)
            {
                WindowState = FormWindowState.Maximized;
                ShowEditorTools(false);
                FormMain.Instance.ShowFileList(false);
            }
            else
            {
                WindowState = FormWindowState.Normal;
                ShowEditorTools(true);
                FormMain.Instance.ShowFileList(true);
            }
        }

        private void saveScreenshotAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myView.SaveScreenshot();
        }

        private bool SetupForTeleporter()
        {
            object obj = 
                ContentManager.Instance.SelectEntry(
                new Link(LinkDestination.GameBINEntryName, "OBJECT"),
                "OBJECT_GUILD_PEDESTAL_TELEPORT_01");

            if (obj != null)
            {
                ContentObject o = ContentManager.Instance.FindEntry(
                    LinkDestination.GameBINEntryName, obj);

                BINEntry entry = (BINEntry)o.Object;

                if (entry.Definition != "OBJECT")
                {
                    FormMain.Instance.ErrorMessage(
                        "Invalid object selected.");
                    return false;
                }

                myCreatedThing = new FableMod.TNG.Thing("Object");

                myCreatedThing.Create(
                    FileDatabase.Instance.TNGDefinitions,
                    entry.Name);

                myCreatedThing.UID = FableMod.TNG.UIDManager.Generate();
                myCreatedThing.Player = 4;
                myCreatedThing.get_Variables("ScriptData").Value =
                    "Teleport?";

                entry = null;
                o = null;

                CTCBlock block = (CTCBlock)myCreatedThing.ApplyCTC(
                    FileDatabase.Instance.TNGDefinitions,
                    "CTCActionUseScriptedHook");

                block.get_Variables("Usable").Value = true;
                block.get_Variables("ForceConfirmation").Value = true;
                block.get_Variables("TeleportToRegionEntrance").Value =
                    true;

                ElementArray arr = (ElementArray)
                    myCreatedThing.Find("CreateTC");

                Variable var = (Variable)arr.Add();
                var.Value = "CTCActionUseScriptedHook";

                // Create the teleporter marker.

                FableMod.TNG.Thing marker = new FableMod.TNG.Thing("Marker");

                marker.Create(
                    FileDatabase.Instance.TNGDefinitions,
                    "MARKER_BASIC");

                marker.UID = FableMod.TNG.UIDManager.Generate();
                marker.Player = 4;

                // Set the teleport connector.
                block.get_Variables("EntranceConnectedToUID").Value =
                    marker.UID;

                // We're creating a teleporter...
                myTeleporter = marker;

                return true;
            }

            return false;
        }

        private void teleporterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (SetupForTeleporter())
            {
                // Now, we need to pick the position.
                myView.Mode = EditorMode.Create;
                myCreateMode = CreateMode.SelectTeleporterSrc;
            }
        }

        private void FormEditor_Shown(object sender, EventArgs e)
        {
            if (myCreateMode == CreateMode.SelectTeleporterDst)
            {
                myController.UpdateObjects();

                FormMain.Instance.InfoMessage(
                    "Next, select the teleporter destination.");

                myView.Focus();
            }
        }

        private void findByUIDToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FableMod.Forms.FormTextBox form = 
                new FableMod.Forms.FormTextBox();

            form.Text = "Find by UID";
            form.labelInput.Text = "UID:";

            if (form.ShowDialog() == DialogResult.OK)
            {
                myView.FindByUID(form.textBoxInput.Text);
            }
        }

        private void FormEditor_Activated(object sender, EventArgs e)
        {
            myView.Activate(true);
        }

        private void FormEditor_Deactivate(object sender, EventArgs e)
        {
            if (myView != null)
                myView.Activate(false);
        }

        private void unfreezeAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myController.FreezeAll(false);
        }

        private void markersToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myController.ShowOfType("Marker", markersToolStripMenuItem.Checked);
            myView.ClearSelection();
            myView.Render();
        }

        private void creaturesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myController.ShowOfType("AICreature", creaturesToolStripMenuItem.Checked);
            myView.ClearSelection();
            myView.Render();
        }

        private void objectsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myController.ShowOfType("Object", objectsToolStripMenuItem.Checked);
            myView.ClearSelection();
            myView.Render();
        }

        private void sTBDebugFloraToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            myView.STBDebugFlora = sTBDebugFloraToolStripMenuItem.Checked;
        }

        private void sTBDebugBlocksToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myView.STBDebugBlocks = sTBDebugBlocksToolStripMenuItem.Checked;
        }

        private void importHeightfieldToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int minX = int.MaxValue;
            int minY = int.MaxValue;
            int maxX = int.MinValue;
            int maxY = int.MinValue;

            for (int i = 0; i < myMaps.Count; ++i)
            {
                ThingMap map = myMaps[i];
                
                if (map.MapX < minX)
                {
                    minX = map.MapX;
                }

                if (map.MapY < minY)
                {
                    minY = map.MapY;
                }

                if (map.MapX + map.MapWidth > maxX)
                {
                    maxX = map.MapX + map.MapWidth;
                }
                
                if (map.MapY + map.MapHeight > maxY)
                {
                    maxY = map.MapY + map.MapHeight;
                }
            }

            int width = maxX - minX;
            int height = maxY - minY;

            FormMain.Instance.InfoMessage(string.Format("TODO: {0} {1}", width, height));
        }

        private void floraToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < myMaps.Count; ++i)
            {
                Map map = myMaps[i];

                map.EnableFlora(floraToolStripMenuItem.Checked);
            }

            Refresh();
        }
    }
}