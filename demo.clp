; the following declarations are only meant for debugging on my desktop
;(defgeneric display:draw-rect)
;(defgeneric display:fill-rect)
;(defgeneric display:draw-circle)
;(defgeneric display:fill-circle)
;(defgeneric display:draw-pixel)
;(defgeneric display:fill-screen)
;(defgeneric display:draw-line)
;(defgeneric rtc:unixtime)
;(deffunction display:color565 (?r ?g ?b) (random 0 65536))
;(deffunction display:height () 320)
;(deffunction display:width () 240)
(deffunction update-seed
             ()
             (seed (rtc:unixtime)))
(defglobal MAIN
           ?*display-width* = 128
           ?*display-height* = 160
           ?*color-black* = (display:color565 0 0 0)
           ?*color-white* = (display:color565 255 255 255)
           ?*color-red* = (display:color565 255 0 0)
           ?*color-green* = (display:color565 0 255 0)
           ?*color-blue* = (display:color565 0 0 255)
           ?*color-purple* = (display:color565 255 0 255)
           ?*color-yellow* = (display:color565 0 255 255)
           ?*color-red-green* = (display:color565 255 255 0)
           ?*random-colors0* = (create$))
(deffunction check-time
             (?fn $?rest)
             (bind ?start-time (rtc:unixtime))
             (bind ?result 
                   (funcall ?fn (expand$ ?rest)))
             (bind ?end-time 
                   (rtc:unixtime))
             (printout t "(" ?fn " " (expand$ ?rest) ") took: " 
                       (- ?end-time ?start-time) crlf)
             ?result)
(defgeneric clear-screen)
(defmethod clear-screen
  ((?value INTEGER))
  (display:fill-screen ?value))
(defmethod clear-screen
  ((?red INTEGER)
   (?green INTEGER)
   (?blue INTEGER))
  (display:fill-screen 
    (display:color565 ?red 
                      ?green 
                      ?blue)))

(defmethod clear-screen
  ()
  (clear-screen ?*color-black*))
(deffunction compute-divide-count 
             (?count ?fractional ?skip)
             (- (div ?count ?fractional)
                (div ?skip ?fractional)))
(deffunction generate-random-color-selection
             (?count)
             (update-seed)
             (bind ?qcount
                   (compute-divide-count ?count 8 8)) ; skip 8 elements but make the divisor do the work for us
             ; always provide these colors to be on the safe side
             (bind ?colors 
                   ?*color-black*
                   ?*color-white*
                   ?*color-red*
                   ?*color-green*
                   ?*color-blue*
                   ?*color-purple*
                   ?*color-yellow*
                   ?*color-red-green*)
             (loop-for-count ?qcount do
                             (bind ?colors
                                   ?colors
                                   (random 0 65536)
                                   (random 0 65536)
                                   (random 0 65536)
                                   (random 0 65536)
                                   (random 0 65536)
                                   (random 0 65536)
                                   (random 0 65536)
                                   (random 0 65536)))
             ?colors)
(deffunction enter-test-routine 
             (?router $?message)
             (printout ?router (expand$ ?message) crlf)
             (clear-screen)
             (if (= (length$ ?*random-colors0*) 0) then
               (bind ?*random-colors0*
                     (generate-random-color-selection ?*display-height*))
               (update-seed)))

(deffunction clear-screen-test
             ()
             (enter-test-routine t "Clear screen test!")
             (progn$ (?color ?*random-colors0*) do
                     (clear-screen ?color)))

(deffunction unified-offset
             (?max ?advance ?x ?y ?modulo)
             (max ?max 
                  (integer 
                    (mod 
                      (+ ?advance 
                         ?x 
                         ?y) 
                      ?modulo))))
(deffunction get-unified-offset-color
             (?y ?x)
             (nth$ (unified-offset 1 1 ?y ?x ?*display-height*)
                   ?*random-colors0*))
(deffunction get-random-color
             ()
             (nth$ (random 1 ?*display-height*)
                   ?*random-colors0*))

(deffunction generic-circle-test 
             (?router ?msg ?fn)
             (enter-test-routine ?router 
                                 ?msg)
             (loop-for-count (?radius 16 256) do
                             (funcall ?fn 
                                      (random 0 ?*display-width*)
                                      (random 0 ?*display-height*)
                                      ?radius
                                      (get-random-color))))

(deffunction generic-rect/line-test
             (?router ?msg ?fn)
             (enter-test-routine ?router
                                 ?msg)
             (loop-for-count 256 do
                             (funcall ?fn
                                      (bind ?x 
                                            (random 0 ?*display-width*))
                                      (bind ?y 
                                            (random 0 ?*display-height*))
                                      (random (+ ?x 1) 
                                              ?*display-width*)
                                      (random (+ ?y 1) 
                                              ?*display-height*)
                                      (get-random-color))))

(deffunction draw-circle-test () (generic-circle-test t "draw circle test!" display:draw-circle))
(deffunction fill-circle-test () (generic-circle-test t "fill circle test!" display:fill-circle))
(deffunction draw-rect-test () (generic-rect/line-test t "draw rect test!" display:draw-rect))
(deffunction fill-rect-test () (generic-rect/line-test t "fill rect test!" display:fill-rect))
(deffunction draw-line-test () (generic-rect/line-test t "draw line test!" display:draw-line))


(deffunction pixel-test
             ()
             (enter-test-routine t "256 pixel test!")
             (loop-for-count 256 do
                             (display:draw-pixel (random 0 ?*display-width*)
                                                 (random 0 ?*display-height*)
                                                 (get-random-color))))

(deffunction draw-sine-wave
             ()
             (enter-test-routine t "draw sine wave test!")
             (bind ?width
                   (/ ?*display-width* 2))
             (loop-for-count (?a 0 ?*display-width*) do
                             (display:draw-pixel ?a 
                                                 (+ ?width (sin ?a))
                                                 (get-random-color))))

(deffunction draw-cosine-wave
             ()
             (enter-test-routine t "draw cosine wave test!")
             (bind ?width
                   (/ ?*display-width* 2))
             (loop-for-count (?a 0 ?*display-width*) do
                             (display:draw-pixel ?a 
                                                 (+ ?width (cos ?a))
                                                 (get-random-color))))

(deffunction draw-tangent-wave 
             ()
             (enter-test-routine t "draw tangent wave test!")
             (bind ?width
                   (/ ?*display-width* 2))
             (loop-for-count (?a 0 ?*display-width*) do
                             (display:draw-pixel ?a 
                                                 (+ ?width (tan ?a))
                                                 (get-random-color))))
