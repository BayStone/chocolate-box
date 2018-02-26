using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

using FableMod.ContentManagement;
using FableMod.TNG;
using FableMod.BBB;

namespace ChocolateBox
{
    class ModEnvironmentManager
    {
        private void ErrorMessage(string message)
        {
            FormMain.Instance.ErrorMessage(message);
        }
        
        private void InfoMessage(string message)
        {
            FormMain.Instance.InfoMessage(message);
        }

        /// <summary>
        /// Check if the userst.ini file is ok.
        /// </summary>
        /// <returns></returns>
        private bool CheckUserStIni()
        {
            string path = Settings.FableDirectory;
            string userFile = path + "userst.ini";

            try
            {
                StreamReader sr = new StreamReader(userFile);

                string line;

                while ((line = sr.ReadLine()) != null)
                {
                    if (line.ToUpper() == "USELEVELWAD FALSE;")
                        return true;
                }

                sr.Close();
            }
            catch (Exception)
            {
            }

            return false;
        }

        private bool FixUserStIni()
        {
            string path = Settings.FableDirectory;
            string backupFile = path + "backup_userst.ini";
            string userFile = path + "userst.ini";

            try
            {
                if (!File.Exists(backupFile))
                    File.Copy(userFile, backupFile, true);

                FileStream stream = new FileStream(userFile, FileMode.Create);

                StreamReader sr = new StreamReader(backupFile);
                StreamWriter sw = new StreamWriter(stream);

                string line;

                while ((line = sr.ReadLine()) != null)
                {
                    if (line != "UseLevelWAD TRUE;")
                        sw.WriteLine(line);
                    else
                        sw.WriteLine("UseLevelWAD FALSE;");
                }

                sr.Close();
                sw.Close();
                stream.Close();

                return true;
            }
            catch (Exception ex)
            {
                ErrorMessage("userst.ini Error: " + ex.Message);
            }

            return false;
        }

        private bool CheckTngLev()
        {
            string levels = Settings.LevelsDirectory;
            
            return
                File.Exists(levels + "creature_hub.tng") &&
                File.Exists(levels + "creature_hub.tng") &&
                File.Exists(levels + "creature_hub.tng") &&
                File.Exists(levels + "FinalAlbion\\lookoutpoint.tng") &&
                File.Exists(levels + "FinalAlbion\\lookoutpoint.lev");
        }

        private bool Fix()
        {
            string pathLevels = Settings.LevelsDirectory;

            try
            {
                string tasks = "";

                if (!CheckTngLev())
                {
                    // Need to extract this stuff.

                    BBBFile wad = new BBBFile();

                    wad.Open(pathLevels + "FinalAlbion.wad", null);

                    BBBExtractor bbb = new BBBExtractor(wad, Settings.FableDirectory);

                    FormProcess form = new FormProcess(bbb);

                    form.Text = "Extracting...";

                    form.ShowDialog();

                    form.Dispose();

                    wad.Close();

                    tasks += "\tExtracted FinalAlbion.WAD.\r\n";
                }

                if (File.Exists(pathLevels + "FinalAlbion.wad"))
                {
                    // Make sure this doesn't exist.
                    File.Delete(pathLevels + "_FinalAlbion.wad");

                    File.Move(
                        pathLevels + "FinalAlbion.wad",
                        pathLevels + "_FinalAlbion.wad");

                    tasks += "\tRenamed FinalAlbion.WAD.\r\n";
                }

                if (!CheckUserStIni())
                {
                    if (FixUserStIni())
                    {
                        tasks += "\tPatched userst.ini.\r\n";
                    }
                    else
                    {
                        throw new Exception("Failed to update userst.ini.");
                    }
                }

                InfoMessage(
                    "Modding environment successfully updated!\r\n" +
                    "Following tasks were completed:\r\n" +
                    tasks);

                return true;
            }
            catch (Exception ex)
            {
                ErrorMessage(ex.ToString());
            }

            return false;
        }

        /// <summary>
        /// Check if mod environment is ok.
        /// </summary>
        /// <returns>True if mod env. ok.</returns>
        public bool Check()
        {
            string path = Settings.FableDirectory;
            string levels = Settings.LevelsDirectory;

            try
            {
                string err = "";

                if (File.Exists(path))
                {
                    if (File.Exists(path + "userst.ini"))
                    {
                        if (!CheckUserStIni())
                        {
                            err = "\tInvalid \"userst.ini\".\r\n";
                        }
                    }
                    else
                    {
                        err = "\tFile \"userst.ini\" not found.\r\n";
                    }

                    if (File.Exists(levels + "FinalAlbion.wad"))
                    {
                        err += "\tFile \"FinalAlbion.WAD\" already exists.\r\n";
                    }

                    if (!CheckTngLev())
                    {
                        err += "\tUnable to locate TNG and LEV files.\r\n";
                    }

                    if (!string.IsNullOrEmpty(err))
                    {
                        // Not setup properly.

                        if (MessageBox.Show(
                            "Your modding environment has the following problems:\r\n" +
                            err +
                            "\r\nDo you want ChocolateBox to try fix the issues?",
                            FormMain.Instance.Title,
                            MessageBoxButtons.YesNo,
                            MessageBoxIcon.Exclamation) == DialogResult.Yes)
                        {
                            return Fix();
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    ErrorMessage(string.Format("Fable directory \"{0}\" does not exist.", levels));
                    return false;
                }

                return true;
            }
            catch (Exception ex)
            {
                ErrorMessage(ex.ToString());
            }

            return false;
        }
    }
}
