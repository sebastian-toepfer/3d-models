module cable_channel(length = 10, cable_diameter = 1, cable_count = 1, cover_thick = 0, center = true) {
    union() {
        cable_channel_cover(length, cable_diameter, cable_count, cover_thick  + 0.1, center);
        translate([0, center ? 0 : cover_thick * 2, center ? cover_thick / -2 : 0]) {
            cube([length, cable_diameter * cable_count, cable_diameter], center = center);
        }
    }
}

module cable_channel_cover(length = 10, cable_diameter = 1, cable_count = 1, cover_thick = 1, center = true) {
    union() {
        translate([0, 0, cable_diameter * (center ? 1/2 : 1)]) {
            cube([length, cable_diameter * cable_count + cover_thick * 4, cover_thick], center = center);
        }
        translate([0, (center ? (cable_diameter * cable_count + cover_thick) * -1/2: cover_thick), cover_thick * (center ? -1/2 : 0)]) {
            cube([length, cover_thick, cable_diameter], center = center);
        }
        translate([0, (center ? (cable_diameter * cable_count + cover_thick) * 1/2 : cover_thick * 2 + cable_diameter * cable_count), cover_thick * (center ? -1/2 : 0)]) {
            cube([length, cover_thick, cable_diameter], center = center);
        }
    }
}

