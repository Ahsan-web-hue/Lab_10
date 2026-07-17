#ifndef INC_CIRCULARBUFFER_H_
#define INC_CIRCULARBUFFER_H_

#include "main.h"
#include <string.h>

template <class T>
class CircularBuffer
{
public:
    CircularBuffer(uint16_t size)
        : _size(size), _in_pos(0), _out_pos(0), _stored(0)
    {
        _buffer = new T[_size];
        memset(_buffer, 0, sizeof(T) * _size);
    }

    ~CircularBuffer()
    {
        delete[] _buffer;
    }

    // Add data to buffer
    void put(T *data, uint16_t size)
    {
        for (uint16_t i = 0; i < size; i++)
        {
            _buffer[_in_pos] = data[i];

            _in_pos++;
            if (_in_pos >= _size)
            {
                _in_pos = 0;
            }

            if (_stored < _size)
            {
                _stored++;
            }
            else
            {
                // Buffer full, overwrite oldest data
                _out_pos++;
                if (_out_pos >= _size)
                {
                    _out_pos = 0;
                }
            }
        }
    }

    // Read data from buffer
    uint16_t pull(T *data, uint16_t size)
    {
        uint16_t read_size = 0;

        for (uint16_t i = 0; i < size; i++)
        {
            if (_stored == 0)
            {
                break;
            }

            data[i] = _buffer[_out_pos];

            _out_pos++;
            if (_out_pos >= _size)
            {
                _out_pos = 0;
            }

            _stored--;
            read_size++;
        }

        return read_size;
    }

    // Number of stored elements
    uint16_t count() const
    {
        return _stored;
    }

    // Check if buffer is empty
    bool isEmpty() const
    {
        return (_stored == 0);
    }

    // Check if buffer is full
    bool isFull() const
    {
        return (_stored == _size);
    }

    // Clear buffer
    void flush()
    {
        _in_pos = 0;
        _out_pos = 0;
        _stored = 0;

        memset(_buffer, 0, sizeof(T) * _size);
    }

    // Buffer capacity
    uint16_t capacity() const
    {
        return _size;
    }

private:
    T *_buffer;

    uint16_t _size;
    uint16_t _in_pos;
    uint16_t _out_pos;
    uint16_t _stored;
};

#endif /* INC_CIRCULARBUFFER_H_ */
