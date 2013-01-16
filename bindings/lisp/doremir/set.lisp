(in-package :doremir)
(defctype set :pointer)
(defcfun (set-empty "doremir_set_empty") set)
(defcfun (set-single "doremir_set_single") set (a ptr))
(defcfun (set-add "doremir_set_add") set (a ptr) (b set))
(defcfun (set-remove "doremir_set_remove") set (a ptr) (b set))
(defcfun (set-dadd "doremir_set_dadd") set (a ptr) (b set))
(defcfun (set-dremove "doremir_set_dremove") set (a ptr) (b set))
(defcfun (set-copy "doremir_set_copy") set (a set))
(defcfun (set-destroy "doremir_set_destroy") :void (a set))
(defcfun (set-size "doremir_set_size") :int (a set))
(defcfun (set-is-empty "doremir_set_is_empty") :boolean (a set))
(defcfun (set-is-single "doremir_set_is_single") :boolean (a set))
(defcfun (set-has "doremir_set_has") :boolean (a ptr) (b set))
(defcfun (set-is-subset-of "doremir_set_is_subset_of") :boolean (a set) (b set))
(defcfun (set-is-proper-subset-of "doremir_set_is_proper_subset_of") :boolean (a set) (b set))
(defcfun (set-sum "doremir_set_sum") set (a set) (b set))
(defcfun (set-product "doremir_set_product") set (a set) (b set))
(defcfun (set-difference "doremir_set_difference") set (a set) (b set))
(defcfun (set-power "doremir_set_power") set (a set))
(defcfun (set-from-list "doremir_set_from_list") set (a list))
(defcfun (set-to-list "doremir_set_to_list") list (a set))