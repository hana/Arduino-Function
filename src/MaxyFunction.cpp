#include "MaxyFunction.h"

using namespace Maxy;

void Function::begin(const bool _loop) {
    loop = _loop;    

    if(loop) {
        initialValue = read_array(2 * (numPoints - 1));
    }

    float totalTimef = 0.0;
    totalTime = 0;
    for(size_t i = 0; i < numPoints; i++) {
        totalTimef += read_array(2 * i + 1);
        // totalTime += static_cast<unsigned long>(read_array(2 * i + 1));
    }
    totalTime = static_cast<unsigned long>(totalTimef);

    beginTime = millis();
}

// float Function::getNonLoopValue(unsigned long currentTime) const {
//     return 0.0;      
// }

// float Function::getLoopValue(unsigned long currentTime) const {
//     return 0.0;
// }

float Function::get() {        
    if(numPoints == 0) {
        return initialValue;
    }

    unsigned long ellapsedTime = millis() - beginTime;

    while(totalTime < ellapsedTime) {
        ellapsedTime -= totalTime;
    }

    return this->get(ellapsedTime);
}


float Function::get(const float ellapsedTime) {
    const size_t sceneIndex = getSceneIndex(ellapsedTime);

    const float beginValue = getBeginValue(sceneIndex);
    const float endValue = read_array(2 * sceneIndex);
    
    if(beginValue == endValue) {
        return endValue;
    }

    const float diff = endValue - beginValue;

    // get ellapsed time in this scene
    float total_time_before_this_frame = 0.0;
    for(size_t i = 0; i < sceneIndex; i++) {
        total_time_before_this_frame += read_array(2 * i + 1);
    }

    const float ellapsedTime_in_this_frame = ellapsedTime - total_time_before_this_frame;
    const float duration_of_this_frame = read_array(2 * sceneIndex + 1);

    if(duration_of_this_frame == 0) {
        return endValue;
    }

    return diff * ellapsedTime_in_this_frame / duration_of_this_frame + beginValue;    
}