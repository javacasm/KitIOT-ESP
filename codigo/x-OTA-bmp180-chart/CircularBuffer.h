
#define NData 40

struct CircularBuffer{
  int data[NData];
  int index=0;
  int maxValue=-32678;
  int minValue=32677;
  
  void addValue(int iValue){
    bool bRecalculateMin = false;
    bool bRecalculateMax = false;
    if ( data[index] == minValue ) bRecalculateMin = true;
    if ( data[index] == maxValue ) bRecalculateMax = true;
    data[index] = iValue;
    index = ( index + 1 ) % NData ;
    if(maxValue<iValue) maxValue = iValue;
    else if (bRecalculateMax ) {
      for( int i = 0 ; i<NData ; i++ ){
        if(maxValue<data[i]) maxValue = data[i];
      }
    }
    if(minValue>iValue) minValue = iValue;
    else if (bRecalculateMin ) {
      for( int i = 0 ; i<NData ; i++ ){
        if(minValue>data[i]) minValue = data[i];
      }
    }
  }

  
  
  float getAverage(){
    float sum = 0;
    for( int i = 0 ; i<NData ; i++ ){
      sum+=data[i];
    }
    return sum/NData;
  }
  
  int getValue(int i){
    return data[ ( index + i ) % NData ] ;
  }
};
