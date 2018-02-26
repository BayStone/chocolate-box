using System;
using System.Collections.Generic;
using System.Text;

using FableMod.BIN;

namespace ChocolateBox
{
    class NamesBINController : FileController
    {
        private NamesBINFile myBIN;

        public NamesBINController(FileDatabase database, string fileName)
            :
            base(database, fileName)
        {
        }

        protected override bool OnLoad(Progress progress)
        {
            try
            {
                myBIN = new NamesBINFile();

                myBIN.Load(FileName, progress);
            }
            catch (Exception ex)
            {
                FormMain.Instance.ErrorMessage(ex.Message);
                return false;
            }

            return true;
        }

        protected override bool OnSave(string fileName, Progress progress)
        {
            myBIN.Save(fileName);
            return true;
        }

        protected override bool OnForm(Progress progress)
        {
            FormNamesBIN form = new FormNamesBIN();

            form.Build(myBIN, progress);

            CreateForm(form);

            return true;
        }

        public NamesBINFile Names
        {
            get { return myBIN; }
        }

        public override bool Modified
        {
            get { return myBIN.Modified;  }
            set { myBIN.Modified = value; }
        }
    }
}
