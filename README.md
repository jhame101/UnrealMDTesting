# Molecular dynamics in the Unreal Engine

This is **not** a high accuracy simulation, and it cannot be used for molecular dynamics research (for that, look at [GE](https://github.com/USCCACS/GEARS)[A](https://github.com/jhame101/GEARS)[RS](https://github.com/jhame101/GearsEye)). The goal is to create a visually accurate simulation which can be used for education.

## Making a molecule
To make a molecule, derive a Blueprint class from the C++ Molecule class. Then add child actor components for each atom, and set the correct Atom class on each one. Finally, go to the BP graph and run BondBase::CreateBond on each pair of atoms that need to have a bond between them (make sure to set the correct bond class).

## Different types of atoms
All atoms should be Blueprint classes inheriting from the Atom class in C++. BP_Atom is an arbitrary atom for testing purposes which shouldn't be used for anything important (all functionality from BP_Atom should be implemented in the base Atom class eventually). Derived atoms can be isotopes or ions, and should have their atomic number, mass, charge, and colour (from a material) assigned. (Note that neither atomic number nor charge has been implemented yet.)

## Molecular dynamics implementation
So far, the only MD is the near interaction forces, which is a general combination of van der waals forces and the forces which prevent two atoms from occupying the same space. The equation used to calculate the force between two nearby atoms as a function of the distance is shown on [this Desmos graph](https://www.desmos.com/calculator/jhqdsnt4ul).  For now the forces are directly calculated and applied; in the future, however, potential energy should be calculated and then summed first (then the force can be calculated as the negative gradient of energy). (I'm not entirely sure why, but that's the way most simulations do it so I assume they have a good reason. Also, it means that we can keep track of the potential energy throughout the simulation.) The calculations are done entirely in Blueprint at the moment, but they should be converted to C++ so that more atoms can be simulated without slowing things down. The force calculations should run on a second CPU thread to avoid slowing the framerate if there are too many atoms. (This way the physics simulation can get the latest forces for each atom from the calculation thread and the show will go on, even if it's a bit less accurate.) Finally, other forces need to be included, especially the coulomb forces. The functionality of the PhysicsConstraintComponents in the bonds may also be calculated at the same time as all the other forces at some point in the future (but that's fairly low priority as the game already does it for us).

## Virtual Reality
Eventually the pawn I created that is currently in [GearsEye](https://github.com/jhame101/GearsEye) should be imported to this project. Some setup will be needed for the eye tracking (it's described in the GearsEye instructions).
