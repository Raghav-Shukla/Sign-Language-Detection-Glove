#include <SoftwareSerial.h>// import the serial library

SoftwareSerial bt(11, 10); // RX, TX

int hand[]={0,0,0,0,0};
float low[]={0,0,0,0,0};
float high[]={0,0,0,0,0};
float min[]={1023,1023,1023,1023,1023};
float max[]={0,0,0,0,0};
float val[]={0,0,0,0,0};
long NUMITERATIONS=1000;
long AVG=0;



// Define a hashmap-like structure to map arrays to strings
struct ArrayToStringMap {
    int key[5];
    String value;
};


ArrayToStringMap hi[]={
  {{0,1,0,0,0},"One"},
  {{0,1,1,0,0},"Two"},
  {{0,1,1,1,0},"Three"},
  {{0,1,1,1,1},"Four"},
  {{1,1,1,1,1},"Five"},
  {{1,0,0,0,0},"Thumbs-Up"},
  {{1,1,1,0,0},"Gun"},
  {{1,1,0,0,1},"Spiderman"},
  {{0,0,1,1,1},"OK"},
  {{1,0,0,0,1},"Call"},
  {{0,0,0,1,1},"Y"},
  {{0,0,0,0,1},"I"}
};

bool arraysAreEqual(int arr1[], int arr2[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return false; // Arrays are not equal
        }
    }
    return true; // Arrays are equal
}

String getValue(int arr[5]) {
    for (int i = 0; i < 12; i++) {
       
        if (arraysAreEqual(arr,hi[i].key,5))
        {
            return hi[i].value;
        }
    }
    // Key not found, return an empty string
    return "None";
}


unsigned long interval=15000;
void setup() {
  // put your setup code here, to run once:
  bt.begin(9600);
  bt.println("Bluetooth On please wait...");
}

void loop() 
{
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  for(int pin=0;pin<5;pin++)
  val[pin]=analogRead(pin);
  
  if(currentMillis<5000)
  {
    bt.println("Relax");
    for(int pin=0;pin<5;pin++)
    {
      AVG=0;
      for(int i=0;i<NUMITERATIONS;i++)
      {
        val[pin]=analogRead(pin);
        AVG=AVG+val[pin];
      }
  
      AVG/=NUMITERATIONS;
      max[pin]=AVG;
    }

  }
  else if(currentMillis<10000)
  {
    bt.println("Bend");
    for(int pin=0;pin<5;pin++)
    {
      AVG=0;
      for(int i=0;i<NUMITERATIONS;i++)
      {
        val[pin]=analogRead(pin);
        AVG=AVG+val[pin];
      }
  
      AVG/=NUMITERATIONS;
      min[pin]=AVG;
      low[pin]=((max[pin]+min[pin])/2)-((max[pin]-min[pin])/8);
      high[pin]=((max[pin]+min[pin])/2)+((max[pin]-min[pin])/8); 
    }
  }
  else
  {
    for(int pin=0;pin<5;pin++)
    {
      AVG=0;
      for(int i=0;i<NUMITERATIONS;i++)
      {
        val[pin]=analogRead(pin);
        AVG=AVG+val[pin];
      }
  
      AVG/=NUMITERATIONS;
      if(AVG<low[pin])
      hand[pin]=0;
    
      if(AVG>high[pin])
      hand[pin]=1;

      // bt.print(hand[pin]);
      // bt.print(" ");
    }
    bt.println(getValue(hand));
    // bt.println("");
    // delay(100);
  }
}
