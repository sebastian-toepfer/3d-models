/*
  Reference Model – Teufel Concept E 450 Digital 5.1 Set
  Author: sebastian-toepfer
  Date: 2024-03-30
  MPN: 104762000
  Source: https://teufel.de/concept-e-450-digital-51-set-104762000
  Metadata: ./metadata.yaml

  Description:
    This OpenSCAD model is a simplified, manually constructed approximation of the
    Teufel Concept E 450 Digital 5.1 speaker system. It serves as a non-functional placeholder
    to support technical workflows such as enclosure prototyping, spatial integration, or
    accessory development.

  ⚠️  Disclaimer:
    - This is NOT a replica of the original product.
    - It must NOT be used for product reproduction or functional modeling.
    - The model contains no proprietary geometry, logos, or internal structures.
    - There is no affiliation with or endorsement by Lautsprecher Teufel GmbH.

  License:
    Creative Commons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
    https://creativecommons.org/licenses/by-nc/4.0/
*/
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

