#ifndef _LAPINFO_
#define _LAPINFO_

struct LapInfo {
    int current_time[3];
    int current_lap;
    int current_sector;
    int in_stand;
    int abandon;
	int best_time;
	char *car_name;
    int race_done;
};

#endif
