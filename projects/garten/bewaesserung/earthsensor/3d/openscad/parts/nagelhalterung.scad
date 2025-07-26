//
//  File:        nagelhalterung.scad
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
use <../sharedparts/nagel.scad>
use <../sharedparts/heatsetinsert.scad>
use <threadlib/threadlib.scad>
include <own/PG_THREAD_TABLE.scad>

nagelhalterung(
    $fn                    = 360
);

module nagelhalterung(
    nagel_durchmesser      = 4.0,
    nagel_kopf_durchmesser = 9.0,
    nagel_kopf_hoehe       = 1.5,
    kabel_durchmesser      = 1.5,
    schrauben_laenge       = 6,
    versenkung             = 4,
    versenkung_durchmesser = 17.8,
    wandstaerke            = 1.5
) {
    difference() {
        cylinder(
            h = schrauben_laenge * 0.9 + nagel_kopf_hoehe + kabel_durchmesser + versenkung + wandstaerke,
            d = versenkung_durchmesser,
            center = true
        );
        
        translate([0, 0, (nagel_kopf_hoehe + kabel_durchmesser + versenkung + wandstaerke) / 2]) {
            heatsetinsert(height = schrauben_laenge + 0.01);
        }

        translate([0, 0, schrauben_laenge / -2]) {
            nagel_kanal(
                durchmesser            = versenkung_durchmesser,
                hoehe                  = nagel_kopf_hoehe + kabel_durchmesser + versenkung + wandstaerke,
                nagel_kopf_durchmesser = nagel_kopf_durchmesser,
                nagel_kopf_hoehe       = nagel_kopf_hoehe,
                kabel_durchmesser      = kabel_durchmesser
            );
            translate([0, 0, (versenkung + nagel_kopf_hoehe + kabel_durchmesser + wandstaerke) / -2]) {
                specs = thread_specs("PG7-int", table = PG_THREAD_TABLE);
                turns = (versenkung / specs[0]);
                tap("PG7", turns = turns, table = PG_THREAD_TABLE);
            }
        }
    }
    
    module nagel_kanal(
        durchmesser            = 8,
        hoehe                  = 8,
        nagel_kopf_durchmesser = 7,
        nagel_kopf_hoehe       = 1,
        kabel_durchmesser      = 1
    ) {
        union() {             
            cylinder(
                h = hoehe,
                d = nagel_durchmesser + 0.2, center = true
            );

            translate([
                0,
                0,
                (hoehe - nagel_kopf_hoehe - kabel_durchmesser) / 2
            ]) {
                cube(
                    [
                      nagel_kopf_durchmesser + 0.2,
                      durchmesser + 0.2,
                      nagel_kopf_hoehe + kabel_durchmesser
                    ],
                    center = true
                 );
            }
            
            translate([
                0,
                (durchmesser +0.2) / 4,
                0
            ]) {
                cube(
                    [
                      nagel_durchmesser + 0.1,
                      durchmesser / 2 + 0.1,
                      hoehe
                    ],
                    center = true
                );
            }
        }
    }
}