#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<math.h>

int ScreenWidth = 80 , ScreenHeight = 22 ;
float zBuffer[80 * 22] ;
char buffer[80 * 22] ;
float Theta_Spacing = 0.07 , Phi_Spacing = 0.02 ;
int Raduis = 1 ;
int R2 = 2 ;
int DistanceFromCamera = 5 ;
int DistanceFromScreen = 15 ;
float CircleX , CircleY ;
float CalCulateX , CalculateY , CalculateZ ; 
float luminance ; 
int luminace_index ;
float ooz ;
int xp , yp ;
int idx ;
int BackGroundAssciiCode = ' ' ;

void Render_Frame(float Alpha , float Beta){
    for(float Theta = 0 ; Theta < 2 * M_PI ; Theta+=Theta_Spacing){
        for(float Phi = 0 ; Phi < 2 * M_PI ; Phi+=Phi_Spacing){
            CircleX = R2 + Raduis * cos(Theta) ;
            CircleY = Raduis * sin(Theta) ; 

            CalCulateX = (CircleX) * (cos(Beta) * cos(Phi) + sin(Alpha) * sin(Beta) * sin(Phi)) - cos(Alpha) * sin(Beta) * (CircleY) ;
            CalculateY = (CircleX) * (cos(Phi) * sin(Beta) - cos(Beta) * sin(Alpha) * sin(Phi)) + cos(Alpha) * cos(Beta) * (CircleY) ;
            CalculateZ = cos(Alpha) * (CircleX) * sin(Phi) + sin(Alpha) * (CircleY) + DistanceFromCamera ; 

            luminance = cos(Phi) * cos(Theta) * sin(Beta) - cos(Alpha) * cos(Theta) * sin(Phi) - 
                        sin(Alpha) * sin(Theta) + cos(Beta) * (cos(Alpha) * sin(Theta)- cos(Theta) * sin(Alpha) * sin(Phi)) ; 


            ooz = 1/CalculateZ ;
            xp = (int)(ScreenWidth/2 + ooz * CalCulateX * 2 * DistanceFromScreen) ;
            yp = (int)(ScreenHeight/2 + ooz * CalculateY * DistanceFromScreen) ; 

            idx = xp + yp * ScreenWidth ; 
            luminace_index = 8 * luminance ;

            if(ScreenHeight > yp && yp > 0 && ScreenWidth > xp && xp > 0  && ooz > zBuffer[idx]){
                zBuffer[idx] = ooz ;
                buffer[idx] = "101010101010"[luminace_index > 0 ? luminace_index : 0] ; 
            }
        }
    }
}

int main(){
    float A , B ;
    printf("\x1b[2J") ; 
    while(1){
        memset(buffer , BackGroundAssciiCode , ScreenWidth * ScreenHeight) ;
        memset(zBuffer , 0 , ScreenWidth * ScreenHeight * 4) ; 

        Render_Frame(A , B) ; 

        printf("\x1b[H") ; 
        for(int k = 0 ; k < ScreenWidth * ScreenHeight ; k++){
            putchar(k % ScreenWidth ? buffer[k] : 10) ; 
            A+=0.00004 ; 
            B+=0.00002 ; 
        } 
        usleep(20000) ; 
    }
}
