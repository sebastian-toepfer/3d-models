// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <../parts/gehaeuse_oberseite.scad>
use <../parts/gehaeuse_unterseite.scad>

manuell = true;
step_full_manuell = 3;

max_steps = 3;
step_full = manuell 
    ? step_full_manuell
    : min(floor($t * (max_steps * 2 - 1)) + 1, max_steps * 2 - 1);

assembly_step = floor(step_full / 2) + 1;
explodiert = (step_full % 2 == 0);

gehaeuse(
    ueberlappung_gehaeuse = 5,
    assembly_step         = assembly_step,
    explodiert            = explodiert
);

module gehaeuse(
    ueberlappung_gehaeuse = 5,
    assembly_step         = 3,
    explodiert            = true,
) {
    exploded = [
        assembly_step == 2 && explodiert,
        assembly_step == 3 && explodiert
    ];

    if (assembly_step > 0) {
        translate([
                0,
                0,
                gehaeuse_oberseite_dimension().z
                  / -2
                  + ueberlappung_gehaeuse
                  / 2
        ]) {
           color("white") {
               gehaeuse_unterseite(ueberhang = ueberlappung_gehaeuse);
           }
        }
    }

    if (assembly_step > 1) {
        translate([
                0,
                0,
                gehaeuse_oberseite_dimension().z
                  / -2
                  + ueberlappung_gehaeuse
                  + 2.5
                  + (exploded[0] ? 30 : 0)
        ]) {
            import("../../stl/board_pumpensteuerung.stl");
        }
    }

    if (assembly_step > 2) {
        translate([
                0,
                0,
                gehaeuse_unterseite_dimension().z
                  / 2
                  + (exploded[1] ? 30 : 0)
        ]) {
            color("white") {
                gehaeuse_oberseite();
            }
        }
    }
}
