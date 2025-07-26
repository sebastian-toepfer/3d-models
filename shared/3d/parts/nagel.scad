//
//  File:        nagel.scad
//
//  License:     Creative Commons Attribution 4.0 International (CC BY 4.0)
//               https://creativecommons.org/licenses/by/4.0/
//
//  You are free to:
//    - Share: copy and redistribute the material in any medium or format
//    - Adapt: remix, transform, and build upon the material for any purpose
//
//  Under the following terms:
//    - Attribution: You must give appropriate credit, provide a link to the license,
//      and indicate if changes were made. You may do so in any reasonable manner,
//      but not in any way that suggests the licensor endorses you or your use.
//
//  (c) 2025 Sebastian Toepfer – https://github.com/sebastian-toepfer/3d-models
//
//  SPDX-License-Identifier: CC-BY-4.0
nagel();

module nagel(
    durchmesser = 4.0,
    laenge = 100,
    kopf_durchmesser = 6.5,
    kopf_hoehe = 1.2,
    spitzenhoehe = 6.0,
    farbe = "silver"
) {
    color(farbe) {
        union() {
            translate([0, 0, (laenge - kopf_hoehe) / 2]) {
                nagelkopf(hoehe = kopf_hoehe, durchmesser = kopf_durchmesser);
            }
            translate([0, 0, (spitzenhoehe - kopf_hoehe) / 2]) {
                nagelkoerper(laenge = laenge - kopf_hoehe - spitzenhoehe, durchmesser = durchmesser);
            }
            translate([0, 0, (laenge - spitzenhoehe) / -2]) {
                nagelspitze(laenge = spitzenhoehe, durchmesser = durchmesser);
            }
        }
    }
    
    module nagelkopf(hoehe = 1, durchmesser = 6) {
        cylinder(h = hoehe, d = durchmesser, center = true);
    }
    
    module nagelkoerper(laenge = 98, durchmesser = 4) {
        cylinder(h = laenge, d = durchmesser, center = true);
    }
    
    module nagelspitze(laenge = 5, durchmesser = 4) {
        cylinder(h = laenge, d1 = 0, d2 = durchmesser, center = true);
    }
}