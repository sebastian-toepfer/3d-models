//
//  File:        deckel.scad
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
use <own/mirror_copy.fuc>
use <threadlib/threadlib.scad>

skimmer_abdeckung();

module skimmer_abdeckung(
    groesse                 = 213,
    dicke                   = 3.55,
    rundungen_radius        = 5,
    ring_aussen_durchmesser = 200,
    ring_wandstaerke        = 3,
    ring_hoehe              = 20
) {
    union() {
        grundplatte(
            dimension=[
                groesse,
                groesse,
                dicke
            ],
            rundungen_radius = rundungen_radius
        );
        translate([0, 0, (dicke + ring_hoehe) / 2]) {
            ring(
                aussen_durchmesser = ring_aussen_durchmesser,
                wandstaerke = ring_wandstaerke,
                hoehe = ring_hoehe
            );
        }
    }

    module grundplatte(dimension=[100, 100, 5], rundungen_radius = 2) {
        difference() {
            hull() {
                mirror_copy([0, 1, 0]) {
                    mirror_copy([1, 0, 0]) {
                       translate([
                           dimension.x / 2 - rundungen_radius,
                           dimension.y / 2 - rundungen_radius,
                           0
                       ]) {
                           cylinder(
                               r = rundungen_radius,
                               h = dimension.z,
                               center = true
                           );
                       }
                    }
                }
            }

            cylinder(d = 20, h = dimension.z * 1.1, center = true);
        }
    }

    module ring(aussen_durchmesser = 20, wandstaerke = 1, hoehe = 10) {
        specs = thread_specs("M195x6-int");
        turns = (hoehe / specs[0]) - 1;
        translate([0, 0, hoehe / -2]) {
            nut("M195x6", turns=turns, Douter=aussen_durchmesser);
        }
    }
}

