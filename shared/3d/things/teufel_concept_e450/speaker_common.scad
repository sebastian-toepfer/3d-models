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
use <../../libs/own/mirror_copy.fuc>
use <../../libs/own/sector_arc.fuc>

module wall(breite1, breite2, tiefe, hoehe, dicke, radius_abrundungen) {
    difference() {
        inlay(breite1, breite2, tiefe, hoehe);

        linear_extrude(height = hoehe * 1.1, center = true) {
            mirror_copy([1, 0, 0]) {
                //mit welcher funktion kommen wir hier auf die ausgewuerfelte 4?
                translate([breite2 / -2 + 4, tiefe / 2 - radius_abrundungen, 0]) {
                    arc(radius = radius_abrundungen, angles = [90, 180], width = radius_abrundungen);
                }
            }
        }

        translate([0, -dicke, 0]) {
            inlay(
                    breite1 = breite1 - dicke * 2,
                    breite2 = breite2 - dicke * 2,
                    tiefe   = tiefe,
                    hoehe   = hoehe * 1.1
            );
        }

        abrundungen(breite1, tiefe, hoehe, radius_abrundungen);
    }
}

module inlay(breite1, breite2, tiefe, hoehe, radius_abrundungen = 5) {
    difference() {
        linear_extrude(height = hoehe, center = true) {
            polygon(points = [
                    [breite2 / +2, tiefe / +2],
                    [breite2 / -2, tiefe / +2],
                    [breite1 / -2, tiefe / -2],
                    [breite1 / +2, tiefe / -2]
            ]);
        }
        abrundungen(breite1, tiefe, hoehe, radius_abrundungen);
    }
}

module abrundungen(breite, tiefe, hoehe, radius_abrundungen) {
    mirror_copy([0, 0, 1]) {
        translate([0, tiefe / -2 + radius_abrundungen, hoehe / 2 - radius_abrundungen]) {
            rotate([0, 90, 0]) {
                linear_extrude(height = breite * 1.1, center = true) {
                    arc(radius = radius_abrundungen, angles = [180, 270], width = radius_abrundungen);
                }
            }
        }
    }
}

