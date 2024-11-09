//only a placeholder for the dimensions

function simucube_2_pro_psu_dimension() = [
    simucube_2_pro_psu_breite(),
    simucube_2_pro_psu_tiefe(),
    simucube_2_pro_psu_hoehe()
];
function simucube_2_pro_psu_breite() = 250;
function simucube_2_pro_psu_tiefe()  = 115;
function simucube_2_pro_psu_hoehe()  = 50;

module simucube_2_pro_psu(center = true) {
  cube(simucube_2_pro_psu_dimension(), center = center);
}

