int enl=5;
int lmp=4;
int lmn=3;
int enr=6;
int rmp=7;
int rmn=8;
int j=0;
int key=0;
int finish=0; 
int maxspeed=250;
const int minspeed=0;
double kp=1.1;    //0.175   1
double kd=100;    //55     70
double ki=1;  //       0.18
int lastError=0;
int st=0;   //for testing 1 else 0
// String str="SLSSLLRRRLLRLLSSSSLLRRLSRL";
String str="";
String path="";
int maxi[8]={0,0,0,0,0,0,0,0};
int mini[8]={1023,1023,1023,1023,1023,1023,1023,1023};
void setup() {
  Serial.begin(9600);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(11,OUTPUT);
  analogWrite(enr,0);
  analogWrite(enl,0);

  while(digitalRead(2))
  {
    for(int i=0;i<8;i++)
    {
      int val=analogRead(14+i);
      if(maxi[i]<val)
      {
      maxi[i]=val;
      }
      if(mini[i]>val)
      {
        mini[i]=val;
      } 
    }
  }
  while(!digitalRead(2))
  {
    analogWrite(enr,0);
    analogWrite(enl,0);
    digitalWrite(rmp,LOW);
    digitalWrite(rmn,LOW);
    digitalWrite(lmp,LOW);
    digitalWrite(lmp,LOW);
  }
  readings();
  for(int i=0;i<8;i++)
  {
    Serial.print(maxi[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for(int i=0;i<8;i++)
  {
    Serial.print(mini[i]);
    Serial.print(" ");
  }
  Serial.println("");
  
  
  delay(500);
  start();
}

void loop() {

  if(key && !finish)        //shortest path
  {

    if(st==0)
    {
      for(int i=0 ; i<path.length() ;i++)
      {
        if(path[i]=='B')
        {
          path = shortPath(path,i);
          if(path[i-1]=='B')
          {
            if(i>=2)
            {
              i-=2;
            }
          }
          else
          i--;
        }
      }
      str=path;
      path="";
      st=1;
    }
    else{
    if( (map(analogRead(14) , 0 , 1023 , mini[0],maxi[0])<150 && map(analogRead(21) , 0 , 1023 , mini[7],maxi[7])<150) )        //dry run
    {
      lineFollow();
      
    }
    else 
    {
      lineFollow();
      delay(10);
      spp();
    }
    }
    }

  if(!finish && !key)
  {
    if( (map(analogRead(14) , 0 , 1023 , mini[0],maxi[0])<150 && map(analogRead(21) , 0 , 1023 , mini[7],maxi[7])<150) &&( map(analogRead(17) , 0 , 1023 , mini[3],maxi[3]) > 80 || map(analogRead(18) , 0 , 1023 , mini[4],maxi[4]) > 80 || digitalRead(10)==1))         //dry run
    {
      lineFollow();
      // readings();
    }
    else 
    {
      lineFollow();
      delay(15);
      lsrb();
    }
  }

   if(finish)
  {
  analogWrite(enl,220);
  analogWrite(enr,220);
  digitalWrite(rmp, LOW);
  digitalWrite(rmn, HIGH);
  digitalWrite(lmp, HIGH);
  digitalWrite(lmn, LOW);
  delay(25);
    while(digitalRead(2))        //initially switch is 1 
    {
      analogWrite(enl,0);
      analogWrite(enr,0);
      digitalWrite(11,HIGH);
      
    }
    digitalWrite(11,LOW);
    analogWrite(enl,0);
    analogWrite(enr,0);
    delay(1000);
    key=1;
    finish=0;
    start();               // to indicate the start of shortest path after dry run
  }

}

void spp()
{
  if(map(analogRead(21) , 0 , 1023 , mini[7],maxi[7])>150){
    if(str[j]=='L')
    {
      
      turnLeft();
      j++;
      // lineFollow();
      // delay(100);
      Serial.print(" L ");
      Serial.print(j);
      
    }
  }
  
 if(digitalRead(10)==1&&(map(analogRead(21) , 0 , 1023 , mini[7],maxi[7])>150 || map(analogRead(14) , 0 , 1023 , mini[0],maxi[0])>150)){
 {
  if(str[j]=='S')
    {
    analogWrite(enr,220);
    analogWrite(enl,220);
    digitalWrite(lmp, LOW);
    digitalWrite(lmn, HIGH);
    digitalWrite(rmp, LOW);
    digitalWrite(rmn, HIGH);
    delay(10);
    analogWrite(enr,0);
    analogWrite(enl,0);
    delay(20);
    analogWrite(enr,140);
    analogWrite(enl,140);
    digitalWrite(lmp, LOW);
    digitalWrite(lmn, HIGH);
    digitalWrite(rmp, HIGH);
    digitalWrite(rmn, LOW);
    delay(20);
    while((map(analogRead(17) , 0 , 1023 , mini[3],maxi[3])>150 || map(analogRead(18) , 0 , 1023 , mini[4],maxi[4])>150) && (map(analogRead(14) , 0 , 1023 , mini[0],maxi[0])<150 && map(analogRead(21) , 0 , 1023 , mini[7],maxi[7])<150))
    {
      lineFollow();
    }
    j++;
    Serial.print(" S ");
    Serial.print(j);
    start();
  }
 }
 }
 
 if(map(analogRead(14) , 0 , 1023 , mini[0],maxi[0]) > 150)
 {
  if(str[j]=='R')
  {
    turnRight();
    j++;
    // lineFollow();
    // delay(100);
    Serial.print(" R ");
    Serial.print(j);
  }
 }
}

String shortPath(String path, int pathLength)
{
    String a;
    if (path[pathLength - 1] == 'L' && path[pathLength + 1] == 'R')
    {
        a = 'B';
    }
    else if (path[pathLength - 1] == 'L' && path[pathLength + 1] == 'S' )
    {
        a = 'R';
    }
    else if (path[pathLength - 1] == 'R' && path[pathLength + 1] == 'L')
    {
        a = 'B';
    }
    else if (path[pathLength - 1] == 'S' && path[pathLength + 1] == 'L' )
    {
        a = 'R';
    }
    else if (path[pathLength - 1] == 'S' && path[pathLength + 1] == 'S' )
    {
        a = 'B';
    }
 else if (path[pathLength - 1] == 'L' && path[pathLength + 1] == 'L')
    {
        a = 'S';
    }
    
    path=path.substring(0,pathLength-1)+a+path.substring(pathLength+2);
    // Serial.print(path);
    return path;
      

}

void lsrb()
{
  if( map(analogRead(21) , 0  ,1023 , mini[7] , maxi[7] )> 150 || map(analogRead(14) , 0  ,1023 , mini[0] , maxi[0] )> 150)
    {
        if ( map(analogRead(21) , 0  ,1023 , mini[7] , maxi[7] )> 150 )  //for sharp left turn
      {
        turnLeft();
        path=path+"L";
      }
      else if (digitalRead(10)==1)        //for straight movement
      {
        analogWrite(enr,220);
        analogWrite(enl,220);
        digitalWrite(lmp, LOW);
        digitalWrite(lmn, HIGH);
        digitalWrite(rmp, LOW);
        digitalWrite(rmn, HIGH);
        delay(10);
        analogWrite(enr,0);
        analogWrite(enl,0);
        delay(20);
        analogWrite(enr,140);
        analogWrite(enl,140);
        digitalWrite(lmp, LOW);
        digitalWrite(lmn, HIGH);
        digitalWrite(rmp, HIGH);
        digitalWrite(rmn, LOW);
        delay(20);
        while( ( map(analogRead(17) , 0  ,1023 , mini[3] , maxi[3] )>150 || map(analogRead(18) , 0  ,1023 , mini[4] , maxi[4] )>150) && ( map(analogRead(14) , 0  ,1023 , mini[0] , maxi[0] )<150 && map(analogRead(21) , 0  ,1023 , mini[7] , maxi[7] )<150) )
        {
          lineFollow();
        }
        path=path+"S";
        start();
      }
       else if( map(analogRead(14) , 0  ,1023 , mini[0] , maxi[0] )> 150)    //for sharp right turn
      { 
        turnRight();
        path=path+"R";
      } 
    }
    else if( map(analogRead(14),0,1023,mini[0],maxi[0])< 150 && map(analogRead(15),0,1023,mini[1],maxi[1])< 150 && map(analogRead(16),0,1023,mini[2],maxi[2])< 150 && map(analogRead(17),0,1023,mini[3],maxi[3])< 150 && map(analogRead(18) , 0  ,1023 , mini[4] , maxi[4] )< 150 && map(analogRead(19) , 0  ,1023 , mini[5] , maxi[5] )< 150 && map(analogRead(20) , 0  ,1023 , mini[6] , maxi[6] )< 150 && map(analogRead(21) ,0,1023,mini[7],maxi[7])< 150)
    { 
      path=path+"B";      
      uturn();
      start();
    }
    Serial.println(path);
}

void lineFollow()
{
   int error=( map(analogRead(17) , 0  ,1023 , mini[3] , maxi[3] )+ map(analogRead(16) , 0  ,1023 , mini[2] , maxi[2] )+ map(analogRead(15) , 0  ,1023 , mini[1] , maxi[1] )) - ( map(analogRead(18) , 0  ,1023 , mini[4] , maxi[4] )+ map(analogRead(19) , 0  ,1023 , mini[5] , maxi[5] )+ map(analogRead(20) , 0  ,1023 , mini[6] , maxi[6] ));
  //  int error=( map(analogRead(18),0,1023,min[4],max[4])+map(analogRead(17),0,1023,min[3],max[3]) ) - (map(analogRead(16),0,1023,min[2],max[2])+map(analogRead(15),0,1023,min[1],max[1]));
  int P=error;
  int I = I + error;
  int D = error - lastError;
  int adjustment = kp*P + kd*D + ki*I;
   lastError = error;
  int lsp=maxspeed + adjustment;
  int rsp=maxspeed - adjustment;
  if(lsp>maxspeed)
  {
    lsp=maxspeed;
  }
  if(lsp<minspeed)
  {
    lsp=minspeed;
  }
  if(rsp>maxspeed)
  {
    rsp=maxspeed;
  }
  if(rsp<minspeed)
  {
    rsp=minspeed;
  }
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  analogWrite(6, rsp);
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
  analogWrite(5, lsp);

  // Serial.print(adjustment);
  // Serial.print(" ");
  // Serial.print(lsp);
  // Serial.print(" ");
  // Serial.print(rsp);
  // Serial.println("");
}

void turnLeft()
{
  analogWrite(enl,240);
  analogWrite(enr,240);
  digitalWrite(lmp, LOW);
  digitalWrite(lmn, HIGH);
  digitalWrite(rmp, HIGH);
  digitalWrite(rmn, LOW);
  delay(13);
  analogWrite(enl,0);
  analogWrite(enr,250);
  digitalWrite(rmp, HIGH);
  digitalWrite(rmn, LOW);
  delay(400);
  if( map(analogRead(16) , 0  ,1023 , mini[2] , maxi[2] )>150 && map(analogRead(17) , 0  ,1023 , mini[3] , maxi[3] )>150 && map(analogRead(18) , 0  ,1023 , mini[4] , maxi[4] )>150 && map(analogRead(19) , 0  ,1023 , mini[5] , maxi[5] )>150)     // to check the finish line
  {
    finish=1;
  }
  while(digitalRead(10)==0)
  {
  analogWrite(enl,0);
  analogWrite(enr,250);
  digitalWrite(rmp, HIGH);
  digitalWrite(rmn, LOW);
 }
 
  analogWrite(enl,240);
  analogWrite(enr,240);
  digitalWrite(rmp, LOW);
  digitalWrite(rmn, HIGH);
  digitalWrite(lmp, HIGH);
  digitalWrite(lmn, LOW);
  delay(25);
}

void turnRight()
{
  analogWrite(enr,240);
  analogWrite(enl,240);
  digitalWrite(rmp, LOW);
  digitalWrite(rmn, HIGH);
  digitalWrite(lmp, HIGH);
  digitalWrite(lmn, LOW);
  delay(13);
  analogWrite(enr,0);
  analogWrite(enl,250);
  digitalWrite(lmp, HIGH);
  digitalWrite(lmn, LOW);
  delay(400);
  while(digitalRead(10)==0)
  {
  analogWrite(enr,0);
  analogWrite(enl,250);
  digitalWrite(lmp, HIGH);
  digitalWrite(lmn, LOW);
 }
 
  analogWrite(enr,220);
  analogWrite(enl,220);
  digitalWrite(lmp, LOW);
  digitalWrite(lmn, HIGH);
  digitalWrite(rmp, HIGH);
  digitalWrite(rmn, LOW);
  delay(25);

}

void uturn()
{
  analogWrite(enr,220);
  analogWrite(enl,220);
  digitalWrite(lmp, LOW);
  digitalWrite(lmn, HIGH);
  digitalWrite(rmp, LOW);
  digitalWrite(rmn, HIGH);
  delay(10);
  while(digitalRead(10)==0)
  {
  analogWrite(enr,80);
  analogWrite(enl,80);
  digitalWrite(lmp, LOW);
  digitalWrite(lmn, HIGH);
  digitalWrite(rmp, HIGH);
  digitalWrite(rmn, LOW);
  }
  analogWrite(enr,150);
  analogWrite(enl,150);
  digitalWrite(lmp, HIGH);
  digitalWrite(lmn, LOW);
  digitalWrite(rmp, LOW);
  digitalWrite(rmn, HIGH);
  delay(10);
}
 void readings()
{
  Serial.print(analogRead(14));
  Serial.print(" ");
    Serial.print(analogRead(15));
  Serial.print(" ");
    Serial.print(analogRead(16));
  Serial.print(" ");
    Serial.print(analogRead(17));
  Serial.print(" ");
    Serial.print(analogRead(18));
  Serial.print(" ");
    Serial.print(analogRead(19));
  Serial.print(" ");
    Serial.print(analogRead(20));
  Serial.print(" ");
    Serial.print(analogRead(21));
  Serial.print(" ");
  Serial.println(" ");

}
 void start()
 {
  for(int i=170;i<=250;i+=10)
  {
  maxspeed=i;
  lineFollow();
  delay(10);
  }
  maxspeed=250;
 }
// pin 10 for front ir sensor white 0  black 1
// pin 11 for finish led
// pin 2 for switch
// SLRRLLSSLLRRRLLRLLSSSSLLRRLSRL


