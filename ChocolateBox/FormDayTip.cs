using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace ChocolateBox
{
    public partial class FormDayTip : Form
    {
        private string[] myTips = new string[5];
        private int myTipIndex = 0;
        public Process myProcess = new Process();

        public FormDayTip()
        {
            InitializeComponent();

            NextTip();
        }

        void ParseLine(RichTextBox rtb, string line)
        {
            Regex r = new Regex("<([a-z]+|/[a-z]+)>");
            
            string[] tokens = r.Split(line);

            Color curColor = rtb.ForeColor;
            Font curFont = rtb.Font;

            foreach (string token in tokens)
            {
                if (token == "")
                    continue;
                else if (token == "title")
                {
                    curFont = new Font(rtb.Font, FontStyle.Bold);
                }
                else if (token == "/title")
                {
                    curColor = rtb.ForeColor;
                    curFont = rtb.Font;
                }
                else
                {
                    rtb.SelectionFont = curFont;
                    rtb.SelectionColor = curColor;

                    rtb.SelectedText = token;
                }
            }

            rtb.SelectedText = "\n";
        }

        private bool IsNewTip(string tip)
        {
            for (int i = 0; i < 5; ++i)
            {
                if (myTips[i] == tip)
                    return false;
            }
            return true;
        }

        private void NextTip()
        {
            string[] files = Directory.GetFiles(
                Settings.DataDirectory+"tips", "*.txt");

            richTextBoxTip.Clear();

            if (files.Length == 0)
            {
                richTextBoxTip.Text = "No tips available";
                return;
            }

            Random rnd = new Random(DateTime.Now.Millisecond);

            int count = 0;
            int max = files.Length;

            string file = files[rnd.Next(max)];

            while (!IsNewTip(file) && count < 64)
            {
                file = files[rnd.Next(max)];
                ++count;
            }

            myTips[myTipIndex] = file;
            myTipIndex = (myTipIndex+1) % 5;

            TextReader reader = new StreamReader(file);

            string line;

            while ((line = reader.ReadLine()) != null)
            {
                ParseLine(richTextBoxTip, line);
            }

            reader.Close();
        }

        private void buttonNext_Click(object sender, EventArgs e)
        {
            NextTip();
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void richTextBoxTip_LinkClicked(object sender, LinkClickedEventArgs e)
        {
            myProcess = Process.Start("IExplore.exe", e.LinkText);
        }
    }
}