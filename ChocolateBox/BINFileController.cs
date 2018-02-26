using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using FableMod.BIN;

namespace ChocolateBox
{
    class BINFileController : FileController
    {
        private BINFile myBIN;

        public BINFileController(FileDatabase database, string fileName)
            :
            base(database, fileName)
        {
        }

        protected NamesBINController GetNamesController()
        {
            return (NamesBINController)Database.Get(
                Settings.FableDirectory+
                Settings.GetString("Files", "Names"));
            // Or load from the file directory?
        }

        public override void LoadProcess(List<Processor> processors,
            bool form)
        {
            NamesBINController c = GetNamesController();

            c.LoadProcess(processors, false);

            base.LoadProcess(processors, form);
        }

        protected override bool OnLoad(Progress progress)
        {
            try
            {
                NamesBINController c = GetNamesController();

                myBIN = new BINFile(c.Names);

                myBIN.Load(FileName, progress);
            }
            catch (Exception ex)
            {
                FormMain.Instance.ErrorMessage(ex.Message);
                return false;
            }

            return true;
        }

        protected override bool OnForm(Progress progress)
        {
            FormBIN form = new FormBIN();

            form.Build(myBIN, progress);

            CreateForm(form);

            return true;
        }


        protected override bool OnSave(string fileName, Progress progress)
        {
            try
            {
                myBIN.Save(fileName, progress);

                NamesBINController c = GetNamesController();

                if (c.Modified)
                {
                    string namesBin = 
                        Path.GetDirectoryName(fileName)+"\\names.bin";

                    c.Names.Save(c.FileName);

                    if (namesBin == c.FileName)
                        c.Modified = false;
                }
            }
            catch (Exception ex)
            {
                FormMain.Instance.ErrorMessage(ex.Message);
                return false;
            }

            return true;
        }

        public BINFile BIN
        {
            get { return myBIN; }
        }

        public override bool Modified
        {
            get
            {
                if (FileLoaded)
                    return myBIN.Modified;
                else
                    return base.Modified;
            }

            set
            {
                base.Modified = value;
            }
        }
    }
}
