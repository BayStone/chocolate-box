using System;
using System.Collections.Generic;
using System.Text;

namespace ChocolateBox
{
    enum FileProcessorMode
    {
        LoadFile = 0,
        LoadForm = 1,
        SaveFile = 2
    };

    /// <summary>
    /// Base class for file based processors.
    /// </summary>
    class FileProcessor : Processor
    {
        private FileProcessorMode myMode;
        private FileController myController;
        private string myFileName = null;

        public FileProcessor(FileController c, 
                             FileProcessorMode mode)
        {
            myController = c;
            myMode = mode;
        }

        public FileProcessor(FileController c, string fileName)
        {
            myController = c;
            myMode = FileProcessorMode.SaveFile;
            myFileName = fileName;
        }

        public override void Run(Progress progress)
        {
            if (myMode == FileProcessorMode.LoadFile)
                myController.LoadFile(progress);
            else if (myMode == FileProcessorMode.LoadForm)
                myController.LoadForm(progress);
            else if (myMode == FileProcessorMode.SaveFile)
                myController.SaveFile(myFileName, progress);
        }
    }
}
