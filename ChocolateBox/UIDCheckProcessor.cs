using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using FableMod.TNG;

namespace ChocolateBox
{
    class UIDCheckProcessor : Processor
    {
        List<string> myUIDs = new List<string>(20000);
        List<string> mySaves = new List<string>(128);
        int myCount = 0;
        int mySaved = 0;

        public int ChangedCount
        {
            get { return myCount; }
        }

        public int SavedCount
        {
            get { return mySaved; }
        }
        
        protected bool IsValid(Thing thing)
        {
            return
                UIDManager.IsNormal(thing.UID) &&
                (thing.Name == "Building" ||
                 thing.Name == "Object");
        }

        protected List<CTCBlock> GetOwned(TNGFile tng)
        {
            List<CTCBlock> ctcs = new List<CTCBlock>(100);

            for (int i = 0; i < tng.SectionCount; ++i)
            {
                foreach (Thing thing in tng.get_Sections(i).Things)
                {
                    CTCBlock ctc = thing.get_CTCs("CTCOwnedEntity");

                    if (ctc != null)
                    {
                        Variable v = ctc.get_Variables("OwnerUID");

                        if (UIDManager.IsNormal(v.StringValue))
                        {
                            ctcs.Add(ctc);
                        }
                    }
                }
            }

            return ctcs;
        }

        protected void UpdateThing(Thing thing, List<CTCBlock> ctcs)
        {
            string newUID = UIDManager.Generate();

            foreach (CTCBlock ctc in ctcs)
            {
                Variable v = ctc.get_Variables("OwnerUID");

                if (v.StringValue == thing.UID)
                {
                    v.Value = newUID;
                }
            }

            thing.UID = newUID;
        }

        protected void CheckUIDs(string[] files, Progress progress)
        {
            string info = "Checking ";

            int index = 0;

            for (int i = 0; i < files.Length; ++i)
            {
                progress.Info = 
                    info+Path.GetFileName(files[i])+"...";

                TNGFile tng = new TNGFile(
                    FileDatabase.Instance.TNGDefinitions);

                tng.Load(files[i]);

                List<CTCBlock> ctcs = GetOwned(tng);

                for (int j = 0; j < tng.SectionCount; ++j)
                {
                    foreach (Thing thing in tng.get_Sections(j).Things)
                    {
                        if (IsValid(thing))
                        {
                            int found = myUIDs.IndexOf(thing.UID);

                            if (found >= 0 && found != index)
                            {
                                /*
                                Console.WriteLine(
                                    "Update: Index={0} Found={1} Thing={2}", 
                                    index, 
                                    found, 
                                    thing.DefinitionType);
                                */

                                UpdateThing(thing, ctcs);

                                ++myCount;
                            }
                            else
                            {
                                // New uid, add to the list.
                                myUIDs.Add(thing.UID);
                            }

                            ++index;
                        }
                    }
                }

                if (tng.Modified)
                {
                    if (mySaves.IndexOf(files[i]) < 0)
                        mySaves.Add(files[i]);

                    tng.Save(files[i]);
                }

                ctcs = null;

                tng.Dispose();

                progress.Update();
            }
        }

        public override void Run(Progress progress)
        {
            string baseDir = Settings.FableDirectory+"data\\Levels";

            string[] filesLevels = Directory.GetFiles(
                baseDir, "*.tng");

            string[] filesAlbion = Directory.GetFiles(
                baseDir+"\\FinalAlbion", "*.tng");

            string[] files = 
                new string[filesLevels.Length+filesAlbion.Length];

            Array.Copy(filesLevels, 
                files, filesLevels.Length);
            Array.Copy(filesAlbion, 
                0, files, filesLevels.Length, filesAlbion.Length);

            progress.Begin(2);

            progress.Begin(files.Length);

            CheckUIDs(files, progress);
            
            progress.End();
            
            progress.Begin(files.Length);

            CheckUIDs(files, progress);
            
            progress.End();

            progress.End();

            files = null;
            filesLevels = null;
            filesAlbion = null;

            myUIDs.Clear();
            myUIDs = null;

            mySaved = mySaves.Count;
            mySaves.Clear();
            mySaves = null;
        }
    }
}
