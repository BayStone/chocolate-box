using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FableMod.WLD
{
    public class WLDFile : FableMod.Script.Parser
    {
        private List<Map> m_Maps = new List<Map>();
        private List<Region> m_Regions = new List<Region>();
	    private List<string> m_Quests = new List<string>();
        private int m_MapUIDCount = 0;	
	    private int m_ThingManagerUIDCount = 0;	

	    private enum ParserState
	    {
		    Basic = 0,
		    Quests,
		    Map,
		    Region
	    }

	    private ParserState m_ParserState;

	    private static System.Globalization.CultureInfo m_Culture =
            new System.Globalization.CultureInfo("en-US");

        public Map FindMap(string levelName)
        {
            return m_Maps.Find(m => m.LevelName == levelName);
        }

        protected override void ParseArgument(string argument, string value)
        {
	        if (value.StartsWith("\"") && value.EndsWith("\""))
		        value = value.Substring(1, value.Length-2);

	        if (m_ParserState == ParserState.Basic)
	        {
		        if (argument == "MapUIDCount")
		        {
			        m_MapUIDCount = int.Parse(value);
		        }
		        else if (argument == "ThingManagerUIDCount")
		        {
			        m_ThingManagerUIDCount = int.Parse(value);
		        }
		        else if (argument == "NewMap")
		        {
			        Map map = new Map();
			        map.ID = int.Parse(value);
			        m_Maps.Add(map);
			        m_ParserState = ParserState.Map;
		        }
		        else if (argument == "NewRegion")
		        {
			        Region region = new Region();
			        region.ID = int.Parse(value);
			        m_Regions.Add(region);
			        m_ParserState = ParserState.Region;
		        }
	        }
	        else if (m_ParserState == ParserState.Map)
	        {
		        ParseMapArgument(argument, value);
	        }
	        else if (m_ParserState == ParserState.Region)
	        {
		        ParseRegionArgument(argument, value);
	        }
        }

        private void ParseMapArgument(String argument, String value)
        {
	        Map map = m_Maps[m_Maps.Count-1];

	        if (argument == "MapX")
		        map.X = int.Parse(value);
	        else if (argument == "MapY")
		        map.Y = int.Parse(value);
	        else if (argument == "LevelName")
		        map.LevelName = value;
	        else if (argument == "LevelScriptName")
		        map.LevelScriptName = value;
	        else if (argument == "MapUID")
		        map.UID = UInt32.Parse(value);
	        else if (argument == "IsSea")
		        map.Sea = bool.Parse(value);
	        else if (argument == "LoadedOnPlayerProximity")
		        map.LoadedOnProximity = bool.Parse(value);
        }

        private void ParseRegionArgument(String argument, String value)
        {
	        Region region = m_Regions[m_Regions.Count-1];

	        if (argument == "RegionName")
		        region.RegionName = value;
	        else if (argument == "NewDisplayName")
		        region.NewDisplayName = value;
	        else if (argument == "RegionDef")
		        region.RegionDef = value;
	        else if (argument == "MiniMapGraphic")
		        region.MiniMap = value;
	        else if (argument == "MiniMapScale")
		        region.MiniMapScale = float.Parse(value, m_Culture);
	        else if (argument == "AppearOnWorldMap")
		        region.AppearOnWorldMap = bool.Parse(value);
	        else if (argument == "MiniMapOffsetX")
		        region.MiniMapOffsetX = float.Parse(value, m_Culture);
	        else if (argument == "MiniMapOffsetY")
		        region.MiniMapOffsetY = float.Parse(value, m_Culture);
	        else if (argument == "WorldMapOffsetX")
		        region.WorldMapOffsetX = float.Parse(value, m_Culture);
	        else if (argument == "WorldMapOffsetY")
		        region.WorldMapOffsetY = float.Parse(value, m_Culture);
	        else if (argument == "NameGraphicOffsetX")
		        region.NameGraphicOffsetX = float.Parse(value, m_Culture);
	        else if (argument == "NameGraphicOffsetY")
		        region.NameGraphicOffsetY = float.Parse(value, m_Culture);
	        else if (argument == "ContainsMap")
		        region.ContainsMaps.Add(FindMap(value));
	        else if (argument == "SeesMap")
		        region.SeesMaps.Add(FindMap(value));	
        }

        protected override void ParseCommand(String command)
        {
	        if (m_ParserState == ParserState.Basic)
	        {
		        if (command == "START_INITIAL_QUESTS")
		        {
			        m_ParserState = ParserState.Quests;
		        }
	        }
	        else if (m_ParserState == ParserState.Quests)
	        {
		        if (command == "END_INITIAL_QUESTS")
		        {
			        m_ParserState = ParserState.Basic;
		        }
		        else
		        {
			        m_Quests.Add(command);
		        }
	        }
	        else if (m_ParserState == ParserState.Map)
	        {
		        if (command == "EndMap")
		        {
			        m_ParserState = ParserState.Basic;
		        }
	        }
	        else if (m_ParserState == ParserState.Region)
	        {
		        if (command == "EndRegion")
		        {
			        m_ParserState = ParserState.Basic;
		        }
	        }	
        }

        public int RegionCount
        {
            get { return m_Regions.Count; }
        }

        public Region GetRegion(int index)
        {
            return m_Regions[index];
        }

        public Region GetRegion(string name)
        {
            return m_Regions.Find(r => string.Compare(r.RegionName, name, true) == 0);
        }
    }
}
