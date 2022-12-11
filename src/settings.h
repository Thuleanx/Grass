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
	float bladeWidth = 0.05f;
	float bladeWidthVariance = 0.03f;
	float bladeHeight = 1.0f;
    float bladeHeightVariance = 0.75f;
    float bladeHeightScale = 5.0f;

	float bladePosVariance = 1.0f;
	float bladeHeightNoiseScale = 0.2f;

	float bladeBendMax = 0.8f;
	float bladeCurvature = 8.0f;
	int bladeSegments = 1;

	bool applyFXAA = false;

	std::string grassColorTip = "#eff9d6";
	std::string grassColorTop = "#ba5044";
	std::string grassColorBottom = "#7a1c4b";
	std::string grassColorAmbientOcclusion = "#1b0326";
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
