use <../things/teufel_concept_e450/speaker_center_cs35_c_mk3.scad>
use <../things/treq/eckwinkel.scad>

center_template();

module center_template(
    winkel_alu   = 45,
    wand_staerke = 10,
    hoehe        = 112,
    tiefe        = 120
) {
    aur = sin(90 - winkel_alu) * eckwinkel_hoehe();
    aul = cos(90 - winkel_alu) * eckwinkel_tiefe();

    difference() {
        union() { 
            translate([0, (tiefe - aul - aur) / - 2, hoehe / 2]) {
                union() {
                    rotate([180 - winkel_alu, 0, 180]) {
                        eckwinkel();
                        translate([(eckwinkel_breite()) / -2, 0, 0]) {
                            cube([wand_staerke, eckwinkel_tiefe(), eckwinkel_hoehe()], center = true);
                        }
                    }
                }
            }

            translate([eckwinkel_breite() / 2, 0, 0]) {
                cube([wand_staerke, tiefe, hoehe], center = true);
            }
        }
      
        translate([(speaker_center_cs35_c_mk3_breite() + eckwinkel_breite()) / 2, 0, 0]) {
            rotate([speaker_center_cs35_c_mk3_winkel(), 0, 0]) {
                speaker_center_cs35_c_mk3();
            }
        }
    }
}

