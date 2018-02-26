using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace FableMod.Script
{
    public class Parser
    {
	    private TextReader mReader;

        public virtual void Load(string fileName)
        {
            System.Byte[] data;

	        using (FileStream file = File.Open(
		        fileName, 
		        FileMode.Open, 
		        FileAccess.ReadWrite,
		        FileShare.Read))
            {
    	        data = new System.Byte[file.Length];

	            file.Read(data, 0, data.Length);

    	        file.Close();
	        }

            using (MemoryStream memStream = new MemoryStream(data))
            {
                using (mReader = new StreamReader(memStream))
                {
                    string line = null;

                    while ((line = NextLine()) != null)
                    {
                        ParseLine(line);
                    }

                    mReader.Close();
                }
                
                memStream.Close();
            }
        }
        
        private string NextLine()
        {
            string line = mReader.ReadLine();

            if (!string.IsNullOrEmpty(line))
                line = line.Trim();

            return line;
        }

	    protected virtual void ParseLine(string line)
        {
            if (string.IsNullOrEmpty(line))
                return;

            int index = line.IndexOf(' ');

            if (index > 0)
            {
                ParseArgument(
                    line.Substring(0, index),
                    line.Substring(index + 1, line.Length - (index + 1) - 1));
            }
            else
            {
                ParseCommand(line.Substring(0, line.Length - 1));
            }
        }

	    protected virtual void ParseArgument(string argument, string value)
        {
        }

        protected virtual void ParseCommand(string command)
        {
        }
    }
}
