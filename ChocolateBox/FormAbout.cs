using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.IO;

namespace ChocolateBox
{
    public partial class FormAbout : Form
    {
        public FormAbout()
        {
            InitializeComponent();

            string tmp = labelInfo.Text;

            tmp = tmp.Replace("[Version]",
                Assembly.GetExecutingAssembly().GetName().Version.ToString());
            
            labelInfo.Text = tmp.Replace("[Date]",
                File.GetLastWriteTime(Application.ExecutablePath).ToString());
        }
    }
}