
#define NData 40

struct CircularBuffer{
  int data[NData];
  int index=0;
  int maxValue=-32678;
  int minValue=32677;
  void addValue(int iValue){
    data[index] = iValue;
    index = ( index + 1 ) % NData ;
    if(maxValue<iValue) maxValue = iValue;
    if(minValue>iValue) minValue = iValue;
  }
  
  
  int getValue(int i){
    return data[ ( index + i ) % NData ] ;
  }
};
