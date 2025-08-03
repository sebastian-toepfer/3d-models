/*
  Reference Model – Simucube 2 Pro
  Author: sebastian-toepfer
  Date: 2024-10-20
  MPN: SC2-PRO
  Source: https://simucube.com/simucube-2-pro/
  Metadata: ./metadata.yaml

  Description:
    This OpenSCAD model is a simplified, manually constructed approximation of the
    Simucube 2 Pro Direct Drive system. It is provided as a form reference only,
    and is intended to support workflows such as bracket design, mounting integration,
    and spatial planning in simulator projects.

  ⚠️  Disclaimer:
    - This model is NOT a replica of the original hardware.
    - It must NOT be used for product reproduction or functional emulation.
    - It contains no logos, internal components, or proprietary geometry.
    - No affiliation with or endorsement by Simucube or Granite Devices is claimed.

  License:
    Creative Commons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
    https://creativecommons.org/licenses/by-nc/4.0/
*/
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

