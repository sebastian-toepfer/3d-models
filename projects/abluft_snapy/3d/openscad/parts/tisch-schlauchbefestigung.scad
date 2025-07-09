// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <../sharedparts/screw/hole.scad>
use <../libs/own/mirror_copy.fuc>

schlauch_durchmesser    = 75;

// ein paar berechnete werte ...
basis_laenge            = schlauch_durchmesser + 10;
basis_breite            = 15;
basis_hoehe             = schlauch_durchmesser;
basis_hoehen_versatz    = 15; //ausgewuerfelter wert!

 //etwas laenger damit es auch vollstaendig weg ist
schlauch_laenge         = basis_breite * 1.10;

//ist ja nicht wirklich eine schraube ... aber hier ein paar berechnete werte ...
schrauben_hoehe         = basis_hoehen_versatz;
schrauben_durchmesser   = basis_breite * 0.33;
schrauben_kopf_hoehe    = basis_hoehen_versatz * 0.50;

schraube_plazierung_z = basis_hoehe / -2 + schrauben_hoehe / 2 - schrauben_kopf_hoehe / 2;

rotate([90, 0, 0]) {
    difference() {
        cube([basis_laenge, basis_breite, basis_hoehe], center=true);

        translate([0, 0, basis_hoehen_versatz]) {
            rotate([90, 0, 0]) {
                cylinder(
                    h = schlauch_laenge,
                    d = schlauch_durchmesser,
                    center = true
                );
            }
        }

        translate([0, 0, schraube_plazierung_z]) {
            screw_hole(
                    length = schrauben_hoehe,
                    diameter = schrauben_durchmesser,
                    head_diameter = schrauben_durchmesser * 1.75,
                    head_length = schrauben_kopf_hoehe,
                    center = true
            );
        }

        //meine version von abgerundeten ecken :)
        radius = 2;
        mirror_copy([1, 0, 0]) {
            translate([basis_laenge / 2 - radius , 0, basis_hoehe / 2 - radius]) {
                abrundung(radius);
            }
        }

        mirror_copy([1, 0, 0]) {
            translate([(sqrt(pow(schlauch_durchmesser / 2, 2) - pow(basis_hoehe / 2 - basis_hoehen_versatz, 2)) + radius) * -1 , 0, basis_hoehe / 2 - radius]) {
                abrundung(2);
            }
        }
    }
}

module abrundung(radius = 1) {
    difference() {
        translate([0, - basis_breite * 1.25 / 2, 0]) {
            cube([radius * 1.25, basis_breite * 1.25 , radius * 1.25]);
        }

        rotate([90, 0, 0]) {
            cylinder(h = basis_breite * 1.50, r = radius, center = true);
        }
    }
}

