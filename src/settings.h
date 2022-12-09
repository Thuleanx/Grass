#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct Settings {
	int bladeCntX = 1;
	int bladeCntZ = 1;
	float density = 20;
	float bladeWidth = 0.05f;
    float bladeHeight = 1.0f;
    float nearPlane = 1;
    float farPlane = 1;
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
