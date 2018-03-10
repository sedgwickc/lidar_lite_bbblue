LidarLite v2 for Beaglebone Blue
================================

This process uses the robotics cape API to sweep a Lidar Lite v2 on a servo
through it's movement limit. The distance is read at points through out the
movement based on the pwm resolution set in the code. The default is set to 90 
so 180 distance reading are taken from one complete sweep of the servo. 

