/*******************************************************************************
* rc_project_template.c
*
* This is meant to be a skeleton program for robotics cape projects. 
* Change this description and file name 
*******************************************************************************/

// usefulincludes is a collection of common system includes for the lazy
// This is not necessary for roboticscape projects but here for convenience
#include <rc_usefulincludes.h> 
// main roboticscape API header
#include <roboticscape.h>

#define CLOCK 1
#define ANTICLOCK -1
#define SERVO_MAX 1.5
#define FREQ_DEFAULT 90

/*******************************************************************************
* int main() 
*
* This template main function contains these critical components
* - call to rc_initialize() at the beginning
* - main while loop that checks for EXITING condition
* - rc_cleanup() at the end
*******************************************************************************/
int main(){

    uint16_t val = 0;
	printf("\nStarting Range Finder Test\n");

	// always initialize cape library first
	if(rc_initialize()){
		fprintf(stderr,"ERROR: failed to initialize rc_initialize(), are you root?\n");
		return -1;
	}

	if(rc_i2c_get_in_use_state(1)){
		printf("i2c bus claimed by another process\n");
        return -1;
    }

	if(rc_i2c_init(1,0x62)<0){
		printf("ERROR: failed to initialize i2c bus\n");
		printf("aborting range_finder\n");
		return -1;
	}
    rc_i2c_claim_bus(1);

    if( rc_enable_servo_power_rail()<0){
		printf("ERROR: failed to initialize servo power rail\n");
		printf("aborting range_finder\n");
		return -1;
    }

	printf("\nRange Finder intiilization complete\n");

    uint8_t range[2];
    float position = 0.0;
    int8_t swp_drctn = CLOCK;
    if( rc_send_servo_pulse_normalized(7, position) < 0){
        printf("ERROR: failed to move to servo to position %f\n", position);
    }
    rc_set_state(RUNNING);
    while( rc_get_state() == RUNNING){
            position += swp_drctn * SERVO_MAX / FREQ_DEFAULT;
        if( position > SERVO_MAX){
            position = SERVO_MAX;
            swp_drctn = ANTICLOCK;
        }
        if (position < -SERVO_MAX){
            position = -SERVO_MAX;
            swp_drctn = CLOCK;
        }
            
        if( rc_send_servo_pulse_normalized(7, position) < 0){
            printf("ERROR: failed to move to servo to position %f\n", position);
            break;
        }
        // sleep roughly enough to maintain frequency_hz
        rc_usleep(1000000/FREQ_DEFAULT);

        val = 0;
        if(rc_i2c_write_byte(1, 0x00, 0x04)<0){
            printf("ERROR: failed to write to range finder\n");
            printf("aborting range_finder\n");
            rc_set_state(EXITING);
        } else {
            rc_i2c_read_word(1, 0x8f, &val);
            printf("distance: %d | servo position: %f\n", val, position);
        }
	}
	
	// exit cleanly
    rc_i2c_release_bus(1);
    rc_disable_servo_power_rail();
	rc_cleanup(); 
	return 0;
}

