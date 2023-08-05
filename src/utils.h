#ifndef _LCOM_UTILS_H_
#define _LCOM_UTILS_H_

#include <assert.h>

/** @brief type pun the given expression as the type */
#define TYPE_PUN(typ, exp) (*(typ *) &exp)

/**
 * @brief Run deferer (after defer label) code before returning
 *
 * @param value Return value
 */
#define return_defer(value) \
  do {                      \
    result = (value);       \
    goto defer;             \
  } while (0)

/**
 * @brief Add an element to a dynamic array growing if necessary
 *
 * @param da Dynamic array
 * @param item Element to be added
 */
#define da_append(da, item)                                                      \
  do {                                                                           \
    if ((da)->count >= (da)->capacity) {                                         \
      (da)->capacity = (da)->capacity == 0 ? 256 : (da)->capacity * 2;           \
      (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items)); \
      assert((da)->items != NULL && "Buy more RAM lol");                         \
    }                                                                            \
                                                                                 \
    (da)->items[(da)->count++] = (item);                                         \
  } while (0)

/**
 * @brief Appends an item to a queue
 *
 * @param q Queue
 * @param item Item to be added
 */
#define queue_append(q, item)                                                                                                  \
  do {                                                                                                                         \
    if ((q)->count >= (q)->capacity) {                                                                                         \
      (q)->capacity = (q)->capacity == 0 ? 256 : (q)->capacity * 2;                                                            \
      (q)->items = realloc((q)->items, (q)->capacity * sizeof(*(q)->items));                                                   \
      assert((q)->items != NULL && "Buy more RAM lol");                                                                        \
      if ((q)->capacity != 256) {                                                                                              \
        memmove(&((q)->items[(q)->begin + (q)->count - ((q)->capacity / 2)]), (q)->items, ((q)->begin) * sizeof(*(q)->items)); \
      }                                                                                                                        \
    }                                                                                                                          \
                                                                                                                               \
    (q)->items[((q)->begin + (q)->count++) % (q)->capacity] = (item);                                                          \
  } while (0)

/**
 * @brief Pops an element from a queue
 *
 * @param q Queue
 */
#define queue_pop(q) \
  do {               \
    (q)->count--;    \
    (q)->begin++;    \
  } while (0)

#endif
