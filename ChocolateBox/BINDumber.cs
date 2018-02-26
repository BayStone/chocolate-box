using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using FableMod.BIN;

namespace ChocolateBox
{
    /// <summary>
    /// Processor for extracting BBB files.
    /// </summary>
    public class BINDumber : Processor
    {
        private BINFile myBIN;
        private string myFolder;

        public BINDumber(BINFile bin, string folder)
        {
            myBIN = bin;
            myFolder = folder;
        }

        public override void Run(Progress progress)
        {
            string fileFormat = Settings.GetString(
                "BIN", "DumpFormat", "ID-NAME.dat");

            progress.Begin(myBIN.EntryCount);

            for (int i = 0; i < myBIN.EntryCount; ++i)
            {
                BINEntry entry = myBIN.get_Entries(i);

                string name = "UNTITLED";
                
                if (entry.Name != "")
                    name = entry.Name;

                string filename = fileFormat;

                filename = filename.Replace("ID", 
                    string.Format("{0:D5}", entry.ID));
                
                filename = filename.Replace("NAME", name);
                filename = filename.Replace("DEFINITION", entry.Definition);

                progress.Info = filename;

                using (BinaryWriter writer = new BinaryWriter(
                    new FileStream(myFolder+"\\"+filename, FileMode.Create)))
                {
                    writer.Write(entry.Data, 0, entry.Data.Length);
                    writer.Close();
                }

                progress.Update();
            }

            progress.End();
        }
    }
}
