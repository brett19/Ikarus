#pragma once

namespace RoseCIType
{
	enum RoseCIType
	{
		MASK = 0,
		HAIR,
		CAP,
		BODY,
		ARM,
		FOOT,
		FACEITEM,
		BACK,
		WEAPON,
		SUBWPN
	};
};

namespace RoseItemType
{
	enum RoseItemType
	{
		MASK = 0,
		CAP = 1,
		BODY = 2,
		ARM = 3,
		FOOT = 4,
		BACK = 5,
		WEAPON = 7,
		SUBWPN = 8,
		PAT = 13,
		FACE = 14,
		HAIR = 15
	};

	//char* NAMES[] = { "Mask", "Cap", "Body", "Arm", "Foot", "Back", 0, "Weapon", "Sub-Weapon", 0, 0, 0, 0, "PAT", "Face", "Hair" };
};

struct PartItem
{
	union
	{
		struct
		{
			unsigned int itemno : 10;
			unsigned int gemop : 9;
			unsigned int hassocket : 1;
			unsigned int grade : 4;
		};
		unsigned int value;
	};
};