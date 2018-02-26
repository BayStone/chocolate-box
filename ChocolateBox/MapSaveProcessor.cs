using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace ChocolateBox
{
    /// <summary>
    /// Processor for saving TNGs and LEVs.
    /// </summary>
    class MapSaveProcessor : Processor
    {
        private FableMod.LEV.LEVFile myLEV;
        private FableMod.TNG.TNGFile myTNG;

        public MapSaveProcessor(FableMod.TNG.TNGFile tng,
                                FableMod.LEV.LEVFile lev)
        {
            myTNG = tng;
            myLEV = lev;
        }

        private string GetName(string pathName)
        {
            FileInfo info = new FileInfo(pathName);
            return info.Name;
        }

        public override void Run(Progress progress)
        {
            int mod = 0;

            if (myTNG.Modified)
                ++mod;

            bool lev = 
                Settings.GetBool("Settings", "SaveLEV", false) &&
                myLEV.Modified;

            if (lev)
                ++mod;

            progress.Begin(mod);

            if (myTNG.Modified)
            {
                progress.Info =
                    "Saving "+GetName(myTNG.FileName)+"...";
                myTNG.Save(myTNG.FileName);
                progress.Update();
            }

            if (lev)
            {
                progress.Info = 
                    "Saving "+GetName(myLEV.FileName)+"...";
                myLEV.Save(myLEV.FileName, progress);
            }

            progress.End();

            base.Run(progress);
        }
    }
}
