#ifndef SETTINGS_H
#define SETTINGS_H

#define HOME_PATH getenv("HOME")
#define SEM_QUEUE "queue"
#define SEM_ARMCHAIR "armchair"
#define SEM_HAIRDRESSER "hairdresser"

#define NO_CLIENTS 10 // number of customers
#define NO_HARIDRESSERS 4 // number of employees
#define NO_ARMCHAIRS 4 // number of seats
#define QUEUE_MAX 5 // length of queue in our company

#define HAIRCUTS_TYPES 100 // number of different types of haircuts
#define HAIRCUT_BASE_TIME 100 // base haircut time
#define WAITING_MAX 3000 // after this time hairdresser go home

#define QUEUE_SIZE 1024 // size of queue buffer (not number of queue seats)

#endif
