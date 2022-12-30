#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <glm/glm.hpp>
#include <vector>

struct Settings {
	int bladeCnt = 150;
	int bladeCntTmp = 1;
	int chunking = 0;
	float density = 1;
    float nearPlane = 1;
    float farPlane = 1;

	// can't be modified / not exposed
	float bladeWidth = 0.1f;
	float bladeWidthVariance = 0.05f;
	float bladeHeight = 1.0f;
    float bladeHeightVariance = 0.75f;
    float bladeHeightScale = 5.0f;

	float bladePosVariance = 1.0f;
	float bladeHeightNoiseScale = 0.4f;

	float bladeBendMax = 0.6f;
	float bladeCurvature = 1.0f;
	int bladeSegments = 4;
	int tempBladeSegments = 4;

	float hillHeightMax = 10.0f;
	float hillHeightNoiseScale = 0.01f;

	float partingRange = 2;

	bool applyFXAA = false;

	std::vector<std::string> grassColorTip = {
		"#6e5181",
		"#93a167",
		"#7ec4c1",
		"#d46e33",
		"#b2d942",
		"#ee8695"
	};
	std::vector<std::string> grassColorTop = {
		"#6f1d5c",
		"#6eaa78",
		"#34859d",
		"#e34262",
		"#52c33f",
		"#4a7a96"
	};
	std::vector<std::string> grassColorBottom = {
		"#4f1446",
		"#557064",
		"#17434b",
		"#94353d",
		"#166e7a",
		"#333f58"
	};
	std::vector<std::string> grassColorAmbientOcclusion = {
		"#2e0a30",
		"#387080",
		"#1f0e1c",
		"#57253b",
		"#254d70",
		"#292831"
	};
	std::vector<std::string> grassColorOld = {
		"#6ceded",
		"#9d9f7f",
		"#f5edba",
		"#f2b63d",
		"#fcf660",
		"#fbbbad"
	};
	std::vector<std::string> clearColor = {
		"#0d001a",
		"#666092",
		"#8c8fae",
		"#2c1b2e",
		"#201533",
		"#333f58"
	};
	std::vector<std::vector<std::string>> playerColor = {
		{
			"#2e0a30"
		},
		{
			"#8b5580",
			"#666092",
			"#9a4f50",
			"#7ca1c0",
			"#be955c",
			"#68aca9"
		},
		{
			"#584563",
			"#9a6348",
			"#c0c741",
			"#e4943a",
			"#9d303b",
			"#d26471",
			"#70377f"
		},
		{
			"#fff4e0",
			"#4b3b9c",
			"#285763",
			"#8fcccb",
			"#449489",
			"#6d8c32",
			"#457cd6"
		},
		{
			"#252446",
			"#166e7a",
			"#ffffff"
		},
		{
			"#fbbbad",
			"#ee8695",
			"#4a7a96",
			"#333f58",
			"#292831"
		}
	};

	std::vector<glm::vec2> extraPlayers = {
		glm::vec2(5,0.5),
		glm::vec2(7,0.3),
		glm::vec2(11,0.2),
		glm::vec2(13,0.1),
		glm::vec2(17,0.07),
		glm::vec2(23,0.08),
		glm::vec2(31,0.3),
		glm::vec2(37,0.2),
		glm::vec2(41,0.1),
		glm::vec2(47,0.05)
	};

	int colorPalette = 0;
	bool applyColor = true;
	bool applyColorVariance = true;
	bool applyWind = true;
	bool drawFirstPlayer = true;
	// bool drawFriendPlayer = true;
	int friends = 0;

	bool applyPosWidthVariance = true;
	bool applyHeightVariance = true;
	bool applyBendVariance = true;
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
