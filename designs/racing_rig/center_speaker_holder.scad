use <../../things/teufel_concept_e450/speaker_center_cs35_c_mk3.scad>
use <../../things/treq/eckwinkel.scad>

links        = true;

winkel_alu   = 45;
wand_staerke = 10;
hoehe        = 112;
tiefe        = 120;

//TODO access winkel dimensions
winkel_breite = 36;
winkel_hoehe  = 36;
winkel_tiefe  = 38;

//TODO: aus speaker berechnen lassen?
speaker_breite   = 225;
rotation_speaker = 8.973;

aur = sin(90 - winkel_alu) * winkel_hoehe;
aul = cos(90 - winkel_alu) * winkel_tiefe;

difference() {
    mirror(links ? [0, 0, 0] : [1, 0, 0]) {
        union() { 
            translate([(winkel_breite + speaker_breite) / -2, (tiefe - aul - aur) / - 2, hoehe / 2]) {
                union() {
                    rotate([180 - winkel_alu, 0, 180]) {
                        eckwinkel();
                        translate([(winkel_breite) / -2, 0, 0]) {
                            cube([wand_staerke, winkel_tiefe, winkel_hoehe], center = true);
                        }
                    }
                }
            }

            translate([speaker_breite / -2, 0, 0]) {
                cube([wand_staerke, tiefe, hoehe], center = true);
            }
        }
    }
  
    rotate([rotation_speaker, 0, 0]) {
      speaker_center_cs35_c_mk3();
    }
}

