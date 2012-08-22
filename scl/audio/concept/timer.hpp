
#pragma once

#include <scl/concept.hpp>

namespace scl
{
  /**
    Synopsis:

        concept Timer<typename X>
        {
          typename time_type;

          requires (LessThanComparable<time_type>);
          requires (LowerBounded<time_type>);

          improving<time_type> X::time();
        }

    Models:
      none
  */
  template <class X>
  struct Timer
  {
    BOOST_CONCEPT_USAGE(Timer)
    {
    }
  private:
    X t;
  };
}
