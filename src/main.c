#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#define  PIN 17

int main(int argc, char* argv[]) 
{
//initialize pigpio library
if (gpioInitialise() < 0) {
	printf("pigpio intialization failed.\n");
	return 1;
}
//set pin 17 for an output for the LED
gpioSetMode(PIN, PI_OUTPUT);

float temp;
float sum = 0.0;
int count = 0;
char choice;

//open file to log the tempature 
FILE*logFile = fopen("data/temperature_log.txt", "w");
if(logFile == NULL){
	printf("Error opening the file!\n");
	gpioTerminate();
	return 1;
}

fprintf(logFile, "Temperature(F)\tStatus\t\tAverage(F)\n");
fprintf(logFile, "-------------------------------------------\n");
do {
 printf("\nEnter classroom temperature in Fahrenheit: ");
 scanf("%f", &temp);

 count++;
 sum += temp;
 float average = sum / count;

// Check valid Range 
if (temp >= 68.0 && temp <= 76.0){
printf("Status; NORMAL\t| LED: OFF\t| Current Avg: %.2fF[cite: 5]\n", average);
gpioWrite(PIN, 0); // TURNS LED OFF
fprintf(logFile, "%.2fF\t\tNORMAL\t\t%.2fF\n", temp, average);
}else{
printf("Status: WARNING\t| LED: ON\t| Current Avg: %.2fF[cite: 5]\n", average);
gpioWrite(PIN, 1); // TURNS LED ON
fprintf(logFile, "%.2fF\t\tWARNING\t\t%.2fF\n", temp, average);
}

printf("Would you like to enter another temperature? (y/n): ");
scanf(" %c", &choice);

}while (choice == 'y' || choice =='Y');

//Close file
fclose(logFile);
printf("\nData successfully saved to data/temperature_log.txt[cite: 5]\n");

gpioWrite(PIN, 0);
gpioTerminate();
return 0;
}
