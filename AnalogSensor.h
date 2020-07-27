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

float mapf(float a, float in_min, float in_max, float out_min, float out_max);

// DataPoint is a "linked list node"
// it holds a Value and a Pointer to the next item in the list.
class DataPoint
{
public:
    // value stored in this DataPoint
    int value;
    // pointer to the next DataPoint in this list.
    DataPoint* next;

    // Constuctor, initializes DataPoint with a value and NULL for the next ptr.
    DataPoint(int value);
};

// SensorDataQueue keeps track of a Queue of DataPoints.
// It holds a particular number of DataPoints and keeps track of the Average value.
class SensorDataQueue
{
public:
    float runningAverage;

    // Add a datapoint to the Queue and update the running average.
    void addDataPoint(int v);

    // constructors/destuctors.
    SensorDataQueue();
    SensorDataQueue(int len);
    ~SensorDataQueue();
private:
    // maximum length of this queue.
    // once the max length is reached, the addDataPoint method drops the *head after
    // adding the new datapoint to the queue
    int queueLength;

    // Pointer to the first item in the Queue.
    DataPoint* head;
};

// AnalogSensor encapsulates the
class AnalogSensor
{
public:
    // calibrate the sensor for 30 seconds
    void calibrate();

    // calibrate the sensor for a specified number of milliseconds.
    void calibrate(unsigned long ms);

    // Get the Sensor running average mapped to the configured output range.
    float getValue();

    // Update the sensor data with a new DataPoint.
    void update();

    // constructors and destructor.
    AnalogSensor(int pin);
    AnalogSensor(int pin, float min, float max);
    AnalogSensor(int pin, int len);
    AnalogSensor(int pin, int len, float min, float max);
    ~AnalogSensor();
private:
    // Analog pin the sensor is connected to.
    int sensorPin;

    // calibrated sensor input range
    int inputMin;
    int inputMax;

    // defined output range.
    float outputMax;
    float outputMin;

    // pointer to the DataQueue where sensor data is calculated.
    SensorDataQueue* data;
};


#endif //AnalogSensor_h