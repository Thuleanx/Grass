#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "realtime.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    void initialize();
    void finish();

private:
	void createLabel(QVBoxLayout *&layout, std::string label, bool isTitle = false);

	void createCheckbox(QVBoxLayout *&layout, QCheckBox *&checkbox, std::string text, bool &syncedValue);

	void createSlider(QVBoxLayout *&layout, QSlider *&slider, QSpinBox *&spinBox, 
        int tickInterval, int minimum, int maximum, int defaultValue, int &syncedValue);	

	void createSliderDouble(QVBoxLayout *&layout, QSlider *&slider, QDoubleSpinBox *&spinBox, 
		int tickIntervalSlider, int minimumSlider, int maximumSlider, int defaultValueSlider,
        float tickfloaterval, float minimum, float maximum, float defaultValue, float &syncedValue);

    Realtime *realtime;

	QSlider *bladeCntSlider;
	QSpinBox *bladeCntBox;

	QSlider *densitySlider;
	QDoubleSpinBox *densityBox;

	QSlider *bladeHeightScaleSlider;
	QDoubleSpinBox *bladeHeightScaleBox;

	QSlider *bladeSegmentCntSlider;
	QSpinBox *bladeSegmentCntBox;

	QCheckBox *applyPosWidthVariance;
	QCheckBox *applyHeightVariance;

	QSlider *hillHeightSlider;
	QDoubleSpinBox *hillHeightBox;

	QPushButton *forceRedrawButton;


	QCheckBox *applyColorVariance;
	QCheckBox *applyColor;
	QCheckBox *applyWind;
	QCheckBox *drawPlayer;
	QCheckBox *drawFriends;


    // QSlider *nearSlider;
    // QSlider *farSlider;
    // QDoubleSpinBox *nearBox;
    // QDoubleSpinBox *farBox;
};
