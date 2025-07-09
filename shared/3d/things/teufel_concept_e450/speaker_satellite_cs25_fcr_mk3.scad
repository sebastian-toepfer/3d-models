// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <./speaker_common.scad>

function speaker_satellite_cs25_fcr_mk3_dimension() = [
    speaker_center_cs25_fcr_mk3_breite(),
    speaker_center_cs25_fcr_mk3_tiefe(),
    speaker_center_cs25_fcr_mk3_hoehe()
];

function speaker_satellite_cs25_fcr_mk3_breite() = 101;
function speaker_satellite_cs25_fcr_mk3_tiefe() = 95;
function speaker_satellite_cs25_fcr_mk3_hoehe() = 140;
function speaker_satellite_cs25_fcr_mk3_breite_vorn() = 101;
function speaker_satellite_cs25_fcr_mk3_breite_hinten() = 71;
function speaker_satellite_cs25_fcr_mk3_winkel() =
    90 - atan(speaker_satellite_cs25_fcr_mk3_tiefe() / ((speaker_satellite_cs25_fcr_mk3_breite_vorn() - speaker_satellite_cs25_fcr_mk3_breite_hinten()) / 2));

module speaker_satellite_cs25_fcr_mk3() {
    radius_abrundungen = 5;
    dicke              = 5;

    union() {
        color("black") {
            wall(
                    speaker_satellite_cs25_fcr_mk3_breite_vorn(),
                    speaker_satellite_cs25_fcr_mk3_breite_hinten(),
                    speaker_satellite_cs25_fcr_mk3_tiefe(),
                    speaker_satellite_cs25_fcr_mk3_hoehe(),
                    dicke,
                    radius_abrundungen
            );
        }
        color("grey") {
            translate([0, dicke / -2, 0]) {
                inlay(
                        speaker_satellite_cs25_fcr_mk3_breite_vorn() - dicke * 2,
                        speaker_satellite_cs25_fcr_mk3_breite_hinten() - dicke * 2,
                        speaker_satellite_cs25_fcr_mk3_tiefe() - dicke,
                        speaker_satellite_cs25_fcr_mk3_hoehe()
                );
            }
        }
    }
}

