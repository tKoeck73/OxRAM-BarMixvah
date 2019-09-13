boolean pumps[5];

void setup(){
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Pump controller");
}

int countSplitCharacters(String text, char splitChar) {
    int returnValue = 0;
    for(int i=0; i<=text.length()-1; i++){
      if(text.charAt(i)==splitChar){
          returnValue++;
      }
    }
    return returnValue;
} 

String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void togglePump(int i){
  pumps[i-8] = not pumps[i-8];
  digitalWrite(i,pumps[i-8]);
}

void snooze(int t){
  delay(1000*t);
}
int getMin(int arr[]){
  int minVal = 31000;
  for(int i=0; i<5; i++){
    if (arr[i] < minVal and arr[i] > 0){
      minVal = arr[i];
    }
  }
  return minVal;
}

boolean isZero(int arr[]){
  boolean val = true;
  for(int i=0; i<=4; i++){
    val = val and (arr[i]==0);
  }
  return val;
}

void loop() {
  if (Serial.available()){
    String s = Serial.readString();
    //Serial.println(String(s.length()));
    int splits = countSplitCharacters(s,',');
    //Serial.println(String(splits));
    int t;
    int id;
    switch (splits){
      case 0:
        Serial.print("toggled pump " + s);
        togglePump(s.toInt());
        break;
      case 1:
        t = getValue(s,',',1).toInt();
        id = getValue(s,',',0).toInt();
        Serial.println("toggle pump " + String(id) + " on for " + String(t) + "seconds" );
        togglePump(id);
        snooze(t);
        togglePump(id);
        break;
      case 4:
        int times[5];
        for(int i=8; i<=12; i++){
          times[i-8] = getValue(s,',',i-8).toInt();
          pumps[i-8] = times[i-8];
          digitalWrite(i,pumps[i-8]);
        }
        //Serial.println(getMin(times));
        //Serial.println(isZero(times));
        while (not(isZero(times))){
          t = getMin(times);
          snooze(t);
          Serial.println(t);
          for(int i=0; i<=4; i++){
            if (times[i]>=t){
              times[i]-=t;
            }
            digitalWrite(i+8,times[i]);
            pumps[i]=times[i];
          }
        }
    }
  }
}




