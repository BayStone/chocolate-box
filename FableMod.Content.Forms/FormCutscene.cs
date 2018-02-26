using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using FableMod.BIN;
using FableMod.ContentManagement;

namespace FableMod.Content.Forms
{
    public partial class FormCutscene : Form
    {
        private BINEntry myCutscene = null;
        private DefinitionType myDefType = null;
        private FableMod.ContentManagement.Control myMode1 = null;
        private FableMod.ContentManagement.Control myMode2 = null;
        private FableMod.ContentManagement.Control myMode3 = null;
        private bool myModified = false;

        public FormCutscene(BINEntry cutscene)
        {
            InitializeComponent();

            myCutscene = cutscene;

            Text = "Cutscene: "+myCutscene.Name;

            myDefType = ContentManager.Instance.Definitions.GetDefinition(
                myCutscene.Definition);

            myDefType.ReadIn(myCutscene);

            myMode1 = myDefType.FindControl(0x5A1E6CA9);
            myMode2 = myDefType.FindControl(0xC1CABAFF);
            myMode3 = myDefType.FindControl(0x4FC34512);

            textBox1.Text = GetScript(myMode1);
            textBox2.Text = GetScript(myMode2);
            textBox3.Text = GetScript(myMode3);

            myModified = false;
        }

        private string GetScript(FableMod.ContentManagement.Control c)
        {
            if (c == null)
                return "";

            Member memberCount = (Member)c.Members[0];
            ArrayMember memberLines = (ArrayMember)c.Members[1];

            StringBuilder sb = new StringBuilder();

            uint count = (uint)memberCount.Value;

            for (int i = 0; i < count; ++i)
            {
                Member memberLine = (Member)memberLines.Elements[i][0];

                sb.AppendLine(memberLine.Value.ToString());
            }

            return sb.ToString();
        }

        private void ApplyScript(
            FableMod.ContentManagement.Control c,
            string script)
        {
            string[] lines = script.Split(
                new string[] { "\r\n" }, 
                StringSplitOptions.RemoveEmptyEntries);
            
            Member memberCount = (Member)c.Members[0];
            ArrayMember memberLines = (ArrayMember)c.Members[1];

            memberCount.Value = (UInt32)lines.Length;
            memberLines.Elements.Clear();

            for (int i = 0; i < lines.Length; ++i)
            {
                MemberCollection mc = memberLines.CreateElement();

                ((Member)mc[0]).Value = lines[i];

                memberLines.Elements.Add(mc);
            }
        }

        private void ApplyChanges()
        {
            if (!myModified)
                return;

            ApplyScript(myMode1, textBox1.Text);
            ApplyScript(myMode2, textBox2.Text);
            ApplyScript(myMode3, textBox3.Text);

            myDefType.Write(myCutscene);

            myModified = false;
        }

        private void textBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                ApplyChanges();
        }

        private void FormCutscene_FormClosing(object sender, FormClosingEventArgs e)
        {
            ApplyChanges();
        }

        private void textBox_Changed(object sender, EventArgs e)
        {
            myModified = true;
        }
    }
}