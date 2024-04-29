playseat_stand_breite      = 55;
playseat_stand_wandstaerke = 2.6;

threaded_insert_d3         = 5.6;
threaded_insert_w          = 2.1;
threaded_insert_l          = 4.0;

difference() {
    cube([
            playseat_stand_breite - playseat_stand_wandstaerke * 2 - 2,
            threaded_insert_d3 + threaded_insert_w * 2,
            threaded_insert_l
        ],
        center = true
    );

    cylinder(
        d = threaded_insert_d3,
        h = threaded_insert_l + 1.1,
        center = true
    );
}

