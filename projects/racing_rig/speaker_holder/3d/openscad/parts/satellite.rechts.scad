// This file is licensed under CC0 1.0 Universal.
// See https://creativecommons.org/publicdomain/zero/1.0/
use <satellite.template.scad>

satellite_rechts();

module satellite_rechts() {
   mirror([1, 0, 0]) {
       satellite_template();
   }
}

