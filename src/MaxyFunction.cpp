#include "MaxyFunction.h"

using namespace Maxy;

void Function::begin(const bool _loop) {
    loop = _loop;    

    if(loop) {
        initialValue = read_array(2 * (numPoints - 1));
    }

    for(size_t i = 0; i < numPoints; i++) {
        totalTime += read_array(2 * i + 1);
    }

    // Serial.print(F("Total Time: "));
    // Serial.println(totalTime);
    beginTime = millis();
}

float Function::getNonLoopValue(unsigned long currentTime) const {
       
}

float Function::getLoopValue(unsigned long currentTime) const {

}

float Function::get() {        
    if(numPoints == 0) {
        return initialValue;
    }

    float ellapsedTime = static_cast<float>(millis() - beginTime);

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