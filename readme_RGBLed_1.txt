Part  1 : RGBLed_1 : A user-space application for RGB LED display

This project is done by Team8 for GPIO Control in Linux
Before running this project , kindly note the following points 

TEAM 8 
KHYATI MARDIA  1215346587
CHANDRIKA C.S  1215321133


Instructions to run a program on target :

1.  Connect pins on Galileo Board according to your file input.

2. Kindly run the program by using command : ./RGBLed_1

3. Kindly give valid file path inclusive of file name.

4. We are reading inputs from file “Todo.txt”.
The file contains one line input with 4 arguments ie (% of Duty Cycle, IO for R , IO for G and  IO for B) which is (50,1,2,3 )   

5.  Mouse is read on Intel Galileo Board by following below path :
/dev/input/event2

6. Make file is modified for X86 architecture by giving relevant  compiler paths and also PATH_variable.

5. We have reassigned the duration for each Led ( to 0.5 sec or 1 sec) to appreciate the desired output.

6. We have calculated the actual values of the duration for each led as per assignment problem and have mentioned as comments. You may uncomment that part in order to check the reliability of the program.

eg. To calculate time  period of each sequence (duration). 
We know that the period is 0.5 sec, this is for 25 cycles . Therefore each cycle is for 20 ms. 
Therefore we run the thread for a certain time of time and make it sleep for the rest of the time given by remainingDuration variable in the code. We repeat this for 25 cycles.  This causes to reducing the intensity of LEDs. This happens in an infinite loop.
Also, realise that we have another thread thats reading the mouse events . Therefore we have 2 threads running infinitely . The program has to be terminated using ctrl+c .

7. We have set cycles to 5 instead of 25 to understand the program during mouse event. You may modify the number of cycle as per assignment problem to check the output.
 
8. Program is properly commented and warning free.

Instructions to run a program on host :

1. In order to grant access to reading the mouse event file (evdev) , kindly run the following command in terminal before running the application
sudo chmod +rw /dev/input/event6

2. Other points are applicable as mentioned above.

