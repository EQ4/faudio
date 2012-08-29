
#pragma once

#include <algorithm>
#include <utility>
#include <scl/utility.hpp>
#include <scl/audio/audio_types.hpp>
#include <scl/audio/processor.hpp>

namespace scl
{
  namespace audio
  {
    namespace processor
    {
      using std::pair;

      // template <class A, class B>
      // class constant_processor
      //   : public processor < void, void, void,
      //     void, void,
      //     A, B >
      // {
      // public:
      //   constant_processor(output_type value)
      //     : value(value) {}
      //
      //   void prepare(const argument_type& argument)
      //   {
      //   }
      //   void cleanup(result_type& result)
      //   {
      //   }
      //   void load(const state_type& state)
      //   {
      //   }
      //   void store(state_type& state)
      //   {
      //   }
      //   bool is_ready()
      //   {
      //     return value ? true : false;
      //   }
      //   void process(const list<input_message_type>& input_messages,
      //                const input_type& input,
      //                output_type& output,
      //                list<output_message_type>& output_messages)
      //   {
      //     output = value;
      //   }
      //
      // private:
      //   output_type value;
      // };





      // _ ~> a
      class raw_constant_processor : public raw_processor
      {
        audio_type dummy_type;
        audio_type type;
        raw_buffer buffer;
      public:
        using parent_type = raw_processor;

        raw_constant_processor(audio_type dummy_type, audio_type type)
          : dummy_type(dummy_type), type(type) {}

        audio_type input_type()
        {
          return dummy_type;
        }

        audio_type output_type()
        {
          return type;
        }

        void load(ptr_t state) {}
        void store(ptr_t state) {}

        void prepare(ptr_t arg)
        {
          size_t size = type.size();
          buffer.reset(size);
          scl::raw_copy(arg, arg + size, buffer.begin());
        }

        void cleanup(ptr_t res)
        {
          buffer.clear();
        }

        bool is_ready()
        {
          return buffer.size() > 0;
        }

        void process(ptr_t in_msg,
                     ptr_t input,
                     ptr_t output,
                     ptr_t out_msg)
        {
          scl::raw_copy(buffer.begin(), buffer.end(), output);
        }
      };

    }
  }
}

