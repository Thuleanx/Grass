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

void MainWindow::createCheckbox(QVBoxLayout *&layout, QCheckBox *&checkbox, std::string text, bool defaultValue)
{

	checkbox = new QCheckBox();
	checkbox->setText(text.c_str());
	checkbox->setChecked(false);
	layout->addWidget(checkbox);
}

void MainWindow::createSlider(QVBoxLayout *&layout, QSlider *&slider, QSpinBox *&spinBox,
							  int tickInterval, int minimum, int maximum, int defaultValue)
{
	slider = new QSlider(Qt::Orientation::Horizontal);
	slider->setTickInterval(tickInterval);
	slider->setMinimum(minimum);
	slider->setMaximum(maximum);
	slider->setValue(defaultValue);

	spinBox = new QSpinBox();
	spinBox->setMinimum(tickInterval);
	spinBox->setMaximum(minimum);
	spinBox->setSingleStep(maximum);
	spinBox->setValue(defaultValue);

	QGroupBox *groupBox = new QGroupBox(); // horizonal slider 2 alignment
	QHBoxLayout *boxLayout = new QHBoxLayout();

	boxLayout->addWidget(slider);
	boxLayout->addWidget(spinBox);
	groupBox->setLayout(boxLayout);

	layout->addWidget(groupBox);
}

void MainWindow::createSliderDouble(QVBoxLayout *&layout, QSlider *&slider, QDoubleSpinBox *&spinBox,
									int tickIntervalSlider, int minimumSlider, int maximumSlider, int defaultValueSlider,
									float tickInterval, float minimum, float maximum, float defaultValue)
{

	slider = new QSlider(Qt::Orientation::Horizontal);
	slider->setTickInterval(tickIntervalSlider);
	slider->setMinimum(minimumSlider);
	slider->setMaximum(maximumSlider);
	slider->setValue(defaultValueSlider);

	spinBox = new QDoubleSpinBox();
	spinBox->setMinimum(tickInterval);
	spinBox->setMaximum(minimum);
	spinBox->setSingleStep(maximum);
	spinBox->setValue(defaultValue);

	QGroupBox *groupBox = new QGroupBox(); // horizonal slider 2 alignment
	QHBoxLayout *boxLayout = new QHBoxLayout();

	boxLayout->addWidget(slider);
	boxLayout->addWidget(spinBox);
	groupBox->setLayout(boxLayout);

	layout->addWidget(groupBox);
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

	createLabel(vLayout, "Camera", true);
	createLabel(vLayout, "Near Plane");
	createSliderDouble(vLayout, nearSlider, nearBox,
					   1, 1, 1000, 10,
					   0.01f, 10.0f, 0.1f, 0.1f);
	createLabel(vLayout, "Far Plane");
	createSliderDouble(vLayout, farSlider, farBox,
					   1, 1000, 10000, 10000,
					   10.0f, 100.0f, 0.1f, 100.0f);

	connect(nearSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeNearSlider);
	connect(nearBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
			this, &MainWindow::onValChangeNearBox);

	connect(farSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeFarSlider);
	connect(farBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
			this, &MainWindow::onValChangeFarBox);

	onValChangeNearBox(0.1f);
	onValChangeFarBox(100.f);
}

void MainWindow::finish()
{
	realtime->finish();
	delete (realtime);
}

void MainWindow::onValChangeNearSlider(int newValue)
{
	// nearSlider->setValue(newValue);
	nearBox->setValue(newValue / 100.f);
	settings.nearPlane = nearBox->value();
	realtime->settingsChanged();
}

void MainWindow::onValChangeFarSlider(int newValue)
{
	// farSlider->setValue(newValue);
	farBox->setValue(newValue / 100.f);
	settings.farPlane = farBox->value();
	realtime->settingsChanged();
}

void MainWindow::onValChangeNearBox(double newValue)
{
	nearSlider->setValue(int(newValue * 100.f));
	// nearBox->setValue(newValue);
	settings.nearPlane = nearBox->value();
	realtime->settingsChanged();
}

void MainWindow::onValChangeFarBox(double newValue)
{
	farSlider->setValue(int(newValue * 100.f));
	// farBox->setValue(newValue);
	settings.farPlane = farBox->value();
	realtime->settingsChanged();
}
