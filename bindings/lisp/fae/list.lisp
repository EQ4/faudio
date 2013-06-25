(in-package :cl-user)
(define-foreign-type list-type () () (:actual-type :pointer))
(define-parse-method list () (make-instance 'list-type))
(defclass list () ((list-ptr :initarg :list-ptr)))
(defmethod translate-to-foreign (x (type list-type)) (slot-value x 'list-ptr))
(defmethod translate-from-foreign (x (type list-type)) (make-instance 'list :list-ptr x))
(defcfun (list-empty "fae_list_empty") list)
(defcfun (list-single "fae_list_single") list (a ptr))
(defcfun (list-cons "fae_list_cons") list (a ptr) (b list))
(defcfun (list-dcons "fae_list_dcons") list (a ptr) (b list))
(defcfun (list-repeat "fae_list_repeat") list (a :int) (b ptr))
(defcfun (list-enumerate "fae_list_enumerate") list (a :int) (b :int))
(defcfun (list-copy "fae_list_copy") list (a list))
(defcfun (list-destroy "fae_list_destroy") :void (a list))
(defcfun (list-is-empty "fae_list_is_empty") :boolean (a list))
(defcfun (list-is-single "fae_list_is_single") :boolean (a list))
(defcfun (list-length "fae_list_length") :int (a list))
(defcfun (list-head "fae_list_head") ptr (a list))
(defcfun (list-tail "fae_list_tail") list (a list))
(defcfun (list-dtail "fae_list_dtail") list (a list))
(defcfun (list-init "fae_list_init") list (a list))
(defcfun (list-dinit "fae_list_dinit") list (a list))
(defcfun (list-last "fae_list_last") ptr (a list))
(defcfun (list-append "fae_list_append") list (a list) (b list))
(defcfun (list-dappend "fae_list_dappend") list (a list) (b list))
(defcfun (list-reverse "fae_list_reverse") list (a list))
(defcfun (list-dreverse "fae_list_dreverse") list (a list))
(defcfun (list-sort "fae_list_sort") list (a list))
(defcfun (list-dsort "fae_list_dsort") list (a list))
(defcfun (list-take "fae_list_take") list (a :int) (b list))
(defcfun (list-dtake "fae_list_dtake") list (a :int) (b list))
(defcfun (list-drop "fae_list_drop") list (a :int) (b list))
(defcfun (list-ddrop "fae_list_ddrop") list (a :int) (b list))
(defcfun (list-index "fae_list_index") ptr (a :int) (b list))
(defcfun (list-range "fae_list_range") list (a :int) (b :int) (c list))
(defcfun (list-insert "fae_list_insert") list (a :int) (b ptr) (c list))
(defcfun (list-dinsert "fae_list_dinsert") list (a :int) (b ptr) (c list))
(defcfun (list-insert-range "fae_list_insert_range") list (a :int) (b list) (c list))
(defcfun (list-dinsert-range "fae_list_dinsert_range") list (a :int) (b list) (c list))
(defcfun (list-remove "fae_list_remove") list (a :int) (b list))
(defcfun (list-dremove "fae_list_dremove") list (a :int) (b list))
(defcfun (list-remove-range "fae_list_remove_range") list (a :int) (b :int) (c list))
(defcfun (list-dremove-range "fae_list_dremove_range") list (a :int) (b :int) (c list))
(defcfun (list-has "fae_list_has") :boolean (a ptr) (b list))
(defcfun (list-find "fae_list_find") ptr (a pred) (b ptr) (c list))
(defcfun (list-index-of "fae_list_index_of") :int (a ptr) (b list))
(defcfun (list-find-index "fae_list_find_index") :int (a pred) (b ptr) (c list))
(defcfun (list-filter "fae_list_filter") list (a pred) (b ptr) (c list))
(defcfun (list-dfilter "fae_list_dfilter") list (a pred) (b ptr) (c list))
(defcfun (list-map "fae_list_map") list (a unary) (b ptr) (c list))
(defcfun (list-dmap "fae_list_dmap") list (a unary) (b ptr) (c list))
(defcfun (list-join-map "fae_list_join_map") list (a unary) (b ptr) (c list))
(defcfun (list-djoin-map "fae_list_djoin_map") list (a unary) (b ptr) (c list))
(defcfun (list-join "fae_list_join") list (a list))
(defcfun (list-djoin "fae_list_djoin") list (a list))
(defcfun (list-fold-left "fae_list_fold_left") ptr (a binary) (b ptr) (c ptr) (d list))
(defcfun (list-dfold-left "fae_list_dfold_left") ptr (a binary) (b ptr) (c ptr) (d list))
(defcfun (list-to-list "fae_list_to_list") list (a list))