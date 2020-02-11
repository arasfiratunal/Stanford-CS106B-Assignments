#include "DoctorsWithoutOrders.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "vector.h"
#include "set.h"
using namespace std;

/* * * * Doctors Without Orders * * * */

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */


//The helper function of canAllPatientsBeSeen.Uses recursion.
//Takes the same parameters as its main function but uses the vectors doctors and patients as constants.
bool canAllPatientsBeSeenHelper(const Vector<Doctor> &doctors,
                          const Vector<Patient> &patients,Map<string, Set<string>>& schedule) {
    if(patients.isEmpty()){//base case
        return true;
    }
    else {
        for (int i=0;i<doctors.size();i++) {//to iterate over all the coctors that are available.
            if(patients[0].hoursNeeded<=doctors[i].hoursFree){

                Vector<Doctor> copy1=doctors;
                copy1[i].hoursFree-=patients[0].hoursNeeded;//uses copys to prevent changing doctors and patients vectors.
                Vector<Patient> copy2=patients;
                copy2.remove(0);
                schedule[doctors[i].name].add(patients[0].name);//choose
                if(canAllPatientsBeSeenHelper(copy1,copy2,schedule)){//explore
                    return true;
                }
                else {
                    schedule[doctors[i].name].remove(patients[0].name);//unchoose

                }
            }
        }
    }
    return false;
}


bool canAllPatientsBeSeen(Vector<Doctor> &doctors,
                          Vector<Patient> &patients,
                          Map<string, Set<string>>& schedule) {

    return canAllPatientsBeSeenHelper(doctors,patients,schedule);
}

