#pragma once

#include <ArxContainer.h>

namespace Maxy {
    class Function {
        public:
            struct TimePoint {
                unsigned int duration;
                float duration_reciprocal;
                unsigned int endTime;
                // float beginValue;  // goal   
                float endValue;                        
                float diff;   // difference from the previous value
                // unsigned long beginTime;                
            };

            Function();
            void add(const float val, const unsigned long duration);
            void begin(const bool _loop = false);
            float get();            

        private:
            arx::vector<TimePoint> scenes;
            unsigned long beginTime;       
            unsigned long totalTime;
            bool loop;
            using getptr = float (Maxy::Function::*)(const unsigned long)const;
            getptr funcPtr;
            
            size_t getPreviousIndex(const size_t index) const {
                if(index == 0) {
                    return scenes.size() - 1;
                } else {
                    return index - 1;
                }
            };
            size_t getIndex(const unsigned long ellapsedTime) const;
            bool isInScene(const unsigned long ellapsedTime, const size_t index) const;

            float getLoopValue(unsigned long currentTime) const;
            float getNonLoopValue(unsigned long currentTime) const;
    };
}
