schlauch_durchmesser = 75;

// ein paar berechnete werte ...
schlauch_laenge = 30;
basis_laenge = schlauch_durchmesser + 10;
basis_breite = 15;
basis_hoehe  = schlauch_durchmesser;

basis_plazierung_x = basis_laenge / -2;
basis_plazierung_y = basis_breite / -2;
basis_plazierung_z = basis_hoehe / -2 - basis_breite;

//ist ja nicht wirklich eine schraube ... aber hier ein paar berechnete werte ...
schrauben_hoehe       = basis_breite * 1.25;
schrauben_durchmesser = 6; 
schrauben_kopf_hoehe  = basis_breite * 0.25;

schraube_plazierung_z = basis_hoehe / -2 - schrauben_hoehe - schrauben_kopf_hoehe / 2;

difference() {
    translate([basis_plazierung_x, basis_plazierung_y, basis_plazierung_z]) {
        cube([basis_laenge, basis_breite, basis_hoehe]);
    }

    translate([0, schlauch_laenge / 2, 0]) {
        rotate([90, 0, 0]) {
            cylinder(
                h = schlauch_laenge,
                d = schlauch_durchmesser
            );
        }
    }

    translate([0, 0, schraube_plazierung_z]) {
        union() {
            cylinder(h = schrauben_hoehe, d = schrauben_durchmesser);
            translate([0, 0, schrauben_hoehe]) {
                cylinder(h = schrauben_kopf_hoehe, d = schrauben_durchmesser + 3);
            }
        }
    }
}
