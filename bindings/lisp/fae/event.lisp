(in-package :cl-user)
(define-foreign-type event-type () () (:actual-type :pointer))
(define-parse-method event () (make-instance 'event-type))
(defclass event () ((event-ptr :initarg :event-ptr)))
(defmethod translate-to-foreign (x (type event-type)) (slot-value x 'event-ptr))
(defmethod translate-from-foreign (x (type event-type)) (make-instance 'event :event-ptr x))
(defcfun (event-never "fae_event_never") event)
(defcfun (event-now "fae_event_now") event (a ptr))
(defcfun (event-delay "fae_event_delay") event (a time) (b event))
(defcfun (event-merge "fae_event_merge") event (a event) (b event))
(defcfun (event-switch "fae_event_switch") event (a event) (b event) (c event))
(defcfun (event-receive "fae_event_receive") event (a message-sender) (b message-address))
(defcfun (event-send "fae_event_send") event (a message-receiver) (b message-address) (c event))
(defcfun (event-destroy "fae_event_destroy") :void (a event))
(defcfun (event-first "fae_event_first") event (a event))
(defcfun (event-rest "fae_event_rest") event (a event))
(defcfun (event-later "fae_event_later") event (a time) (b ptr))
(defcfun (event-repeat "fae_event_repeat") event (a time) (b ptr))
(defcfun (event-after "fae_event_after") event (a event) (b event))
(defcfun (event-before "fae_event_before") event (a event) (b event))
(defcfun (event-sample "fae_event_sample") event (a event) (b event))
(defcfun (event-toggle "fae_event_toggle") event (a event) (b event))
(defcfun (event-filter "fae_event_filter") event (a pred) (b ptr) (c event))
(defcfun (event-dfilter "fae_event_dfilter") event (a pred) (b ptr) (c event))
(defcfun (event-map "fae_event_map") event (a unary) (b ptr) (c event))
(defcfun (event-dmap "fae_event_dmap") event (a unary) (b ptr) (c event))
(defcfun (event-map2 "fae_event_map2") event (a binary) (b ptr) (c event) (d event))
(defcfun (event-dmap2 "fae_event_dmap2") event (a binary) (b ptr) (c event) (d event))
(defcfun (event-offset "fae_event_offset") time (a event))
(defcfun (event-add-sync "fae_event_add_sync") :void (a (:pointer (:pointer :void))) (b ptr) (c event))
(defcfun (event-values "fae_event_values") list (a time) (b time) (c event))
(defcfun (event-has-values "fae_event_has_values") :boolean (a time) (b time) (c event))
(defcfun (event-has-more "fae_event_has_more") :boolean (a time) (b event))