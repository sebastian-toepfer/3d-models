use <corner.template.scad>

psu_holder_corner_rechts();

module psu_holder_corner_rechts() {
    mirror([1, 0, 0]) {
        psu_holder_corner_template();
    }
}

