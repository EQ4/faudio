
#|
    DoReMIR Audio Engine
    Copyright (c) DoReMIR Music Research 2012-2013
    All rights reserved.
|#

(in-package :faudio)

#|
    Miscellaneous primitives.
|#

; ---------------------------------------------------------------------------------------------------

(defcfun (string-from-utf8# "fa_string_from_utf8") :pointer (a :pointer))
(defcfun (string-to-utf8#  "fa_string_to_utf8") :pointer (a :pointer))
(defcfun (string-destroy# "fa_string_destroy") :void (a :pointer))

(defmethod translate-to-foreign (x (type string-type))
  (string-from-utf8#
    (foreign-string-alloc x :encoding :utf-8)))

(defmethod translate-from-foreign (x (type string-type))
  (foreign-string-to-lisp
    (string-to-utf8# x) :encoding :utf-8))

(defmethod free-translated-object (x (type string-type) a)
  (declare (ignore a))
  (string-destroy# x))

; ---------------------------------------------------------------------------------------------------

(defcfun (ratio-create# "fa_ratio_create") :pointer (a :int32) (b :int32))
(defcfun (ratio-destroy# "fa_ratio_destroy") :void (a :pointer))
(defcfun (ratio-num# "fa_ratio_num") :int32 (a :pointer))
(defcfun (ratio-denom# "fa_ratio_denom") :int32 (a :pointer))

(defun export-ratio# (x)
  (ratio-create# (numerator x) (denominator x)))

(defun import-ratio# (x)
  (/ (ratio-num# x) (ratio-denom# x)))


(defmethod translate-to-foreign (x (type ratio-type))
  (ratio-create# (numerator x) (denominator x)))

(defmethod translate-from-foreign (x (type ratio-type))
  (/ (ratio-num# x) (ratio-denom# x)))

(defmethod free-translated-object (x (type ratio-type) a)
  (declare (ignore a))
  ; FIXME makes (time...) etc fail when object is printed (and otherwise?)
  ; See #112 
  ;(ratio-destroy# x)
  )


; ---------------------------------------------------------------------------------------------------

(defcfun (pair-create# "fa_pair_create") :pointer (a ptr) (b ptr))
(defcfun (pair-destroy# "fa_pair_destroy") :void (a :pointer))
(defcfun (pair-first# "fa_pair_first") ptr (a :pointer))
(defcfun (pair-second# "fa_pair_second") ptr (a :pointer))

(defun export-pair# (x)
  (pair-create# (car x) (cdr x)))

(defun import-pair# (x)
  (cons (pair-first# x) (pair-second# x)))

; TODO auto-import?

(defun export-pair (x)
  (pair-create (car x) (cdr x)))

(defun import-pair (x)
  (cons (pair-first x) (pair-second x)))


; ---------------------------------------------------------------------------------------------------

(defcfun (list-empty# "fa_list_empty") :pointer)
(defcfun (list-cons#  "fa_list_cons")  :pointer (a ptr) (b :pointer))
(defcfun (list-dcons# "fa_list_dcons") :pointer (a ptr) (b :pointer))
(defcfun (list-head#  "fa_list_head") ptr (a :pointer))
(defcfun (list-tail#  "fa_list_tail") :pointer (a :pointer))

(defun export-list# (x)
  (cond
    ((null x)               (list-empty#))
    ((consp x)              (list-cons# (car x) (export-list# (cdr x))))
    ((eq (type-of x) 'list) (slot-value x 'list-ptr))))

(defun import-list# (x)
  (cond
    ((list-is-empty x)  nil)
    (t                  (cons (list-head x) (import-list# (list-tail x))))))

(defmethod translate-to-foreign (x (type list-type))
  (export-list# x))
(defmethod translate-from-foreign (x (type list-type))
    (make-instance 'list :list-ptr x))

(defun export-list (x)
  (cond
    ((null x)               (list-empty))
    ((consp x)              (list-cons (car x) (export-list (cdr x))))
    ((eq (type-of x) 'list) (slot-value x 'list-ptr))))

(defun import-list (x)
  (cond
    ((list-is-empty x)  nil)
    (t                  (cons (list-head x) (import-list (list-tail x))))))


; ---------------------------------------------------------------------------------------------------

; (defun export-type# (x)
;   (cond
;     ((eq x :unit)      (type-simple 0))
;     ((eq x :i8)        (type-simple 1))
;     ((eq x :i16)       (type-simple 2))
;     ((eq x :i32)       (type-simple 3))
;     ((eq x :i64)       (type-simple 4))
;     ((eq x :f32)       (type-simple 5))
;     ((eq x :f64)       (type-simple 6))
;     ((eq x :ptr)       (type-simple 7))
;     ((eq x nil)        (type-simple 0))
;     ((consp x)
;       (cond
;         ((eq :frame  (car x))     (type-frame (export-type# (cadr x))))
;         ((eq :vector (car x))     (type-vector (export-type# (cadr x)) (caddr x)))
;         ((eq :pair   (car x))     (type-pair (export-type# (cadr x)) (export-type# (caddr x))))
;         (t                        (type-pair (export-type# (car x)) (export-type# (cdr x))))))
;     ((eq (type-of x) 'type) (slot-value x 'type-ptr))
;     (t                      x)))

; (defmethod translate-to-foreign (x (type type-type))
  ; (export-type# (export-type# x)))
; (defmethod translate-from-foreign (x (type type-type))
    ; (make-instance 'type :type-ptr x))

; (defun type (x)
  ; (export-type# x)) ; FIXME should not do slot-value??


; ---------------------------------------------------------------------------------------------------

(defcfun (dynamic-get-type# "fa_dynamic_get_type") dynamic-type-repr (a :pointer))

(defun import-dynamic# (x)
  (ecase (dynamic-get-type# x)
    (7 (from-pointer 'pair x))
    (8 (from-pointer 'list x))
    (9 (from-pointer 'set x))
    (10 (from-pointer 'map x))
    (11 (from-pointer 'string x))
    (12 (from-pointer 'ratio x))
    (13 (from-pointer 'midi-message x))))

; ---------------------------------------------------------------------------------------------------

(defmacro midi (&rest args) `(midi-message-create-simple ,@args))

; ---------------------------------------------------------------------------------------------------

(defun to-error (x)
  (from-pointer 'error (to-pointer x)))

; ---------------------------------------------------------------------------------------------------

(defun to-sender (x) (setf s (from-pointer 'message-sender (to-pointer x))))
(defun to-receiver (x) (setf r (from-pointer 'message-receiver (to-pointer x))))

; ---------------------------------------------------------------------------------------------------

(defun to-processor (x) (setf s (from-pointer 'processor (to-pointer x))))

; ---------------------------------------------------------------------------------------------------

(defun make-nullary-function (f)
  (assert (functionp f))
  (lambda (&rest args)
    (declare (ignore args))
    (funcall f)))

; ---------------------------------------------------------------------------------------------------

(defun time (&key days hours minutes
                  seconds milliseconds nanoseconds
                  min sec ms ns)

  (let* ((zero-time         (time-create 0 0 0 0))

         (days-time         (if days    (time-create days 0 0 0) nil))
         (hours-time        (if hours   (time-create 0 hours 0 0) nil))
         (minutes-time      (if minutes (time-create 0 0 minutes 0) nil))
         (seconds-time      (if seconds (time-create 0 0 0 (rational seconds)) nil))
         (milliseconds-time (if milliseconds (time-create 0 0 0 (/ (rational milliseconds) 1000)) nil))
         (nanoseconds-time  (if nanoseconds (time-create 0 0 0 (/ (rational nanoseconds) 1000000)) nil))

         (time-exprs  (remove nil (cl:list days-time hours-time minutes-time
                                           seconds-time milliseconds-time nanoseconds-time))))
    (reduce (lambda (x y)
      (from-pointer 'time (add x y))) time-exprs :initial-value zero-time)))

(defun hours (x) (time :hours x))
(defun minutes (x) (time :minutes x))
(defun seconds (x) (time :seconds x))
(defun milliseconds (x) (time :milliseconds x))

; ---------------------------------------------------------------------------------------------------

(defvar *exported-closures#* (make-hash-table)) ; can be declared "single-threaded", since it is always accessed through a lock (?)

(defvar *exported-closures-lock* (mp:make-lock
                                  :name "exported-closures-lock"))

(defun func-to-int# (f)
  (mp:with-lock (*exported-closures-lock*)
    (let ((n (hash-table-count *exported-closures#*)))
      (setf (gethash n *exported-closures#*) f)
      n)))

(defun int-to-func# (n)
  (mp:with-lock (*exported-closures-lock*)
    (let ((f (gethash n *exported-closures#*)))
      (if f f (cl:error "Uknown func in INT-TO-FUNC")))))

(defcallback funcall0# ptr ((f ptr))
  (funcall (int-to-func# f)))

(defcallback funcall0+time# ptr ((f ptr) (time time))
  (funcall (int-to-func# f) time))

(defcallback funcall1# ptr ((f ptr) (x ptr))
  (funcall (int-to-func# f) x))

(defcallback dfuncall1# :double ((f ptr) (x :double))
  (funcall (int-to-func# f) x))

#+win32
(defcallback predcall1# (:boolean :unsigned-char) ((f ptr) (x ptr))
  (funcall (int-to-func# f) x))
#-win32
(defcallback predcall1# :boolean ((f ptr) (x ptr))
  (funcall (int-to-func# f) x))

(defcallback list2listcall# list ((f ptr) (x list))
  (funcall (int-to-func# f) x))

(defcallback iocall# :void ((f ptr) (x buffer))
  (funcall (int-to-func# f) x))

(defun list-map* (f xs)
  (list-map (callback funcall1#) (func-to-int# f) xs))

(defun list-join-map* (f xs)
  (list-join-map (callback funcall1#) (func-to-int# f) xs))

(defun list-filter* (f xs)
  (list-filter (callback predcall1#) (func-to-int# f) xs))

(defun list-find* (f xs)
  (list-find (callback predcall1#) (func-to-int# f) xs))

(defun list-find-index* (f xs)
  (list-find-index (callback predcall1#) (func-to-int# f) xs))

(defun action-if* (f a)
 (action-if (callback predcall1#) (func-to-int# (make-nullary-function f)) a))

(defun action-while* (f a)
  (action-while (callback predcall1#) (func-to-int# (make-nullary-function f)) a))

(defun action-until* (f a)
 (action-until (callback predcall1#) (func-to-int# (make-nullary-function f)) a))

(defun action-do* (f)
  (action-do (callback funcall0#) (func-to-int# (lambda () (funcall f) nil))))

(defun action-do-with-time* (f)
  (cl:error "Using action-do-with-time* is not recommended, please use action-do-with-ms* instead!")
  (action-do-with-time (callback funcall0+time#) (func-to-int# (lambda (time) (funcall f time) nil))))

(defun action-do-with-ms* (f)
  (action-do-with-time (callback funcall0+time#) (func-to-int#
                                                  (lambda (time)
                                                    (let ((ms (time-to-milliseconds time)))
                                                      (funcall f ms)
                                                      (destroy time))
                                                    nil))))

(defun action-accum* (ch f)
  (action-accum ch (callback dfuncall1#) (func-to-int# f)))

(defun action-get* (ch f)
  (let ((f2 (lambda (x) (funcall f x) 0.0d0)))
    (action-get ch (callback dfuncall1#) (func-to-int# f2))))

; TODO move
(defun translate-nil (type x)
 (if x
     x
   (from-pointer type (cffi::null-pointer))))

(defun audio-open-stream* (i o f)
 (assert (or i o))
 (audio-open-stream (translate-nil 'audio-device i) (translate-nil 'audio-device o) (callback list2listcall#) 
   (func-to-int# 
     (lambda (inputs) 
       (export-list
         (funcall f (mapcar (lambda (x) (from-pointer 'signal x)) 
           (import-list inputs))))))))

; (defun audio-open-stream* (i o f)
;   (audio-open-stream i o (callback list2listcall#) 
;     (func-to-int# 
;       (lambda (inputs) 
;         (export-list
;           (funcall f (mapcar (lambda (x) (from-pointer 'signal x)) 
;             (import-list inputs))))))))

(defun io-pull* (source f)
  (io-pull source (callback iocall#) (func-to-int# (lambda (data)
    (if (cffi:null-pointer-p (to-pointer data))
      (funcall f nil)
      (funcall f data))))))

(defun io-push* (sink buffer)
  (io-push sink (if (not buffer) (from-pointer 'buffer (cffi:null-pointer)) buffer)))


; ---------------------------------------------------------------------------------------------------

(defun audio-add-status-callback* (f session)
  (audio-add-status-callback 
    (callback funcall0#) 
    (func-to-int# f)
    session))

(defun audio-add-message-callback* (f stream)
  (audio-add-message-callback 
    (callback funcall1#) 
    (func-to-int# f)
    stream))

; ---------------------------------------------------------------------------------------------------

(defun midi-add-status-callback* (f session)
  (midi-add-status-callback 
    (callback funcall0#) 
    (func-to-int# f)
    session))

(defun midi-add-message-callback* (f stream)
  (midi-add-message-callback 
   (callback funcall1#) 
   (func-to-int# (lambda (time-msg-pair)
                   (let* ((time (from-pointer 'time (pair-first time-msg-pair)))
                          (msg  (pair-second time-msg-pair)))
                     (funcall f time msg))
                   nil))
   stream))
    
; ---------------------------------------------------------------------------------------------------

(defun thread-create* (f)
  (thread-create (callback funcall0#) (func-to-int# f)))


; ---------------------------------------------------------------------------------------------------

(defcfun (string-show# "fa_string_show") string (a :pointer))

(defmethod print-object ((x buffer) out) (format out "~a" (string-show# (slot-value x 'buffer-ptr))))

(defmethod print-object ((x midi-message) out) (format out "~a" (string-show# (slot-value x 'midi-message-ptr))))
(defmethod print-object ((x list) out) (format out "~a" (string-show# (slot-value x 'list-ptr))))
(defmethod print-object ((x pair) out) (format out "~a" (string-show# (slot-value x 'pair-ptr))))
(defmethod print-object ((x set) out) (format out "~a" (string-show# (slot-value x 'set-ptr))))
(defmethod print-object ((x map) out) (format out "~a" (string-show# (slot-value x 'map-ptr))))
(defmethod print-object ((x ratio) out) (format out "~a" (string-show# (slot-value x 'ratio-ptr))))

(defmethod print-object ((x atomic) out) (format out "~a" (string-show# (slot-value x 'atomic-ptr))))
(defmethod print-object ((x atomic-queue) out) (format out "~a" (string-show# (slot-value x 'atomic-queue-ptr))))
(defmethod print-object ((x atomic-stack) out) (format out "~a" (string-show# (slot-value x 'atomic-stack-ptr))))
(defmethod print-object ((x atomic-ring-buffer) out) (format out "~a" (string-show# (slot-value x 'atomic-ring-buffer-ptr))))

(defmethod print-object ((x signal) out) (format out "~a" (string-show# (slot-value x 'signal-ptr))))

(defmethod print-object ((x thread) out) (format out "~a" (string-show# (slot-value x 'thread-ptr))))

(defmethod print-object ((x time) out) (format out "~a" (string-show# (slot-value x 'time-ptr))))

(defmethod print-object ((x audio-session) out) (format out "~a" (string-show# (slot-value x 'audio-session-ptr))))
(defmethod print-object ((x audio-stream) out) (format out "~a" (string-show# (slot-value x 'audio-stream-ptr))))
(defmethod print-object ((x audio-device) out) (format out "~a" (string-show# (slot-value x 'audio-device-ptr))))
(defmethod print-object ((x midi-session) out) (format out "~a" (string-show# (slot-value x 'midi-session-ptr))))
(defmethod print-object ((x midi-stream) out) (format out "~a" (string-show# (slot-value x 'midi-stream-ptr))))
(defmethod print-object ((x midi-device) out) (format out "~a" (string-show# (slot-value x 'midi-device-ptr))))
(defmethod print-object ((x clock) out) (format out "#<faudio::clock for ~a>" (string-show# (slot-value x 'clock-ptr))))


