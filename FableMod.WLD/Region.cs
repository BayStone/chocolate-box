using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace FableMod.WLD
{
    public class Region
    {
        public int ID;
	    public bool AppearOnWorldMap;
	    public string RegionName;
	    public string NewDisplayName;
	    public string RegionDef;
	    public string MiniMap;
	    public float MiniMapScale;
	    public float MiniMapOffsetX;
	    public float MiniMapOffsetY;
	    public float WorldMapOffsetX;
	    public float WorldMapOffsetY;
	    public float NameGraphicOffsetX;
	    public float NameGraphicOffsetY;
	    public List<Map> ContainsMaps = new List<Map>();
        public List<Map> SeesMaps = new List<Map>();

        public void Save(TextWriter writer)
        {
        }
    }
}
