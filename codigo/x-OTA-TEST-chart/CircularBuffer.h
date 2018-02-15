
#define NMaxSize 500

struct CircularBuffer{
  int data[NMaxSize];
  int NData = 0;
  int iNextPosition = 0;
  int iFirstPosition = 0;
  
  int maxValue = -32678;
  int minValue = 32677;
  long historicalCount = 0;
  
  void addValue(int iValue){
    bool bRecalculateMin = false;
    bool bRecalculateMax = false;

    data[iNextPosition] = iValue;
    
    iNextPosition = ( iNextPosition + 1 ) % NMaxSize ;
    
    if ( NData < NMaxSize ) {
      NData = ( NData + 1 ) ;
    } else  {
        if (iNextPosition == 0 ) iFirstPosition = NMaxSize - 1;
          else iFirstPosition = iNextPosition -1 ;
    }
    historicalCount ++ ;
  }

  int getValue(int i){
    return data[ ( iFirstPosition + i ) % NData ] ;
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
      sum += getValue(i);
    }
    return sum/NData;
  }
  

};
