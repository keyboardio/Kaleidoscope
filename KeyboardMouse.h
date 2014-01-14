double mouseActiveForCycles = 0;
float carriedOverX =0;
float carriedOverY =0;

double mouse_accel (double cycles)
{
    double accel = atan((cycles/50)-5);
    accel += 1.5707963267944; // we want the whole s curve, not just the bit that's usually above the x and y axes;
    accel = accel *0.85;
    if (accel<0.25) {
        accel =0.25;
    }
    return accel;
}

void handle_mouse_movement( char x, char y)
{

    if (x!=0 || y!=0) {
        mouseActiveForCycles++;
        double accel = (double) mouse_accel(mouseActiveForCycles);
        float moveX=0;
        float moveY=0;
        if (x>0) {
            moveX = (x*accel) + carriedOverX;
            carriedOverX = moveX - floor(moveX);
        } else if(x<0) {
            moveX = (x*accel) - carriedOverX;
            carriedOverX = ceil(moveX) - moveX;
        }

        if (y >0) {
            moveY = (y*accel) + carriedOverY;
            carriedOverY = moveY - floor(moveY);
        } else if (y<0) {
            moveY = (y*accel) - carriedOverY;
            carriedOverY = ceil(moveY) - moveY;
        }
#ifdef DEBUG_SERIAL
        Serial.println();
        Serial.print("cycles: ");
        Serial.println(mouseActiveForCycles);
        Serial.print("Accel: ");
        Serial.print(accel);
        Serial.print("	moveX is ");
        Serial.print(moveX);
        Serial.print(" moveY is ");
        Serial.print(moveY);
        Serial.print(" carriedoverx is ");
        Serial.print(carriedOverX);
        Serial.print(" carriedOverY is ");
        Serial.println(carriedOverY);
#endif
        Mouse.move(moveX,moveY, 0);
    } else {
        mouseActiveForCycles=0;
    }

}
