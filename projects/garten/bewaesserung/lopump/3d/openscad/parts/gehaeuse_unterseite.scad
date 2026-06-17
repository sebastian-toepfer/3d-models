//
//  File:        gehaeuse_unterseite.scad
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
//  (c) 2026 Sebastian Toepfer – https://github.com/sebastian-toepfer/3d-models
//
//  SPDX-License-Identifier: CC-BY-4.0
//
// din rail mounting is inspired by: https://github.com/hzeller/din-rail-clip-mount
// which was created by:             h.zeller@acm.org
// code that is copied from it, is prefix with the github owner (hzeller), if it changed it is suffixed with mod
include <../libs/own/mirror_copy.fuc>

function pcb_dimension() = [
    100,
    85,
    1.6
];

function gehaeuse_unterseite_dimension(pcb_dimension = pcb_dimension(), wandstaerke = 1) = [
   ///5.08 entspricht einer TE -> wir wollen 1/4 TE's nutzen
   ceil((pcb_dimension.x * 1.01 + wandstaerke * 4) / (5.08 / 4)) * (5.08 / 4),
   pcb_dimension.y * 1.01 + wandstaerke * 4,
   7.8
];

gehaeuse_unterseite();

module gehaeuse_unterseite(
    pcb_dimension = pcb_dimension(),
    wandstaerke   = 1,
    ueberhang     = 5,
    hutschiene    = [36, 5.02]
) {
    gap               = 0.25;
    spring_thickness  = 3;
    spring_wide       = 25;

    dimension   = gehaeuse_unterseite_dimension(pcb_dimension, wandstaerke);
    union() {
        difference() {
            korpus(dimension = dimension, wandstaerke = wandstaerke, ueberhang = ueberhang);
            translate([0, 0, dimension.z / 2]) {
                innenraum(
                    dimension = [
                      pcb_dimension.x * 1.01,
                      pcb_dimension.y * 1.01,
                      ueberhang + 0.1
                    ],
                    board_befestigung = 10
                );
            }
            translate([0, 0, (ueberhang + dimension.z - hutschiene.y) / -2]) {
                hutschienen_befestigung(
                    dimension  = dimension,
                    feder_slot = [spring_wide, spring_thickness, gap],
                    hutschiene = hutschiene
                );
            }
        }

        // wir machen das etwas anderes als in dem original
        // in der hoffnungs das wir das spaeter animieren koennen ...
        // leider ist es nicht richtig parameterisierbar ... irgendwo habe ich da einen dreher drin
        translate([0, -13.25 - gap - 0.1, spring_thickness / -2 + 0.4]) {
            rotate([0, 0, 270]) {
                feder(
                    thickness  = spring_thickness - gap * 2,
                    wide       = spring_wide - gap * 2,
                    gap        = gap,
                    hutschiene = hutschiene
               );
           }
        }
    }

    module korpus(dimension = [10, 10, 10], wandstaerke = 1, ueberhang = 2) {
        union() {
            cube(
                [
                    dimension.x - wandstaerke * 2 - 0.3,
                    dimension.y - wandstaerke * 2 - 0.3,
                    dimension.z + ueberhang
                ],
                center = true
            );
            translate([0, 0, ueberhang / -2]) {
                cube(dimension, center = true);

                translate([0, 0, dimension.z / 2 + 1.5]) {
                    mirror_copy([0, 1, 0]) {
                        mirror_copy() {
                            translate([
                                dimension.x / -2  + wandstaerke - 1,
                                dimension.y / 4,
                                0
                            ]) {
                                rotate([270, 270, 0]) {
                                    linear_extrude(3.5, center = true) {
                                        polygon(
                                            points = [
                                                [0, 0],
                                                [0, 1],
                                                [1, 1],
                                            ]
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    module innenraum(dimension = [10, 10, 10], board_befestigung = 8) {
        difference() {
            cube(
                [
                    dimension.x,
                    dimension.y,
                    dimension.z
                ],
                center = true
            );

            mirror_copy([0, 1, 0]) {
                mirror_copy() {
                    translate([
                        (dimension.x - board_befestigung) / 2,
                        (dimension.y - board_befestigung) / 2,
                        - 1 
                    ]) {
                        difference() {
                            cube(
                                [
                                  board_befestigung,
                                  board_befestigung,
                                  dimension.z - 1.3
                                ],
                                center = true
                            );
                            translate([1, 1, 0.1]) {
                                cylinder(d = 3.5, h = dimension.z, center = true);
                            }
                        }
                    }
                }
            }
        }
    }

    module feder(
        thickness  = 3,
        wide       = 25,
        gap        = 0.25,
        hutschiene = [36, 5.02]
    ) {
        border             = dimension.y - hutschiene.x;
        height             = hutschiene.x + border / 2;

        union() {
            difference() {
                hull() {
                    mirror_copy([0, 1, 0]) {
                        rotate([90, 0, 90]) {
                            translate([wide / -2, 0, 0])  {
                                rotate([0, 0, 270]) {
                                    hzeller_dovetail_mod(
                                        height = height,
                                        thick  = thickness,
                                        wide   = wide,
                                        gap    = gap
                                    );
                                }
                            }
                        }
                    }
                }
                translate([(height - 25) / -2, 0, 0])  {
                    cube([25, wide - 5, thickness * 1.5], center = true);
                }
            }
            translate([-32.5, 0, 0])  {
                hzeller_springs_mod(len = 27, wide = 15, delta = 5, thick = 1.5, height = 1);
            }
            translate([35, 0, 0]) {
                hzeller_open_pull_lug_mod(height = 2, center = true);
            }
            translate([4.5, 0, -2]) {
                rotate([0, 180, 0]) {
                    hzeller_hook_mod(wide = wide - 5, thickness = 3);
                }
            }
        }
    }     

    //kein schoener modulename, vor allem ist es gar keine befestigung :(
    module hutschienen_befestigung(
        dimension  = [10, 10, 10],
        feder_slot = [25, 3, 0.25],
        hutschiene = [36, 5.02]
    ) {
        border = dimension.y - hutschiene.x;
        translate([0, border / -4, feder_slot.y - feder_slot.z]) {
            feder_slot(
                height     = hutschiene.x + border / 2,
                thickness  = feder_slot.y,
                wide       = feder_slot.x,
                gap        = feder_slot.z,
                hutschiene = hutschiene
            );
        }

        //hutschienenaussparrung ... evtl. hacken raus und als module ...
        translate([0, 0, (dimension.z - 5) / -2]) {
            difference() {
                cube([dimension.x * 1.1, hutschiene.x, hutschiene.y], center = true);
                mirror_copy() {
                    for( i = [10:20:dimension.x / 2]) {
                        translate([i, hutschiene.x / 2 - 3.5 , -2.5]) {
                            rotate([90, 0, 90]) {
                                linear_extrude(3.5, center = true) {
                                    polygon(
                                        points = [
                                            [0, 0],
                                            [3.5, 0],
                                            [3.5, 4],
                                            [0, 1]
                                        ]
                                    );
                                }
                            }
                        }
                    }
                }
            }
        }

        module feder_slot(
            height     = 36,
            thickness  = 3,
            wide       = 25,
            gap        = 0.25,
            hutschiene = [36, 5.02]
        ) {
            hull() {
                mirror_copy([1, 0, 0]) {
                    rotate([90, 0, 0]) {
                        translate([wide / -2, 0, 0])  {
                            rotate([0, 0, 270]) {
                                hzeller_dovetail_mod(
                                    height = height + gap,
                                    thick  = thickness,
                                    gap    = gap
                                );
                            }
                        }
                    }
                }
            }
            // 15 is hook breite, tiefe what ever
            translate([0, (height - hutschiene.x - 35 / 4 + gap * 2) / -2, -thickness]) {
                cube([wide - gap * 4, 10, hutschiene.y], center = true);
            }
        }

    }

    //modifications:
    //- center parameter
    //- use mirror_copy to create symmetry
    module hzeller_dovetail_mod(height = 20, wide = 20, thick = 3, gap=0.25, center = true) {
        mirror_copy([1, 0, 0]) {
            hull() {
                translate([thick / -2, thick / 4, 0]) {
                    cylinder(r = gap, h = height, center = center); 
                }
                translate([thick / -10, thick / -4, 0]) {
                    cylinder(r = gap, h = height, center = center); 
                }
            }
            hull() {
                translate([thick / -10, thick / -4, 0]) {
                    cylinder(r = gap, h = height, center = center); 
                }
                translate([thick / 10, thick / -4, 0]) {
                    cylinder(r = gap, h = height, center = center); 
                }
            }
        }
    }

    //modifications:
    //- rename form sping_symmetric to springs
    //- add other spring related modules as submodule (not usable outside the module)
    //- remove positioning
    module hzeller_springs_mod(len = 20, wide = 15, delta = 5, thick = 1, height = 1) {
        half_w = wide / 2 - delta / 2;

        mirror_copy([0, 1, 0]) {
            translate([0, delta / -2, 0]) {
                hzeller_spring(len, half_w, thick, height);
            }
        }

        module hzeller_spring(len = 20, wide = 15, thick = 1, height = 1) {
            linear_extrude(height = height, center = true) {
                hzeller_spring_polygon(len, wide, thick);
            }

            module hzeller_spring_polygon(len = 20, wide = 15, thick = 1) {
                p = 3;  // periods
                n = 50;
                actual_w = wide / 2 - thick / 2;
                points = [
                  for (i = [0:n]) [ len*i/n, actual_w*cos(i * p * 360 / n) + thick/2],
                  for (i = [0:n]) [ len*(n-i)/n, actual_w*cos((n-i) * p * 360 / n) - thick/2],
                ];

                translate([0, -actual_w, 0]) {
                    polygon(points);
                }
            }
        }
    }

    //modifications:
    //- centerable
    //- symtery via mirror_copy
    //- more paramter, less global
    module hzeller_open_pull_lug_mod(
            height = 2,
            wide   = 6.5,  // half-wide. Good enough for pocket knife blade.
            length = 10, 
            center = true
    ) {
        r    = 2;
        difference() {
            hull() {
                cube([length, 2 * wide, height], center = center);
                mirror_copy([0, 1, 0]) {
                    translate([length / 2, r - wide, 0]) {
                        cylinder(r = r, h = height, center = center, $fn = 20);
                    }
                }
            }
            hull() {
                mirror_copy([0, 1, 0]) {
                    translate([(length - 0.5) / 2, 0.5 - wide + r, 0]) {
                        cylinder(r = 1.8 / 2, h = height * 1.1, center = center, $fn = 20);
                    }
                }
            }
        }
    }

    //modifications:
    //- inline polygon
    //- remove do_slide param
    //- change global variables to parameter
    module hzeller_hook_mod(
        wide      = 15,
        thickness = 3
    ) {
        translate([0, wide / 2, 0]) {
            rotate([90, 0, 0]) {
                linear_extrude(height = wide) {
                    hzeller_hook_polygon();
                }
            }
        }

        module hzeller_hook_polygon() {
            din_sheet=1;           // thickness of the din-rail metal
            w=1;
            h=0.8;
            ledge=1;
            polygon([[-2*w, - thickness], [0, - thickness], [0, din_sheet],
                     [h, h + din_sheet], [h, h + ledge + din_sheet],
                     [-1, h + ledge + din_sheet+3],
                     [-2, h + ledge + din_sheet+3],
                     [-3, 0],
                     [-2*w, 0],
            ]);
        }
    }

}
