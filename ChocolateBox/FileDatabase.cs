using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Text.RegularExpressions;

using FableMod.ContentManagement;
using FableMod.Content.Forms;
using FableMod.BIN;
using FableMod.BIG;
using FableMod.TNG;
using FableMod.Gfx.Integration;

namespace ChocolateBox
{
    public class FileDatabase : ContentManager
    {
        private List<FileController> myFiles = 
            new List<FileController>();

        private FileInterface myInterface = null;
        private DefinitionDB myDefinitions = null;
        private TNGDefinitions myTNGDefinitions = null;
        
        public FileDatabase(FileInterface fileInterface, 
                            DefinitionDB definitions,
                            TNGDefinitions tngdefinitions)
        {
            myInterface = fileInterface;
            myDefinitions = definitions;
            myTNGDefinitions = tngdefinitions;
            SetInstance(this);
        }

        public void CloseFiles()
        {
            foreach (FileController c in myFiles)
            {
                c.Close();
            }

            ResetBIG();
        }

        public FileController AutoLoad(string fileName)
        {
            FileController c = Get(fileName);

            if (c != null && !c.FileLoaded)
            {
                List<Processor> processors = new List<Processor>();

                c.LoadProcess(processors);

                FormProcess form = new FormProcess(processors);

                form.ShowDialog();

                form.Dispose();
            }

            return c;
        }

        protected override NamesBINFile LoadNames()
        {
            NamesBINController c = (NamesBINController)
                AutoLoad(Settings.GetString("Files", "Names"));

            if (c == null)
                return null;

            return c.Names;
        }

        protected override BINFile LoadObjects()
        {
            BINFileController c = (BINFileController)
                AutoLoad(Settings.GetString("Files", "Objects"));

            if (c == null)
                return null;

            return c.BIN;
        }

        protected override BINFile LoadScripts()
        {
            BINFileController c = (BINFileController)
                AutoLoad(Settings.GetString("Files", "Scripts"));

            if (c == null)
                return null;

            return c.BIN;
        }

        protected override BIGBank LoadGraphics()
        {
            BIGFileController c = (BIGFileController)AutoLoad(
                Settings.GetString("Files", "Graphics"));

            if (c == null)
                return null;

            return c.BIG.FindBankByName(
                Settings.GetString("Banks", "Graphics"));
        }

        protected override BIGFile LoadTextures()
        {
            BIGFileController c = (BIGFileController)AutoLoad(
                Settings.GetString("Files", "Textures"));

            if (c == null)
                return null;

            return c.BIG;
        }

        protected override BIGBank LoadFrontEndTextures()
        {
            BIGFileController c = (BIGFileController)AutoLoad(
                Settings.GetString("Files", "FrontEndTextures"));

            return c.BIG.FindBankByName(
                Settings.GetString("Banks", "FrontEndTextures"));
        }


        protected override BIGBank GetMainTextureBank(BIGFile textures)
        {
            return textures.FindBankByName(
                Settings.GetString("Banks", "Textures"));
        }

        protected override BIGBank GetGUITextureBank(BIGFile textures)
        {
            return textures.FindBankByName(
                Settings.GetString("Banks", "GUITextures"));
        }

        protected override BIGBank LoadText()
        {
            BIGFileController c = (BIGFileController)AutoLoad(
                Settings.GetString("Files", "Text"));

            if (c == null)
                return null;

            return c.BIG.FindBankByName(
                Settings.GetString("Banks", "Text"));
        }

        public override void ShowEntry(object o, bool dialog)
        {
            Form form = null;

            if (o.GetType() == typeof(BINEntry))
            {
                form = new FormBINEntry((BINEntry)o);
            }
            else if (o.GetType() == typeof(AssetEntry))
            {
                form = new FormBIGEntry((AssetEntry)o);
            }

            if (form != null)
            {
                if (dialog)
                {
                    form.ShowDialog();
                    form.Dispose();
                }
                else
                {
                    FormMain.Instance.AddMDI(form);
                    form = null;
                }
            }
        }

        /// <summary>
        /// Select a game object.
        /// </summary>
        /// <param name="dst"></param>
        /// <param name="current"></param>
        /// <returns>Returns an object depending on the link type</returns>
        public override object SelectEntry(
           Link link, object current)
        {
            if (link.To == LinkDestination.NamesBINOffset)
            {
                if (AutoLoadNames())
                {
                    NamesBINEntry entry = Names.GetEntryByOffset(
                        uint.Parse(current.ToString()));

                    string curr = "";

                    if (entry != null)
                        curr = entry.Name;

                    FormSelectNamesEntry form = new FormSelectNamesEntry(
                        Names, curr, link.Restriction);

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        entry = form.Selected;

                        form.Dispose();

                        return entry.Offset;
                    }

                    form.Dispose();
                }
            }
            else if (link.To == LinkDestination.NamesBINEnum)
            {
                if (AutoLoadNames())
                {
                    NamesBINEntry entry = Names.GetEntryByEnum(
                        uint.Parse(current.ToString()));

                    string curr = "";

                    if (entry != null)
                        curr = entry.Name;

                    FormSelectNamesEntry form = new FormSelectNamesEntry(
                        Names, curr, link.Restriction);

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        entry = form.Selected;

                        form.Dispose();

                        return entry.Enum;
                    }

                    form.Dispose();
                }
            }
            else if (link.To == LinkDestination.GameBINEntryName)
            {
                if (AutoLoadObjects())
                {
                    FormSelectBINEntry form = new FormSelectBINEntry(
                        Objects, 
                        current.ToString());

                    form.CDefType = link.Restriction;

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        BINEntry entry = form.Selected;

                        form.Dispose();

                        return entry.Name;
                    }

                    form.Dispose();
                }
            }
            else if (link.To == LinkDestination.GameBINEntryID)
            {
                if (AutoLoadObjects())
                {
                    ContentObject o = FindEntry(
                       link.To, UInt32.Parse(current.ToString()));

                    FormSelectBINEntry form = new FormSelectBINEntry(
                        Objects, o.Name);

                    form.CDefType = link.Restriction;

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        BINEntry entry = form.Selected;

                        form.Dispose();
                        
                        return entry.ID;
                    }

                    form.Dispose();
                }
            }
            else if (link.To == LinkDestination.ModelID)
            {
                if (AutoLoadGraphics())
                {
                    ContentObject o = FindEntry(
                        link.To, UInt32.Parse(current.ToString()));

                    FormSelectBIGEntry form = null;

                    if (o != null)
                    {
                        form = new FormSelectBIGEntry(
                            GraphicsBank, link.To, o.Name);
                    }
                    else
                    {
                        form = new FormSelectBIGEntry(
                            GraphicsBank, link.To, null);
                    }

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        AssetEntry entry = form.Selected;
                        
                        form.Dispose();

                        return entry.ID;
                    }
                    
                    form.Dispose();
                }
            }
            else if (link.To == LinkDestination.MainTextureID)
            {
                if (AutoLoadTextures())
                {
                    ContentObject o = FindEntry(
                        link.To, UInt32.Parse(current.ToString()));

                    FormSelectBIGEntry form = null;

                    if (o != null)
                    {
                        form = new FormSelectBIGEntry(
                            MainTextureBank, link.To, o.Name);
                    }
                    else
                    {
                        form = new FormSelectBIGEntry(
                            MainTextureBank, link.To, null);
                    }

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        AssetEntry entry = form.Selected;

                        form.Dispose();

                        return entry.ID;
                    }

                    form.Dispose();
                }
            }
            else if (link.To == LinkDestination.GUITextureID)
            {
                if (AutoLoadTextures())
                {
                    ContentObject o = FindEntry(
                        link.To, UInt32.Parse(current.ToString()));

                    FormSelectBIGEntry form = null;

                    if (o != null)
                    {
                        form = new FormSelectBIGEntry(
                            GUITextureBank, link.To, o.Name);
                    }
                    else
                    {
                        form = new FormSelectBIGEntry(
                            GUITextureBank, link.To, null);
                    }

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        AssetEntry entry = form.Selected;

                        form.Dispose();

                        return entry.ID;
                    }

                    form.Dispose();
                }
            }
            else if (link.To == LinkDestination.FrontEndTextureID)
            {
                if (AutoLoadFrontEndTextures())
                {
                    ContentObject o = FindEntry(
                        link.To, UInt32.Parse(current.ToString()));

                    FormSelectBIGEntry form = null;

                    if (o != null)
                    {
                        form = new FormSelectBIGEntry(
                            FrontEndTextureBank, link.To, o.Name);
                    }
                    else
                    {
                        form = new FormSelectBIGEntry(
                            FrontEndTextureBank, link.To, null);
                    }

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        AssetEntry entry = form.Selected;

                        form.Dispose();

                        return entry.ID;
                    }

                    form.Dispose();
                }
            }
            else if (link.To == LinkDestination.ModelName)
            {
                if (AutoLoadGraphics())
                {
                    FormSelectBIGEntry form = new FormSelectBIGEntry(
                        GraphicsBank, link.To, current.ToString());
                
                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        AssetEntry entry = form.Selected;

                        form.Dispose();

                        return entry.DevSymbolName;
                    }

                    form.Dispose();
                }
            }
            return null;
        }

        /// <summary>
        /// Convert a game asset into a ContentObject.
        /// </summary>
        /// <param name="o"></param>
        /// <returns></returns>
        public ContentObject GetContentObject(object o)
        {
            if (o.GetType() == typeof(AssetEntry))
            {
                AssetEntry entry = (AssetEntry)o;

                ContentType ctype = ContentType.Unknown;

                if (entry.Bank.GetType() == typeof(BIGBank))
                {
                    BIGBank b = (BIGBank)entry.Bank;

                    if (b.Name == 
                        Settings.GetString("Banks", "Graphics"))
                        ctype = ContentType.Graphics;
                    else if (b.Name == 
                        Settings.GetString("Banks", "Text"))
                        ctype = ContentType.Text;
                    else if (b.Name == 
                        Settings.GetString("Banks", "Textures"))
                        ctype = ContentType.MainTextures;
                    else if (b.Name == 
                        Settings.GetString("Banks", "GUITextures"))
                        ctype = ContentType.GUITextures;
                    else if (b.Name == 
                        Settings.GetString("Banks", "FrontEndTextures"))
                        ctype = ContentType.FrontEndTextures;
                }

                return new ContentObject(
                    entry.DevSymbolName,
                    entry,
                    ctype);
            }
            else if (o.GetType() == typeof(BINEntry))
            {
                if (!AutoLoadObjects())
                    return null;

                BINEntry entry = (BINEntry)o;

                ContentType ctype = ContentType.Objects;

                if (Objects.GetEntryByName(entry.Name) != null)
                    ctype = ContentType.Objects;
                else if (entry.Definition == "CCutsceneDef")
                    ctype = ContentType.Scripts;

                return new ContentObject(
                    entry.Name,
                    entry,
                    ctype);
            }

            return null;
        }

        /// <summary>
        /// Create a linker object... needed for mod packages.
        /// </summary>
        /// <param name="contentType"></param>
        /// <param name="entry"></param>
        /// <returns></returns>
        public override AssetLinker CreateAssetLinker(
            ContentType contentType, AssetEntry entry)
        {
            if (((BIGBank)entry.Bank).Name == 
                Settings.GetString("Banks", "Graphics"))
            {
                if (entry.Type == 0x1 ||
                    entry.Type == 0x2 ||
                    entry.Type == 0x4 ||
                    entry.Type == 0x5)
                {
                    return new GfxModel(entry);
                }
            }

            return null;
        }

        public FileController Find(string fileName)
        {
            for (int i = 0; i < FileCount; ++i)
            {
                if (string.Compare(
                    GetFileAt(i).FileName,fileName, true) == 0)
                {
                    return GetFileAt(i);
                }
            }

            return null;
        }

        private string GetAbsoluteFileName(string fileName)
        {
            string fablePath = Settings.FableDirectory;

            if (fileName.IndexOf(":\\") < 0)
            {
                fileName = fablePath+fileName;
            }

            return fileName;
        }

        public string GetRelativeFileName(string fileName)
        {
            string fablePath = Settings.FableDirectory;

            int index = fileName.IndexOf(fablePath);

            if (index == 0)
                fileName = fileName.Substring(fablePath.Length);

            return fileName;
        }

        public WLDFileController GetWLD()
        {
            return (WLDFileController)AutoLoad(
                Settings.GetString("Files", "World"));
        }

        public BBBFileController GetSTB()
        {
            return (BBBFileController)AutoLoad(
                Settings.GetString("Files", "STB"));
        }

        public FileController Get(string fileName)
        {
            fileName = GetAbsoluteFileName(fileName);

            FileController c = Find(fileName);

            if (c != null)
            {
                // Already in the system.
                return c;
            }

            FileInfo info = new FileInfo(fileName);

            string ext = info.Extension.ToUpper();

            if (ext == ".BIG")
            {
                c = new BIGFileController(this, fileName);
            }
            else if (ext == ".BIN")
            {
                if (info.Name.ToUpper() == "NAMES.BIN")
                {
                    c = new NamesBINController(this, fileName);
                }
                else
                {
                    c = new BINFileController(this, fileName);
                }
            }
            else if (ext == ".STB")
            {
                c = new BBBFileController(
                    this, new FableMod.STB.STBFile(), fileName);
            }
            else if (ext == ".WAD")
            {
                c = new BBBFileController(
                    this, new FableMod.BBB.BBBFile(), fileName);
            }
            else if (ext == ".WLD")
            {
                c = new WLDFileController(this, fileName);
            }

            if (c != null)
            {
                myFiles.Add(c);
            }

            return c;
        }
		
        public static new FileDatabase Instance
        {
            get { return (FileDatabase)ContentManager.Instance; }
        }

        public int FileCount
        {
            get { return myFiles.Count; }
        }

        public FileController GetFileAt(int index)
        {
            return myFiles[index];
        }

        public override DefinitionDB Definitions
        {
            get { return myDefinitions; }
        }

        public TNGDefinitions TNGDefinitions
        {
            get { return myTNGDefinitions; }
        }

        public FileInterface Interface
        {
            get { return myInterface; }
        }
        
        public override string DataDirectory
        {
            get { return Settings.FableDirectory; }
        }
    }
}
