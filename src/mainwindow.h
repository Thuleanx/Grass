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

	void createCheckbox(QVBoxLayout *&layout, QCheckBox *&checkbox, std::string text, bool defaultValue);

	void createSlider(QVBoxLayout *&layout, QSlider *&slider, QSpinBox *&spinBox, 
        int tickInterval, int minimum, int maximum, int defaultValue);

	void createSliderDouble(QVBoxLayout *&layout, QSlider *&slider, QDoubleSpinBox *&spinBox, 
		int tickIntervalSlider, int minimumSlider, int maximumSlider, int defaultValueSlider,
        float tickfloaterval, float minimum, float maximum, float defaultValue);

    Realtime *realtime;
    QSlider *nearSlider;
    QSlider *farSlider;
    QDoubleSpinBox *nearBox;
    QDoubleSpinBox *farBox;

private slots:
    void onValChangeNearSlider(int newValue);
    void onValChangeFarSlider(int newValue);
    void onValChangeNearBox(double newValue);
    void onValChangeFarBox(double newValue);
};
