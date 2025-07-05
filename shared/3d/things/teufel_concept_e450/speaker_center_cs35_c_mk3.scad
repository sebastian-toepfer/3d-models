use <./speaker_common.scad>

function speaker_center_cs35_c_mk3_size() = [
    speaker_center_cs35_c_mk3_breite(),
    speaker_center_cs35_c_mk3_tiefe(),
    speaker_center_cs35_c_mk3_hoehe()
];
function speaker_center_cs35_c_mk3_breite() = 225;
function speaker_center_cs35_c_mk3_tiefe() = 95;
function speaker_center_cs35_c_mk3_hoehe() = 101;
function speaker_center_cs35_c_mk3_hoehe_vorn() = 101;
function speaker_center_cs35_c_mk3_hoehe_hinten() = 71;
function speaker_center_cs35_c_mk3_winkel()
    = 90 - atan(speaker_center_cs35_c_mk3_tiefe() / ((speaker_center_cs35_c_mk3_hoehe_vorn() - speaker_center_cs35_c_mk3_hoehe_hinten()) / 2));

module speaker_center_cs35_c_mk3() {
    hoehe_vorn         = speaker_center_cs35_c_mk3_hoehe_vorn(); //schraeg
    hoehe_hinten       = speaker_center_cs35_c_mk3_hoehe_hinten();  //schraeg
    tiefe              = speaker_center_cs35_c_mk3_tiefe();
    breite             = speaker_center_cs35_c_mk3_breite();
    radius_abrundungen = 5;

    rotate([0, 90, 0]) {
        rotate([0, 0, speaker_center_cs35_c_mk3_winkel() * -1]) {
            union() {
                color("black") {
                    wall(hoehe_vorn, hoehe_hinten, tiefe, breite, 5, radius_abrundungen);
                }
                color("grey") {
                    translate([0, 5 / -2, 0]) {
                        inlay(hoehe_vorn - 5 * 2, hoehe_hinten - 5 * 2, tiefe - 5, breite);
                    }
                }
            }
        }
    }
}

