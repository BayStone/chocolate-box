using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using FableMod.Content.Forms;
using FableMod.Gfx.Integration;
using FableMod.BIG;
using FableMod.Forms;

namespace ChocolateBox
{
    public partial class FormTextureBIG : ChocolateBox.FormBIG
    {
        public FormTextureBIG()
        {
            InitializeComponent();
        }

        public override void Build(BIGFile big, Progress progress)
        {
            base.Build(big, progress);

            addEntryToolStripMenuItem.Text = "Add Texture";
        }

        protected override AssetEntry CreateEntry(BIGBank bank)
        {
            FormOpenTexture form = new FormOpenTexture();

            AssetEntry entry = null;

            if (form.ShowDialog(this) == DialogResult.OK)
            {
                GfxTexture texture;

                try
                {
                    texture = new GfxTexture(form.FileName, form.Format);
                }
                catch (Exception ex)
                {
                    FormMain.Instance.ErrorMessage(ex.Message);
                    return null;
                }

                string name = Path.GetFileNameWithoutExtension(form.FileName);

                name.Replace(' ', '_');
                name.Replace('-', '_');
                name.Replace('.', '_');
                name = name.ToUpper();

                FormTextBox textForm = new FormTextBox();

                textForm.Text = "New Texture";
                textForm.labelInput.Text = "Texture Symbol Name:";

                while (true)
                {
                    textForm.textBoxInput.Text = name;

                    if (textForm.ShowDialog() == DialogResult.OK)
                    {
                        if (bank.FindEntryBySymbolName(
                            textForm.textBoxInput.Text) != null)
                        {
                            FormMain.Instance.ErrorMessage(
                                "Symbol already exists. Try again.");
                        }
                        else
                        {
                            name = textForm.textBoxInput.Text;
                            break;
                        }
                    }
                }

                textForm.Dispose();

                entry = new AssetEntry(
                    name,
                    bank.GetNewID(),
                    0,
                    bank);

                texture.ApplyToEntry(entry);
            }

            form.Dispose();

            return entry;
        }
    }
}

