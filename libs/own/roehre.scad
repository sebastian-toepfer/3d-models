module roehre(durchmesser = 20, laenge = 10,  wand_staerke = 2, center = true) {
    difference() {
        cylinder(h = laenge, d = durchmesser, center = center);
        cylinder(h = laenge * 1.1, d = durchmesser - wand_staerke * 2, center = center);
    }
}

