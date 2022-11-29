

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleInner(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::map<int, std::vector<int>> map_workers,
    unsigned int worker,
    unsigned int day
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    unsigned int number_days = avail.size();
    unsigned int number_workers = avail[0].size();

    std::map<int, std::vector<int>> map_workers;

    for (unsigned int day = 0; day < number_days; ++day)
    {
        sched.push_back(std::vector<Worker_T>(dailyNeed, -1));
    }
    for (unsigned int worker = 0; worker < number_workers; ++worker)
    {
        map_workers[worker] = std::vector<int>();
    }

    return scheduleInner(
        avail,
        dailyNeed,
        maxShifts,
        sched,
        map_workers,
        0,
        0
    );
}

bool scheduleInner(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::map<int, std::vector<int>> map_workers,
    unsigned int worker,
    unsigned int day
)
{
    // iterate through all worker options for given day
    for (unsigned int i = 0; i < map_workers.size(); ++i)
    {
        // check if worker is available on that day
        bool bAvailable = avail[day][i];

        // if i in sched[day]
        if (std::count(sched[day].begin(), sched[day].end(), i)) {
            continue;
        }

        // check if worker has exceeded maxShifts
        bool bMoreShiftsAllowed = map_workers[i].size() < maxShifts;

        if (bAvailable && bMoreShiftsAllowed)
        {
            sched[day][worker] = i;
            map_workers[i].push_back(day);

            // recurse if daily need is not met
            bool bDailyNeedNotMet = (worker + 1) < dailyNeed;
            if (bDailyNeedNotMet) { // fill in row for day
                bool worked = scheduleInner(
                    avail,
                    dailyNeed,
                    maxShifts,
                    sched,
                    map_workers,
                    worker + 1,
                    day
                );
                if (!worked) {
                    // backtrack
                    sched[day][worker] = -1;
                    map_workers[i].pop_back();
                }
                else {
                    return true;
                }
            } 
            else if (day < (avail.size() - 1)) { // move on to next day
                bool worked = scheduleInner( 
                    avail,
                    dailyNeed,
                    maxShifts,
                    sched,
                    map_workers,
                    0,
                    day + 1
                );
                if (!worked) {
                    // backtrack
                    sched[day][worker] = -1;
                    map_workers[i].pop_back();
                }
                else {
                    return true;
                }
            }
            else { // at last day and worker
                return true;
            }
        }
    }
    return false;
}