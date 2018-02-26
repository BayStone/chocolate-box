using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using FableMod.BBB;

namespace ChocolateBox
{
    public class BBBFileController : FileController
    {
        private BBBFile myBBB;
        private List<FileController> myFiles = new List<FileController>();

        public BBBFileController(
            FileDatabase database, 
            BBBFile bbb,
            string fileName)
            :
            base(database, fileName)
        {
            myBBB = bbb;
        }

        protected override bool OnLoad(Progress progress)
        {
            try
            {
                myBBB.Open(FileName, progress);
            }
            catch (Exception ex)
            {
                FormMain.Instance.ErrorMessage(ex.Message);
            }

            return true;
        }

        protected override bool OnForm(Progress progress)
        {
            FormBBB form = new FormBBB();

            form.Build(myBBB, progress);

            CreateForm(form);

            return true;
        }

        public bool HasFile(string fileName)
        {
            return myBBB.get_Entries(
                Database.GetRelativeFileName(fileName)) != null;
        }

        public override bool Modified
        {
            get
            {
                return false;
            }
        }

        public BBBFile BBB
        {
            get { return myBBB; }
        }
    }
}
