const int pd[3] = {A5,A6,A7};
int pd_val[3] = {0,0,0}; 
int pd_val_max[3] ={0,0,0}; 
int pd_val_min[3] = {1023,1023,1023};
int pd_avg[3];
void pd_calib();
                                                          //ldr block
const int block =  A3;
int block_val;
int block_avg;
int block_max = 0;
int block_min = 1023;

                                                        //ultrsonic sensor 
const int trigPin = 7;
const int echoPin = 9;
long duration;
int distance; 
                                                             //ir sensor
int LED = 12;
int obstaclePin = A0;
int hasObstacle = HIGH;

void setup() 
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(pd[0], INPUT);
  pinMode(pd[1],INPUT);
  pinMode(pd[2],INPUT);
  pinMode(echoPin,INPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(obstaclePin,INPUT);
  pinMode(block,INPUT);
  pd_calib();
  block_calib();
}

void loop() 
{
  int i;
  for(i=0;i<3;i++)
  {
  pd_val[i] = analogRead(pd[i]);
  }
  
    Serial.println(pd_val[0]);
    Serial.println(pd_val[1]);
     Serial.println(pd_val[2]); 
  
    
                                                                                        //  Serial.println(sensor);
  if (pd_val[0] < pd_avg[0] && pd_val[1] > pd_avg[1] && pd_val[2] < pd_avg[2] ) 
  {
    Serial.println("White,Black,White");
    
    straight();    
  }
  else if (pd_val[0] > pd_avg[0] && pd_val[1] > pd_avg[1] && pd_val[2] < pd_avg[2] )
  {
    Serial.println("Black,Black,White");
    leftslow(); 
  }
  else if (pd_val[0] < pd_avg[0] && pd_val[1] > pd_avg[1] && pd_val[2] > pd_avg[2] )
  {
    Serial.println("White,Black,Black");
    rightslow(); 
  }
    else if (pd_val[0] > pd_avg[0] && pd_val[1] > pd_avg[1] && pd_val[2] > pd_avg[2] )
  {
    Serial.println("Black,Black,Black");
    straight();
  }
  else if (pd_val[0] > pd_avg[0] && pd_val[1] < pd_avg[1] && pd_val[2] < pd_avg[2] )
  {
    Serial.println("Black,White,White");
    left();
  }
  else if (pd_val[0] < pd_avg[0] && pd_val[1] < pd_avg[1] && pd_val[2] > pd_avg[2] )
  {
    Serial.println("White,White,Black");
    right();
  }
  else if (pd_val[0] > pd_avg[0] && pd_val[1] < pd_avg[1] && pd_val[2] > pd_avg[2] )
  {
    Serial.println("Black,White,Black");
    tpoint();
  }
  delay(100);
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                                                                       //ultrasonic sensor
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH);

  distance=duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  if(distance >= 15 && distance <=25)
  {
    digitalWrite(LED,HIGH);
    pause();
    delay(2000);
    digitalWrite(LED,LOW);
    straight();
  }
  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                                                                             //ir sensor
  block_val = analogRead(block);
  hasObstacle = digitalRead(obstaclePin);
  if(hasObstacle == LOW)
  {
    Serial.println("STOP");
    if(block_val < block_avg);
    digitalWrite(LED,HIGH);
    pause();
    delay(2000);
    digitalWrite(LED,LOW);
    straight;
  }
  else
  {
    Serial.println("Path is Clear");
    digitalWrite(LED,LOW);
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                                                                //Functions
void straight()
{
    analogWrite(3,160);
    analogWrite(11, 0);
    analogWrite(5,160);
    analogWrite(6,0);
}
void left()
{
    analogWrite(3,160);
    analogWrite(11,0);
    analogWrite(5,0);
    analogWrite(6,160);
}
void leftslow()
{
    analogWrite(3,120);
    analogWrite(11, 0);
    analogWrite(5,40);
    analogWrite(6,0);
}

void right()
{
  analogWrite(3,0);
  analogWrite(11,160);
  analogWrite(5,160);
  analogWrite(6,0);
}
void rightslow()
{
  analogWrite(3,40);
  analogWrite(11,0);
  analogWrite(5,120);
  analogWrite(6,0);
}

void allwhite()
{
  analogWrite(3,0);
  analogWrite(11,180);
  analogWrite(5,180);
  analogWrite(6,0);
}
void tpoint()
{
  analogWrite(3,160);
  analogWrite(11,0);
  analogWrite(5,0);
  analogWrite(6,160);
}

void pause()
{
  analogWrite(3,0);
  analogWrite(11,0);
  analogWrite(5,0);
  analogWrite(6,0);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                                                                      //calibration
void pd_calib() 
{
  Serial.println("Calibrating...");
  int i,j;
  for(i=1;i<=1000;i++)
  {
    for (j=0;j<3;j++) 
    {
      pd_val[j] = analogRead(pd[j]);
      if (pd_val[j] > pd_val_max[j])
        pd_val_max[j] = pd_val[j];
      if (pd_val[j] < pd_val_min[j])
        pd_val_min[j] = pd_val[j];
      delay(5);
    }  
  }

  
  for(j=0;j<=2;j++)
  {
   pd_avg[j] = (pd_val_max[j] + pd_val_min[j]) / 2;
    Serial.print("Average = ");
  Serial.println(pd_avg[j]);
  Serial.print("Max : ");
  Serial.println(pd_val_max[j]);
  Serial.print("Min : ");
  Serial.println(pd_val_min[j]);
  Serial.println("Done!"); 
  }
}

void block_calib()
{
  Serial.print("Calibrating for block.....");
  int i;
  for(i=0;i<100;i++)
  {
    block_val = analogRead(block);
    if(block_val < block_min)
    {
      block_min = block_val;
    }
    if(block_val > block_max)
    {
      block_max = block_val;
    }
  }
  block_avg = (block_min + block_max)/2;
  Serial.print("Max :  ");
  Serial.println(block_max);
  Serial.print("Min :  ");
  Serial.println( block_min);
  Serial.print("Avg :  ");
  Serial.println(block_avg);
  Serial.println("Calibration Done!!");
}
