#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "tinyxml.h"
#include "tinystr.h"
using namespace std;

#define NUM_SYS_TASK_LIMIT 10000
 
double
randDoubleFrom(double min, double max)
{
	double ret = 0.0;
	double range = max - min;
	double div = RAND_MAX / range;

	ret = min + (rand() / div);

	assert(ret >= min && ret <= max);

	return ret;
}

double
getRandDouble(double task_util_min, double task_util_max, double weight_lower)
{
	double cur_util = 0.0;

	/* uniform distribution */
	if (weight_lower == -1.0) {
		cur_util = randDoubleFrom(task_util_min, task_util_max);
	} else {
		uniform_int_distribution<> prob(1, 100);
		std::random_device rd;
		std::mt19937 gen(rd());
		double task_util_min1 = task_util_min;
		double task_util_max1 = 0.4;
		double task_util_min2 = 0.4;
		double task_util_max2 = task_util_max;

		assert(weight_lower >= 0.0 && weight_lower <= 1.0);

		/* pick which of the two distributions, lower or higher */
		if (prob(gen) <= (weight_lower * 100)) {
			/* lower */
			cur_util = randDoubleFrom(task_util_min1, task_util_max1);
		} else {
			/* higher */
			cur_util = randDoubleFrom(task_util_min2, task_util_max2);
		}
	}
	return cur_util;
}

/* returns task util */
double
create_and_xmlwrite_task(TiXmlElement *component, int *task_idx,
		double task_util_min, double task_util_max, double weight_lower,
		double pre_util, double period_min, double period_max, int fd,
		int *task_array, int *task_array_idx)
{
	double wcet = 0.0, period = 0.0, cur_util = 0.0;
	char name_buf[100]; /* 100 is arbitrary */
	char xml_buf[100];  /* 100 is arbitrary */
	char txt_buf[1000];  /* 1000 is arbitrary */

	//task name="0" p="4795.376230080735" d="4795.376230080735" ref_e="751.461109" ref_util="0.16"
	
	if (fd == -1) assert(0);

	/* get random util in range */
	if (pre_util == -1.0)
		cur_util = getRandDouble(task_util_min, task_util_max, weight_lower);
	else
		cur_util = pre_util;
	//printf("task util: %f, ", cur_util);
	
	/* get random period in range */
	period = getRandDouble(period_min, period_max, -1.0);
	//printf("period: %f\n", period);

	/* calculate wcet for task */
	wcet = cur_util * period;
	//printf("wcet: %f, ", wcet);

	/* OUTPUT TO XML */
	TiXmlElement *xmltask = new TiXmlElement("task");
	component->LinkEndChild(xmltask);
	memset(name_buf, '\0', sizeof(name_buf));
	sprintf(name_buf, "%d", *task_idx);
	xmltask->SetAttribute("name", name_buf);
	task_array[*task_array_idx] = *task_idx;
	*task_array_idx += 1;

	/* keep these in order */
	memset(xml_buf, '\0', sizeof(xml_buf));
	sprintf(xml_buf, "%f", period);
	xmltask->SetAttribute("p", xml_buf);
	xmltask->SetAttribute("d", xml_buf);

	memset(xml_buf, '\0', sizeof(xml_buf));
	sprintf(xml_buf, "%f", wcet);
	xmltask->SetAttribute("e", xml_buf);

	memset(xml_buf, '\0', sizeof(xml_buf));
	sprintf(xml_buf, "%f", cur_util);
	xmltask->SetAttribute("u", xml_buf);

	/* OUTPUT TO TXT */
	memset(txt_buf, '\0', sizeof(txt_buf));
	int num_to_write= sprintf(txt_buf, "task %d, p: %f, d: %f, e: %f, u: %f\n",
				*task_idx, period, period, wcet, cur_util);
	printf("text to write: %s\n", txt_buf);
	int num_written = 0;
	while (num_written < num_to_write) {
		int ret = write(fd, txt_buf + num_written, num_to_write - num_written);
		if (ret == -1) {
			printf("errno: %d\n", errno);
			assert(0);
		}
		num_written += ret;
	}

	(*task_idx)++;
	assert(*task_idx < NUM_SYS_TASK_LIMIT);

	return cur_util;
}

int
main(int argc, char *argv[])
{
	int num_tasksets_per_util = 0;
	double util_start = 0.0, util_end = 0.0, util_step = 0.0;
	double task_util_min = 0.0, task_util_max = 0.0, weight_lower = 0.0;
	double period_min = 0.0, period_max = 0.0, dep_prob = 0.0;

	util_start                    = atof(argv[1]);
	util_end                      = atof(argv[2]);
	util_step                     = atof(argv[3]);
	num_tasksets_per_util         = atof(argv[4]);
	string task_util_distribution = argv[5];
	period_min		      = atof(argv[6]);
	period_max		      = atof(argv[7]);
	dep_prob		      = atof(argv[8]);
	string exp_name               = argv[9];

	if (task_util_distribution.compare("uniform_light") == 0) {
		task_util_min = 0.01;
		task_util_max = 0.10;
		weight_lower  = -1.0;
	} else if (task_util_distribution.compare("uniform_medium") == 0) {
		task_util_min = 0.10;
		task_util_max = 0.40;
		weight_lower  = -1.0;
	} else if (task_util_distribution.compare("uniform_heavy") == 0) {
		task_util_min = 0.40;
		task_util_max = 0.90;
		weight_lower  = -1.0;
	} else if (task_util_distribution.compare("bimodal_light") == 0) {
		task_util_min = 0.01; /* 8/9 */
		task_util_max = 0.90; /* 1/9 */
		weight_lower  = 8.0/9.0;
	} else if (task_util_distribution.compare("bimodal_medium") == 0) {
		task_util_min = 0.01; /* 6/9*/
		task_util_max = 0.90; /* 3/9 */
		weight_lower  = 6.0/9.0;
	} else if (task_util_distribution.compare("bimodal_heavy") == 0) {
		task_util_min = 0.01; /* 4/9 */
		task_util_max = 0.90; /* 5/9 */
		weight_lower  = 5.0/9.0;
	} else {
		printf("ERROR UNKNOWN TASK UTIL DISTRIBUTION\n");
		return -1;
	}

	printf("Generating modes w/ tasksets\n");
	printf("ARGS: util_start: %f, util_end: %f, util_step: %f\n",
			util_start, util_end, util_step);
	printf("ARGS CONT: num_tasksets_per_util: %d, task_util_distribution: %s, task_util_min: %f, task_util_max: %f\n",
			num_tasksets_per_util, task_util_distribution.c_str(), task_util_min, task_util_max);
	printf("experiment name %s\n", exp_name.c_str());

	/* Init Randomness */
	srand(time(NULL));

	/* Generate tasksets for each util step */
	for (double target_taskset_util = util_start ; target_taskset_util < util_end ; target_taskset_util += util_step) {
		printf("TASKSET UTIL: %f\n", target_taskset_util);

		/* For each taskset util, we gen many tasksets */
		for (int taskset_idx = 0 ; taskset_idx < num_tasksets_per_util ; taskset_idx++) {
			double cur_taskset_util = 0.0, cur_task_util = 0.0;
			int task_idx = 0;
			int task_array_idx = 0;
			int *task_array = (int *)malloc(sizeof(int) * NUM_SYS_TASK_LIMIT);
			assert(task_array);

			/* Init XML output */
			TiXmlDocument doc;
			TiXmlElement *system_elem;
			system_elem = new TiXmlElement("system");
			doc.LinkEndChild(system_elem);

			/* Output directory name */
			string tasksetFolderBase = "./experiments/" + exp_name + "/" + "tasksetUtilRange=" + to_string(util_start) + "-" + to_string(util_step) + "-" + to_string(util_end) + "_tasksetNum=" + to_string(num_tasksets_per_util) + "_taskUtil=" + to_string(task_util_min) + "-" + to_string(task_util_max) + "/" + to_string(target_taskset_util) + "/";
			char addr_buf[10000];
			sprintf(addr_buf, "mkdir -p %s", tasksetFolderBase.c_str());
			int sys_ret = system(addr_buf); /* create directory structure */
			assert(sys_ret == 0);

			/* OUTPUT Text file name */
			string tasksetFolderText = tasksetFolderBase + "input_tasksetUtil=" + to_string(target_taskset_util) + "_index=" + to_string(taskset_idx) + ".txt";
			printf("taskset folder for text output: %s\n", tasksetFolderText.c_str());
			int fd = open(tasksetFolderText.c_str(), O_RDWR | O_CREAT, 0666);
			if (fd == -1) {
				printf("Open error: %d\n", errno);
				assert(0);
			}

			/* Generate tasks of util between start and end until our summed task util == taskset util */
			assert(cur_task_util == 0.0);
			do {
				cur_taskset_util += cur_task_util;
				cur_task_util = create_and_xmlwrite_task(system_elem, &task_idx,
						task_util_min, task_util_max, weight_lower, -1.0,
						period_min, period_max, fd, task_array, &task_array_idx);

			} while ((cur_taskset_util + cur_task_util) < target_taskset_util);

			/**
			 * create last task to get us to target_taskset_util using left over util, if left over util is out of range (too small),
			 * just leave that util available
			 */
			//printf("util after loop: %f, remaining: %f\n", cur_taskset_util,
			//	target_taskset_util - cur_taskset_util);

			if ((target_taskset_util - cur_taskset_util) > task_util_min) {
				//printf("room to allocate one more task\n");
				cur_task_util = create_and_xmlwrite_task(system_elem, &task_idx,
						task_util_min, task_util_max, weight_lower,
						target_taskset_util-cur_taskset_util,
						period_min, period_max, fd, task_array, &task_array_idx);
				cur_taskset_util += cur_task_util;
			}
			//printf("FINAL TASKSET UTIL: %f\n", cur_taskset_util);

			/**
			 * If we are doing task dependencies, calculate these now
			 */
			if (dep_prob > 0.0) {
				/**
				 * This is done in tiers until we run out of tasks,
				 * each tier has a random number of nodes in it
				 * The tier above is depedendent on the tier above where any task in the tier above
				 * has probability dep_prob to be dependent on a task from the prev tier
				 */
				int tiers[100][100];
				int num_rem = task_array_idx;
				int tier_idx = 0;
				int prev_num_selected = 0;

				while (num_rem > 0) {
					/* select a random number of tasks to go into this tier */
					int num_selected = rand() % num_rem;
					if (num_selected == 0) num_selected = 1;
					int tmp = num_selected;

					/* select random tasks to go into the tier */
					while (tmp > 0) {
						int selection = rand() % task_array_idx;
						if (task_array[selection] == -1) continue;
						tiers[tier_idx][tmp-1] = task_array[selection];
						task_array[selection] = -1;
						tmp--;
					}

					num_rem -= num_selected;

					/* if tiers > 0, we need to use dep_prob to see if there is a dependency between all task of prev tier */
					if (tier_idx > 0) {
						int num_to_write = 0;
						char txt_buf[100000]; /* arbitrary */
						for (int i = 0 ; i < num_selected ; i++) {
							bool has_dep = false;
							int bookmark = num_to_write;
							num_to_write += sprintf(txt_buf + num_to_write, "task %d depends on:", tiers[tier_idx][i]);
							for (int j = 0 ; j < prev_num_selected ; j++) {
								int num = rand() % 100;
								if (num > (100 * dep_prob)) continue;
								/* there is a dependency! */
								num_to_write += sprintf(txt_buf + num_to_write, " %d", tiers[tier_idx-1][j]);
								has_dep = true;
							}
							num_to_write += sprintf(txt_buf + num_to_write, "\n");
							/* if we didn't have a dep, don't write anything*/
							if (has_dep == false) num_to_write = bookmark;
						}

						int num_written = 0;
						while (num_written < num_to_write) {
							int ret = write(fd, txt_buf + num_written, num_to_write - num_written);
							if (ret == -1) {
								printf("errno: %d\n", errno);
								assert(0);
							}
							num_written += ret;
						}
					}

					prev_num_selected = num_selected;

					tier_idx++;
				}
			}

			/* OUTPUT XML file name */
			string tasksetFolderXML = tasksetFolderBase + "input_tasksetUtil=" + to_string(target_taskset_util) + "_index=" + to_string(taskset_idx) + ".xml";
			//printf("FULL SAVE PATH: %s\n", tasksetFolderXML.c_str());
			doc.SaveFile( tasksetFolderXML.c_str() );

			free(task_array);
			close(fd);
		}
	}

	return 0;
}
