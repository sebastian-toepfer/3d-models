// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
//https://en.wikibooks.org/wiki/OpenSCAD_Tutorial/Chapter_1
$fa = 1;
$fs = 0.4;

cube([60, 20, 10], center=true);
translate([5, 0, 10 - 0.001])
    cube([30, 20, 10], center=true);
//front left wheel
translate([-20, -15, 0])
    rotate([90, 0, 0])
        cylinder(h=3, r=8, center=true);
//front right wheel
translate([-20, 15, 0])
    rotate([90, 0, 0])
        cylinder(h=3, r=8, center=true);
//front axle
translate([-20, 0, 0])
    rotate([90, 0, 0])
        cylinder(h=30, r=2, center=true);
//rear left wheel
translate([20, -15, 0])
    rotate([90, 0, 0])
        cylinder(h=3, r=8, center=true);
//rear rigth wheel
translate([20, 15, 0])
    rotate([90, 0, 0])
        cylinder(h=3, r=8, center=true);
//rear axle
translate([20, 0, 0])
    rotate([90, 0, 0])
        cylinder(h=30, r=2, center=true);        