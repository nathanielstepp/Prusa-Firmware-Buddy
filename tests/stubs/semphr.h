#pragma once

#include <mutex>
#include "FreeRTOS.h"

// This is stub for tests
typedef std::mutex StaticSemaphore_t;

inline SemaphoreHandle_t xSemaphoreCreateBinary() { return NULL; }
inline SemaphoreHandle_t xSemaphoreCreateMutexStatic(StaticSemaphore_t *semaphoreBuffer) { return NULL; }
#define vSemaphoreDelete(...)
#define xSemaphoreGive(...)
inline bool xSemaphoreTake([[maybe_unused]] SemaphoreHandle_t, [[maybe_unused]] TickType_t) { return pdTRUE; }
