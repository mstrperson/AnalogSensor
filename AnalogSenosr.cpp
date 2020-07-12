/*
 * Implementation of the AnalogSensor library
 * ~Jason Cox
 */

#include "AnalogSensor.h"
#include <Arduino.h>

SensorDataQueue::SensorDataQueue()
{
    this->queueLength = 25;
    this->runningAverage = 0;
}

SensorDataQueue::SensorDataQueue(int len)
{
    this->queueLength = len;
    this->runningAverage = 0;
}

// Clean up that linked list memory!
SensorDataQueue::~SensorDataQueue()
{
    DataPoint* cur = head;
    while(cur != NULL)
    {
        DataPoint* temp = cur;
        cur = cur -> next;
        delete temp;
    }
}

// Constructors!
// Pin number must be provided, all other settings have default values.
AnalogSensor::AnalogSensor(int pin)
{
    this->sensorPin = pin;
    this->outputMin = 0;
    this->outputMax = 1024;
    this->data = new SensorDataQueue();
}
AnalogSensor::AnalogSensor(int pin, float min, float max)
{
    this->sensorPin = pin;
    this->outputMin = min;
    this->outputMax = max;
    this->data = new SensorDataQueue();
}
AnalogSensor::AnalogSensor(int pin, int len)
{
    this->sensorPin = pin;
    this->outputMin = 0;
    this->outputMax = 1024;
    this->data = new SensorDataQueue(len);
}
AnalogSensor::AnalogSensor(int pin, int len, float min, float max)
{
    this->sensorPin = pin;
    this->outputMin = min;
    this->outputMax = max;
    this->data = new SensorDataQueue(len);
}

// Destructor Method--make sure to free up all the manually allocated memory.
AnalogSensor::~AnalogSensor()
{
    delete data;
}



// *******************************************************
// The actual methods start here!
// yep.  there's only three of them!
// *******************************************************

// get the value from the SensorDataQueue and map it to the expected output range.
float AnalogSensor::getValue()
{
    return map(data->runningAverage, 0, 1024, outputMin, outputMax);
}

void SensorDataQueue::addDataPoint(int v)
{
    DataPoint* dataPoint = new DataPoint();
    dataPoint->value = v;

    if(this->head == NULL)
    {
        this->head = dataPoint;
        this->runningAverage = v;
    }
    else
    {
        // counter going to be used later...
        int count = 0;

        // find the end of the list
        DataPoint* cur = this->head;
        while(cur->next != NULL)
        {
            cur = cur->next;
            count++;
        }

        // stick the new datapoint on the end
        cur->next = dataPoint;

        // check to see if we've exceeded the max length
        // if so, drop the first item in the list until we have the appropriate length
        while(count > queueLength)
        {
            DataPoint* temp = this->head;
            this->head = this->head->next;
            delete temp; // always clean up after yourself!
            count--;
        }

        // now compute the running average

        this->runningAverage = 0;

        cur = this->head; // go back to the start...
        // add up all the values.
        while(cur != NULL)
        {
            this->runningAverage += cur->value;
        }
        // divide by the number of values.
        this->runningAverage /= (float)count;
    }
}

void AnalogSensor::update()
{
    int value = analogRead(sensorPin);
    data->addDataPoint(value);
}