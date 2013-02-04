(in-package :audio-engine)
(define-foreign-type device-audio-type () () (:actual-type :pointer))
(define-parse-method device-audio () (make-instance 'device-audio-type))
(defclass device-audio () ((device-audio-ptr :initarg :device-audio-ptr)))
(defmethod translate-to-foreign (x (type device-audio-type)) (slot-value x 'device-audio-ptr))
(defmethod translate-from-foreign (x (type device-audio-type)) (make-instance 'device-audio :device-audio-ptr x))
(define-foreign-type device-midi-type () () (:actual-type :pointer))
(define-parse-method device-midi () (make-instance 'device-midi-type))
(defclass device-midi () ((device-midi-ptr :initarg :device-midi-ptr)))
(defmethod translate-to-foreign (x (type device-midi-type)) (slot-value x 'device-midi-ptr))
(defmethod translate-from-foreign (x (type device-midi-type)) (make-instance 'device-midi :device-midi-ptr x))
(define-foreign-type device-file-type () () (:actual-type :pointer))
(define-parse-method device-file () (make-instance 'device-file-type))
(defclass device-file () ((device-file-ptr :initarg :device-file-ptr)))
(defmethod translate-to-foreign (x (type device-file-type)) (slot-value x 'device-file-ptr))
(defmethod translate-from-foreign (x (type device-file-type)) (make-instance 'device-file :device-file-ptr x))
(define-foreign-type device-buffer-type () () (:actual-type :pointer))
(define-parse-method device-buffer () (make-instance 'device-buffer-type))
(defclass device-buffer () ((device-buffer-ptr :initarg :device-buffer-ptr)))
(defmethod translate-to-foreign (x (type device-buffer-type)) (slot-value x 'device-buffer-ptr))
(defmethod translate-from-foreign (x (type device-buffer-type)) (make-instance 'device-buffer :device-buffer-ptr x))