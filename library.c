#include <avr/io.h>

// this function sets up the ports

void setup (void)

{

// setup ports

    DDRB = 0b11000111;

    DDRC = 0b00000011;

    DDRD = 0b11111111;

    PORTB = 0b11111111;

    PORTC = 0b01111100;

    PORTD = 0b11111111;

}

// this function delays for d microseconds. The argument

// must be in the range 56<d<41478. It returns the

// tap signal bit PINC7 * 128

char delay_us (int d)

{

    d = (d-56) * 1.58;

    char tap=0;

    while (d>0)

    {

        tap = PINC & 0b1000000;

        d--;

    }

    return (tap);

}

// display_digit takes a decimal number (0-9) in the 'number'

// parameter and displays it on a 7-segment LED digit. The

// digit is specified in the 'digit' parameter and must be in

// the range of 1 (leftmost) to 4 (rightmost).

void display_digit (char number, char digit)

{

// turn on the appropriate segments

    switch (number)

    {

        case 0:

            PORTD = 0b00001001;

            break;

        case 1:

            PORTD = 0b11101011;

            break;

        case 2:

            PORTD = 0b10000101;

            break;

        case 3:

            PORTD = 0b11000001;

            break;

        case 4:

            PORTD = 0b01100011;

            break;

        case 5:

            PORTD = 0b01010001;

            break;

        case 6:

            PORTD = 0b00010001;

            break;

        case 7:

            PORTD = 0b11001011;

            break;

        case 8:

            PORTD = 0b00000001;

            break;

        case 9:

            PORTD = 0b01000011;

            break;

        default:

            PORTD = 0b11111110;

    }

// turn on the appropriate digit

    switch (digit)

    {

        case 1:

            PORTB = 0b11111101;

            break;

        case 2:

            PORTB = 0b11111011;

            break;

        case 3:

            PORTB = 0b10111111;

            break;

        case 4:

            PORTB = 0b01111111;

            break;

        default:

            PORTB = 0b00111000;

    }

    delay_us (2700); // wait

    PORTB = 0b11111111; // turn off digit

}

int main (void)

{

// setup microcontroller

    setup();

// declare variables

    char

            ticks = 0,

            secs = 0,

            mins = 0,

            hrs = 0;

// repeat forever

    while (1)

    {

/* this WHILE has no body, only the test part. The test

repeatedly reads PINC and then ANDS it with the constant

0b1000000. The result of this is either 128 (if PC7 was

high) or 0 (if PC7 was low). This statement will evaluate as

TRUE while PC7 is high. PC7 is connected to the transformer

and will go from TRUE to FALSE 60 times per second*/

        while (PINC & 0b10000000)

        {

// nothing here

        }

/* the following code increments the time. Ticks increment

from 0 to 59. When ticks reaches 59, the next step is

to reset the ticks to zero and increment the seconds.

Seconds increment from 0 to 59. When the seconds count

reaches 59, the next step is to reset the seconds and

increment the minutes.

*/

        if (ticks < 59) // check to see if ticks are maxed out

            ticks++; // increment tick count

        else

        {

            ticks = 0; // reset tick count

            if (secs < 59)

                secs++; // increment seconds

            else

            {

                secs = 0;

                if (mins < 59)

                    mins++;

                else

                {

                    mins = 0;

                    if (hrs < 23)

                        hrs++;

                    else

                        hrs = 0;

                }

            }

        }

#define sec_time 200

        PORTB =( 0b11111110);// turn on digit zero

        PORTD = ~ (secs | 0b11000000);

        delay_us(sec_time);//seconds out to port;

        PORTD =( 0b00111111);

        delay_us(2700-sec_time);

        display_digit (hrs/10, 1);

        display_digit (hrs%10, 2);

        display_digit (mins/10, 3);

        display_digit (mins%10, 4);

        delay_us (2700); // delay for 2.7 ms

// hold the clock until PC7 goes high

        if(ticks%30 == 0){

            if ((PINB & 0b00111000) == 0b00110000)

            {

// ALR is pressed - sound a G

                PORTC = PORTC | 0b00000011; // bend bender, light on

                delay_us(1275); // wait

                PORTC = PORTC & 0b11111101; // release bender

                if (mins < 59)

                    mins++;

                else

                {

                    mins = 0;

                    if (hrs < 23)

                        hrs++;

                    else

                        hrs = 0;

                }

                ticks = 0;

                secs = 0;

                delay_us(1275); // wait

            }

            else if ((PINB & 0b00111000) == 0b00101000)

            {

// INC is pressed - sound a C

                PORTC = PORTC | 0b00000011; // bend bender

                delay_us(1911); // wait

                PORTC = PORTC & 0b11111101; // release bender

                if (hrs < 23)

                    hrs++;

                else

                    hrs = 0;

                ticks = 0;

                secs = 0;

                delay_us(1911); // wait

            }

            else if ((PINB & 0b00111000) == 0b00011000)

            {

// SET is pressed - sound an E

                PORTC = PORTC | 0b00000011; // bend bender

                delay_us(1517);

                PORTC = PORTC & 0b11111101; // release bender

                hrs = 0;

                mins = 0;

                ticks = 0;

                secs = 0;

                delay_us(1517); // wait

            }

            else

                PORTC = PORTC & 0b11111110; // light off

        }

        while (!(PINC & 0b10000000))

        {

// nothing here

        }

    }

}