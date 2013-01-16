(in-package :doremir)
(defctype processor-info (:pointer :void))
(defctype processor-samples buffer)
(defctype processor-interface (:pointer :void))
(defctype processor :pointer)
(defcfun (processor-unary "doremir_processor_unary") processor (a type) (b type) (c unary))
(defcfun (processor-binary "doremir_processor_binary") processor (a type) (b type) (c type) (d binary))
(defcfun (processor-ternary "doremir_processor_ternary") processor (a type) (b type) (c type) (d type) (e ternary))
(defcfun (processor-identity "doremir_processor_identity") processor (a type))
(defcfun (processor-constant "doremir_processor_constant") processor (a type) (b type) (c ptr))
(defcfun (processor-delay "doremir_processor_delay") processor (a type) (b :int32))
(defcfun (processor-split "doremir_processor_split") processor (a type))
(defcfun (processor-seq "doremir_processor_seq") processor (a processor) (b processor))
(defcfun (processor-par "doremir_processor_par") processor (a processor) (b processor))
(defcfun (processor-loop "doremir_processor_loop") processor (a processor))