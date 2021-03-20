
#define DECODE_NEC 1
#include <IRremote.h>

int IR_RECEIVE_PIN = 11;

void setup() {
    Serial.begin(115200);
    
    // Start the receiver, enable feedback LED and take LED feedback pin from the internal boards definition
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
}

void loop() {
    // IR receiver listener
    if (IrReceiver.decode()) {
        IrReceiver.printIRResultShort(&Serial);
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            // We have an unknown protocol here, print more info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        Serial.println();

        /*
         * !!!Important!!! Enable receiving of the next value,
         * since receiving has stopped after the end of the current received data packet.
         */
        IrReceiver.resume(); // Enable receiving of the next value
        

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.command == 0x10) {
            // do something
        } else if (IrReceiver.decodedIRData.command == 0x11) {
            // do something else
        }


        if (IrReceiver.decodedIRData.command == 0x45) {
            Serial.println("\n\nyou'v pressed One!\n\n");
        }


        
    }
}
