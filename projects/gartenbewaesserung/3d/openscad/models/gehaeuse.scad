use <../parts/gehaeuse_oberseite.scad>
use <../parts/gehaeuse_unterseite.scad>

translate([0, 0, 7.8 / 2]) {
    gehaeuse_oberseite();
}

translate([0, 0, -18.0]) {
    import("../../stl/board_pumpensteuerung_tht.stl");
}
translate([0, 0, -25.3]) {
   gehaeuse_unterseite();
}

