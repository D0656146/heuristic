#include "hill_climbing.h"

#include <stdbool.h>
#include <stdlib.h>

#include "heuristic_algorithm.h"

HillClimbing* NewHillClimbing_MA() {
    HillClimbing* instance = malloc(sizeof(HillClimbing));
    instance->IsAccept = AcceptBetter;
    instance->IsStop = StopIfNotBetter;
    instance->NextStatus = NoStatus;
    return instance;
}
