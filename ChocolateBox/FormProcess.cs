using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Reflection;

using FableMod.Forms;

namespace ChocolateBox
{
    public partial class FormProcess : Form
    {
        delegate void CloseDelegate();

        Progress myProgress = null;
        Thread myThread = null;
        List<Processor> myProcessors = new List<Processor>();
        int myCurrentBitmap = 0;

        static List<Bitmap> myBitmaps;

        static FormProcess()
        {
            myBitmaps = new List<Bitmap>();

            for (int i = 1; i <= 9; ++i)
            {
                try
                {
                    Bitmap bmp = new Bitmap(Settings.Directory+
                        string.Format("data\\loading\\loading0{0}.png", i));

                    myBitmaps.Add(bmp);
                }
                catch (Exception)
                {
                   
                }
            }
        }

        public FormProcess(Processor processor)
        {
            InitializeComponent();

            myProgress = new Progress();

            myProcessors.Add(processor);
            
            Text = FormMain.Instance.Title;
        }

        public FormProcess(List<Processor> processors)
        {
            InitializeComponent();

            myProgress = new Progress();

            myProcessors.AddRange(processors);
            
            Text = FormMain.Instance.Title;
        }

        private void CloseForm()
        {
            if (InvokeRequired)
            {
                CloseDelegate cb = new CloseDelegate(CloseForm);
                Invoke(cb);
            }
            else
            {
                Close();
            }
        }

        private void ThreadFunc()
        {
            try
            {
                myProgress.Begin(myProcessors.Count);

                for (int i = 0; i < myProcessors.Count; ++i)
                {
                    myProcessors[i].Run(myProgress);
                }

                myProgress.End();
            }
            catch (ThreadAbortException)
            {
            }

            CloseForm();
        }

        private void FormProcess_Shown(object sender, EventArgs e)
        {
            myThread = new Thread(new ThreadStart(ThreadFunc));
            myThread.Start();

            timerUpdate.Enabled = true;
        }

        private void FormProcess_FormClosing(object sender, FormClosingEventArgs e)
        {
            timerPicture.Enabled = false;
            timerUpdate.Enabled = false;

            if (myThread != null)
                myThread.Abort();
        }

        private void timerUpdate_Tick(object sender, EventArgs e)
        {
            labelInfo.Text = myProgress.Info;

            int val = (int)((float)progressBar.Maximum*myProgress.Value);

            if (val > progressBar.Maximum)
                val = progressBar.Maximum;

            progressBar.Value = val;
        }

        private void FormProcess_Load(object sender, EventArgs e)
        {
            pictureBoxImage.Image = myBitmaps[myCurrentBitmap];
        }

        private void timerPicture_Tick(object sender, EventArgs e)
        {
            ++myCurrentBitmap;

            if (myCurrentBitmap >= myBitmaps.Count)
                myCurrentBitmap = 0;

            pictureBoxImage.Image = myBitmaps[myCurrentBitmap];
            pictureBoxImage.Update();
        }
    }
}