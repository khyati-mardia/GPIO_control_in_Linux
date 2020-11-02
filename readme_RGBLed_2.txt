Part  2 : RGBLed_2 : PWM signals for LED display and intensity control

This project is done by Team8 for GPIO Control in Linux
Before running this project , kindly note the following points 

TEAM 8 
KHYATI MARDIA  1215346587
CHANDRIKA C.S  1215321133


Instructions to run a program on target :

1.  Connect pins on Galileo Board according to your file input.

2. Kindly run the program by using command : ./RGBLed_2

3. Kindly give valid file path inclusive of file name.

4. We are reading inputs from file “input.txt”.
The file contains one line input with 4 arguments ie (% of Duty Cycle, PWM for R ,PWM for G and  PWM for B) which is (10,3,5,6 )   

5.  Mouse is read on Intel Galileo Board by following below path :
/dev/input/event2

6. Make file is modified for X86 architecture by giving relevant paths.

5. We have reassigned the duration for each Led ( to 0.5 sec or 1 sec) to appreciate the desired output.

6. We have calculated the actual values of the duration for each led as per assignment problem and have mentioned as comments. You may uncomment that part in order to check the reliability of the program.
In this PWM assignment, the amplitude is varied by setting the dutycycle. Therefore we run the thread for a certain time followed by sleep for the remaining time. We do not run any cycles for this as the flickering is achieved by PWM. This sequence is in a infinite loop.
Also, realise that we have another thread thats reading the mouse events . Therefore we have 2 threads running infinitely . The program has to be terminated using ctrl+c .


7. Program is properly commented and warning free.
 

Instructions to run a program on host :

1. In order to grant access to reading the mouse event file (evdev) , kindly run the following command in terminal before running the application

sudo chmod +rw /dev/input/event6

2. Other points are applicable as mentioned above. 


