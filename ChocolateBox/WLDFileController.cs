using System;
using System.Collections.Generic;
using System.Text;

using FableMod.WLD;

namespace ChocolateBox
{
    public class WLDFileController : FileController
    {
        private WLDFile myWLD;

        public WLDFileController(FileDatabase database, string fileName)
            :
            base(database, fileName)
        {
            myWLD = new WLDFile();
        }

        protected override bool OnLoad(Progress progress)
        {
            progress.Begin(1);

            myWLD.Load(FileName);

            progress.End();

            return true;
        }

        public WLDFile WLD
        {
            get { return myWLD; }
        }

        public override bool UserAccess
        {
            get
            {
                // No user access.
                return false;
            }
        }
    }
}
