// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
//https://en.wikibooks.org/wiki/OpenSCAD_Tutorial/Chapter_8
$fa = 1;
$fs = 0.4;

wheel_radius  = 20;
wheel_width   = 6;
tyre_diameter = 4;
axle_diameter = 5;

module rounded_simple_wheel(wheel_radius=12, wheel_width=4, tyre_diameter=6, axle_diameter=3) {
    difference() {
        rotate([90, 0, 0]) {
            difference() {
                cylinder(r = wheel_radius - tyre_diameter / 2, h = wheel_width, center = true);
                rotate_extrude(angle = 360) {
                    translate([wheel_radius - tyre_diameter / 2, 0]) {
                        circle(d = tyre_diameter);
                    }
                }
            }
        }

        rotate([90, 0, 0]) {
            cylinder(h = wheel_width + 1, r = axle_diameter / 2, center = true);
        }
    }
}

module extruded_car_body(length = 80, rear_height = 20, rear_width = 25, scaling_factor = 0.5, rounded = false) {
    rotate([0, -90, 0]) {
        linear_extrude(height = length, center = true, scale = scaling_factor) {
            resize([rear_height, rear_width]) {
                circle(d = rear_height);
            }
        }
    }
    if (rounded) {
        translate([length / 2, 0, 0]) {
            resize([rear_height, rear_width]) {
                sphere(d = rear_height); 
            }
        }
        translate([length / -2, 0, 0]) {
            scale(scaling_factor) {
                resize([rear_height, rear_width]) {
                    sphere(d = rear_height); 
                }
            }
        }
    }
}

module linar() {
    linear_extrude(height = 50, center = true, twist = 120, scale = 1.5) {
        scale([2,1,1]) {
            circle(d = 10);
        }
    }
}

//rounded_simple_wheel();
extruded_car_body(rounded = true);
