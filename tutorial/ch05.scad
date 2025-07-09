// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
//https://en.wikibooks.org/wiki/OpenSCAD_Tutorial/Chapter_5
use <vehicle_parts.inc>

$fa = 1;
$fs = 0.4;

base_height         = 10;
base_length         = 100;
top_height          = 14;
top_length          = 75;
top_offset          = 5;
track               = 40;
body_roll           = -5;

wheels_turn         = 20;

front_wheel_radius        = 10;
front_side_spheres_radius = 50;
front_hub_thickness       = 4;
front_cylinder_radius     = 2;

rear_wheel_radius        = 12;
rear_side_spheres_radius = 30;
rear_hub_thickness       = 8;
rear_cylinder_radius     = 3;

module body(base_height=10, top_height=14, base_length=60, top_length=30, width=20, top_offset=5) {
    // Car body base
    cube([base_length,width,base_height],center=true);
    // Car body top
    translate([top_offset,0,base_height/2+top_height/2 - 0.001])
        cube([top_length,width,top_height],center=true);
}

//body
rotate([body_roll,0,0]) {
    body();
}

//front axle
translate([-20, 0, 0]) {
    axle_wheelset(wheel_turn = wheels_turn) {
        simple_wheel(front_wheel_radius, 4);
        simple_wheel(front_wheel_radius, 4);
    }
}

//rear axle
translate([20, 0, 0]) {
    axle_wheelset() {
        complex_wheel(rear_wheel_radius, rear_side_spheres_radius, rear_hub_thickness, rear_cylinder_radius);
        complex_wheel(rear_wheel_radius, rear_side_spheres_radius, rear_hub_thickness, rear_cylinder_radius);
    }
}

