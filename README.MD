A repo that contains my hardwares projects.
They are very simple, I am still learning. Most of the names are in German. My kicad stuff are mostly dangerous.

## ⚠️ Legal Disclaimer

This project and all associated hardware, firmware, and design files are provided **without any form of certification or regulatory approval**. They are intended **solely for private, educational, or research use**.

Any use of this project in a commercial, professional, or publicly accessible context may **require certification** (e.g. CE, FCC, RED) and compliance with national and international regulations.

It is **your sole responsibility** to ensure that any use, deployment, or modification of this project complies with applicable laws and technical standards, particularly in regard to **radio frequency emissions, electromagnetic compatibility (EMC), and product safety**.

The authors disclaim any liability for damages, legal issues, or regulatory violations resulting from the use of this project.

## License
until 2025/07/08: [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/)  
from 2025/07/08 mutli licenses, see [LICENSE](LICENSE)

I'm making this move to express that the purpose of this repo is no longer just 3d (creative) content, but also includes PCBs and firmware.

## Structure

| folder                                               | description |
|------------------------------------------------------|-------------|
|projects                                              | contains the project, each is an umbrella to stuff for the same problem |
|projects/Makefile                                     | create the hole project (e.g. generated 3mf, stl or generate step from the pcb, build the firmware, flash firmware) |
|projects/&lt;prjname&gt;/3d                           | contains 3d stuff for the project |
|projects/&lt;prjname&gt;/3d/3mf                       | 3mf files for the project (currently all should be printable) |
|projects/&lt;prjname&gt;/3d/openscad                  | contains the openscad files of the project |
|projects/&lt;prjname&gt;/3d/openscad/libs             | link to the shared 3d libs (see below) |
|projects/&lt;prjname&gt;/3d/openscad/models           | a openscad model with the full assemblied model for the project (like a full case with bottom and top part) |
|projects/&lt;prjname&gt;/3d/openscad/parts            | openscad models which use to create the hole model and must be printed |
|projects/&lt;prjname&gt;/3d/openscad/sharedparts      | link to the shared 3d parts (see below) |
|projects/&lt;prjname&gt;/3d/step                      | step files used by the projekt (like the pcb) |
|projects/&lt;prjname&gt;/3d/stl                       | stl files, some can be printed some are created from the step to be usable in openscad |
|projects/&lt;prjname&gt;/docs                         | some docs |
|projects/&lt;prjname&gt;/docs/assembly                | how to assembly the parts to create the model |
|projects/&lt;prjname&gt;/firmware                     | the source for the firmware |
|projects/&lt;prjname&gt;/firmware/Makefile            | to create and flash the firmware |
|projects/&lt;prjname&gt;/kicad                        | kicad stuff for the project |
|projects/&lt;prjname&gt;/kicad/libraries              | project specific kicad stuff |
|projects/&lt;prjname&gt;/kicad/libraries/footprints   | custom footprints |
|projects/&lt;prjname&gt;/kicad/<kicadproject>         | the kicad project (or project's if we need more than one pcb) for the project |
|projects/&lt;prjname&gt;tools                         | link to shared tools (see below)  |
|shared                                                | stuff has no clean project focus or can be used by more than one project (subfolders should be linked into the projects if needed!)  |
|shared/3d/parts                                       | 3d parts can be used in more than one project  |
|shared/3d/libs                                        | 3d libs or reusable functions  |
|shared/3d/things                                      | 3d models of real existing things (like a speaker, a screen) |
|shared/tools                                          | tools without any project scope (like extract pinmapping from schematic and generate header and board.c) |

all parts are optional, this means a project with only 3d parts must not contains a firmware or a kicad folder.
bigger projects can contains projects it self!

a project should contains at least one Makefile to build it. if no Makefile in a project it means that it not generate or build anything (like tutorials).
