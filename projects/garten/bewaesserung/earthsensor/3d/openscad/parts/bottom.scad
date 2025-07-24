//
//  File:        bottom.scad
//
//  License:     Creative Commons Attribution 4.0 International (CC BY 4.0)
//               https://creativecommons.org/licenses/by/4.0/
//
//  You are free to:
//    - Share: copy and redistribute the material in any medium or format
//    - Adapt: remix, transform, and build upon the material for any purpose
//
//  Under the following terms:
//    - Attribution: You must give appropriate credit, provide a link to the license,
//      and indicate if changes were made. You may do so in any reasonable manner,
//      but not in any way that suggests the licensor endorses you or your use.
//
//  (c) 2025 Sebastian Toepfer – https://github.com/sebastian-toepfer/3d-models
//
//  SPDX-License-Identifier: CC-BY-4.0
use <threadlib/threadlib.scad>
include <own/PG_THREAD_TABLE.scad>

bottom();

module bottom(thread = "M70x4", floor_height = 4, wall_height = 20) {
    union() {
        translate([0, 0, floor_height / 2]) {
            wall(thread = thread, height = wall_height);
        }
        translate([0, 0, wall_height / -2]) {
            floor(thread = thread, height = floor_height, abstand_gnd = 20);
        }
    }
    
    module wall(thread = "M52x4", height = 5) {
        specs = thread_specs(str(thread, "-ext"));
        union() {
            difference() {
                cylinder(h = height, d = specs[2], center = true);
                cylinder(h = height + 0.2, d = specs[2] - 6, center = true);
                translate([0, 0, (height - 2) / 2 + 0.1]) {
                    intersection() {
                        cylinder(h = 2, d = specs[2] - 4, center = true);
                        translate([0, (specs[2] - 4) / -2, 0]) {
                            cube([specs[2] - 4, (specs[2] - 4), 2], center = true);
                        }
                    }
                }
            }
            turns = (height / specs[0]);
            P = specs[0];
            translate([0, 0, height / -2 + P / 2]) {
                thread(str(thread, "-ext"), turns = turns -1);
            }
        }
    }
        
    module floor(thread = "M52x4", height = 4, abstand_gnd = 20) {
        diameter = thread_specs(str(thread, "-int"))[2] + 4;
        difference() {
            cylinder(d = diameter, h = height, center = true);
            //gnd
            elektrode(height = height);
            //e1
            translate([abstand_gnd, 0, 0]) {
                elektrode(height = height);
            }
            //e2
            translate([0, abstand_gnd, 0]) {
                elektrode(height = height);
            }
            //ph
            translate([-abstand_gnd, 0, 0]) {
                elektrode(height = height);
            }
        }
    }
        
    module elektrode(height = 4, nagelhalter_diameter = 18) {
        translate([0, 0, height / -2]) {
            specs = thread_specs("PG7-int", table = PG_THREAD_TABLE);
            //calculate turns based on height :(
            turns = (height / specs[0]) + 1;
            tap("PG7", turns = ceil(turns), table = PG_THREAD_TABLE);
        }
        translate([0, 0, height / 4]) {
            cylinder(h = height / 2 + 0.1, d = nagelhalter_diameter, center = true);
        }
    }
}
