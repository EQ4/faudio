(in-package :audio-engine)
(defctype plot-function (:pointer (:pointer :void)))
(defcfun (plot-functions "doremir_plot_functions") :void (a plot-function) (b ptr) (c nullary) (d ptr))
(defcfun (plot-buffer-float "doremir_plot_buffer_float") :void (a buffer) (b nullary) (c ptr))
(defcfun (plot-buffer-double "doremir_plot_buffer_double") :void (a buffer) (b nullary) (c ptr))
(defcfun (plot-use-gnu "doremir_plot_use_gnu") :void)
(defcfun (plot-use-core "doremir_plot_use_core") :void)