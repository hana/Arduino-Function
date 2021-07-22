#pragma once

#include <Arduino.h>

namespace Maxy {
    class Function {
        public:
            Function(const float* _points, const size_t _numPoints, const float _initial_value = 0.0) : points(_points), numPoints(_numPoints), initialValue(_initial_value) {};            
            void begin(const bool _loop = false);
            float get();      
            float get(const float ellapsedTime);     
                        
            inline size_t getSceneIndex(const float ellapsedTime) const {
                size_t sceneIndex = 0;
                size_t nextEndTime = read_array(2 * sceneIndex + 1);

                while(nextEndTime < ellapsedTime) {
                    sceneIndex++;
                    
                    if(numPoints <= sceneIndex && !this->loop) {
                        return this->numPoints - 1; //returns last scene
                    }      

                    nextEndTime += read_array(2 * sceneIndex + 1);       
                }
                return sceneIndex;
            };


            inline size_t getNextScene(const size_t sceneIndex) const {
                size_t nextScene = sceneIndex + 1;

                if(numPoints < nextScene) {
                    nextScene = 0; 
                }
                
                return nextScene;
            }

        private:
            const float* points;
            const size_t numPoints;
            unsigned long beginTime;
            bool loop;
            float totalTime;
            float initialValue;

            inline float read_array(const size_t index) const {
                return pgm_read_float(points + index);        
            }

            inline float getBeginValue(const size_t sceneIndex) const {     
                if(sceneIndex == 0) {
                    return 0.0;
                } else {
                    return read_array(2 * (sceneIndex - 1));
                }
            };
                        
            float getLoopValue(unsigned long currentTime) const;
            float getNonLoopValue(unsigned long currentTime) const;
    };
};
