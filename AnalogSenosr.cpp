/*
 * Implementation of the AnalogSensor library
 * ~Jason Cox
 */

#include "AnalogSensor.h"
#include <Arduino.h>

//#define DEBUG

DataPoint::DataPoint(int v)
{
    this->value = v;
    this->next = NULL;
#ifdef DEBUG
    Serial.println((unsigned int)this, HEX);
    Serial.println((unsigned int)this->next, HEX);
#endif
}

SensorDataQueue::SensorDataQueue()
{
#ifdef DEBUG
    Serial.println("Initializing default SensorDataQueue...");
#endif
    this->queueLength = 25;
    this->runningAverage = 0;
    this->head = NULL;

#ifdef DEBUG
    Serial.println((unsigned int)this->head, HEX);
#endif
}

SensorDataQueue::SensorDataQueue(int len)
{
#ifdef DEBUG
    Serial.println("Initializing custom SensorDataQueue...");
#endif
    this->queueLength = len;
    this->runningAverage = 0;
    this->head=NULL;
#ifdef DEBUG
    Serial.println((unsigned int)this->head, HEX);
#endif
}

// Clean up that linked list memory!
SensorDataQueue::~SensorDataQueue()
{
#ifdef DEBUG
    Serial.println("destroying SensorDataQueue...");
#endif

    DataPoint* cur = head;
    while(cur != NULL)
    {
#ifdef DEBUG
    Serial.println((unsigned int)cur, HEX);
#endif
        DataPoint* temp = cur;
        cur = cur -> next;
        delete temp;
    }
}

// Constructors!
// Pin number must be provided, all other settings have default values.
AnalogSensor::AnalogSensor(int pin)
{
#ifdef DEBUG
    Serial.println("Initializing default AnalogSensor...");
#endif
    this->sensorPin = pin;
    this->outputMin = 0;
    this->outputMax = 1024;

    this->inputMin = 0;
    this->inputMax = 1024;

    this->data = new SensorDataQueue();
}
AnalogSensor::AnalogSensor(int pin, float min, float max)
{
#ifdef DEBUG
    Serial.println("Initializing AnalogSensor...");
#endif
    this->sensorPin = pin;
    this->outputMin = min;
    this->outputMax = max;

    this->inputMin = 0;
    this->inputMax = 1024;

    this->data = new SensorDataQueue();
}
AnalogSensor::AnalogSensor(int pin, int len)
{
#ifdef DEBUG
    Serial.println("Initializing AnalogSensor...");
#endif
    this->sensorPin = pin;
    this->outputMin = 0;
    this->outputMax = 1024;

    this->inputMin = 0;
    this->inputMax = 1024;

    this->data = new SensorDataQueue(len);
}
AnalogSensor::AnalogSensor(int pin, int len, float min, float max)
{
#ifdef DEBUG
    Serial.println("Initializing AnalogSensor...");
#endif
    this->sensorPin = pin;
    this->outputMin = min;
    this->outputMax = max;

    this->inputMin = 0;
    this->inputMax = 1024;

    this->data = new SensorDataQueue(len);
}

// Destructor Method--make sure to free up all the manually allocated memory.
AnalogSensor::~AnalogSensor()
{
#ifdef DEBUG
    Serial.println("Destroying AnalogSensor...");
#endif
    delete data;
}



// *******************************************************
// The actual methods start here!
// yep.  there's only three of them!
// *******************************************************

// Helper method, calculate the map function with float values instead of int.
float mapf(float x, float x0, float x1, float y0, float y1)
{
    float m = (y1 - y0)/(x1 - x0);
    float output = (m * (x - x0)) + y0;
    return output;
}

// get the value from the SensorDataQueue and map it to the expected output range.
float AnalogSensor::getValue()
{
#ifdef DEBUG
    Serial.println("getValue() called.");
#endif
    return mapf(data->runningAverage, inputMin, inputMax, outputMin, outputMax);
}



void SensorDataQueue::addDataPoint(int v)
{
#ifdef DEBUG
    Serial.print("addDataPoint(");
    Serial.print(v);
    Serial.println(") called.");
#endif

    DataPoint* dataPoint = new DataPoint(v);
#ifdef DEBUG
    Serial.println((unsigned int)dataPoint, HEX);
#endif

#ifdef DEBUG
    Serial.println("Created new DataPoint.");
#endif
    if(this->head == NULL)
    {
#ifdef DEBUG
    Serial.println("This is the first DataPoint in the list~!");
#endif
        this->head = dataPoint;
        this->runningAverage = v;
#ifdef DEBUG
    Serial.println((unsigned int)this->head, HEX);
#endif
    }
    else
    {
#ifdef DEBUG
    Serial.println("Adding a new DataPoint to the Queue");
#endif

        // counter going to be used later...
        int count = 1;

        // find the end of the list
        DataPoint* cur = this->head;
#ifdef DEBUG
    Serial.println((unsigned int)cur, HEX);
#endif
        while(cur->next != NULL)
        {
            cur = cur->next;
#ifdef DEBUG
    Serial.println((unsigned int)cur, HEX);
#endif
            count++;
#ifdef DEBUG
    Serial.print(count);
    Serial.print("\t");
#endif
        }

#ifdef DEBUG
    Serial.print(count);
    Serial.println((unsigned int)cur, HEX);
    Serial.println("items in the list...");
#endif

        // stick the new datapoint on the end
        cur->next = dataPoint;

#ifdef DEBUG
    Serial.println((unsigned int)cur->next, HEX);
#endif
        // check to see if we've exceeded the max length
        // if so, drop the first item in the list until we have the appropriate length
        while(count > queueLength)
        {
#ifdef DEBUG
    Serial.println("Queue is too long!");
#endif

            DataPoint* temp = this->head;
            this->head = this->head->next;
            delete temp; // always clean up after yourself!
            count--;

#ifdef DEBUG
            Serial.println((unsigned int)temp, HEX);
            Serial.println((unsigned int)this->head, HEX);
            Serial.println("Deleted the head pointer.");
#endif
        }

        // now compute the running average

#ifdef DEBUG
        Serial.println("Computing Running average now...");
#endif
        this->runningAverage = 0;

        cur = this->head; // go back to the start...
        count = 0;
        // add up all the values.
        while(cur != NULL)
        {
            this->runningAverage += cur->value;
            count++;
            cur = cur->next;
        }
        // divide by the number of values.
        this->runningAverage /= count;
    }
#ifdef DEBUG
    Serial.println("addDataPoint completed.");
#endif
}

void AnalogSensor::update()
{
#ifdef DEBUG
    Serial.println("updating the sensor reading...");
#endif

    int value = analogRead(sensorPin);
    data->addDataPoint(value);
}

// Default calibration period is 30 seconds.
void AnalogSensor::calibrate()
{
    this->calibrate(30000);
}

// Calibrate the sensor for a given period of time (ms).
void AnalogSensor::calibrate(unsigned long ms)
{
    long startTime = millis();
    long currentTime = startTime;

#ifdef DEBUG
    Serial.print("Beginning Calibration at ");
    Serial.print(startTime);
    Serial.println(" ms");
#endif

    this->inputMin = analogRead(this->sensorPin);
    this->inputMax = this->inputMin+1;

    while(currentTime - startTime < ms)
    {
        int v = analogRead(this->sensorPin);
        // move minimum down if this reading is lower...
        if(v < this -> inputMin)    this->inputMin = v;
        // move maximum up if this reading is higher...
        if(v > this -> inputMax)    this->inputMax = v+1;

        // check the clock
        currentTime = millis();

#ifdef DEBUG
        Serial.println("========================");
        Serial.print("[ ");
        Serial.print(this->inputMin);
        Serial.print(", ");
        Serial.print(this->inputMax);
        Serial.println(" )");
#endif

        delay(20);
        // this is ignoring the possibility that millis might roll over
        // because that is unlikely to happen while youre calibrating the sensor.
    }
#ifdef DEBUG
    Serial.println("=======================");
    Serial.println("Calibration complete!");
#endif
}