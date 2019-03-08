#pragma once

#include <functional>

#include "CWaggle.h"

typedef std::function<size_t(const SensorReading &)> HashFunction;

struct HashFunctionData
{
    HashFunction Function;
    size_t MaxHashSize = 0;
};

namespace Hash
{
    const HashFunctionData & GetHashData(const std::string & hashFunctionName);

    size_t OriginalHash(const SensorReading & reading)
    {
        const static std::string & name = "Original";
        const static size_t maxHashSize = GetHashData(name).MaxHashSize;

        const size_t MaxHashSize = (1 << 8);
        size_t hash = 0;

        hash += (reading.leftPucks == 0) ? 0 : (1 << 0);
        hash += (reading.rightPucks == 0) ? 0 : (1 << 1);
        hash += (reading.leftNest < reading.midNest) ? 0 : (1 << 2);
        hash += (reading.rightNest < reading.midNest) ? 0 : (1 << 3);

        size_t midNestReading = (size_t)(floor(reading.midNest * 16));
        if (midNestReading == 16) midNestReading = 15;
        hash += midNestReading * (1 << 4);

        if (hash >= maxHashSize) { std::cerr << "WARNING: " << name << " hash size too large: " << hash; }
        return hash;
    }

    size_t PuckMid4(const SensorReading & reading)
    {
        const static std::string & name = "PuckMid4";
        const static size_t maxHashSize = GetHashData(name).MaxHashSize;
        size_t hash = 0;

        hash += (reading.leftPucks == 0) ? 0 : (1 << 0);
        hash += (reading.rightPucks == 0) ? 0 : (1 << 1);

        size_t midNestReading = (size_t)(floor(reading.midNest * 4));
        if (midNestReading == 4) midNestReading = 3;
        hash += midNestReading * (1 << 2);

        if (hash >= maxHashSize) { std::cerr << "WARNING: " << name << " hash size too large: " << hash; }
        return hash;
    }

    size_t PuckMid16(const SensorReading & reading)
    {
        const static std::string & name = "PuckMid16";
        const static size_t maxHashSize = GetHashData(name).MaxHashSize;
        size_t hash = 0;

        hash += (reading.leftPucks == 0) ? 0 : (1 << 0);
        hash += (reading.rightPucks == 0) ? 0 : (1 << 1);

        size_t midNestReading = (size_t)(floor(reading.midNest * 16));
        if (midNestReading == 16) midNestReading = 15;
        hash += midNestReading * (1 << 2);

        if (hash >= maxHashSize) { std::cerr << "WARNING: " << name << " hash size too large: " << hash; }
        return hash;
    }

    const HashFunctionData & GetHashData(const std::string & hashFunctionName)
    {
        static std::map<std::string, HashFunctionData> hashData;

        // set up the hash function data the first time we call this function
        // it will only be called once per experiment
        if (hashData.empty())
        {
            hashData["Original"]    = { OriginalHash,   (1 << 8) };
            hashData["PuckMid4"]    = { PuckMid4,       (1 << 4) };
            hashData["PuckMid16"]   = { PuckMid16,      (1 << 6) };
        }

        if (hashData.find(hashFunctionName) == hashData.end())
        {
            std::cerr << "Warning: Hash Function Not Found: " << hashFunctionName << "\n";
            exit(-1);
        }

        return hashData[hashFunctionName];
    }

}