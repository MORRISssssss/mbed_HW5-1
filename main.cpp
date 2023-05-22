#include "mbed.h"
#include "bbcar.h"


DigitalOut led1(LED1);
Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D10), servo1_f(D12);
PwmOut servo0_c(D11), servo1_c(D13);

DigitalInOut pin8(D8);

BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

Thread turnThread;
double data[1000] = {0};

void turning() {
    car.spin(60);
    ThisThread::sleep_for(2000ms);
    car.stop();
}

int main() {
    parallax_laserping ping1(pin8);
    turnThread.start(turning);
    for (int i = 0; i < 400; i++){
        data[i] = (float)ping1;
        ThisThread::sleep_for(3ms);
    }
    bool split = 0;
    double prev = 0;
    int a = 0;
    int b = 0;
    int w = 0;
    double dis = 0;
    double rec = 0;
    for (int i = 0; i < 1000; i++){
        
        if (data[i]){
            //printf("%d: %lf\n", i, data[i]);
            printf("%d:\t", i);
            int n = data[i] / 10;
            for (int j = 0; j < n; j++){
                printf("*");
            }
            printf("\n");
            if (data[i] - prev > 60){
                split = 1;
                b = i;
                if (a != 0 && b - a > w){
                    w = b - a;
                    dis = (double)w * 3 / 2 * 3.14159 * 3 / 2000 * prev;
                    rec = prev;
                }
            }else if (prev - data[i] > 60){
                split = 0;
                a = i;
                
            }
            
            prev = data[i];
        }else{
            break;
        }
        
    }
    printf("%d, %lf, %lfcm\n", w, rec, dis);
}