use <center.template.scad>

center_rechts();

module center_rechts() {
    mirror([1, 0, 0]) {
        center_template();
    }
}

