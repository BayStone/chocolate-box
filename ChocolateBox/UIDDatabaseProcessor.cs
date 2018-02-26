using System;
using System.Collections.Generic;
using System.IO;

using FableMod.TNG;

namespace ChocolateBox
{
    class UIDDatabaseProcessor : Processor
    {
        protected void ProcessFiles(
            string[] files, Progress progress)
        {
            string info = "Reading UIDs from ";

            for (int i = 0; i < files.Length; ++i)
            {
                progress.Info = 
                    info+Path.GetFileName(files[i])+"...";

                TNGFile tng = new TNGFile(
                    FileDatabase.Instance.TNGDefinitions);

                tng.Load(files[i]);

                for (int j = 0; j < tng.SectionCount; ++j)
                {
                    foreach (Thing thing in tng.get_Sections(j).Things)
                    {
                        UIDManager.Add(thing.UID);
                    }
                }

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

            progress.Begin(filesLevels.Length+filesAlbion.Length);

            ProcessFiles(filesLevels, progress);
            ProcessFiles(filesAlbion, progress);

            progress.End();
        }
    }
}
