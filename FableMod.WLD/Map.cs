using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace FableMod.WLD
{
    public class Map
    {
        public int ID;
	    public UInt32 UID;
	    public int X;
	    public int Y;
	    public string LevelName;
	    public string LevelScriptName;
	    public bool Sea;
	    public bool LoadedOnProximity;

	    public void Save(TextWriter writer)
        {
            writer.WriteLine("NewMap {0};", ID);
            writer.WriteLine("MapX {0};", X);
            writer.WriteLine("MapY {0};", Y);
            writer.WriteLine("LevelName \"{0}\";", LevelName);
            writer.WriteLine("LevelScriptName {0};", LevelScriptName);
            writer.WriteLine("MapUID {0};", UID);
            writer.WriteLine("IsSea {0};", Sea.ToString().ToUpper());
            writer.WriteLine("LoadedOnPlayerProximity {0};",
                LoadedOnProximity.ToString().ToUpper());
            writer.WriteLine("EndMap;");
            writer.WriteLine("");
        }
    }
}
