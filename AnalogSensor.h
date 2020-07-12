/*
 * AnalogSensor is an example library developed by Jason Cox
 * as a simple encapsulation of an Analog sensor with a single
 * input.  The library is written specifically to highlight
 * the "running-average" method of cleaning up messy analog
 * data, and is not intended to be particularly efficient.
 */

#ifndef AnalogSensor_h  // C++ for only include this library once~ every library starts like this.
#define AnalogSensor_h  // this definition alerts the compiler that this code has been added and
                        // does not need to be re-added if it is included multiple times.

// DataPoint is a "linked list node"
// it holds a Value and a Pointer to the next item in the list.
class DataPoint
{
public:
    int value;
    DataPoint* next;
    DataPoint(int value);
};

// SensorDataQueue keeps track of a Queue of DataPoints.
// It holds a particular number of DataPoints and keeps track of the Average value.
class SensorDataQueue
{
public:
    float runningAverage;
    void addDataPoint(int v);
    SensorDataQueue();
    SensorDataQueue(int len);
    ~SensorDataQueue();
private:
    int queueLength;
    DataPoint* head;
};

// AnalogSensor encapsulates the
class AnalogSensor
{
public:
    float getValue();
    void update();
    AnalogSensor(int pin);
    AnalogSensor(int pin, float min, float max);
    AnalogSensor(int pin, int len);
    AnalogSensor(int pin, int len, float min, float max);
    ~AnalogSensor();
private:
    int sensorPin;
    float outputMax;
    float outputMin;
    SensorDataQueue* data;
};


#endif //AnalogSensor_h