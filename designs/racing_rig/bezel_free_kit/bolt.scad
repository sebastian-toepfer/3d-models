use <own/polar_pattern.fuc>
use <threadlib/threadlib.scad>

bolt();

module bolt(spec = "M6-ext", laenge = 10) {
    specs = thread_specs(spec);
    turns = 10 / specs[0];
    H = (turns + 1) * specs[0];
    kopf_durchmesser = specs[1] + specs[2];
    translate([0, 0, kopf_durchmesser * 0.5 / 2]) {
        gewindestange(spec, turns);
    }
    translate([0, 0, (H - kopf_durchmesser * 0.5) / -2]) {
        pils_kopf(d = kopf_durchmesser);
    }
}

module pils_kopf(d = 6) {
    difference() {
        scale([1, 1, 0.25]) {
            sphere(d = d * 1.5);
        }
        translate([0, 0, d * 0.6 / 2]) {
            cube([d * 1.6, d * 1.6, d * 0.6], center = true);
        }

        polar_pattern(6) {
            translate([(d + d/2)/ 2, 0, 0]) {
                cylinder(d = d / 2, h = d, center = true);
            }
        }
    }
}

module gewindestange(spec, turns) {
    specs = thread_specs(spec);
    cylinder(h = (turns + 1) * specs[0], d=specs[2], center = true);
    translate([0, 0, ((turns + 1) * specs[0] - specs[0]) / -2]) {
        thread(spec, turns = turns);
    }
}

