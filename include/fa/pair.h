
#ifndef _FA_PAIR
#define _FA_PAIR

#include <fa.h>
#include <fa/list.h>

/** @addtogroup FaPair
 
    @addtogroup FaPair
    
    Immutable pair data structure.

    @par Literals
    - `pair(1, 2)`

    @par Implements 
    - fa_equal_t
    - fa_order_t
    - fa_copy_t
    - fa_destroy_t
    - fa_dynamic_t
    - fa_string_show_t

    @see 
    - [Data structures](@ref DataStructures)

 
    @defgroup Fa Fa
    @{
    @defgroup FaPair Pair
    @{
    */


typedef struct _fa_pair_t * fa_pair_t;


typedef struct {
            fa_ptr_t first; fa_ptr_t second;
        } fa_pair_struct_t;


fa_pair_t fa_pair_create(fa_ptr_t, fa_ptr_t);


fa_pair_t fa_pair_read(fa_pair_struct_t *);


void fa_pair_write(fa_pair_struct_t *, fa_pair_t);


fa_pair_t fa_pair_copy(fa_pair_t);


void fa_pair_destroy(fa_pair_t);


fa_ptr_t fa_pair_first(fa_pair_t);


fa_ptr_t fa_pair_second(fa_pair_t);


fa_pair_t fa_pair_duplicate(fa_ptr_t);


fa_pair_t fa_pair_swap(fa_pair_t);


fa_pair_t fa_pair_assoc(fa_pair_t);


fa_pair_t fa_pair_unassoc(fa_pair_t);


fa_list_t fa_pair_to_list(fa_pair_t);

/** @}
    @}
    */

#endif // _FA_PAIR

