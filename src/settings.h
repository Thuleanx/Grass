#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <glm/glm.hpp>

struct Settings {
	int bladeCnt = 1;
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

	float hillHeightMax = 2.0f;
	float hillHeightNoiseScale = 0.01f;

	bool applyFXAA = false;

	std::string grassColorTip = "#c9d4b8";
	std::string grassColorTop = "#90c0a0";
	std::string grassColorBottom = "#6f919c";
	std::string grassColorAmbientOcclusion = "#575f7e";

	std::string grassColorOld = "#d4beb8";
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
