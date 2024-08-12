//https://en.wikibooks.org/wiki/OpenSCAD_Tutorial/Chapter_2
$fa = 1;
$fs = 0.4;

wheel_radius = 10;
base_height  = 10;
top_height   = 14;
track        = 40;
wheel_width  = 10;
body_roll    = -5;
wheels_turn  = 20;

//body
rotate([body_roll, 0, 0]) {
    cube([60, 20, base_height], center=true);
    translate([5, 0, base_height / 2 + top_height / 2 - 0.001])
        cube([30, 20, top_height], center=true);
}
//front left wheel    
translate([-20, track / -2, 0])
    rotate([90, 0, wheels_turn])
        cylinder(h=wheel_width, r=wheel_radius, center=true);
//front right wheel
translate([-20, track / 2, 0])
    rotate([90, 0, wheels_turn])
        cylinder(h=wheel_width, r=wheel_radius, center=true);
//front axle
translate([-20, 0, 0])
    rotate([90, 0, 0])
        cylinder(h=track, r=2, center=true);
//rear left wheel
translate([20, track / -2, 0])
    rotate([90, 0, 0])
        cylinder(h=wheel_width, r=wheel_radius, center=true);
//rear rigth wheel
translate([20, track / 2, 0])
    rotate([90, 0, 0])
        cylinder(h=wheel_width, r=wheel_radius, center=true);
//rear axle
translate([20, 0, 0])
    rotate([90, 0, 0])
        cylinder(h=track, r=2, center=true);
