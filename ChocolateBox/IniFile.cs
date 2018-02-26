using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace EasyCSharp.Core
{
	/// <summary>
	/// IniFile class implements a simple INI file controller.
	/// </summary>
	public class IniFile
	{
        private string myPath;
        
        [DllImport("kernel32")]
		private static extern long WritePrivateProfileString(string section,string key,string val,string filePath);
		[DllImport("kernel32")]
		private static extern int GetPrivateProfileString(string section,string key,string def,StringBuilder retVal,int size,string filePath);

		/// <summary>
		/// INIFile Constructor.
		/// </summary>
		/// <param name="INIPath"></param>
		public IniFile(string inipath)
		{
			myPath = inipath;
		}

		/// <summary>
		/// Write Data to the INI file.
		/// </summary>
		/// <param name="Section"></param>
		/// <param name="Key"></param>
		/// <param name="Value"></param>
		public void WriteString(string Section,string Key,string Value)
		{
			WritePrivateProfileString(Section,Key,Value,myPath);
		}
		
		public void WriteInt(string Section,string Key,int Value)
		{
			WriteString(Section,Key,Value.ToString());
		}
	
		public void WriteFloat(string Section,string Key,float Value)
		{
			System.Globalization.CultureInfo culture = 
				new System.Globalization.CultureInfo("en-US");

			WriteString(Section,Key,Value.ToString(culture));
		}
	
		public void WriteBool(string Section,string Key,bool Value)
		{
			string temp = (Value) ? "true" : "false";
			WriteString(Section,Key,temp);
		}
	
		public string ReadString(string Section,string Key)
		{
            return ReadString(Section, Key, "");
		}

        public string ReadString(string Section, string Key, string Default)
        {
            try
            {
                StringBuilder temp = new StringBuilder(128);

                int i = GetPrivateProfileString(
                    Section, Key, Default, temp, 128, myPath);

                return temp.ToString();
            }
            catch (Exception)
            {
                return Default;
            }
        }

		public int ReadInt(string Section,string Key, int Default)
		{
			string temp = ReadString(Section,Key);

			if (temp.Length == 0)
				return Default;

			try
			{
				return int.Parse(temp);
			}
			catch (Exception)
			{
			}

			return Default;
		}

		public int ReadInt(string Section,string Key)
		{
			return ReadInt(Section,Key,0);
		}
		
		public float ReadFloat(string Section,string Key, float Default)
		{
			string temp = ReadString(Section,Key);

			if (temp.Length == 0)
				return Default;
			
			System.Globalization.CultureInfo culture = 
				new System.Globalization.CultureInfo("en-US");

			try
			{
				return float.Parse(ReadString(Section,Key),culture);
			}
			catch (Exception)
			{
			}

			return Default;
		}
	
		public float ReadFloat(string Section,string Key)
		{
			return ReadFloat(Section,Key,0.0f);
		}
		
		public bool ReadBool(string Section,string Key, bool Default)
		{
			string temp = ReadString(Section,Key);
		
			if (temp.Length == 0)
				return Default;

			return temp.ToUpper().Equals("TRUE") || temp.Equals("1");
		}

		public bool ReadBool(string Section,string Key)
		{
			return ReadBool(Section,Key,false);
		}

        public string Path
        {
            get { return myPath; }
            set { myPath = value; }
        }
	}
}
