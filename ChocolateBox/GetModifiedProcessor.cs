using System;
using System.Collections.ObjectModel;
using System.Text;

using FableMod.ContentManagement;
using FableMod.BIG;
using FableMod.BIN;

namespace ChocolateBox
{
    /// <summary>
    /// Processor for finding modified objects in the asset files.
    /// </summary>
    class GetModifiedProcessor : Processor
    {
        private Collection<ContentObject> myObjects;

        public GetModifiedProcessor()
        {
            myObjects = new Collection<ContentObject>();
        }

        public Collection<ContentObject> Objects
        {
            get { return myObjects; }
        }

        private void FindModified(BIGFile big, Progress progress)
        {
            progress.Begin(big.BankCount);

            for (int i = 0; i < big.BankCount; ++i)
            {
                FindModified(big.get_Banks(i), progress);
            }

            progress.End();
        }

        private void FindModified(BIGBank bank, Progress progress)
        {
           progress.Begin(bank.EntryCount);

            for (int j = 0; j < bank.EntryCount; ++j)
            {
                AssetEntry entry = bank.get_Entries(j);

                if (entry.Modified)
                {
                    myObjects.Add(
                        FileDatabase.Instance.GetContentObject(entry));
                }

                progress.Update();
            }

            progress.End();
        }

        private void FindModified(BINFile bin, Progress progress)
        {
            progress.Begin(bin.EntryCount);

            for (int i = 0; i < bin.EntryCount; ++i)
            {
                BINEntry entry = bin.get_Entries(i);

                if (entry.Modified)
                {
                    myObjects.Add(
                        FileDatabase.Instance.GetContentObject(entry));
                }

                progress.Update();
            }

            progress.End();
        }

        public override void Run(Progress progress)
        {
            progress.Begin(4);
            
            progress.Info = "Checking graphics...";
            FindModified(ContentManager.Instance.GraphicsBank, progress);
            progress.Info = "Checking textures...";
            FindModified(ContentManager.Instance.Textures, progress);
            progress.Info = "Checking front end textures...";
            FindModified(ContentManager.Instance.FrontEndTextureBank, progress);
            progress.Info = "Checking text...";
            FindModified(ContentManager.Instance.TextBank, progress);
            progress.Info = "Checking objects...";
            FindModified(ContentManager.Instance.Objects, progress);
            progress.Info = "Checking scripts...";
            FindModified(ContentManager.Instance.Scripts, progress);
            
            progress.End();
        }
    }
}
