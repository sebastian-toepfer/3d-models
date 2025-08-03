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

