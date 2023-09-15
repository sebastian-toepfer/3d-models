//https://en.wikibooks.org/wiki/OpenSCAD_Tutorial/Chapter_6
use <vehicle_parts.inc>

$fa = 1;
$fs = 0.4;

body_version = "l"; //s-short, n-normal, l-large, r-rectangular
wheels_version = "l"; //s-small, m-medium, l-large

top = true;
front_bumper = false;
rear_bumper = false;

base_height         = 10;
base_length         =
(body_version == "l") ? 80:
(body_version == "s") ? 60:
(body_version == "r") ? 65:70;
top_height          = 14;
top_length          =
(body_version == "l") ? 50:
(body_version == "s") ? 30:
(body_version == "r") ? 65:40;
top_offset          =
(body_version == "l") ? 10:
(body_version == "s") ? 5:
(body_version == "r") ? 0:7.5;
track               = 40;
body_roll           = -5;

wheels_turn         = 20;

front_wheel_radius  =
(wheels_version == "l") ? 10:
(wheels_version == "m") ? 8:6;

front_wheel_width =
(wheels_version == "l") ? 8:
(wheels_version == "m") ? 6:4;

rear_wheel_radius  =
(wheels_version == "l") ? 10:
(wheels_version == "m") ? 8:6;

rear_wheel_width =
(wheels_version == "l") ? 8:
(wheels_version == "m") ? 6:4;

//body
rotate([body_roll,0,0]) {
    body(base_length=base_length, top_length=top_length, top_offset=top_offset, top = top, front_bumper=front_bumper, rear_bumper=rear_bumper);
}

//front axle
translate([-20, 0, 0]) {
    axle_wheelset(wheel_turn = wheels_turn) {
        simple_wheel(front_wheel_radius, front_wheel_width);
        simple_wheel(front_wheel_radius, front_wheel_width);
    }
}

//rear axle
translate([20, 0, 0]) {
    axle_wheelset() {
        simple_wheel(rear_wheel_radius, rear_wheel_width);
        simple_wheel(rear_wheel_radius, rear_wheel_width);
    }
}

