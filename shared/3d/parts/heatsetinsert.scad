//
//  File:        heatsetinsert.scad
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
heatsetinsert();

// M2x3.0 -> change to a lookup later!
module heatsetinsert(height = 4) {
    translate([0, 0, 0.1 / 2]) {
        cylinder(h = height + 0.1, d = 3.2, center = true);
    }
}
