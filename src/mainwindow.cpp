#include "mainwindow.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QSettings>
#include <QLabel>
#include <QGroupBox>
#include <iostream>

using namespace std;

void MainWindow::createLabel(QVBoxLayout *&layout, string labelStr, bool isTitle)
{
	QLabel *label = new QLabel(); // Parameters label
	label->setText(labelStr.c_str());
	if (isTitle)
	{
		QFont font;
		font.setPointSize(12);
		font.setBold(true);
		label->setFont(font);
	}
	layout->addWidget(label);
}

void MainWindow::createCheckbox(QVBoxLayout *&layout, QCheckBox *&checkbox, std::string text, bool &syncedValue)
{

	checkbox = new QCheckBox();
	checkbox->setText(text.c_str());
	checkbox->setChecked(syncedValue);
	layout->addWidget(checkbox);
    auto onTick = 
	[&]() {
		syncedValue ^= true;
        checkbox->setChecked(syncedValue);
		realtime->settingsChanged();
	};
	connect(checkbox, &QCheckBox::clicked, this, onTick);
}

void MainWindow::createSlider(QVBoxLayout *&layout, QSlider *&slider, QSpinBox *&spinBox,
                              int tickInterval, int minimum, int maximum, int defaultValue, int &syncedValue)
{
	slider = new QSlider(Qt::Orientation::Horizontal);
	slider->setTickInterval(tickInterval);
	slider->setMinimum(minimum);
	slider->setMaximum(maximum);
	slider->setValue(defaultValue);

	spinBox = new QSpinBox();
	spinBox->setMinimum(minimum);
	spinBox->setMaximum(maximum);
	spinBox->setSingleStep(tickInterval);
	spinBox->setValue(defaultValue);

	QGroupBox *groupBox = new QGroupBox(); // horizonal slider 2 alignment
	QHBoxLayout *boxLayout = new QHBoxLayout();

	boxLayout->addWidget(slider);
	boxLayout->addWidget(spinBox);
	groupBox->setLayout(boxLayout);

	layout->addWidget(groupBox);

    auto onValueChangeWrapper = 
	[&](int newValue) {
		slider->setValue(newValue);
		spinBox->setValue(newValue);
		syncedValue = newValue;
		realtime->settingsChanged();
	};

    connect(slider, &QSlider::valueChanged, this, onValueChangeWrapper);
	connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, onValueChangeWrapper);
	syncedValue = defaultValue;
}

void MainWindow::createSliderDouble(QVBoxLayout *&layout, QSlider *&slider, QDoubleSpinBox *&spinBox,
                                    int tickIntervalSlider, int minimumSlider, int maximumSlider, int defaultValueSlider,
                                    float tickInterval, float minimum, float maximum, float defaultValue, float &syncedValue)
{
	slider = new QSlider(Qt::Orientation::Horizontal);
	slider->setTickInterval(tickIntervalSlider);
	slider->setMinimum(minimumSlider);
	slider->setMaximum(maximumSlider);
	slider->setValue(defaultValueSlider);

	spinBox = new QDoubleSpinBox();
	spinBox->setMinimum(minimum);
	spinBox->setMaximum(maximum);
	spinBox->setSingleStep(tickInterval);
	spinBox->setValue(defaultValue);

	QGroupBox *groupBox = new QGroupBox(); // horizonal slider 2 alignment
	QHBoxLayout *boxLayout = new QHBoxLayout();

	boxLayout->addWidget(slider);
	boxLayout->addWidget(spinBox);
	groupBox->setLayout(boxLayout);

	layout->addWidget(groupBox);

    auto onValueChangeWrapperSlider = [&](int newValue) {
		// inverse lerp 
        float fValue = float(newValue - slider->minimum()) / (slider->maximum() - slider->minimum()) *
            (spinBox->maximum() - spinBox->minimum()) + spinBox->minimum();
		slider->setValue(newValue);
		spinBox->setValue(fValue);
		syncedValue = fValue;
		realtime->settingsChanged();
	};

    auto onValueChangeWrapper = [&](double newValue) {
        int iValue = round(float(newValue - spinBox->minimum()) / (spinBox->maximum() - spinBox->minimum()) *
            (slider->maximum() -slider->minimum())) +slider->minimum();
		slider->setValue(iValue);
		spinBox->setValue(newValue);
		syncedValue = newValue;
		realtime->settingsChanged();
	};

	connect(slider, &QSlider::valueChanged, this, onValueChangeWrapperSlider);
	connect(spinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
			this, onValueChangeWrapper);
	syncedValue = defaultValue;
}

void MainWindow::initialize()
{
	realtime = new Realtime;

	QHBoxLayout *hLayout = new QHBoxLayout;	  // horizontal alignment
	QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
	vLayout->setAlignment(Qt::AlignTop);
	hLayout->addLayout(vLayout);
	hLayout->addWidget(realtime, 1);
	this->setLayout(hLayout);

	// createLabel(vLayout, "Camera", true);
	settings.nearPlane = 0.1;
	settings.farPlane = 100;
	// createLabel(vLayout, "Near Plane");
	// createSliderDouble(vLayout, nearSlider, nearBox,
	// 				   1, 1, 1000, 10,
	// 				   0.1f, 0.1f, 10.0f, 0.1f, settings.nearPlane);
	// createLabel(vLayout, "Far Plane");
	// createSliderDouble(vLayout, farSlider, farBox,
	// 				   1, 1000, 10000, 10000,
	// 				   0.1f, 10.0f, 100.0f, 100.0f, settings.farPlane);
	createLabel(vLayout, "Grass Generation", true);
	createLabel(vLayout, "Blade Count");
	createSlider(vLayout, bladeCntSlider, bladeCntBox, 1, 1, 500, 150, settings.bladeCntTmp);
	createLabel(vLayout, "Blade Segment Count");
	createSlider(vLayout,bladeSegmentCntSlider, bladeSegmentCntBox, 1, 1, 5, 4, settings.tempBladeSegments);
	createLabel(vLayout, "Density");
	createSliderDouble(vLayout, densitySlider, densityBox,
					   1, 1, 1000, 100,
                       0.1f, 0.1f, 100.0f, 10.0f, settings.density);
	createLabel(vLayout, "Blade Height Scale");
	createSliderDouble(vLayout, bladeHeightScaleSlider, bladeHeightScaleBox,
					   1, 1, 100, 10,
                       0.1f, 0.1f, 10.0f, 1.0f, settings.bladeHeightScale);

	createLabel(vLayout, "Blade Randomizations");
    createCheckbox(vLayout, applyPosWidthVariance, "Apply Positional + Width Variance", settings.applyPosWidthVariance);
    createCheckbox(vLayout, applyHeightVariance, "Apply Height Variance", settings.applyHeightVariance);

    forceRedrawButton = new QPushButton();
    forceRedrawButton->setText(QStringLiteral("Regenerate"));
    auto onRegenerate = [&]() {
		realtime->forceRedraw();
	};
	connect(forceRedrawButton, &QPushButton::clicked, this, onRegenerate);
	vLayout->addWidget(forceRedrawButton);

	createLabel(vLayout, "Tweakables", true);
    createCheckbox(vLayout, applyColor, "Apply Color", settings.applyColor);
    createCheckbox(vLayout, applyColorVariance, "Apply Color Variance", settings.applyColorVariance);
    createCheckbox(vLayout, applyWind, "Apply Wind", settings.applyWind);
    createCheckbox(vLayout, drawPlayer, "Player", settings.drawFirstPlayer);
    createCheckbox(vLayout, drawPlayer, "Friends", settings.drawFriendPlayer);
	createLabel(vLayout, "Hill height");
	createSliderDouble(vLayout, hillHeightSlider, hillHeightBox,
					   1, 0, 100, 10,
                       0.1f, 0.0f, 10.0f, 1.0f, settings.hillHeightMax);
	createLabel(vLayout, "Palette");
	createSlider(vLayout, paletteSlider, paletteBox, 1, 0, 0, 0, settings.colorPalette);

	realtime->settingsChanged();
}

void MainWindow::finish()
{
	realtime->finish();
	delete (realtime);
}
