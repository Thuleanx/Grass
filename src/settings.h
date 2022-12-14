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
		"#6e5181"
	};
	std::vector<std::string> grassColorTop = {
		"#6f1d5c"
	};
	std::vector<std::string> grassColorBottom = {
		"#4f1446" 
	};
	std::vector<std::string> grassColorAmbientOcclusion = {
		"#2e0a30"
	};
	std::vector<std::string> grassColorOld = {
		"#6ceded"
	};
	std::vector<std::string> clearColor = {
		"#0d001a"
	};
	std::vector<std::string> playerColor = {
		"#000000"
	};

	std::vector<glm::vec2> extraPlayers = {
		glm::vec2(5,0.5),
		glm::vec2(7,0.3),
		glm::vec2(11,0.2),
		glm::vec2(13,0.1)
	};

	int colorPalette = 0;
	bool applyColor = true;
	bool applyColorVariance = true;
	bool applyWind = true;
	bool drawFirstPlayer = true;
	bool drawFriendPlayer = true;

	bool applyPosWidthVariance = true;
	bool applyHeightVariance = true;
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
