using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;
using System.Windows.Forms;

namespace ChocolateBox
{
    class Settings
    {
        private static string myFableDirectory = null;
        private static EasyCSharp.Core.IniFile myIni = null;

        public static string Directory
        {
            get
            {
                string dir = AppDomain.CurrentDomain.BaseDirectory;

                if (dir == null)
                    return System.Environment.CurrentDirectory;
                else
                    return dir;
            }
        }

        public static string DataDirectory
        {
            get
            {
                return Directory+"data\\";
            }
        }

        public static string FableDirectory
        {
            get { return myFableDirectory; }
        }

        public static string LevelsDirectory
        {
            get { return myFableDirectory + "data\\levels\\"; }
        }

        public static string GetString(string Section, string Key)
        {
            if (myIni == null)
                return "";

            return myIni.ReadString(Section, Key);
        }

        public static string GetString(string section, string key, string def)
        {
            if (myIni == null)
                return "";

            return myIni.ReadString(section, key, def);
        }

        public static bool GetBool(string Section, string Key, bool Default)
        {
            if (myIni == null)
                return false;

            return myIni.ReadBool(Section, Key, Default);
        }

        public static bool GetBool(string Section, string Key)
        {
            return GetBool(Section, Key, false);
        }

        public static float GetFloat(string Section, string Key)
        {
            if (myIni == null)
                return 0.0f;

            return myIni.ReadFloat(Section, Key);
        }

        public static float GetFloat(
            string Section, string Key, float Default)
        {
            if (myIni == null)
                return Default;

            return myIni.ReadFloat(Section, Key, Default);
        }

        public static int GetInt(string Section, string Key)
        {
            if (myIni == null)
                return 0;

            return myIni.ReadInt(Section, Key);
        }

        public static int GetInt(string Section, string Key, int Default)
        {
            if (myIni == null)
                return Default;

            return myIni.ReadInt(Section, Key, Default);
        }

        static public void Load()
        {
            string settingsFile = Directory+"ChocolateBox.ini";

            myIni = new EasyCSharp.Core.IniFile(settingsFile);

            myFableDirectory = GetString("Settings", "MyPath", "");

            if (string.IsNullOrEmpty(myFableDirectory))
            {
                string regPath = 
					GetString("Settings", "FableRegistry", "");

                if (regPath == "")
                {
                    throw new Exception(
                        "Registry information missing from the settings file.");
                }

                try
                {
                    RegistryKey key = Registry.LocalMachine.OpenSubKey(
                        regPath, false);

                    if (key != null)
                    {
                        myFableDirectory = 
                            key.GetValue("SetupPath").ToString();
                    }
                    else
                    {
                        // Funky DVD version?

                        FolderBrowserDialog dlg = new FolderBrowserDialog();

                        dlg.Description = "Locate Fable directory";

                        if (dlg.ShowDialog() == DialogResult.OK)
                        {
                            myFableDirectory = dlg.SelectedPath;
                            
                            // Add it to the registry.

                            key = Registry.LocalMachine.CreateSubKey(regPath);

                            key.SetValue("SetupPath", myFableDirectory);
                        }
                        else
                        {
                            throw new InvalidOperationException(
                                "Unable to locate registry information.");
                        }
                    }
                }
                catch (System.Security.SecurityException)
                {
                    throw new InvalidOperationException(
                        "Registry not available for the current user.");
                }
            }
          
            if (!myFableDirectory.EndsWith("\\"))
                myFableDirectory += "\\";
        }
    }
}
