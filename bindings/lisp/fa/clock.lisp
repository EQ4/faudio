(in-package :faudio)
(defctype clock-clock-interface (:pointer :void))
(define-foreign-type clock-type () () (:actual-type :pointer))
(define-parse-method clock () (make-instance 'clock-type))
(defclass clock () ((clock-ptr :initarg :clock-ptr)))
(defmethod translate-to-foreign (x (type clock-type)) (slot-value x 'clock-ptr))
(defmethod translate-from-foreign (x (type clock-type)) (make-instance 'clock :clock-ptr x))
(defcfun (clock-time "fa_clock_time") time (a clock))
(defcfun (clock-tick-rate "fa_clock_tick_rate") :double (a clock))
(defcfun (clock-ticks "fa_clock_ticks") :int64 (a clock))
(defcfun (clock-get-system-clock "fa_clock_get_system_clock") clock)
(defcfun (clock-get-system-prec-clock "fa_clock_get_system_prec_clock") clock)
(defcfun (clock-get-cpu-clock "fa_clock_get_cpu_clock") clock)
