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
class DataPoint
{
public:
    int value;
    DataPoint* next;
    DataPoint(int value);
};

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
    // struct is just a Data Structure.  No methods, only named variables.
    // this particular structure is a "Linked List Node",
    // A value with a pointer to the next value in the list.


    int sensorPin;
    float outputMax;
    float outputMin;
    SensorDataQueue* data;
};


#endif //AnalogSensor_h