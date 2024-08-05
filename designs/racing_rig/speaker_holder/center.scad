use <../../../things/teufel_concept_e450/speaker_center_cs35_c_mk3.scad>
use <../../../things/treq/eckwinkel.scad>

links        = true;

winkel_alu   = 45;
wand_staerke = 10;
hoehe        = 112;
tiefe        = 120;

aur = sin(90 - winkel_alu) * eckwinkel_hoehe();
aul = cos(90 - winkel_alu) * eckwinkel_tiefe();

difference() {
    mirror(links ? [0, 0, 0] : [1, 0, 0]) {
        union() { 
            translate([(eckwinkel_breite() + speaker_center_cs35_c_mk3_breite()) / -2, (tiefe - aul - aur) / - 2, hoehe / 2]) {
                union() {
                    rotate([180 - winkel_alu, 0, 180]) {
                        eckwinkel();
                        translate([(eckwinkel_breite()) / -2, 0, 0]) {
                            cube([wand_staerke, eckwinkel_tiefe(), eckwinkel_hoehe()], center = true);
                        }
                    }
                }
            }

            translate([speaker_center_cs35_c_mk3_breite() / -2, 0, 0]) {
                cube([wand_staerke, tiefe, hoehe], center = true);
            }
        }
    }
  
    rotate([speaker_center_cs35_c_mk3_winkel(), 0, 0]) {
        speaker_center_cs35_c_mk3();
    }
}

