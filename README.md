# SimFind

Created during my bachelor's thesis, this repository contains code to check the space a human arm can reach in a specified timespan.
The corresponding paper can be found [here](https://mediatum.ub.tum.de/doc/1430188/179228.pdf).

## What's this about?
Guaranteeing safety in human-robot co-existence often requires a prediction of the volume that could be occupied by the human up to a future time, in order to avoid collisions.
Such predictions should be simple and fast for real-time calculation and collision-checking, but account even for unexpected movement.

We use a complex biomechanical model to search for extreme human movement, to validate such a prediction. Since the model has a large input space and highly nonlinear dynamics,
we use an exploration algorithm based on RRTs to efficiently find the extreme movements. We find that the simple prediction encloses all arm positions found by the exploration algorithm,
except where the biomechanical model does not account for collision between body tissue.

## How to use?

1. You'll need [OpenSim](https://simtk.org/projects/opensim) and the appropriate [upper body model](https://simtk.org/projects/up-ext-model)
2. This repository contains the driver code to check reachable space using RRTs. Due to a memory leak in OpenSim at the time of the development we ran the search in batches.
