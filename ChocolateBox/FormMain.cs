using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;
using System.Xml;
using System.Diagnostics;

using FableMod.ContentManagement;
using FableMod.TNG;
using FableMod.Content.Forms;
using FableMod.Forms;
using FableMod.BBB;

namespace ChocolateBox
{
    public partial class FormMain : FableMod.Forms.FormApp,
        FileInterface
    {
        delegate void AddToFileListDelegate(string fileName);
        delegate void AddMDIDelegate(Form form);

        private FileDatabase myFileDB = null;
        private DefinitionDB myDefinitionDB = new DefinitionDB();
        private TNGDefinitions myTNGDefinitions = new TNGDefinitions();
        private string[] myArgs;

        public static new FormMain Instance
        {
            get { return (FormMain)FormApp.Instance; }
        }

        public FormMain(string[] args)
        {
            InitializeComponent();

            myArgs = args;
        }

        public void AddToFileList(string fileName)
        {
            if (listBoxFiles.InvokeRequired)
            {
                AddToFileListDelegate cb = new AddToFileListDelegate(
                    AddToFileList);

                Invoke(cb, fileName);
            }
            else
            {
                if (listBoxFiles.Items.IndexOf(fileName) < 0)
                    listBoxFiles.Items.Add(fileName);
            }
        }

        public void OnLoadFile(FileController c)
        {
            if (c.UserAccess)
            {
                AddToFileList(c.RelativeFileName);
            }
        }

        private void SaveChanges()
        {
            List<Processor> processors = new List<Processor>();

            lock (myFileDB)
            {
                for (int i = 0; i < myFileDB.FileCount; ++i)
                {
                    FileController c = myFileDB.GetFileAt(i);

                    if (c.Modified)
                    {
                        // Add a save processor.
                        processors.Add(new FileProcessor(c, ""));
                    }
                }
            }

            FormProcess form = new FormProcess(processors);

            form.ShowDialog();

            form = null;

            processors.Clear();
            processors = null;
        }

        private void LoadStartupFiles()
        {
            string startupFiles = Settings.Directory+
                Settings.GetString("Settings", "StartupFile");

            List<Processor> processors = new List<Processor>();

            try
            {
                XmlDocument doc = new XmlDocument();

                doc.Load(startupFiles);

                XmlNode root = doc["files"];

                XmlNode child = root.FirstChild;

                while (child != null)
                {
                    if (child.Name == "file")
                    {
                        XmlAttribute fileName = child.Attributes["name"];
                        XmlAttribute form = child.Attributes["form"];

                        if (fileName != null)
                        {
                            FileController c = 
                                myFileDB.Get(fileName.InnerText);

                            processors.Add(new FileProcessor(
                                c, FileProcessorMode.LoadFile));
                            
                            if (bool.Parse(form.InnerText))
                            {
                                processors.Add(new FileProcessor(
                                    c, FileProcessorMode.LoadForm));
                            }
                        }
                    }

                    child = child.NextSibling;
                }

                doc = null;
            }
            catch (Exception ex)
            {
                ErrorMessage(ex.Message);
                Close();
            }

            FormProcess formProcess = new FormProcess(processors);

            formProcess.ShowDialog();

            formProcess.Dispose();

            formProcess = null;
        }

        public void AddMDI(Form form)
        {
            if (InvokeRequired)
            {
                AddMDIDelegate cb = new AddMDIDelegate(AddMDI);
                Invoke(cb, form);
            }
            else
            {
                form.MdiParent = this;
                form.Show();
            }
        }

        private FileController LoadFile(string fileName)
        {
            List<Processor> processors = new List<Processor>();

            FileController c = myFileDB.Get(fileName);

            c.LoadProcess(processors, true);
            
            FormProcess form = new FormProcess(processors);

            form.ShowDialog();

            form.Dispose();

            return c;
        }

        private void listBoxFiles_DoubleClick(object sender, EventArgs e)
        {
            // Open file.
            int index = listBoxFiles.SelectedIndex;

            if (index >= 0)
            {
                LoadFile(listBoxFiles.Items[index].ToString());
            }
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string fablePath = Settings.FableDirectory;

            openFileDialog.InitialDirectory = fablePath;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                List<Processor> processors = new List<Processor>();

                for (int i = 0; i < openFileDialog.FileNames.Length; ++i)
                {
                    FileController c = 
                        myFileDB.Get(openFileDialog.FileNames[i]);

                    if (c == null)
                    {
                        ErrorMessage("Unknown file format");
                        return;
                    }

                    c.LoadProcess(processors);
                }

                FormProcess form = new FormProcess(processors);

                form.ShowDialog();

                form.Dispose();

                form = null;
            }
        }

        private void LoadUIDs()
        {
            string uidfile = 
                Settings.GetString("Settings", "UIDFile");

            if (string.IsNullOrEmpty(uidfile))
                return;

            string outputFile = Folder + uidfile;

            try
            {
                FableMod.TNG.UIDManager.LoadFromFile(outputFile);
            }
            catch (IOException)
            {
                // Couldn't load the UID file, so generate it.
                GenerateUIDDatabase();
            }
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            try
            {
                Settings.Load();
          
                myDefinitionDB.Load(Folder+
                    Settings.GetString("Settings", "DefinitionsFile"));

                DefinitionDB.EnableDeveloperMode(
                    Settings.GetBool("DefTypes", "DevModeOn", false));

                myTNGDefinitions.Load(Folder+
                    Settings.GetString("Settings", "TNGDefinitionsFile"));

                myFileDB = new FileDatabase(
                    this, myDefinitionDB, myTNGDefinitions);

                ModEnvironmentManager envManager = new ModEnvironmentManager();

                if (!envManager.Check())
                {
                    ErrorMessage(
                        "Modding environment is not acceptable.\r\n"+
                        "Application will not exit.");

                    Close();

                    return;
                }

                LoadStartupFiles();

                FableMod.Gfx.Integration.GfxManager.Initialize(
                    this, 
                    Settings.GetInt("Resolution", "Width", 1024),
                    Settings.GetInt("Resolution", "Height", 768));

                FableMod.Gfx.Integration.GfxManager.SetDirectory(Folder);

                LoadUIDs();

                newModPackageToolStripMenuItem.Enabled =
                    Settings.GetBool("Settings", "ModPackages", false);
                loadModPackageToolStripMenuItem.Enabled =
                    newModPackageToolStripMenuItem.Enabled;

                completeUIDCheckToolStripMenuItem.Enabled =
                    Settings.GetBool("Settings", "UIDCheck", false);

                if (myArgs == null || myArgs.Length == 0)
                {
                    timerTip.Enabled = true;
                }
            }
            catch (Exception ex)
            {
                ErrorMessage(ex.Message+"\r\n"+ex.ToString());
                Close();
            }

            ProcessCommandLine();
        }

        private void ProcessCommandLine()
        {
            if (myArgs != null && myArgs.Length > 0)
            {
                for (int i = 0; i < myArgs.Length; ++i)
                {
                    string arg = myArgs[i].ToLower();

                    if ((arg == "/w" || arg == "-w") && i+1 < myArgs.Length)
                    {
                        string[] window = myArgs[++i].ToLower().Trim().Split(new char[] { ':' });
                        
                        if (window[0] == "editor" || window[0] == "regioneditor")
                        {
                            FormEditor formEditor = new FormEditor(window.Length > 0 ? window[1] : "");
                            AddMDI(formEditor);
                        }
                    }
                }
            }
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                SaveUIDDatabase();

                bool modified = false;

                if (myFileDB != null)
                {
                    lock (myFileDB)
                    {
                        for (int i = 0; i < myFileDB.FileCount; ++i)
                        {
                            if (myFileDB.GetFileAt(i).Modified)
                            {
                                modified = true;
                                break;
                            }
                        }
                    }
                }

                if (modified)
                {
                    DialogResult Result = MessageBox.Show(
                        "Save changes?\r\n"+
                        "Warning! You should always have backups of the original files.",
                        Text,
                        MessageBoxButtons.YesNoCancel,
                        MessageBoxIcon.Exclamation);

                    if (Result == DialogResult.Yes)
                    {
                        SaveChanges();
                    }
                    else if (Result == DialogResult.Cancel)
                    {
                        e.Cancel = true;
                        return;
                    }
                }
             
                FableMod.Gfx.Integration.GfxManager.Destroy();
            }
            catch (Exception ex)
            {
                ErrorMessage(ex.Message+"\r\n"+ex.ToString());
                Close();
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void editorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormEditor formEditor = new FormEditor();
            AddMDI(formEditor);
        }

        public void ShowFileList(bool show)
        {
            panelLeft.Visible = show;
            splitterMain.Visible = show;
        }

        private void fileListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            fileListToolStripMenuItem.Checked = 
                !fileListToolStripMenuItem.Checked;
            ShowFileList(fileListToolStripMenuItem.Checked);
        }

        public void CloseChildren(Type exclude)
        {
            for (int i = 0; i < MdiChildren.Length; ++i)
            {
                if (MdiChildren[i].GetType() == typeof(FormEditor))
                {
                    ((FormEditor)MdiChildren[i]).SaveChanges(true);
                    break;
                }
            }

            int j = 0;

            while (j < MdiChildren.Length)
            {
                if (MdiChildren[j].GetType() != exclude)
                {
                    MdiChildren[j].Close();
                }
                else
                    ++j;
            }
        }

        private void saveAllAndRunFableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process[] fable = Process.GetProcessesByName("Fable");
            
            if (fable.Length != 0)
            {
                ErrorMessage("Fable is still running.");
                return;
            }

            for (int i = 0; i < MdiChildren.Length; ++i)
            {
                if (MdiChildren[i].GetType() == typeof(FormEditor))
                {
                    ((FormEditor)MdiChildren[i]).SaveChanges(true);
                    break;
                }
            }
            
            // Close all windows.

            while (MdiChildren.Length > 0)
                MdiChildren[0].Close();

            SaveChanges();

            myFileDB.CloseFiles();

            Directory.SetCurrentDirectory(Settings.FableDirectory);

            // Release gfx resources.
            FableMod.Gfx.Integration.GfxManager.Destroy();

            Process.Start(Settings.FableDirectory+"fable.exe");

            // Reinitialize the graphics engine.
            FableMod.Gfx.Integration.GfxManager.Initialize(
                this,
                Settings.GetInt("Resolution", "Width", 1024),
                Settings.GetInt("Resolution", "Height", 768));
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormAbout form = new FormAbout();

            form.ShowDialog(this);

            form.Dispose();
        }

        private void newModPackageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormModPackage form = new FormModPackage();
            AddMDI(form);
        }

        private void loadModPackageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialogMod.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    FormModPackage form = new FormModPackage(
                        openFileDialogMod.FileName);

                    AddMDI(form);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(
                        this, "Failed to load FMP: "+ex.Message);
                }
            }
        }

        private void FormMain_Shown(object sender, EventArgs e)
        {
            timerTip.Enabled = Settings.GetBool("Settings", "Tips", true);
        }

        private void timerTip_Tick(object sender, EventArgs e)
        {
            timerTip.Enabled = false;

            FormDayTip form = new FormDayTip();
            form.Owner = this;

            form.ShowDialog();

            form.Dispose();
        }

        private void SaveUIDDatabase()
        {
            string uidfile = Settings.GetString("Settings", "UIDFile");

            if (!string.IsNullOrEmpty(uidfile))
            {
                // Save the file. 
                // Note that the UIDManager checks if it has anything.
                FableMod.TNG.UIDManager.WriteToFile(Folder + uidfile);
            }
        }

        private void GenerateUIDDatabase()
        {
            FormProcess form = new FormProcess(new UIDDatabaseProcessor());

            form.ShowDialog();

            form.Dispose();

            SaveUIDDatabase();
        }

        private void generateUIDDatabaseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult Result = MessageBox.Show(
                "Warning! Are you sure you want to do this?",
                Text,
                MessageBoxButtons.YesNo,
                MessageBoxIcon.Exclamation);

            if (Result != DialogResult.Yes)
            {
                return;
            }

            GenerateUIDDatabase();
        }

        private void completeUIDCheckToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult Result = MessageBox.Show(
                "Warning! Are you sure you want to do this?",
                Text,
                MessageBoxButtons.YesNo,
                MessageBoxIcon.Exclamation);

            if (Result != DialogResult.Yes)
            {
                return;
            }

            UIDCheckProcessor proc = new UIDCheckProcessor();

            FormProcess form = new FormProcess(proc);

            form.ShowDialog();

            form.Dispose();

            InfoMessage(string.Format(
                "Changed {0} object(s) and saved {0} file(s).",
                proc.ChangedCount,
                proc.SavedCount));
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Open file.
            int index = listBoxFiles.SelectedIndex;

            if (index >= 0)
                LoadFile(listBoxFiles.Items[index].ToString());
        }

        private void backupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int index = listBoxFiles.SelectedIndex;

            if (index < 0)
            {
                return;
            }

            string relName = listBoxFiles.Items[index].ToString();
 
            string backupName =
                Settings.FableDirectory+"Backups\\"+relName;

            if (File.Exists(backupName))
            {
                if (MessageBox.Show("Backup already exists. Overwrite?",
                    "ChocolateBox",
                    MessageBoxButtons.YesNo,
                    MessageBoxIcon.Exclamation) != DialogResult.Yes)
                {
                    return;
                }
            }

            Directory.CreateDirectory(
                Path.GetDirectoryName(backupName));

            string tmp = statusStrip.Items[0].Text;
            statusStrip.Items[0].Text = 
                "Backing up "+relName+", please wait...";
            statusStrip.Update();

            File.Copy(Settings.FableDirectory+relName, backupName, true);

            statusStrip.Items[0].Text = tmp;

            InfoMessage("Backup done for "+relName+".");
        }

        private void rollBackToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int index = listBoxFiles.SelectedIndex;

            string relName = listBoxFiles.Items[index].ToString();

            string backupName =
                Settings.FableDirectory+"Backups\\"+relName;

            if (MessageBox.Show("Are you sure you want to rollback?",
                "ChocolateBox",
                MessageBoxButtons.YesNo,
                MessageBoxIcon.Exclamation) == DialogResult.Yes)
            {
                string tmp = statusStrip.Items[0].Text;
                statusStrip.Items[0].Text = 
                    "Rolling back "+relName+", please wait...";
                statusStrip.Update();

                File.Copy(backupName, Settings.FableDirectory+relName, true);
                
                statusStrip.Items[0].Text = tmp;
                
                InfoMessage("Rollback done for "+relName+".");
            }
        }

        private void itemBuilderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormObjectBuilder form = new FormObjectBuilder();
            AddMDI(form);
        }
    }
}