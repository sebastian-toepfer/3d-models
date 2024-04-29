use <../../../libs/own/polar_pattern.fuc>
use <threadlib/threadlib.scad>

turns = 5;
specs = thread_specs("M6-ext");
P = specs[0]; Rrot = specs[1]; Dsupport = specs[2];
H = (turns + 1) * P;
kopf_durchmesser = Rrot + Dsupport;

translate([0, 0, (H - kopf_durchmesser * 0.5) /2]) {
    pils_kopf(d = kopf_durchmesser);
}
translate([0, 0, kopf_durchmesser * 0.5 / -2]) {
    cylinder(h=H, d=Dsupport, center = true);
    translate([0, 0, (H - P) / -2]) {
        thread("M6-ext", turns = turns);
    }
}

module pils_kopf(d = 6) {
    difference() {
        scale([1, 1, 0.5]) {
            sphere(d = d * 1.5);
        }
        translate([0, 0, d * 0.6 / -2]) {
            cube([d * 1.6, d * 1.6, d * 0.6], center = true);
        }

        polar_pattern(6) {
            translate([(d + d/2)/ 2, 0, 0]) {
                cylinder(d = d / 2, h = d, center = true);
            }
        }
    }
}

