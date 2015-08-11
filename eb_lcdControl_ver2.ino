/*
LiquidCrystal Library

The circuit:
* LCD RS pin to digital pin 8
* LCD R/W pin to digital pin 11
* LCD Enable pin to digital pin 9
* LCD D4 pin to digital pin 4
* LCD D5 pin to digital pin 5
* LCD D6 pin to digital pin 6
* LCD D7 pin to digital pin 7
* LCD LEDK pin to PWM digital pin 10
* wiper to LCD VO pin (pin 3)

* Keypad pin to analog pin 0

*/

// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial\SoftwareSerial.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
SoftwareSerial mySlaveSerial(0,1);
//Key message
char msgs[5][15] = { "Right Key OK ",
"Up Key OK    ",
"Down Key OK  ",
"Left Key OK  ",
"Select Key OK" };
int  adc_key_val[5] = { 30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key = -1;
int oldkey = -2;
int brightness = 250;
int checkNum = 0;

void setup() {
	// set up the LCD's number of columns and rows: 
	lcd.begin(16, 2);
	// Print a message to the LCD.
	lcd.print("Test LCD Keypad 1");
	Serial.begin(9600);
	Serial.print("hello");
	mySlaveSerial.begin(9600);
	mySlaveSerial.print("digital");

	
}

void loop()
{

	adc_key_in = analogRead(0);   // read the value from the sensor
	key = get_key(adc_key_in); // convert into key press
	Serial.print(key);
	
	
		
	 
	if (key != oldkey) // Ű�� ������ ������ ����Ÿ ���� ��������� ���� �Ѱ� �̿��� ������ ����Ÿ�� ���������� ����� ������.
	{
		delay(50);  // wait for debounce time
		adc_key_in = analogRead(0);   // read the value from the sensor
		key = get_key(adc_key_in);    // convert into key press
		
		
		if (key != oldkey)
		{
			oldkey = key;
		
			
			if (key == 4)//�޴�Ű�� ���� ���ۼ���, �ΰ��� ��λ�� ���� ����Ű
			{
				if (checkNum == 0)
				{
					lcd.clear();
					lcd.setCursor(1,0);
					lcd.print(msgs[4]);//���� �ᵵ ������ �迭�� �� �غ���....
					lcd.setCursor(3, 1);
					lcd.print("Direction");
					checkNum = 1;
				}
				else
				{
					lcd.clear();
					lcd.setCursor(1, 0);
					lcd.print(msgs[4]);
					lcd.setCursor(3, 1);
					lcd.print("Brightness");
					checkNum = 0;
				}
				
			}
			
			if (checkNum == 0)
			{


				if (key == 1)
				{
					brightness += 25;
					if (brightness > 250) brightness = 0;
					analogWrite(10, brightness);
					show_brightness();
					lcd.setCursor(0, 1);  //x=0, line=2
					lcd.print("brightness: ");
				}

				if (key == 2)
				{
					brightness -= 25; if (brightness < 0) brightness = 250;
					analogWrite(10, brightness);
					show_brightness();
				}
			}

			if (checkNum == 1)// if���� switch���� ����ؼ� ���α׷��߽��ϴ�. �����Ͻö�.
			{

				switch (key)
				{
				default:
					break;
				case 1:
					lcd.clear();
					lcd.setCursor(0, 1);
					lcd.print(msgs[1]);
					Serial.write(key);//������� �����Ϳ��� �����̺�� ���ް�
					break;
				case 2:
					lcd.clear();
					lcd.setCursor(0, 1);  
					lcd.print(msgs[2]);
					Serial.write(key);//���ް�
					break;
				case 3:
					lcd.clear();
					lcd.setCursor(0, 1);  
					lcd.print(msgs[3]);
					break;
				case 0:
					lcd.clear();
					lcd.setCursor(0, 1);  
					lcd.print(msgs[0]);
					break;
				}



				
			}
		}
	}
	
}


int show_brightness(void)
{
	int temp;
	lcd.setCursor(12, 1);  //x=12, line=2
	temp = brightness / 25;
	if (temp>9)
		lcd.print("100%");
	else
	{
		lcd.print(temp % 10 + '0');
		lcd.print("% ");
	}
}

// Convert ADC value to key number
int get_key(unsigned int input)//�迭��ȣ ������ ���� ����
{
	int k;
	for (k = 0; k < NUM_KEYS; k++)
	{
		if (input < adc_key_val[k])
		{
			return k;
		}
	}
	if (k >= NUM_KEYS)
		k = -1;     // No valid key pressed
	return k;
}