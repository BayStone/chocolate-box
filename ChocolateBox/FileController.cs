using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace ChocolateBox
{
    enum FileState : int 
    {
        InSystem = 1,
        FileOK = 2,
        FormOK = 4,
    };

    public class FileController
    {
        private string myFileName;
        private int myState = 0;
        private FormFileController myForm;
        private FileDatabase myDatabase;

        public FileController(FileDatabase database, string fileName)
        {
            myState = (int)FileState.InSystem;
            myFileName = fileName;
            myDatabase = database;
        }

        public virtual bool Close()
        {
            return false;
        }

        public void LoadProcess(List<Processor> processors)
        {
            LoadProcess(processors, false);
        }

        public virtual void LoadProcess(List<Processor> processors,
            bool form)
        {
            if (!FileLoaded)
            {
                processors.Add(new FileProcessor(
                    this, FileProcessorMode.LoadFile));
            }

            if (!FormLoaded && form)
            {
                processors.Add(new FileProcessor(
                    this, FileProcessorMode.LoadForm));
            }
        }

        protected virtual bool OnLoad(Progress progress)
        {
            return false;
        }

        protected virtual bool OnForm(Progress progress)
        {
            return false;
        }

        protected virtual bool OnSave(string fileName, Progress progress)
        {
            return false;
        }

        public bool LoadFile(Progress progress)
        {
            if (!FileLoaded)
            {
                System.IO.FileInfo info = new System.IO.FileInfo(FileName);

                progress.Info = "Loading "+info.Name+"...";

                info = null;

                if (OnLoad(progress))
                {
                    State |= (int)FileState.FileOK;
                    FileDatabase.Instance.Interface.OnLoadFile(this);
                    return true;
                }
            }
            else
            {
                progress.Update();
                return true;
            }

            return false;
        }

        public bool LoadForm(Progress progress)
        {
            if (!FormLoaded)
            {
                progress.Info = "Building interface...";

                if (OnForm(progress))
                {
                     State |= (int)FileState.FormOK;
                     return true;
                }
            }
            else
            {
                progress.Update();
                return true;
            }

            return false;
        }

        public void SaveFile(string fileName, Progress progress)
        {
            if (String.IsNullOrEmpty(fileName))
                fileName = FileName;

            System.IO.FileInfo info = new System.IO.FileInfo(fileName);

            progress.Info = "Saving "+info.Name+"...";

            info = null;

            if (OnSave(fileName, progress))
            {
                // Reset flag.
                Modified = false;
            }
        }
        
        protected void CreateForm(FormFileController form)
        {
            myForm = form;
            myForm.Controller = this;
            FormMain.Instance.AddMDI(myForm);
        }

        /// <summary>
        /// Called from FormFileController when the form is closed
        /// by the user.
        /// </summary>
        /// <param name="form"></param>
        public virtual void CloseForm(System.Windows.Forms.Form form)
        {
            myForm = null;
            State &= ~(int)FileState.FormOK;
        }

        public string FileName
        {
            get { return myFileName; }
        }

        public int State
        {
            get { return myState; }
            set { myState = value; }
        }

        public bool FileLoaded
        {
            get { return ((State & (int)FileState.FileOK) != 0); }
        }

        public bool FormLoaded
        {
            get { return ((State & (int)FileState.FormOK) != 0); }
        }

        public FileDatabase Database
        {
            get { return myDatabase; }
        }

        public virtual bool UserAccess
        {
            get { return true; }
        }

        public string RelativeFileName
        {
            get { return Database.GetRelativeFileName(FileName);  }
        }

        public string Directory
        {
            get
            {
                int index = FileName.LastIndexOf("\\");

                if (index >= 0)
                    return FileName.Substring(0, index);

                return FileName;
            }
        }

        public virtual bool Modified
        {
            get { return false; }
            set { }
        }
    }
}
