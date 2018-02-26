using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using FableMod.BIN;
using FableMod.ContentManagement;

namespace FableMod.Content.Forms
{
    public partial class FormBINEntry : Form
    {
        private BINEntry m_Entry;
        private DefinitionType m_DefType = null;
        private DefinitionTypeDisplay m_DefDisplay = null;

        public FormBINEntry(BINEntry entry)
        {
            InitializeComponent();

            m_Entry = entry;

            Text = m_Entry.Name;

            txtDefinitionType.Text = m_Entry.Definition;
            txtSymbolName.Text = m_Entry.Name;
            textBoxId.Text = m_Entry.ID.ToString();

            RefreshDefinitionDisplay();
        }

        public void RefreshDefinitionDisplay()
        {
            m_DefType = ContentManager.Instance.Definitions.GetDefinition(
                m_Entry.Definition);
            
            if (m_DefType != null)
            {
                try
                {
                    if (m_DefDisplay != null)
                    {
                        // Remove old display.
                        this.spltContainerMain.Panel2.Controls.Remove(m_DefDisplay);
                        this.spltContainerMain.Panel2Collapsed = true;
                        m_DefDisplay.Dispose();
                    }

                    m_DefType.ReadIn(m_Entry);
                    
                    m_DefDisplay = new DefinitionTypeDisplay(m_DefType, 
                        ContentManager.Instance);
                    
                    this.Size = new System.Drawing.Size(Size.Width, 480);
                    
                    this.spltContainerMain.Panel2.Controls.Add(m_DefDisplay);
                    this.spltContainerMain.Panel2Collapsed = false;
                    
                    m_DefDisplay.Dock = DockStyle.Fill;
                }
                catch (Exception exc)
                {
                    this.Size = new System.Drawing.Size(Size.Width, 120);
                    MessageBox.Show(this, exc.Message, "Error Parsing Entry");
                    //txtDefinitionDisplay.Text = "Error parsing entry." + Environment.NewLine + exc.Message;
                }
            }
        }

        private void btnExport_Click(object sender, EventArgs e)
        {
            saveFileDialog.InitialDirectory = 
                ContentManager.Instance.DataDirectory;

            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                FileStream f = File.Create(saveFileDialog.FileName);
                byte[] data = m_Entry.Data;
                f.Write(data, 0, data.Length);
                f.Close();
            }
        }

        private void btnViewData_Click(object sender, EventArgs e)
        {
            FormDataViewer form = new FormDataViewer(m_Entry.Data);
            form.MdiParent = MdiParent;
            form.Show();
        }

        private void btnImport_Click(object sender, EventArgs e)
        {
            openFileDialog.InitialDirectory = 
                ContentManager.Instance.DataDirectory;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                FileStream f = File.OpenRead(openFileDialog.FileName);
                byte[] data = new byte[f.Length];
                f.Read(data, 0, (int)f.Length);
                m_Entry.Data = data;

                RefreshDefinitionDisplay();
            }
        }

        private void btnApplyChanges_Click(object sender, EventArgs e)
        {
            m_Entry.Definition = txtDefinitionType.Text;
            m_Entry.Name = txtSymbolName.Text;

            if (m_DefDisplay != null)
            {
                m_DefDisplay.ApplyChanges();
                m_DefType.Write(m_Entry);
            }
        }

        private void openFileDialog_FileOk(object sender, CancelEventArgs e)
        {

        }
    }
}