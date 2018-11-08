// V1
#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#define NMaxSize 400
#include <Arduino.h>

struct CircularBuffer{
  int data[NMaxSize];
  int NData = 0;
  int index = 0;

  int maxValue = -32678;
  int minValue = 32677;
  long historicalCount = 0;

  void addValue(int iValue){

    data[index] = iValue;

    index = ( index + 1 ) % NMaxSize ;

    if ( NData < NMaxSize ) NData = ( NData + 1 ) ;

    historicalCount ++ ;
  }

  int getValue(int i){
    return data[ ( index + i ) % NData ] ;
  }

  int getMinimum(){
    minValue = getValue(0);

    for( int i = 1 ; i<NData ; i++ ) minValue = min(minValue, getValue(i) );

    return minValue ;
  }

  int getMaximum(){
    maxValue = getValue(0);

    for( int i = 1 ; i<NData ; i++ ) maxValue = max(maxValue, getValue(i) );

    return maxValue;
  }

  float getAverage(){
    float sum = 0;
    for( int i = 0 ; i<NData ; i++ ){
      sum+=data[i];
    }
    return sum/NData;
  }


};

#endif
