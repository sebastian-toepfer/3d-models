//
//  File:        top.scad
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

top(
    thread = "M65x4",
    ceil_height = 4,
    wall_height = 25
);

module top(thread = "M52x4", ceil_height = 4, wall_height = 25) {
    wall(thread = thread);

    module wall(thread = "M52x4", height = 5) {
        specs = thread_specs(str(thread, "-int"));
        diameter = specs[2] + 4;
        difference() {
            cylinder(h = wall_height + ceil_height, d = diameter, center = true);
            translate([0, 0, wall_height / -2 + specs[0] / 2]) {
                tap(thread, turns = (wall_height / specs[0]));
            }
        }
    }
}