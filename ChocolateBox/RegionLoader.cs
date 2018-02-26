using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;

namespace ChocolateBox
{
    /// <summary>
    /// Processor for loading regions.
    /// </summary>
    class RegionLoader : Processor
    {
        private string myBaseDirectory;
        private FableMod.Gfx.Integration.GfxThingController myController;
        private FableMod.WLD.Region myRegion;
        private FableMod.STB.STBFile mySTB;
        private Collection<FableMod.Gfx.Integration.ThingMap> myMaps;
        private bool myAddSees;

        public RegionLoader(
            string baseDirectory,
            FableMod.Gfx.Integration.GfxThingController controller,
            FableMod.WLD.Region region,
            FableMod.STB.STBFile stb,
            bool addSees)
        {
            myBaseDirectory = baseDirectory;
            myRegion = region;
            myController = controller;
            myMaps = new Collection<FableMod.Gfx.Integration.ThingMap>();
            mySTB = stb;
            myAddSees = addSees;
        }

        public override void Run(Progress progress)
        {
            FableMod.WLD.Map[] maps = new FableMod.WLD.Map[
                myRegion.ContainsMaps.Count+
                myRegion.SeesMaps.Count];

            int count = 0;

            for (int i = 0; i < myRegion.ContainsMaps.Count; ++i)
                maps[count++] = myRegion.ContainsMaps[i];

            if (myAddSees)
            {
                for (int i = 0; i < myRegion.SeesMaps.Count; ++i)
                    maps[count++] = myRegion.SeesMaps[i];
            }

            progress.Begin(count*4);

            FableMod.STB.STBFile stb = null;

            if (Settings.GetBool("Editor", "UseSTB"))
            {
                BBBFileController stbc = FileDatabase.Instance.GetSTB();
                stb = (FableMod.STB.STBFile)stbc.BBB;
            }

            for (int i = 0; i < count; ++i)
            {
                FableMod.WLD.Map map = maps[i];

                string levFile = myBaseDirectory+map.LevelName;
                string tngFile = 
                    levFile.Substring(0, levFile.Length-4)+".tng";

                FableMod.TNG.TNGFile tng = new FableMod.TNG.TNGFile(
                    FileDatabase.Instance.TNGDefinitions);
                FableMod.LEV.LEVFile lev = new FableMod.LEV.LEVFile();

                progress.Info =
                    "Loading "+map.LevelScriptName+" objects...";

                progress.Begin(1);
                tng.Load(tngFile);
                progress.End();

                progress.Info = 
                    "Loading "+map.LevelScriptName+" level...";

                progress.Begin(1);
                lev.Open(levFile, null);
                progress.End();

                string stbLevFile = levFile;
                
                if (stbLevFile.StartsWith(Settings.FableDirectory))
                {
                    stbLevFile = stbLevFile.Substring(
                        Settings.FableDirectory.Length);
                }
                
                FableMod.Gfx.Integration.ThingMap gfxMap = 
                    new FableMod.Gfx.Integration.ThingMap();

                progress.Begin(1);
                gfxMap.Create(
                    myController,
                    map.LevelScriptName,
                    map.X,
                    map.Y,
                    lev,
                    stb != null ? stb.GetLevel(stbLevFile) : null,
                    tng);
                progress.End();

                myMaps.Add(gfxMap);
      
                gfxMap = null;
                map = null;
                lev = null;
                tng = null;
            }

            maps = null;

            progress.End();
            
            base.Run(progress);
        }

        public Collection<FableMod.Gfx.Integration.ThingMap> Maps 
        { 
            get { return myMaps; } 
        }
    }
}
