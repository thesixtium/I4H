#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

LCDWIKI_KBV my_lcd(ILI9486,A3,A2,A1,A0,A4); //model,cs,cd,wr,rd,reset

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//param calibration from kbv
#define LEFT 141
#define RIGHT 842
#define TOP 762
#define BOT 301

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

uint16_t color_mask[] = {0xF800,0xFFE0,0x07E0,0x07FF,0x001F,0xF81F}; //color select

#define COLORBOXSIZE my_lcd.Get_Display_Width()/6
#define PENBOXSIZE my_lcd.Get_Display_Width()/4

int16_t old_color, current_color,flag_colour;
int16_t old_pen,current_pen,flag_pen;
boolean show_flag = true;

void show_string(uint8_t *str,int16_t x,int16_t y,uint8_t csize,uint16_t fc, uint16_t bc,boolean mode)
{
    my_lcd.Set_Text_Mode(mode);
    my_lcd.Set_Text_Size(csize);
    my_lcd.Set_Text_colour(fc);
    my_lcd.Set_Text_Back_colour(bc);
    my_lcd.Print_String(str,x,y);
}

void setup(void) 
{
  Serial.begin(115200);
  my_lcd.Init_LCD();
  Serial.println(my_lcd.Read_ID(), HEX);
  my_lcd.Fill_Screen(BLACK);
  current_color = RED;
  current_pen = 1;
  pinMode(13, OUTPUT);
}

#define MINPRESSURE 0
#define MAXPRESSURE 100000

void loop() {
//comme:
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  Serial.print("x:");
  Serial.print(p.x);
  Serial.print(" y:");
  Serial.print(p.y);
  Serial.print(" z:");
  Serial.println(p.z);
  
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
  {
      if (p.y < (TOP-5)) 
      {
          my_lcd.Set_Draw_color(BLACK);
          my_lcd.Fill_Rectangle(0, COLORBOXSIZE, my_lcd.Get_Display_Width()-1, my_lcd.Get_Display_Height()-1);
      }
      p.x = map(p.x, LEFT, RIGHT, 0, my_lcd.Get_Display_Width());
      p.y = map(p.y, TOP, BOT, 0, my_lcd.Get_Display_Height());

      if (((p.y-current_pen) > COLORBOXSIZE/2+20) && ((p.y+current_pen) < my_lcd.Get_Display_Height()))  //drawing
      {
        my_lcd.Set_Draw_color(current_color);
       // if(1 == current_pen)
     //   {
            my_lcd.Draw_Pixel(p.x,  p.y);
     //   }
     //   else
     //   {
          //my_lcd.Fill_Circle(p.x, p.y, current_pen);
      //  }
    }
  }
}
