#include "config.h"
#include "7seg20.h"

TTGOClass *ttgo;
int n=4;

#define orange 0xFC64
#define dark 0x334F
#define light 0xCF3E

#define darkOrange 0xFB20
int posX[4];

int boxSize=45;
int space=3;
int start=25;
String math="";

float numbers[3]={0,0,0};
int fase=0;

char buttons[4][4]={{'7','4','1','0'},{'8','5','2','.'},{'9','6','3','='},{'/','*','-','+'}};
char operation=' ';
bool irq = false;

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    
    ttgo->tft->fillScreen(TFT_WHITE);
   // ttgo->tft->setFreeFont(&DSEG7_Classic_Regular_40);
    ttgo->tft->setTextColor(light);
    ttgo->tft->setTextFont(4);
   pinMode(4,OUTPUT);
  
  

   for(int i=0;i<n;i++)
posX[i]=(start+(i*boxSize)+(space*i));

 ttgo->tft->fillRoundRect(posX[0],3,188,40,4,dark);

   for(int i=0;i<n;i++)
    for(int j=0;j<n;j++){

      if(j<3 && i <3 )
    ttgo->tft->fillRoundRect(posX[i],posX[j]+22,boxSize,boxSize,4,dark);
    else
    ttgo->tft->fillRoundRect(posX[i],posX[j]+22,boxSize,boxSize,4,darkOrange);
    ttgo->tft->drawString(String(buttons[i][j]), posX[i]+16, posX[j]+34);
   
    }
  
    ttgo->tft->setFreeFont(&DSEG7_Classic_Bold_30);
    
    ttgo->tft->drawString("0",180,8);
    
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);

    //!Clear IRQ unprocessed  first
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    ttgo->power->clearIRQ();

 
}
int pres=1;
void loop()
{
    int16_t x, y;
    if (ttgo->getTouch(x, y)) {
       if(y>0 && y<40) 
       reset();
       if(pres==1) 
      for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
          {
            if(x>posX[j] && x<posX[j]+boxSize && y>posX[i]+22 && y<posX[i]+boxSize+22 ){
            ttgo->tft->fillRoundRect(posX[j],posX[i]+22,boxSize,boxSize,4,darkOrange);
             digitalWrite(4,1);
              delay(1);
               digitalWrite(4,0);
               delay(25);
              
             
         if(buttons[j][i]=='0' || buttons[j][i]=='1' || buttons[j][i]=='2' || buttons[j][i]=='3' || buttons[j][i]=='4' || buttons[j][i]=='5' || buttons[j][i]=='6' || buttons[j][i]=='7' || buttons[j][i]=='8' || buttons[j][i]=='9' || buttons[j][i]=='.' ){  
          if(math.length()<7){         
         ttgo->tft->fillRoundRect(posX[0],3,188,40,4,dark); 
         math=math+String(buttons[j][i]);
         }}

         if(buttons[j][i]=='+' || buttons[j][i]=='-' || buttons[j][i]=='*' || buttons[j][i]=='/' ){
          if(numbers[0]==0)
          numbers[0]=math.toFloat();
          
          math="";
          operation=buttons[j][i];
          ttgo->tft->fillRoundRect(posX[0],3,188,40,4,dark); 
         }

         if(buttons[j][i]=='=' ){

          numbers[1]=math.toFloat();
          if(operation=='+')
          numbers[2]=numbers[0]+numbers[1];

          if(operation=='-')
          numbers[2]=numbers[0]-numbers[1];

          if(operation=='*')
          numbers[2]=numbers[0]*numbers[1];

          if(operation=='/')
          numbers[2]=numbers[0]/numbers[1];
          
          numbers[0]=numbers[2];
          int check=numbers[2]*100.00;
          if(check%100==0){
          math=String(int(numbers[2]));}
          else{
          math=String(numbers[2]);}
          ttgo->tft->fillRoundRect(posX[0],3,188,40,4,dark); 
         }
         
           bool decimal=false;

            
          int check2=math.toFloat()*100.00;
          if(check2%100==0){
          decimal=false;}
          else{
          decimal=true;}

            if(decimal==true)
            ttgo->tft->drawString(math,(205-(math.length()*25))+25,8);
            else
            ttgo->tft->drawString(math,205-(math.length()*25),8); 
            
            pres=0;
            x=0;
            y=0;


           ttgo->tft->setTextFont(4);
            
                     if(j<3 && i <3 )
    ttgo->tft->fillRoundRect(posX[j],posX[i]+22,boxSize,boxSize,4,dark);
    else
    ttgo->tft->fillRoundRect(posX[j],posX[i]+22,boxSize,boxSize,4,darkOrange);
    ttgo->tft->drawString(String(buttons[j][i]), posX[j]+16, posX[i]+34);

    ttgo->tft->setFreeFont(&DSEG7_Classic_Bold_30);
             
            }

    
            
            
            }
    }else {pres=1;}

     if (irq) {
        irq = false;
        ttgo->power->readIRQ();

        if (ttgo->power->isPEKShortPressIRQ()) {
          reset();
        }
        ttgo->power->clearIRQ(); }
    

}

void reset(){
  for(int i=0;i<3;i++)
  numbers[i]=0;
  math="";
   ttgo->tft->fillRoundRect(posX[0],3,188,40,4,dark); 
   ttgo->tft->drawString("0",180,8);
  
  }
