#ifndef DECISION_ENGINE_H
#define DECISION_ENGINE_H

#include <tk/tkernel.h>

#define DECISION_PRIORITY 5

/**
 * @brief Initialize the decision engine.
 * 
 * @param flg_id The event flag ID used to synchronize components.
 * @return ER E_OK on success, error code otherwise.
 */
ER sentinel_decision_init(ID flg_id);

#endif // DECISION_ENGINE_H
