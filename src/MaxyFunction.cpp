#include "MaxyFunction.hpp"

using namespace Maxy;

Function::Function() {    
    scenes.clear();
    beginTime = 0;
    totalTime = 0;
    loop = false;
} 

void Maxy::Function::add(const float val, const unsigned long duration) {
    totalTime += duration;

    TimePoint tp;
    tp.duration = duration;
    tp.duration_reciprocal = 1.0 / static_cast<float>(duration);
    tp.endValue = val;

    if(scenes.size() == 0) {          
        tp.endTime = duration;             
        // tp.beginValue = 0.0;
        tp.diff = val;
    } else {
        const TimePoint& previousScene = scenes[scenes.size() - 1];                
        tp.endTime = previousScene.endTime + duration;          
        // tp.beginValue = previousScene.endValue;
        tp.diff = tp.endValue - previousScene.endValue;
    }
    
    scenes.push_back(tp);   
}

void Function::begin(const bool _loop) {
    loop = _loop;

    if(loop) { 
        TimePoint& firstScene = scenes[0];
        const TimePoint& lastScene = scenes[scenes.size() - 1];        
        firstScene.diff = lastScene.endValue - firstScene.endValue;
        funcPtr = &this->getLoopValue;
    } else {
        funcPtr = &this->getNonLoopValue;
    }
    
    beginTime = millis();
}

float Function::getNonLoopValue(unsigned long currentTime) const {
    if(totalTime < currentTime) {
        return scenes[scenes.size() - 1].endValue;
    }

    while(totalTime < currentTime) {
        currentTime -= totalTime;        
    }   
    const size_t index = this->getIndex(currentTime);
    const TimePoint& targetScene = scenes[index];

    size_t previousIndex = getPreviousIndex(index);

    const TimePoint& previousScene = scenes[previousIndex];
    const unsigned long relativeTime = currentTime - previousScene.endTime;

    return static_cast<float>(relativeTime) * targetScene.duration_reciprocal * targetScene.diff + previousScene.endValue;     
}

float Function::getLoopValue(unsigned long currentTime) const {
    while(totalTime < currentTime) {
        currentTime -= totalTime;        
    }   

    const size_t index = this->getIndex(currentTime);
    const TimePoint& targetScene = scenes[index];

    size_t previousIndex = getPreviousIndex(index);

    if(index == 0) {
        return static_cast<float>(currentTime) * targetScene.duration_reciprocal * targetScene.diff;     
    } else {
        const TimePoint& previousScene = scenes[previousIndex];
        const unsigned long relativeTime = currentTime - previousScene.endTime;
        return static_cast<float>(relativeTime) * targetScene.duration_reciprocal * targetScene.diff + previousScene.endValue;     
    }
}

float Function::get() {    
    unsigned long currentTime = millis() - beginTime;
    
    // if(loop) {
    //     return getLoopValue(currentTime);
    // } else {
    //     return getNonLoopValue(currentTime);
    // }

    return (this->*funcPtr)(currentTime);
}

bool Function::isInScene(const unsigned long ellapsedTime, const size_t index) const {
    if(index == 0) {
        if(0 < ellapsedTime && ellapsedTime <= scenes[index].endTime) {
            return true;
        } else {
            return false;
        }
    } else {
        size_t previousIndex = getPreviousIndex(index);
        if(scenes[previousIndex].endTime < ellapsedTime && ellapsedTime <= scenes[index].endTime) {
            return true;
        } else {
            return false;
        }
    }
}

size_t Function::getIndex(const unsigned long ellapsedTime) const {
    if(ellapsedTime == 0) {
        return 0;
    } else {
        size_t index = 0;        
        while(!isInScene(ellapsedTime, index)) {
            index++;
        }
        return index;
    }
}

