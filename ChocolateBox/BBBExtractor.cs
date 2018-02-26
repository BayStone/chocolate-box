using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using FableMod.BBB;

namespace ChocolateBox
{
    /// <summary>
    /// Processor for extracting BBB files.
    /// </summary>
    public class BBBExtractor : Processor
    {
        private BBBFile myBBB;
        private string myPath;

        public BBBExtractor(BBBFile bbb, string path)
        {
            myBBB = bbb;
            myPath = path;
        }

        public override void Run(Progress progress)
        {
            progress.Begin(myBBB.EntryCount);

            for (int i = 0; i < myBBB.EntryCount; ++i)
            {
                progress.Info = myBBB.get_Entries(i).FileName;

                myBBB.ExtractFile(myPath, myBBB.get_Entries(i));

                progress.Update();
            }

            progress.End();
        }
    }
}
