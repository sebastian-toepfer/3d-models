// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
include <../libs/own/mirror_copy.fuc>
use <gehaeuse_unterseite.scad>

function pcb_dimension() = [
    100,
    85,
    1.6
];

function gehaeuse_oberseite_dimension(pcb_dimension = pcb_dimension(), wandstaerke = 1) = [
   ///5.08 entspricht einer TE -> wir wollen 1/4 TE's nutzen
   ceil((pcb_dimension.x * 1.01 + wandstaerke * 4) / (5.08 / 4)) * (5.08 / 4),
   pcb_dimension.y * 1.01 + wandstaerke * 4,
   50.6
];

gehaeuse_oberseite();

module gehaeuse_oberseite(
    pcb_dimension = pcb_dimension(),
    wandstaerke   = 1,
    center        = true
) {
    dimension   = gehaeuse_oberseite_dimension(pcb_dimension, wandstaerke);
    difference() {
        korpus(dimension = dimension, center = center);
        translate([0, 0, - wandstaerke - 0.01]) {
            korpus(
                dimension = [
                    dimension.x - wandstaerke * 2,
                    dimension.y - wandstaerke * 2,
                    dimension.z + 0.01
                ],
                center = center
            );
        }

        //stromanschluss ...
        translate([dimension.x / -2 + 17.6, (dimension.y - 15) / -2, -10]) {
            schraubterminal_loecher(anzahl = 2);
        }

        //oeffnungen fuer die relais
        for(i = [1:3]) {
            translate([
                -3.2 + 18.2 * (i - 1),
                (dimension.y - 15) / -2,
                -10
            ]) {
                schraubterminal_loecher(anzahl = 3);
            }
        }

        //i2c ...
        //translate([1.6, (dimension.y - 15) / 2, -5]) {
        //    schraubterminal_loecher(anzahl = 4);
        //}

        //sma
        translate([20, dimension.y / 2, -12.2]) {
            rotate([90, 0, 0]) {
                cylinder(h = wandstaerke * 3, d = 6.4, center = true);
            }
        }

        mirror_copy([1, 0, 0]) {
            mirror_copy([0, 1, 0]) {
                translate([
                    (dimension.x - wandstaerke + 0.1) / 2,
                    (dimension.y - 45) / 2,
                    (dimension.z - 1.5 - 1.2) / -2
                ]) {
                    rotate([90, 180, 0]) {
                        linear_extrude(height = 4, center = true) {
                            polygon(
                                points = [
                                    [0, 0],
                                    [1.2, 0],
                                    [1.2, 1.2]
                                ]
                            );
                        }
                    }
                    translate([-wandstaerke + 0.5, 0, 1.2 / -2]) {
                        cube([0.5 + 0.01, 5, 1.5 + 1.2 / 2], center = true);
                    }
                }
            }
        }
    }

    module korpus(dimension = [10, 5, 10], center = true) {
        rotate([0, 90, 0]) {
            linear_extrude(height = dimension.x, center = center) {
                mirror_copy([0, 1, 0]) {
                    polygon(
                      points = [
                        [dimension.z / 2, 0],
                        [dimension.z / 2, dimension.y / 2],
                        [dimension.z / 2 - 20, dimension.y / 2],
                        [dimension.z / 2 - 20, dimension.y / 2 - 13.175],
                        [dimension.z / 2 - 20 - 20, dimension.y / 2 - 13.175],
                        [dimension.z / 2 - 20 - 20, dimension.y / 2 - 13.175 - 9.25],
                        [dimension.z / 2 * -1, dimension.y / 2 - 13.175 - 9.25 ],
                        [dimension.z / 2 * -1, 0],
                      ]
                    );
                }
            }
        }
    }

    module schraubterminal_loecher(anzahl = 1) {
        distanz = 5.2;
        begin = distanz * anzahl / -2 + 2.6;
        ende  = begin + distanz * (anzahl - 1);
        for(i = [begin:distanz:ende]) {
            translate([i, 0, 0]) {
                cylinder(d = 6, h = 10, center = true);
            }
        }
        translate([0, 0, -7.5]) {
            cube([ende - begin + distanz, 20, 7.5], center = true);
        }
    }
}

