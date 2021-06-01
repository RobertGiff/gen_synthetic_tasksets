This repository is able to generate generic tasksets, output in both XML and text format.
The workload is purely synthetic and takes both task utilization and period as input, generating a configurable number of tasksets in a utilization range with a specified utilization step.
Currently, the task utilization is uniform or bimodal, but other distributions can be added.
The following utilization strings are accepted as input:
* uniform_light  [0.01, 0.1]
* uniform_medium [0.1, 0.4]
* uniform_heavy  [0.4, 0.9]
* bimodal_light  [0.01, 0.9] 8/9 weighted lower value
* bimodal_medium [0.01, 0.9] 6/9 weighted lower value
* bimodal_heavy  [0.01, 0.9] 4/9 weighted lower value
**You can refer to main.cpp to see how to add more! If you want to add a different bimodal range you will need to also update the random number function in main.cpp**

**Other notes:**
Task deadlines are equal to their periods.
All generation code is in main.cpp, the rest of the files aside from the run script and the makefile are related to XML parsing and the code comes from the tiny xml parser: http://www.grinninglizard.com/tinyxml/

**Current Argument List (SUBJECT TO CHANGE)**
1. Taskset util start
2. Taskset util end
3. Taskset util step
4. Number of tasksets per util
5. Task util distribution range (string, see above)
6. Task period lower value (inclusive)
7. Task period upper value (inclusive)
8. Experiment Name (will be the output directory name)
