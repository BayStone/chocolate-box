using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Diagnostics;
using System.Reflection;
using System.IO;

namespace FableMod.Forms
{
    /// <summary>
    /// FormApp's purpose is to provide some functions and systems
    /// for making application creation easier... like window
    /// state save, etc.
    /// </summary>
    public partial class FormApp : Form
    {
        private static FormApp myStaticMainForm = null;

        private string myRegistry = null;
        private string myTitle = null;

        public FormApp()
        {
            //Debug.Assert(myMainForm == null, "Only one main form allowed");
            myStaticMainForm = this;
            InitializeComponent();

            Messages.SetForm(this);
        }

        /// <summary>
        /// Get application mainform instance. Only one.
        /// </summary>
        static public FormApp Instance
        {
            get { return myStaticMainForm; }
        }

        /// <summary>
        /// Get application folder.
        /// </summary>
        static public string Folder
        {
            get { return AppDomain.CurrentDomain.BaseDirectory; }
        }

        public static Stream GetEmbeddedFile(Assembly assembly, string fileName)
        {
            try
            {
                Stream stream = assembly.GetManifestResourceStream(
                    assembly.GetName().Name+"."+fileName);

                if (stream == null)
                {
                    throw new InvalidOperationException(
                        "could not locate embedded resource '"+fileName+"'");
                }

                return stream;
            }
            catch (Exception e)
            {
                throw new InvalidOperationException(e.Message);
            }
        }

        public static Bitmap GetEmbeddedBitmap(
            Assembly assembly, string fileName)
        {
            Stream stream = GetEmbeddedFile(assembly, fileName);

            if (stream == null)
                return null;

            return new Bitmap(stream);
        }

        protected override void OnLoad(EventArgs e)
        {
            if (Title == "")
                Title = Text;

            LoadRegistry();

            base.OnLoad(e);
        }

        /// <summary>
        /// Gets or sets the registrykey for this application.
        /// </summary>
        [Browsable(true)]
        [Description("Registry key for the application")]
        [Category("Data")]
        public string RegistryKey
        {
            get { return myRegistry; }
            set { myRegistry = value; }
        }


        /// <summary>
        /// Get application title - name in the title bar.
        /// </summary>
        [Browsable(true)]
        [Description("Title aka. application name")]
        [Category("Appearance")]
        public virtual string Title
        {
            get
            {
                return myTitle;
            }
            set
            {
                myTitle = value;
                Text = myTitle;
            }
        }

        /// <summary>
        /// Save application info in the registry.
        /// </summary>
        public void SaveRegistry()
        {
            if (myRegistry == null || myRegistry == "")
                return;

            RegistryKey key =
                Registry.CurrentUser.CreateSubKey(myRegistry);

            if (key != null)
            {
                SaveRegistryData(key);

                key.Close();
            }
        }

        /// <summary>
        /// Load registry information.
        /// </summary>
        public void LoadRegistry()
        {
            if (myRegistry == null || myRegistry == "")
                return;

            RegistryKey key =
                Registry.CurrentUser.OpenSubKey(myRegistry);

            if (key != null)
            {
                LoadRegistryData(key);

                key.Close();
            }
        }

        /// <summary>
        /// Save registry information. Stub for derived classes.
        /// This version saves the window information automatically.
        /// </summary>
        /// <param name="key"></param>
        protected virtual void SaveRegistryData(RegistryKey key)
        {
            RegistryKey SettingsKey = key.CreateSubKey("Settings");

            if (SettingsKey != null)
            {
                SaveRegistrySettings(SettingsKey);
                SettingsKey.Close();
            }

            RegistryKey WindowKey = key.CreateSubKey("Window");

            if (WindowKey != null)
            {
                SaveWindowState(WindowKey);
                WindowKey.Close();
            }
        }

        /// <summary>
        /// Stub for derived classes.
        /// </summary>
        /// <param name="key"></param>
        protected virtual void SaveRegistrySettings(RegistryKey key)
        {
        }

        protected virtual void SaveWindowState(RegistryKey WindowKey)
        {
            WindowKey.SetValue("Top", Top);
            WindowKey.SetValue("Left", Left);
            WindowKey.SetValue("Width", Width);
            WindowKey.SetValue("Height", Height);

            WindowKey.SetValue("Maximized",
                WindowState == FormWindowState.Maximized);
        }

        /// <summary>
        /// Load application information from the registry.
        /// This version loads the window information automatically.
        /// </summary>
        /// <param name="Key"></param>
        protected virtual void LoadRegistryData(RegistryKey Key)
        {
            RegistryKey SettingsKey = Key.OpenSubKey("Settings");

            if (SettingsKey != null)
            {
                LoadRegistrySettings(SettingsKey);
                SettingsKey.Close();
            }

            RegistryKey WindowKey = Key.OpenSubKey("Window");

            if (WindowKey != null)
            {
                LoadWindowState(WindowKey);
                WindowKey.Close();
            }
        }

        protected virtual void LoadRegistrySettings(RegistryKey Key)
        {
        }

        protected virtual void LoadWindowState(RegistryKey WindowKey)
        {
            StartPosition = FormStartPosition.Manual;

            if (((string)WindowKey.GetValue(
                "Maximized", "False")).Equals("True"))
            {
                WindowState = FormWindowState.Maximized;
            }
            else
            {
                Top = (int)WindowKey.GetValue("Top", Top);
                Left = (int)WindowKey.GetValue("Left", Left);
                Width = (int)WindowKey.GetValue("Width", Width);
                Height = (int)WindowKey.GetValue("Height", Bottom);
            }
        }

        virtual public DialogResult ErrorMessage(string message)
        {
            return MessageBox.Show(
                null, message, myTitle,
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);
        }

        virtual public DialogResult InfoMessage(string message)
        {
            return MessageBox.Show(
                null, message, myTitle,
                MessageBoxButtons.OK,
                MessageBoxIcon.Information);
        }

        public DialogResult Message(
            string message, MessageBoxButtons buttons, MessageBoxIcon icon)
        {
            return MessageBox.Show(this, message, myTitle, buttons, icon);
        }
    }
}