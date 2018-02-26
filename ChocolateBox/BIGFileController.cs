using System;
using System.Collections.Generic;
using System.Text;
using FableMod.BIG;
using System.IO;
using System.Windows.Forms;

namespace ChocolateBox
{
    public class BIGFileController : FileController
    {
        private BIGFile myBIG;

        public BIGFileController(FileDatabase database, string fileName)
            :
            base(database, fileName)
        {
        }

        public override bool Close()
        {
            State &= ~(int)FileState.FileOK;
            myBIG.Destroy();
            myBIG = null;
            return true;
        }

        protected override bool OnLoad(Progress progress)
        {
            try
            {
                myBIG = new BIGFile();

                myBIG.Load(FileName, progress);
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
            if (fileName != FileName)
            {
                myBIG.Save(fileName, progress);
            }
            else
            {
                FileInfo file = new FileInfo(fileName);

                string tmpFile = file.DirectoryName+"\\TMP_"+file.Name;

                myBIG.Save(tmpFile, progress);

                myBIG.Destroy();

                try
                {
                    File.Delete(fileName);
                    File.Move(tmpFile, fileName);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }

                // Not loaded.
                State = (int)FileState.InSystem;
            }

            return false;
        }

        protected override bool OnForm(Progress progress)
        {
            FormBIG form = null;

            if (myBIG == FileDatabase.Instance.Textures)
                form = new FormTextureBIG();
            else
                form = new FormBIG();

            form.Build(myBIG, progress);

            CreateForm(form);

            return true;
        }

        public BIGFile BIG
        {
            get { return myBIG; }
        }

        public override bool Modified
        {
            get
            {
                if (FileLoaded)
                    return myBIG.Modified;
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
