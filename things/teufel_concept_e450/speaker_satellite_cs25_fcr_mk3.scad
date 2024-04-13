use <./speaker_common.scad>

module speaker_satellite_cs25_fcr_mk3() {
    breite_vorn        = 101;
    breite_hinten      = 71;
    tiefe              = 95;
    hoehe              = 140;
    radius_abrundungen = 5;
    dicke              = 5;

    union() {
        color("black") {
            wall(breite_vorn, breite_hinten, tiefe, hoehe, dicke, radius_abrundungen);
        }
        color("grey") {
            translate([0, dicke / -2, 0]) {
                inlay(breite_vorn - dicke * 2, breite_hinten - dicke * 2, tiefe - dicke, hoehe);
            }
        }
    }
}

