// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <../things/simucube/simucube_2_pro_psu.scad>
use <../things/treq/eckwinkel.scad>

psu_holder_corner_template();

//max 25 -> wegen kabel ...
module psu_holder_corner_template(
    max_contact = 25,
    scaling = 1.01
) {
    difference() {
        union() {
            translate([eckwinkel_breite() / 2 - eckwinkel_wandstaerke(), 0, 0]) {
                cube([
                        max_contact + eckwinkel_wandstaerke(),
                        eckwinkel_tiefe(),
                        (
                          simucube_2_pro_psu_hoehe() * scaling
                          + eckwinkel_wandstaerke()
                        )
                    ],
                    center = true
                );
            }

            translate([
                (max_contact + eckwinkel_wandstaerke()) / -2,
                0,
                (
                  simucube_2_pro_psu_hoehe() * scaling
                  - eckwinkel_hoehe()
                  + eckwinkel_wandstaerke()
                ) / - 2
            ]) {
                rotate([0, 0, 180]) {
                    eckwinkel();
                }
            }
        }

        translate([
            simucube_2_pro_psu_breite() / 2 + eckwinkel_wandstaerke(),
            simucube_2_pro_psu_tiefe() / 2 - (eckwinkel_tiefe() - max_contact) / 2,
            (
              simucube_2_pro_psu_hoehe() / -2
              + eckwinkel_hoehe() / 2
              + eckwinkel_wandstaerke()
            )
        ]) {
            scale([1, 1, scaling]) {
                simucube_2_pro_psu();
            }
        }
    }
}

