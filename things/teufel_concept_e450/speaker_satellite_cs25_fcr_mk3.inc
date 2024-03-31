use <../../libs/own/mirror_copy.scad>
use <../../libs/own/sector_arc.scad>

module speaker_satellite_cs25_fcr_mk3() {
  breite_vorn        = 100;
  breite_hinten      = 70;
  tiefe              = 95;
  hoehe              = 140;
  radius_abrundungen = 5;
    
  difference() {
    linear_extrude(height = hoehe, center = true) {
      difference() {
        polygon(points = [
            [breite_hinten / -2, tiefe / 2], [breite_hinten / 2, tiefe / 2],
            [breite_vorn / 2, tiefe / -2], [breite_vorn / -2, tiefe / -2]
        ]);

        mirror_copy([1, 0, 0]) {
          //mit welcher funktion kommen wir hier auf die ausgewuerfelte 4?
          translate([breite_hinten / -2 + 4, tiefe / 2 - radius_abrundungen, 0]) {
            arc(radius = radius_abrundungen, angles = [90, 180], width = radius_abrundungen);
          };
        };
      };
    };
    mirror_copy([0, 0, 1]) {
      translate([0, tiefe/ -2 + radius_abrundungen, hoehe/ 2 - radius_abrundungen]) {
        rotate([0, 90, 0]) {
          linear_extrude(height = breite_vorn * 1.1, center = true) {
            arc(radius = radius_abrundungen, angles = [180, 270], width = radius_abrundungen);
          };
        };
      };
    };
  };
}
