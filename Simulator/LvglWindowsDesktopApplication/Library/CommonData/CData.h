#ifndef _CDATA_H
#define _CDATA_H

template<class T>
class CData
{
private:
    T value; // Current value of the data object
    T oldValue; // Previous value before the last update
    bool state; // Change flag (true if value has changed since last reset)

public:
    CData()
    {
        this->state = false;
    }

    ~CData() = default;

    // Returns the current stored value
    T GetValue()
    {
        return this->value;
    }

    // Returns the value prior to the last SetValue call
    T GetOldValue()
    {
        return this->oldValue;
    }

    // Updates the value and sets the change flag if the new value is different
    void SetValue(T value)
    {
        if (value != this->value)
        {
            this->oldValue = this->value;
            this->value = value;
            this->state = true;
        }
    }

    // Checks if the value has been updated recently
    bool GetState()
    {
        return this->state;
    }

    // Resets the change flag to false
    void ResetState()
    {
        if (this->state)
        {
            //this->oldValue = this->value;
            this->state = false;
        }
    }
};

#endif // !_CDATA_H
